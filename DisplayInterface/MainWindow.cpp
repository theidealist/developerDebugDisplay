/////////////////////////////////////////////////////////////////
/// @file      MainWindow.cpp
/// @author    Chris L Baker (clb) <chris@chimail.net>
/// @date      2014.05.20
///
/// @attention Copyright (C) 2014
/// @attention All rights reserved
/////////////////////////////////////////////////////////////////

#include "MainWindow.h"
#include "QOSGWidget.h"
#include "TreeView.h"

#include <QtGui/QTreeView>
#include <QtGui/QActionGroup>
#include <QtGui/QCheckBox>

#include <iostream>

namespace d3
{

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
MainWindow::MainWindow() :
    QMainWindow(),
    m_pOsgWidget(nullptr),
    m_pTree(nullptr),
    m_pMenuBar(),
    m_timer()
{
    // the menu widget
    QWidget* theMenuWidget( new QWidget() );

    // the menu layout
    QHBoxLayout* menuLayout( new QHBoxLayout() );
    menuLayout->setContentsMargins(0,0,0,0);
    menuLayout->setSpacing(0);
    theMenuWidget->setLayout(menuLayout);

    // the menu bar
    m_pMenuBar = new QMenuBar(this);
    m_pMenuBar->setSizePolicy(QSizePolicy::MinimumExpanding,
                              QSizePolicy::Preferred);

    // add the bar to the menu layout
    menuLayout->addWidget(m_pMenuBar);

    // set the menu widget
    setMenuWidget(theMenuWidget);

    // set the main splitter
    QSplitter* splitter(new QSplitter(Qt::Vertical));
    splitter->setChildrenCollapsible(false);
    setCentralWidget(splitter);

    // add this menu
    QMenu* dspMenu = m_pMenuBar->addMenu("Display Interface");
    buildDisplayMenu(dspMenu);

    // setup the action to toggle screen grabs
    {
        QAction* pAction = m_pMenuBar->addAction("Snapshot");
        pAction->setShortcut(QKeySequence(Qt::Key_G));
        QWidget::connect(pAction, SIGNAL(triggered(bool)), this, SLOT(grabSnapshot(bool)));
    }

    // setup the action for continuous frame capture for video
    {
        QAction* pAction = m_pMenuBar->addAction("Video");
        pAction->setShortcut(QKeySequence(Qt::Key_C));
        pAction->setCheckable(true);
        pAction->setChecked(false);
        QWidget::connect(pAction, SIGNAL(triggered(bool)), this, SLOT(continuousCapture(bool)));
    }

    // Quitting from the menu, and as Ctrl-Q
    {
        QAction* pAction = dspMenu->addAction("&Exit");
        pAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q));
        QWidget::connect(pAction, SIGNAL(triggered()), this, SLOT(close()));
    }

    // show everything
    show();
};

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
MainWindow::~MainWindow()
{
    m_timer.stop();
};

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
void MainWindow::setOsgWidget(QOSGWidget* widget)
{
    if ( nullptr == widget ) return;

    // Hold the widget
    m_pOsgWidget = widget;

    // Set the main splitter with this widget
    QSplitter* splitter( static_cast<QSplitter*>(centralWidget()) );
    if ( splitter ) splitter->addWidget(widget);

    // set the render timer
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(render()));
    m_timer.setInterval(33); // 33 = 30fps
    m_timer.start();
};

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
void MainWindow::setTreeView(TreeView* treeView)
{
    if ( nullptr == treeView ) return;

    // Hold this tree view
    //m_pTree = treeView;
    
    // build a dock widget
    QDockWidget *dockWidget = new QDockWidget("Displayed Items");
    dockWidget->setObjectName("Dock");
    dockWidget->setWidget(treeView);
    dockWidget->setTitleBarWidget(nullptr);

    // add the dock widget
    addDockWidget(Qt::RightDockWidgetArea, dockWidget);
};

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
// bool MainWindow::add(const std::string& name,
//                      const osg::ref_ptr<osg::Node> node,
//                      const bool& showNode,
//                      const bool& replace)
// {
//     return m_pTree->add(name, node, showNode, replace);
// };

/////////////////////////////////////////////////////////////////
/////////////// SLOTS //////////////////////////////////////////
///////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
void MainWindow::setFullScreen(bool fullscreen)
{
    if ( isVisible() && (isFullScreen() != fullscreen) )
        setWindowState(windowState() ^ Qt::WindowFullScreen);
};

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
void MainWindow::grabSnapshot(bool)
{
    m_pOsgWidget->getScreenshotCallback()->grab();
};

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
void MainWindow::continuousCapture(bool checked)
{
    m_pOsgWidget->getScreenshotCallback()->setCapture(checked);
    static osg::Vec4 clearColor(m_pOsgWidget->getClearColor());
    if ( checked )
    {
        m_pOsgWidget->setClearColor(osg::Vec4(1.0, 1.0, 1.0, 1.0));
        m_pMenuBar->setStyleSheet("background-color: red");
    }
    else
    {
        m_pOsgWidget->setClearColor(clearColor);
        m_pMenuBar->setStyleSheet("");
    }
};

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
void MainWindow::render()
{
    // make sure we have valid widget and we are visible
    if ( (nullptr != m_pOsgWidget) && isVisible() )
    {
        // try and get the lock
        if ( m_pOsgWidget->try_lock() )
        {
            // update and unlock
            m_pOsgWidget->updateGL();
            m_pOsgWidget->unlock();
        }
    }
};

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
void MainWindow::lock()     {        m_pOsgWidget->lock();     };
bool MainWindow::try_lock() { return m_pOsgWidget->try_lock(); };
void MainWindow::unlock()   {        m_pOsgWidget->unlock();   };

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
void MainWindow::setDarkClear()  { m_pOsgWidget->setClearColor(osg::Vec4(0.1, 0.1, 0.1, 1.0)); };
void MainWindow::setLightClear() { m_pOsgWidget->setClearColor(osg::Vec4(0.5, 0.5, 0.5, 1.0)); };
void MainWindow::setWhiteClear() { m_pOsgWidget->setClearColor(osg::Vec4(1.0, 1.0, 1.0, 1.0)); };

/////////////////////////////////////////////////////////////////
///////////// PRIVATES /////////////////////////////////////////
///////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
void MainWindow::buildDisplayMenu(QMenu* dspMenu)
{
    // F11 toggles fullscreen
    {
        QAction* pAction = dspMenu->addAction("Fullscreen");
        pAction->setCheckable(true);
        pAction->setChecked(false);
        pAction->setShortcut(QKeySequence(Qt::Key_F11));
        QWidget::connect(pAction, SIGNAL(triggered(bool)), this, SLOT(setFullScreen(bool)));
    }

    // setup the ability to control tracking
    {
        QAction* pAction = dspMenu->addAction("Enable Tracking");
        pAction->setChecked(true);
        pAction->setChecked(false);
        pAction->setShortcut(QKeySequence(Qt::Key_T));
        QWidget::connect(pAction, SIGNAL(triggered(bool)), this, SLOT(enableNodeTracking(bool)));
    }

    // setup the background clear color (CC)
    {
        QAction* pActionDark = dspMenu->addAction("CC: dark");
        pActionDark->setCheckable(false);
        QWidget::connect(pActionDark, SIGNAL(triggered()), this, SLOT(setDarkClear()));

        QAction* pActionLight = dspMenu->addAction("CC: light");
        pActionLight->setCheckable(false);
        QWidget::connect(pActionLight, SIGNAL(triggered()), this, SLOT(setLightClear()));

        QAction* pActionWhite = dspMenu->addAction("CC: white");
        pActionWhite->setCheckable(false);
        QWidget::connect(pActionWhite, SIGNAL(triggered()), this, SLOT(setWhiteClear()));
    }
};

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
void MainWindow::closeEvent(QCloseEvent* theEvent)
{
    QMainWindow::closeEvent(theEvent);
};

} // namespace d3
