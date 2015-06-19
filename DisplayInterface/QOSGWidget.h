/////////////////////////////////////////////////////////////////
/// @file      QOSGWidget.h
/// @author    Chris L Baker (clb) <chris@chimail.net>
/// @date      2014.05.20
///
/// @attention Copyright (C) 2014
/// @attention All rights reserved
/////////////////////////////////////////////////////////////////

#pragma once

#include "ScreenshotCallback.h"
#include "MotionEventHandler.h"
#include "KeypressEventHandler.h"
#include "ClickEventHandler.h"

#include <QtCore/QTimer>
#include <QtGui/QKeyEvent>
#include <QtGui/QMouseEvent>
#include <QtOpenGL/QGLWidget>

#include <osgGA/CameraManipulator>

#include <osgViewer/Viewer>
#include <osg/Group>
#include <osg/ClipPlane>
#include <osg/ClipNode>
#include <osgText/Text>

#include <functional>
#include <mutex>

namespace d3
{

/// @brief   The osg widget to hold the scenegraph
class QOSGWidget : public QGLWidget
{
    /// Do the qt macro stuff
    Q_OBJECT;

  public:

    enum class SupportedManipulator
    {
        TRACKBALL = 0,
        SPHERICAL,
        NODE_TRACKER,
        TERRAIN,
        CUSTOM
    };

    /// @brief   Constructor
    QOSGWidget(QWidget* pp = nullptr);

    /// @brief   Destructor
    virtual ~QOSGWidget();

    /// @brief   Init this class
    void initialize();

    /// @brief   Method to control the enablement of tracking a node
    void setManipulator(const SupportedManipulator& manipSelection);

    /// @brief   Method to set a custom manipulator
    void setManipulator(const SupportedManipulator& manipSelection,
                        const osg::ref_ptr<osgGA::CameraManipulator> manipulator);
    
    /// @brief   Set the clear color
    void setClearColor(const osg::Vec4& color = osg::Vec4(0.1, 0.1, 0.1, 1.0));

    /// @brief   Get at the clear color
    const osg::Vec4& getClearColor() const { return m_currentClearColor; };

    /// @brief   Track a node
    /// @param   node The node to start tracking
    /// @param   eye The position of the eye in the node's frame
    /// @param   center The center point of the trackball
    /// @param   up The up vector of the camera
    void trackNode(const osg::ref_ptr<osg::Node>& node,
                   const osg::Vec3d eye,
                   const osg::Vec3d center,
                   const osg::Vec3d up);

    /// @brief   Get the root osg node
    osg::ref_ptr<osg::Group> getRootGroup() const { return m_pRoot; };

    /// @brief   Provide access to the underlying camera
    osg::ref_ptr<osg::Camera> getCamera() const { return m_pOsgViewer->getCamera(); };

    /// @brief   Get a normalized ray in world coordinates for a point clicked
    ///          in the scene.
    /// @param   startPoint the returned starting point of the vecotr (i.e. the
    ///          camera eye position
    /// @param   rayDirection The direction ray in world coordinates normalized
    /// @param   normalizedClick The point clicked in the scene, normalized as
    ///          in event.getXnormalized(), event.getYnormalized()
    ///
    /// I honestly do not understand why this is so difficult for osg to
    /// provide. I'm sure this is not the easiest way to do this, but I'm pretty
    /// confident this is right. What I'd really like to have is a way to get at
    /// a matrix which takes points in the scene or camera frame and returns the
    /// point in the world frame. Then I can do (0,0,0) times this matrix to get
    /// the camera's position, and (xx,yy,1.0) to get the world point position
    /// of the clicked point, and normalize it to essentially get a ray in the
    /// world's location, but that's not immediately obvious to me how to do
    /// that... though I think I've solved that before...
    void getNormalizedWorldRay(osg::Vec3& startPoint,
                               osg::Vec3& rayDirection,
                               const osg::Vec2& normalizedClick);

    /// @{
    /// @name    Provide get/set access to the cull mask
    osg::Node::NodeMask getCullMask() const { return getCamera()->getCullMask(); };
    void setCullMask(osg::Node::NodeMask msk) { getCamera()->setCullMask(msk); };
    /// @}

    /// @{
    /// @name    Provide get/set access to the node mask
    osg::Node::NodeMask getNodeMask() const { return getCamera()->getNodeMask(); };
    void setNodeMask(osg::Node::NodeMask msk) { getCamera()->setNodeMask(msk); };
    /// @}

    /// @brief   Allow to set the root group
    inline void setRootGroup(osg::ref_ptr<osg::Group> group)
    {
        m_pRoot = group;
        m_pOsgViewer->setSceneData( m_pRoot );
    };

    /// @brief   Add a motion event handler
    /// @param   func The func to call for motion
    /// @param   description The description for help
    inline bool addMotionEventHandler(const std::function<bool(const osgGA::GUIEventAdapter&)>& func,
                                      const std::string& description)
    {
        return m_pMotionEventHandler->add(func, description);
    };
    
    /// @brief   Add a handler for a specific key
    /// @param   key The key to handle
    /// @param   func The function to handle the key
    /// @param   description The description for this key
    inline bool addKeyHandler(const osgGA::GUIEventAdapter::KeySymbol& key,
                              const std::function<bool(const osgGA::GUIEventAdapter&)>& func,
                              const std::string& description)
    {
        return m_pKeypressEventHandler->add(key, func, description);
    };

    /// @brief   Add a handler function for a click event
    /// @param   button The button to handle
    /// @param   func The function to call
    /// @param   description The description of the handler
    inline bool addClickHandler(const osgGA::GUIEventAdapter::MouseButtonMask& button,
                                const std::function<bool(const osgGA::GUIEventAdapter&)>& func,
                                const std::string& description)
    {
        return m_pClickEventHandler->add(button, func, description);
    };

    /// @brief   non-const access to the manipulator
    inline osg::ref_ptr<osgGA::CameraManipulator> getManipulator() { return m_currentManipulator; };

    /// @brief   Get at the screenshot callback
    inline osg::ref_ptr<ScreenshotCallback>& getScreenshotCallback() { return m_pScreenshotCallback; };

    /// @brief   Update the GL for the widget
    virtual void updateGL();

    /// @{
    /// @name    Locking and unlocking mechanisms
    void lock()     { m_osgLock.lock();            };
    bool try_lock() { return m_osgLock.try_lock(); };
    void unlock()   { m_osgLock.unlock();          };
    /// @}

  private Q_SLOTS:

  private:

    /// @brief   Define the type for the manipulators map
    typedef std::map<std::string, osg::Vec4> ClearColorMap;

    /// @{
    /// @name    Handle all the window interfaces and pass them along to osg
    virtual void mousePressEvent( QMouseEvent* qEvent );
    virtual void mouseReleaseEvent( QMouseEvent* qEvent );
    virtual void mouseMoveEvent( QMouseEvent* qEvent );
    virtual void mouseDoubleClickEvent( QMouseEvent* qEvent );
    virtual void keyPressEvent( QKeyEvent* theEvent );
    virtual void keyReleaseEvent( QKeyEvent* theEvent );
    virtual void wheelEvent( QWheelEvent* theEvent );

    ///
    inline virtual void resizeGL( int ww, int hh )
    {
        m_pEventQueue->windowResize(0, 0, ww, hh );
        m_pGraphicsWindow->resized(0, 0, ww, hh);
    };
    /// @}

    /// @brief   Get the osg key symbol
    osgGA::GUIEventAdapter::KeySymbol toOsg(QKeyEvent *event);

    /// The graphics window
    osg::ref_ptr<osgViewer::GraphicsWindowEmbedded>                     m_pGraphicsWindow;

    /// Keep a pointer to the event queue
    osg::ref_ptr<osgGA::EventQueue>                                     m_pEventQueue;

    /// The osg viewer
    osg::ref_ptr<osgViewer::Viewer>                                     m_pOsgViewer;

    std::map<SupportedManipulator, osg::ref_ptr<osgGA::CameraManipulator>> m_availableManipulators;
        
    /// The current manipulator
    osg::ref_ptr<osgGA::CameraManipulator>                              m_currentManipulator;

    /// So we can handle motion events
    osg::ref_ptr<MotionEventHandler>                                    m_pMotionEventHandler;
    
    /// So we can handle keypress events
    osg::ref_ptr<KeypressEventHandler>                                  m_pKeypressEventHandler;

    /// So we can process click events
    osg::ref_ptr<ClickEventHandler>                                     m_pClickEventHandler;

    /// The map of the names to clear colors
    osg::Vec4                                                           m_currentClearColor;

    /// The root osg node
    osg::ref_ptr<osg::Group>                                            m_pRoot;

    /// The mutex to allow for external locking of the osg stuff
    std::recursive_mutex                                                m_osgLock;

    /// The screencapture
    osg::ref_ptr<ScreenshotCallback>                                    m_pScreenshotCallback;
};

} // namespace d3

