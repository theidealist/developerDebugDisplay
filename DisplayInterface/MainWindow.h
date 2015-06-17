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

#pragma once

#include <QtGui/QApplication>
#include <QtGui/QtGui>
#include <QtGui/QSplitter>

#include <osg/Node>
#include <mutex>

namespace d3
{

class QOSGWidget;
class TreeView;

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

    /// @brief   Add the tree view
    void setTreeView(TreeView* treeView);

  public Q_SLOTS:

    /// @brief   Method to make things go full screen
    void setFullScreen(bool fullscreen);

    /// @brief   Method to grab a screen capture
    void grabSnapshot(bool);

    /// @brief   Method to start capturing frames
    void continuousCapture(bool checked);

    /// @brief   Activate a frame render
    void render();

    /// @{
    /// @name    Public locking functionality
    void lock();
    bool try_lock();
    void unlock();
    /// @}

    /// @{
    /// @name    Set various clear colors
    void setDarkClear();
    void setLightClear();
    void setWhiteClear();
    /// @}

  private:

    /// @brief   Method to encapsulate a bunch of ugly builder code
    /// @param   dspMenu The display menu to build
    void buildDisplayMenu(QMenu* dspMenu);

    /// @brief   The method for window closing
    virtual void closeEvent(QCloseEvent *ev);

    /// The osg widget
    QOSGWidget*               m_pOsgWidget;

    /// The tree
    TreeView*                 m_pTree;

    /// The menu bar
    QMenuBar*                 m_pMenuBar;

    /// The timer to render the frame
    QTimer                    m_timer;
};

} // namespace d3

