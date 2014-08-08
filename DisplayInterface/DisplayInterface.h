/////////////////////////////////////////////////////////////////
/// @file      DisplayInterface.h
/// @author    Chris L Baker (clb) <chris@chimail.net>
/// @date      2014.05.20
///
/// @attention Copyright (C) 2014
/// @attention All rights reserved
/////////////////////////////////////////////////////////////////

#ifndef _DDD__DISPLAY_INTERFACE__DISPLAY_INTERFACE_H_
#define _DDD__DISPLAY_INTERFACE__DISPLAY_INTERFACE_H_

#include <DDDisplayInterface/MainPage.h>

#include <osg/Node>
#include <osgViewer/Viewer>

#include <queue>
#include <condition_variable>
#include <thread>

class QWidget;
class QDockWidget;

namespace d3
{

/// forward declar for the static singleton access below
class DisplayInterface;

/// Provide easy access to the static singleton by way of
/// @code
/// d3::di()
/// @endcode
DisplayInterface& di();

/// forward declare the stuff that makes life good
class MainWindow;
class QOSGWidget;

/// @brief   Class to instantiate a simple drawing window and add stuff to
///          it.
///
/// This class manages the main display thread loop. Within this thread we setup
/// the main QApplication and process events. The main thread loop is created
/// but does not actually run until something has been added to the
/// display. Thus, embedding this in applications for testing purposes has very
/// little overhead if you don't actually use it. First, the singleton accessor
/// object is only created the first time d3::di() or
/// d3::DisplayInterface::get() is called. Even then, nothing is drawn and the
/// event loops are not started until something is actually draw. The tradeoff
/// here is that you have to wait for processing that initial time, but after
/// that, your application is not subject to a idle function or some timer wait.
///
/// Here is the basic setup:
/// @code
/// // create some osg node
/// osg::ref_ptr<osg::Node> node( createMyNode() );
///
/// // add this node to the display
/// d3::di().add( "myNode", node );
/// @endcode
/// That's it! There are several ways within this package to create nodes in the
/// DisplayObjects folder, such as Lines.
///
/// To add a line from (0,0,0) to (1,1,1), do something like this:
/// @code
/// #include <DisplayObjects/Lines.h>
/// #include <DisplayInterface/DisplayInterface.h>
///
/// int main()
/// {
///     d3::di().add( "first::line",
///                    d3::get(d3::Line{osg::Vec3d(0,0,0),    // begin
///                                     osg::Vec3d(1,1,1),    // end
///                                     d3::nextColor()}) ); // color
///     d3::di().blockForClose();
///
///     return EXIT_SUCCESS;
/// }
/// @endcode
/// Thus enabling fast 3D display prototyping debugging and displaying
class DisplayInterface
{
  public:

    /// @{
    /// @name Noncopyable
    DisplayInterface(const DisplayInterface&) = delete;
    DisplayInterface& operator=(const DisplayInterface&) = delete;
    /// @}

    /// @brief   The singleton static instance
    static DisplayInterface& get();

    /// @brief   Method to add stuff to the display
    /// @param   name The name of the thing we are adding - a note on the naming
    ///          convention is below
    /// @param   node The osg node we are adding
    /// @param   replace Should we replace the node if "name" already exists
    ///          (i.e. if false, it will just be appended to the current node
    ///          group)
    ///
    /// This is the main method used to add an osg node to the display by
    /// name. The name of the item will be added to the tree view on the right
    /// of the display with a checkbox for toggling on and off the item in the
    /// display.
    ///
    /// The name can be a structured element for the tree view list showing the
    /// displayed objects. Since the names are the keys to the map containing
    /// the nodes and the show/hide ability, names must be unique. However, the
    /// names are embedded in a tree view, thus a parenting structure can be
    /// used. If a name has the key text '::' it is structured such that it will
    /// be in a child/parent relationship.
    ///
    /// For example,
    /// @code
    /// d3::di().add( "AA::BB::CC", node0 );
    /// d3::di().add( "AA::BB::DD", node1 );
    /// d3::di().add( "AA::CC",     node2 );
    /// d3::di().add( "BB",         node3 );
    /// @endcode
    /// Will work as expected in a tree such as:
    /// @verbatim
    /// *AA
    ///  |---BB
    ///  |    |---CC
    ///  |    |---DD
    ///  |
    ///  |---CC
    ///
    /// *BB
    ///  |
    /// @endverbatim
    /// And the two elements, AA::BB and top level BB are independent. Also, in
    /// the tree view with show/hide checkboxes, the top level checkbox will
    /// apply to the children as well. This provides a convenient way for
    /// organizing displays and turning things on and off to view only what is
    /// desired.
    bool add(const std::string& name,
             const osg::ref_ptr<osg::Node> node,
             const bool& replace = true);

    /// @brief   Method to add a function bound to a keypress
    /// @param   key The key to bind to this function
    /// @param   func The function to call when the key is pressed
    /// @param   description The description of the function
    ///
    /// This is the add function that allows arbitrary functions to be tied to
    /// key events.
    bool add(const osgGA::GUIEventAdapter::KeySymbol& key,
             const std::function<bool()>& func,
             const std::string& description = "NONE");

    /// @brief   Method to add a function bound to a keypress
    /// @param   key The key to bind to this function (char version)
    /// @param   func The function to call when the key is pressed
    /// @param   description The description of the function
    ///
    /// This is the add function that allows arbitrary functions to be tied to
    /// key events, which just forwards the call to the add function which takes
    /// a proper KeySymbol
    bool add(const char& key,
             const std::function<bool()>& func,
             const std::string& description = "NONE");

    /// @brief   Track a node with the camera
    /// @param   node The node to track
    ///
    /// This method will track a node with the camera. The current manipulator
    /// mode is the trackball which can still be adjusted, but the camera will
    /// move with the node and will for look at and orbit around the node.
    bool track(const osg::ref_ptr<osg::Node>& node);

    /// @brief   Method to wait for a display to close
    ///
    /// When the main loop of the application is running, it's sometimes desired
    /// to wait for the window to close before stopping all processing. This
    /// method allows this by not returning until the main window has closed.
    void blockForClose();

    /// @brief   Check to see if the display is running
    /// @return  boolean True if the window is open and the display is running
    bool running() const;

    /// @{
    /// @name    When we are manipulating stuff out from under osg
    ///
    /// Occasionally, complex osg structures and full subtrees need to (and can
    /// be) added to this display. In this case, the main event loop may
    /// traverse this part of the tree when the user is manipulating the
    /// tree. This can result in osg segfaulting. So, this provides a method to
    /// lock the display while the user manipulates things, and then unlock to
    /// allow the event processing and display updates to continue.

    /// @brief   Method to lock the osg window
    /// @return  boolean True if successful lock is obtained
    bool lock();

    /// @brief   Method to unlock a previously locked session
    /// @return  boolean True if successful lock is released (really just if the
    ///          window has not been created, we can't "unlock" it, so false is
    ///          returned in this case
    bool unlock();
    /// @}

    /// @brief   Provide access to the underlying root group
    /// @return  osg::ref_ptr<osg::Group> The osg root group
    ///
    /// There may be reason to get the root group, embed it in a node (such as a
    /// scale node to scale the entire world) and then use the setRootGroup()
    /// method. Access here also allows setting top level state_set information.
    osg::ref_ptr<osg::Group> getRootGroup() const;

    /// @brief   Allow us to set the underlying root group to anything
    /// @return  boolean This method dereferences the osg widget object, so we
    ///          return false if that object has not yet been created.
    ///
    /// Since there may be occations to get the root group and make some top
    /// level modifications, or apply a complete tree node at the root, the user
    /// may wish to manually assign the root group. This is the method you are
    /// looking for in this case.
    bool setRootGroup(osg::ref_ptr<osg::Group> rootGroup);

  private:

    /// @brief   Hidden constructor
    ///
    /// This class is a singleton, and so the default constructor is private by
    /// default. The intent here is to use syntax like
    /// @code
    /// d3::di().add("name", node);
    /// @endcode
    /// Where d3 is the namespace, di() is the singleton accessor, and add is
    /// the method by which node can be added with the name "name" to the display.
    DisplayInterface();

    /// @brief   Hidden destructor
    ///
    /// This singleton class does not need a public destructor
    ~DisplayInterface();

    /// @brief   Method to unify the setup of the main window
    /// @return  boolean True if the window can be setup properly
    ///
    /// A single point to setup the main window. This is called many times from
    /// many places and only does work if the main window is not already created
    bool setupMainWindow();

    /// @brief   The display loop runs in a thread
    ///
    void displayThreadLoop();

    /// The main window that is displayed
    MainWindow*                   m_pMainWindow;

    /// The osg widget
    QOSGWidget*                   m_pOsgWidget;

    /// The mutext to add stuff
    std::mutex                    m_mutex;

    /// The notifier for the first add
    std::condition_variable       m_addNotify;

    /// Flag which gets set once we have data to display
    bool                          m_haveData;

    /// Flag to indicate when the setup is complete
    bool                          m_setupComplete;

    /// Thread Running
    std::thread                   m_displayThread;

    /// flag for thread
    bool                          m_threadShouldRun;
};

} // namespace d3

#endif // _DDD__DISPLAY_INTERFACE__DISPLAY_INTERFACE_H_
