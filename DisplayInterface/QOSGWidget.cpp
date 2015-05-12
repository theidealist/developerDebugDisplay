/////////////////////////////////////////////////////////////////
/// @file      QOSGWidget.cpp
/// @author    Chris L Baker (clb) <chris@chimail.net>
/// @date      2014.05.20
///
/// @attention Copyright (C) 2014
/// @attention All rights reserved
/////////////////////////////////////////////////////////////////

#include "QOSGWidget.h"

#include <osg/MatrixTransform>
#include <osgViewer/ViewerEventHandlers>
#include <osg/Point>
#include <osgDB/ReadFile>

#include <osgGA/TrackballManipulator>
#include <osgGA/SphericalManipulator>
#include <osgGA/NodeTrackerManipulator>
#include <osgGA/TerrainManipulator>

#include <QtGui/QActionGroup>
#include <QtGui/QtGui>

namespace d3
{

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
QOSGWidget::QOSGWidget() :
    QGLWidget(),
    m_pGraphicsWindow(new osgViewer::GraphicsWindowEmbedded(0, 0, width(), height())),
    m_pEventQueue(m_pGraphicsWindow->getEventQueue()),
    m_pOsgViewer(new osgViewer::Viewer()),

    m_availableManipulators(),
    m_currentManipulator(),

    m_pKeypressEventHandler(new KeypressEventHandler()),
    m_pClickEventHandler(new ClickEventHandler()),

    m_currentClearColor(),

    m_pRoot(new osg::Group()),
    m_osgLock(),

    m_pScreenshotCallback(new ScreenshotCallback(GL_BACK))
{
    // Allow this widget to get click focus (for setting focus on key events and
    // such)
    setFocusPolicy(Qt::ClickFocus);

    // set mouse tracking so we can get continuous updates of the mouse events
    // (i.e. moves) even if we don't use them right now
    setMouseTracking(true);

    // init the set of available manipulators
    m_availableManipulators[SupportedManipulator::TRACKBALL] = new osgGA::TrackballManipulator();
    m_availableManipulators[SupportedManipulator::SPHERICAL] = new osgGA::SphericalManipulator();
    m_availableManipulators[SupportedManipulator::NODE_TRACKER] = new osgGA::NodeTrackerManipulator();
    m_availableManipulators[SupportedManipulator::TERRAIN] = new osgGA::TerrainManipulator();
    m_availableManipulators[SupportedManipulator::CUSTOM] = new osgGA::TrackballManipulator();
};

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
QOSGWidget::~QOSGWidget()
{
};

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
void QOSGWidget::initialize()
{
    // lock the viewer while we are initializing
    lock();

    // set the root
    m_pOsgViewer->setSceneData( m_pRoot );

    // Set the SceneRoot to normalise normals when scaling is applied to objects.
    m_pRoot->getOrCreateStateSet()->setMode(GL_NORMALIZE, osg::StateAttribute::ON);

    // here is a simple camera
    m_pOsgViewer->getCamera()->setViewport(new osg::Viewport(0,0,width(),height()));
    m_pOsgViewer->getCamera()->setGraphicsContext(m_pGraphicsWindow);
    
    // The trackball is the best!
    osg::ref_ptr<osgGA::TrackballManipulator>
        trackballManipulator(dynamic_cast<osgGA::TrackballManipulator*>
                             (m_availableManipulators[SupportedManipulator::TRACKBALL].get()));
    if ( trackballManipulator )
    {
        trackballManipulator->setHomePosition(osg::Vec3d(20,20,40),
                                              osg::Vec3d(0,0,0),
                                              osg::Vec3d(0,0,1));
        trackballManipulator->setWheelZoomFactor(-2.0 * trackballManipulator->getWheelZoomFactor());
        trackballManipulator->setMinimumDistance(0.01);
        trackballManipulator->setTrackballSize(0.75);
        trackballManipulator->setAllowThrow(true);
    }

    // we also want to support spherical
    osg::ref_ptr<osgGA::SphericalManipulator>
        sphericalManipulator(dynamic_cast<osgGA::SphericalManipulator*>
                             (m_availableManipulators[SupportedManipulator::SPHERICAL].get()));
    if ( sphericalManipulator )
    {
        sphericalManipulator->setHomePosition(osg::Vec3d(20,20,40),
                                              osg::Vec3d(0,0,0),
                                              osg::Vec3d(0,0,1));
        sphericalManipulator->setRotationMode(osgGA::SphericalManipulator::RotationMode::MAP);
        sphericalManipulator->home(1.0);
        sphericalManipulator->setAllowThrow(true);
    }

    // what about terrain mode
    osg::ref_ptr<osgGA::TerrainManipulator>
        terrainManipulator(dynamic_cast<osgGA::TerrainManipulator*>
                           (m_availableManipulators[SupportedManipulator::TERRAIN].get()));
    if ( terrainManipulator )
    {
        terrainManipulator->setRotationMode(osgGA::TerrainManipulator::RotationMode::ELEVATION_AZIM);
        terrainManipulator->setAllowThrow(true);
    }

    // set the current manipulator
    setManipulator(SupportedManipulator::TRACKBALL);
    
    // Set the default clear color
    setClearColor();

    // set the camera's culling mode
    m_pOsgViewer->getCamera()->setCullingMode(m_pOsgViewer->getCamera()->getCullingMode() &
                                              ~osg::CullSettings::SMALL_FEATURE_CULLING);

    // set the screencapture callback
    m_pOsgViewer->getCamera()->setFinalDrawCallback(m_pScreenshotCallback);

    // add the key press event handler
    m_pOsgViewer->getEventHandlers().push_front(m_pKeypressEventHandler);
    m_pOsgViewer->getEventHandlers().push_front(m_pClickEventHandler);

    // done with init, so unlock things
    unlock();
};

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
void QOSGWidget::setManipulator(const SupportedManipulator& manipSelection)
{
    lock();
    m_currentManipulator = m_availableManipulators[manipSelection];
    m_pOsgViewer->setCameraManipulator(m_availableManipulators[manipSelection]);
    unlock();
};

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
void QOSGWidget::setManipulator(const SupportedManipulator& manipSelection,
                                const osg::ref_ptr<osgGA::CameraManipulator> manipulator)
{
    m_availableManipulators[SupportedManipulator::CUSTOM] = manipulator;
    setManipulator(SupportedManipulator::CUSTOM);
};

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
void QOSGWidget::setClearColor(const osg::Vec4& color)
{
    // set this as the clear color
    lock();
    m_pOsgViewer->getCamera()->setClearColor(color);
    unlock();

    // store the current clear color for external access
    m_currentClearColor = color;
};

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
void QOSGWidget::trackNode(const osg::ref_ptr<osg::Node>& node,
                           const osg::Vec3d eye,
                           const osg::Vec3d center,
                           const osg::Vec3d up)
{
    osg::ref_ptr<osgGA::NodeTrackerManipulator>
        nodeTracker(dynamic_cast<osgGA::NodeTrackerManipulator*>
                    (m_availableManipulators[SupportedManipulator::NODE_TRACKER].get()));
    if ( nodeTracker )
    {
        nodeTracker->setTrackerMode(osgGA::NodeTrackerManipulator::NODE_CENTER_AND_ROTATION);
        nodeTracker->setRotationMode(osgGA::NodeTrackerManipulator::TRACKBALL);
        nodeTracker->setHomePosition({ 0.0, 0.0,100.0},
                                     { 0.0, 0.0,  0.0},
                                     { 0.0, 1.0,  0.0});
        nodeTracker->setTrackNode(node);
        setManipulator(SupportedManipulator::NODE_TRACKER);
    }
};

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
void QOSGWidget::updateGL()
{
    // do the frame and update
    if ( m_pOsgViewer && try_lock() )
    {
        makeCurrent();
        m_pOsgViewer->frame();
        QGLWidget::updateGL();
        unlock();
    }
};

/////////////////////////////////////////////////////////////////
//////// PRIVATES //////////////////////////////////////////////
///////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
void QOSGWidget::mousePressEvent(QMouseEvent* qEvent)
{
    switch ( qEvent->button() )
    {
    case Qt::LeftButton:  m_pEventQueue->mouseButtonPress(qEvent->x(), qEvent->y(), 1); break;
    case Qt::MidButton:   m_pEventQueue->mouseButtonPress(qEvent->x(), qEvent->y(), 2); break;
    case Qt::RightButton: m_pEventQueue->mouseButtonPress(qEvent->x(), qEvent->y(), 3); break;
    default:;
    }
};

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
void QOSGWidget::mouseReleaseEvent(QMouseEvent* qEvent)
{
    switch ( qEvent->button() )
    {
    case Qt::LeftButton:  m_pEventQueue->mouseButtonRelease(qEvent->x(), qEvent->y(), 1); break;
    case Qt::MidButton:   m_pEventQueue->mouseButtonRelease(qEvent->x(), qEvent->y(), 2); break;
    case Qt::RightButton: m_pEventQueue->mouseButtonRelease(qEvent->x(), qEvent->y(), 3); break;
    default:;
    }
};

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
void QOSGWidget::mouseMoveEvent(QMouseEvent* qEvent)
{
     m_pEventQueue->mouseMotion(qEvent->x(), qEvent->y());
};

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
void QOSGWidget::mouseDoubleClickEvent( QMouseEvent* qEvent )
{
    switch ( qEvent->button() )
    {
    case Qt::LeftButton:  m_pEventQueue->mouseDoubleButtonPress(qEvent->x(), qEvent->y(), 1); break;
    case Qt::MidButton:   m_pEventQueue->mouseDoubleButtonPress(qEvent->x(), qEvent->y(), 2); break;
    case Qt::RightButton: m_pEventQueue->mouseDoubleButtonPress(qEvent->x(), qEvent->y(), 3); break;
    default:;
    }
};

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
void QOSGWidget::keyPressEvent( QKeyEvent* theEvent )
{
    if ( not theEvent->isAutoRepeat() )
    {
        m_pEventQueue->keyPress( toOsg(theEvent) );
    }
};

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
void QOSGWidget::keyReleaseEvent( QKeyEvent* theEvent )
{
    if ( not theEvent->isAutoRepeat() )
    {
        m_pEventQueue->keyRelease( toOsg(theEvent) );
    }
};

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
void QOSGWidget::wheelEvent( QWheelEvent* theEvent )
{
    m_pEventQueue->mouseScroll(theEvent->delta() < 0 ?
                               osgGA::GUIEventAdapter::SCROLL_UP :
                               osgGA::GUIEventAdapter::SCROLL_DOWN);
};

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
osgGA::GUIEventAdapter::KeySymbol QOSGWidget::toOsg(QKeyEvent *theEvent)
{
    // @todo: might need to add more of these for non-alphanumeric keys...
    switch ( theEvent->key() )
    {
    case Qt::Key_Left:    return osgGA::GUIEventAdapter::KEY_Left;
    case Qt::Key_Right:   return osgGA::GUIEventAdapter::KEY_Right;
    case Qt::Key_Up:      return osgGA::GUIEventAdapter::KEY_Up;
    case Qt::Key_Down:    return osgGA::GUIEventAdapter::KEY_Down;
    case Qt::Key_Shift:   return osgGA::GUIEventAdapter::KEY_Shift_L;
    case Qt::Key_Control: return osgGA::GUIEventAdapter::KEY_Control_L;
    case Qt::Key_Meta:    return osgGA::GUIEventAdapter::KEY_Meta_L;
    default: return (osgGA::GUIEventAdapter::KeySymbol) *(theEvent->text().toAscii().data() );
    }
};

} // namespace d3
