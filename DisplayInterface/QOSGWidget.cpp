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
#include <osgGA/TerrainManipulator>
#include <osgGA/TrackballManipulator>
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

    m_trackballManipulator(new osgGA::TrackballManipulator),
    m_nodeTrackerManipulator(new osgGA::NodeTrackerManipulator),

    m_pKeypressEventHandler(new KeypressEventHandler()),

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
    m_trackballManipulator->setHomePosition(osg::Vec3d(20,20,40),
                                            osg::Vec3d(0,0,0),
                                            osg::Vec3d(0,0,1));
    m_trackballManipulator->setWheelZoomFactor(-2.0 * m_trackballManipulator->getWheelZoomFactor());
    m_trackballManipulator->setMinimumDistance(0.01);
    m_trackballManipulator->setTrackballSize(0.75);
    m_trackballManipulator->setAllowThrow(true);
    m_pOsgViewer->setCameraManipulator(m_trackballManipulator);

    // Set the default clear color
    setClearColor();

    // set the camera's culling mode
    m_pOsgViewer->getCamera()->setCullingMode(m_pOsgViewer->getCamera()->getCullingMode() &
                                              ~osg::CullSettings::SMALL_FEATURE_CULLING);

    // set the screencapture callback
    m_pOsgViewer->getCamera()->setFinalDrawCallback(m_pScreenshotCallback);

    // add the key press event handler
    m_pOsgViewer->getEventHandlers().push_front(m_pKeypressEventHandler);

    // done with init, so unlock things
    unlock();
};

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
void QOSGWidget::setEnableTracking(bool enable)
{
    lock();
    if ( enable ) m_pOsgViewer->setCameraManipulator(m_nodeTrackerManipulator);
    else          m_pOsgViewer->setCameraManipulator(m_trackballManipulator);
    unlock();
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
void QOSGWidget::trackNode(const osg::ref_ptr<osg::Node>& node)
{
    m_nodeTrackerManipulator->setTrackerMode(osgGA::NodeTrackerManipulator::NODE_CENTER_AND_ROTATION);
    m_nodeTrackerManipulator->setRotationMode(osgGA::NodeTrackerManipulator::TRACKBALL);
    m_nodeTrackerManipulator->setHomePosition({20.0,20.0,40.0},
                                              { 0.0, 0.0, 0.0},
                                              { 0.0, 0.0, 1.0});
    m_nodeTrackerManipulator->setTrackNode(node);
    setEnableTracking(true);
};

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
void QOSGWidget::updateGL()
{
    // do the frame and update
    lock();
    m_pOsgViewer->frame();
    QGLWidget::updateGL();
    unlock();
};

/////////////////////////////////////////////////////////////////
//////// PRIVATES //////////////////////////////////////////////
///////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void QOSGWidget::post(QMouseEvent *qEvent)
{
    // get the button from the event
    int button(0);
    switch ( qEvent->button() )
    {
    case(Qt::LeftButton):  button = osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON;   break;
    case(Qt::MidButton):   button = osgGA::GUIEventAdapter::MIDDLE_MOUSE_BUTTON; break;
    case(Qt::RightButton): button = 3;                                           break;
    case(Qt::NoButton):    // fall through
    default:;
    }

    // convert the qEvent type and signal an event
    switch(qEvent->type())
    {
    case QEvent::MouseButtonPress:   m_pEventQueue->mouseButtonPress(qEvent->x(), qEvent->y(), button);   break;
    case QEvent::MouseButtonRelease: m_pEventQueue->mouseButtonRelease(qEvent->x(), qEvent->y(), button); break;
    case QEvent::MouseMove:          m_pEventQueue->mouseMotion(qEvent->x(), qEvent->y());                break;
    default:;
    }
};

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
osgGA::GUIEventAdapter::KeySymbol QOSGWidget::toOsg(QKeyEvent *theEvent)
{
    // @todo: might need to add more of these for non-alphanumeric keys...
    switch ( theEvent->key() )
    {
    case Qt::Key_Left:  return osgGA::GUIEventAdapter::KEY_Left;
    case Qt::Key_Right: return osgGA::GUIEventAdapter::KEY_Right;
    case Qt::Key_Up:    return osgGA::GUIEventAdapter::KEY_Up;
    case Qt::Key_Down:  return osgGA::GUIEventAdapter::KEY_Down;
    default: return (osgGA::GUIEventAdapter::KeySymbol) *(theEvent->text().toAscii().data() );
    }
};

} // namespace d3
