/////////////////////////////////////////////////////////////////
/// @file      MainWindow.h
/// @author    Chris L Baker (clb) <chris@chimail.net>
/// @date      2014.05.20
///
/// @attention Copyright (C) 2014
/// @attention
/// @attention
/// @attention All rights reserved
/// @attention
/////////////////////////////////////////////////////////////////

#ifndef _DDD__DISPLAY_INTERFACE__MAIN_WINDOW_H_
#define _DDD__DISPLAY_INTERFACE__MAIN_WINDOW_H_

#include <QtGui/QApplication>
#include <QtGui/QtGui>
#include <QtGui/QSplitter>

#include <osg/Node>
#include <mutex>

namespace d3
{

class QOSGWidget;

/////////////////////////////////////////////////////////////////
/// @brief   Class to contain the main window in a Q-magic way
/////////////////////////////////////////////////////////////////
class MainWindow : public QMainWindow
{
    /// standard q-craziness
    Q_OBJECT;

  public:

    /// @brief   Constructor
    MainWindow();

    /// @brief   Destructor
    virtual ~MainWindow();

    /// @brief   Method to set the osg widget
    void setOsgWidget(QOSGWidget* widget);

    /// @brief   Method to add an object to the display
    bool add(const std::string& name,
             const osg::ref_ptr<osg::Node> node,
             const bool& showNode,
             const bool& replace);

  public Q_SLOTS:

    /// @brief   Method to make things go full screen
    void setFullScreen(bool fullscreen);

    /// @brief   Method to control tracking
    void enableNodeTracking(bool enable);

    /// @brief   Method to grab a screen capture
    void grabSnapshot(bool);

    /// @brief   Method to start capturing frames
    void continuousCapture(bool checked);

    /// @brief   Activate a frame render
    void render();

    /// @brief   Method to call when the frame is clicked
    void clicked(const QModelIndex& index);

    /// @{
    /// @name    Public locking functionality
    inline void lock()     { m_mutex.lock(); };
    inline bool try_lock() { return m_mutex.try_lock(); };
    inline void unlock()   { m_mutex.unlock(); };
    /// @}

    /// @{
    /// @name    Set various clear colors
    void setDarkClear();
    void setLightClear();
    void setWhiteClear();
    /// @}

  private:

    /////////////////////////////////////////////////////////////////
    /// @brief   Override the standard qitem for the tree view so we can hold an
    ///          osg node and name of the item
    /////////////////////////////////////////////////////////////////
    class d3DisplayItem : public QStandardItem
    {
      public:
        /// @brief   Construct with a name and node
        /// @param   name The name of the node
        /// @param   node The node to add to this item
        d3DisplayItem(const std::string& name,
                      const osg::ref_ptr<osg::Node> node) :
            QStandardItem(QString(name.c_str())),
            m_name(name),
            m_node(node)
        {
            setEditable(true);
            setCheckable(true);
            setCheckState(Qt::Checked);
        };

        /// @brief   The destructor
        virtual ~d3DisplayItem() {};

        /// @{
        /// @brief   Accessors
        const std::string& getName() const { return m_name; };
        const osg::ref_ptr<osg::Node> getNode()  { return m_node; };
        void setNode(osg::ref_ptr<osg::Node> node) { m_node = node; };
        /// @}

      private:

        /// The name
        std::string                 m_name;

        /// The node
        osg::ref_ptr<osg::Node>     m_node;
    };

    /// @brief   Method to encapsulate a bunch of ugly builder code
    /// @param   dspMenu The display menu to build
    void buildDisplayMenu(QMenu* dspMenu);

    /// @brief   Internal ethod to add an object to the osg display
    /// @param   name The name to use in the model view
    /// @param   node The node to display
    /// @param   enableNode Should the entry be enabled (grayed out or not?)
    /// @param   showNode Should the node be displayed or not (checked or not?)
    /// @param   replace Should we append or overwrite an existing node with the
    ///          same name?
    /// @param   myParent The parent so we can call this recursively
    bool add(const std::string& name,
             const osg::ref_ptr<osg::Node> node,
             const bool& enableNode,
             const bool& showNode,
             const bool& replace,
             d3DisplayItem* myParent);

    /// @brief   Create a new entry in the model view
    /// @param   name The name to use in the model view
    /// @param   node The node to display
    /// @param   enableNode Should the entry be enabled (grayed out or not?)
    /// @param   showNode Should the node be displayed or not (checked or not?)
    /// @param   myParent The parent so we can call this recursively
    bool createNewEntry(const std::string& name,
                        const osg::ref_ptr<osg::Node> node,
                        const bool& enableNode,
                        const bool& showNode,
                        d3DisplayItem* myParent);

    /// @brief   Method to add to an existing entry
    /// @param   entry The entry to add to
    /// @param   node The node to display
    /// @param   enableNode Should the entry be enabled (grayed out or not?)
    /// @param   replace Should we append or overwrite an existing node with the
    ///          same name?
    /// @param   myParent The parent so we can call this recursively
    bool addToEntry(d3DisplayItem* entry,
                    const osg::ref_ptr<osg::Node> node,
                    const bool& enableNode,
                    const bool& replace,
                    d3DisplayItem* myParent);

    /// @brief   Method to replace a node
    /// @param   entry The entry to add to
    /// @param   node The node to display
    /// @param   enableNode Should the entry be enabled (grayed out or not?)
    /// @param   myParent The parent so we can call this recursively
    bool replaceNode(d3DisplayItem* entry,
                     const osg::ref_ptr<osg::Node> node,
                     const bool& enableNode,
                     d3DisplayItem* myParent);

    /// @brief   Method to append to a node
    /// @param   entry The entry to add to
    /// @param   node The node to display
    /// @param   enableNode Should the entry be enabled (grayed out or not?)
    /// @param   myParent The parent so we can call this recursively
    bool appendNode(d3DisplayItem* entry,
                    const osg::ref_ptr<osg::Node> node,
                    const bool& enableNode,
                    d3DisplayItem* myParent);

    /// @brief   Place to add the parent
    /// @param   parentName The name of the parent to add
    /// @param   childName The *rest* of the name to pass back for recursion
    /// @param   node The final child node that will get added (recursion)
    /// @param   enableNode The enableNode flag (recursion)
    /// @param   showNode The showNode flag (recursion)
    /// @param   replace The replace flag (recursion)
    /// @parem   myParent The parent node to add this parent to
    bool addParent(const std::string& parentName,
                   const std::string& childName,
                   const osg::ref_ptr<osg::Node> node,
                   const bool& enableNode,
                   const bool& showNode,
                   const bool& replace,
                   d3DisplayItem* myParent);

    /// @brief   Method to find and return a pointer to the child of parent with name
    static d3DisplayItem* findChild(const d3DisplayItem* myParent,
                                    const std::string& name);

    /// @brief
    static void updateChildren(d3DisplayItem* item,
                               const bool& checked);

    /// @brief   The method for window closing
    virtual void closeEvent(QCloseEvent *ev);

    /// The osg widget
    QOSGWidget*               m_pOsgWidget;

    /// The tree model
    QStandardItemModel*       m_pModel;

    /// The tree
    QTreeView*                m_pTree;

    /// The menu bar
    QMenuBar*                 m_pMenuBar;

    /// The timer to render the frame
    QTimer                    m_timer;

    /// The model protection
    std::recursive_mutex      m_mutex;
};

} // namespace d3

#endif // _DDD__DISPLAY_INTERFACE__MAIN_WINDOW_H_
