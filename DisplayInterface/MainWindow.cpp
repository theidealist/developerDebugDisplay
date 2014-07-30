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

#include <QtGui/QActionGroup>
#include <QtGui/QCheckBox>

#include <iostream>

namespace d3
{

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
MainWindow::MainWindow():
    QMainWindow(),
    m_pOsgWidget(nullptr),
    m_pModel(nullptr),
    m_pTree(nullptr),
    m_pMenuBar(),
    m_timer(),
    m_mutex()
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

    // setup the m_pTree veiw for all the objets
    m_pTree = new QTreeView();
    QObject::connect(m_pTree,
                     SIGNAL(clicked(QModelIndex)),
                     this,
                     SLOT(clicked(QModelIndex)));

    // make the qmodel
    m_mutex.lock();
    m_pModel = new QStandardItemModel();

    // add the model
    m_pTree->setModel( m_pModel );
    m_pTree->header()->hide();
    m_pTree->setAllColumnsShowFocus(true);
    m_mutex.unlock();

    // build a dock widget
    QDockWidget *dockWidget = new QDockWidget("Displayed Items");
    dockWidget->setObjectName("Dock");
    dockWidget->setWidget(m_pTree);
    dockWidget->setTitleBarWidget(nullptr);

    // add the dock widget
    addDockWidget(Qt::RightDockWidgetArea, dockWidget);

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

    // setup the top level entry for the root node
    m_mutex.lock();
    d3DisplayItem* topEntry(new d3DisplayItem("All Displayed Items", m_pOsgWidget->getRootGroup()));
    m_pModel->appendRow(topEntry);
    m_mutex.unlock();

    // Set the main splitter with this widget
    QSplitter* splitter( static_cast<QSplitter*>(centralWidget()) );
    if ( splitter ) splitter->addWidget(widget);

    // expand the first depth
    m_mutex.lock();
    m_pTree->expandToDepth(0);
    m_mutex.unlock();

    connect(&m_timer, SIGNAL(timeout()), this, SLOT(render()));
    m_timer.setInterval(33); // 33 = 30fps
    m_timer.start();
};

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
bool MainWindow::add(const std::string& name,
                     const osg::ref_ptr<osg::Node> node,
                     const bool& showNode,
                     const bool& replace)
{
    static const bool enableNode(true);

    // make sure the osg widget has been set
    if ( (nullptr != m_pOsgWidget) && isVisible() )
    {
        // pass this along to the internal adder which does the adds based on items
        // and give it the top level item in the model
        m_mutex.lock();
        d3DisplayItem* myItem(static_cast<d3DisplayItem*>(m_pModel->item(0)));
        bool rv( add(name, node, enableNode, showNode, replace, myItem) );
        m_mutex.unlock();

        // we're done
        return rv;
    }

    // we have no widget - we have failed to add to the non-existent widget
    return false;
};

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
void MainWindow::enableNodeTracking(bool enable)
{
    if ( isVisible() )
    {
        if ( nullptr != m_pOsgWidget )
        {
            static bool shouldEnable(true);
            shouldEnable = not shouldEnable;
            m_pOsgWidget->lock();
            m_pOsgWidget->setEnableTracking(shouldEnable);
            m_pOsgWidget->unlock();
        }
    }
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
void MainWindow::clicked(const QModelIndex& index)
{
    // make sure we have a valid osg and the window is visible
    if ( (nullptr != m_pOsgWidget) && isVisible() )
    {
        // lock the model view
        m_mutex.lock();

        // get the item from the menu index
        d3DisplayItem* item = static_cast<d3DisplayItem*>(m_pModel->itemFromIndex(index));

        // lock osg and set the node mask based on the checked state
        m_pOsgWidget->lock();
        if ( Qt::Checked == item->checkState() ) item->getNode()->setNodeMask(~0);
        else                                     item->getNode()->setNodeMask(0);

        // now we have to recursively process the children to have them match
        // this entry's checked state
        for ( int ii(0) ; ii<item->rowCount() ; ++ii )
            updateChildren(static_cast<d3DisplayItem*>(item->child(ii)),
                           Qt::Checked == item->checkState());

        // unlock osg and the model view
        m_pOsgWidget->unlock();
        m_mutex.unlock();
    }
};

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
bool MainWindow::add(const std::string& name,
                     const osg::ref_ptr<osg::Node> node,
                     const bool& enableNode,
                     const bool& showNode,
                     const bool& replace,
                     d3DisplayItem* myParent)
{
    static const std::string splitIndicator("::");

    // get the name split point
    size_t nameSplit( name.find(splitIndicator) );

    // see if we have children
    if ( std::string::npos != nameSplit )
    {
        // the first part is the parent's name
        std::string pre( name.substr(0, nameSplit) );

        // the second part is the children
        std::string post( name.substr(nameSplit + splitIndicator.size(), name.size()) );

        // call the add parent
        return addParent(pre, post, node, enableNode, showNode, replace, myParent);
    }

    // this must be a leaf node... do we already have this child?
    d3DisplayItem* entry(findChild(myParent, name));

    // see if we need to create a new entry
    if ( nullptr == entry )
    {
        return createNewEntry(name, node, enableNode, showNode, myParent);
    }
    else
    {
        return addToEntry(entry, node, enableNode, replace, myParent);
    }

    return false;
};

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
bool MainWindow::createNewEntry(const std::string& name,
                                const osg::ref_ptr<osg::Node> node,
                                const bool& enableNode,
                                const bool& showNode,
                                d3DisplayItem* myParent)
{
    // create the entry for the item model
    d3DisplayItem* entry( new d3DisplayItem(name, node) );
    entry->setEnabled(enableNode);

    // add this entry to the item model
    m_mutex.lock();
    myParent->appendRow(entry);

    // add the node to the osg tree
    m_pOsgWidget->lock();
    myParent->getNode()->asGroup()->addChild(node);
    m_pOsgWidget->unlock();
    if ( not showNode )
    {
        entry->setCheckState(Qt::Unchecked);
        clicked(m_pModel->indexFromItem(entry));
    }
    m_mutex.unlock();

    return true;
};

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
bool MainWindow::addToEntry(d3DisplayItem* entry,
                            const osg::ref_ptr<osg::Node> node,
                            const bool& enableNode,
                            const bool& replace,
                            d3DisplayItem* myParent)
{
    // add in the new node (maybe replace)
    if ( replace ) return replaceNode(entry, node, enableNode, myParent);
    return                appendNode(entry, node, enableNode, myParent);
};

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
bool MainWindow::replaceNode(d3DisplayItem* entry,
                             const osg::ref_ptr<osg::Node> node,
                             const bool& enableNode,
                             d3DisplayItem* myParent)
{
    // get the lock
    m_pOsgWidget->lock();

    // remove this entry child from the display
    myParent->getNode()->asGroup()->removeChild(entry->getNode());

    // set the new node mask to match the old one
    node->setNodeMask(entry->getNode()->getNodeMask());

    // now lock the model view
    m_mutex.lock();

    // set the node and enabled flags
    entry->setNode(node);
    entry->setEnabled(enableNode);

    // unlock the model view
    m_mutex.unlock();

    // now the entry is ready, so add it back as a child to my parent
    myParent->getNode()->asGroup()->addChild(entry->getNode());

    // unlock osg
    m_pOsgWidget->unlock();

    return true;
};

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
bool MainWindow::appendNode(d3DisplayItem* entry,
                            const osg::ref_ptr<osg::Node> node,
                            const bool& enableNode,
                            d3DisplayItem* myParent)
{
    // get the osg lock
    m_pOsgWidget->lock();

    // get this node as a group
    osg::ref_ptr<osg::Group> group(entry->getNode()->asGroup());
    if ( not group )
    {
        // it's not a group, so make a new group and add this child
        group = new osg::Group();
        group->addChild(entry->getNode());

        // since this is a new group, the node mask should match the entry
        group->setNodeMask(entry->getNode()->getNodeMask());

        // in this case we also need to remove this child from the display
        myParent->getNode()->asGroup()->removeChild(entry->getNode());

        // now lock the model view and set the node and enabled flag for
        // our new group
        m_mutex.lock();
        entry->setNode(group);
        entry->getNode()->setNodeMask(~0);
        m_mutex.unlock();

        // now add this new group
        myParent->getNode()->asGroup()->addChild(entry->getNode());
    }

    // set the enableNode flag
    m_mutex.lock();
    entry->setEnabled(enableNode);
    m_mutex.unlock();

    // add this node to the group
    group->addChild(node);

    // now we can unlock osg
    m_pOsgWidget->unlock();

    return true;
};

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
bool MainWindow::addParent(const std::string& parentName,
                           const std::string& childName,
                           const osg::ref_ptr<osg::Node> node,
                           const bool& enableNode,
                           const bool& showNode,
                           const bool& replace,
                           d3DisplayItem* myParent)
{
    // see if the parent we are about to add as an offspring already exists
    d3DisplayItem* entry(findChild(myParent, parentName));

    // if it's null, we must create it
    if ( nullptr == entry )
    {
        // here is the entry named by the firstPart as a group
        entry = new d3DisplayItem(parentName, new osg::Group());
        entry->setEnabled(true);

        // lock
        m_mutex.lock();

        // add the node to the osg tree
        m_pOsgWidget->lock();
        myParent->getNode()->asGroup()->addChild(entry->getNode());
        m_pOsgWidget->unlock();

        // add this entry to the item model
        myParent->appendRow(entry);

        // unlock
        m_mutex.unlock();
    }

    // make sure this entry is a group
    osg::Group* group( entry->getNode()->asGroup() );
    if ( group )
    {
        // now we can recurse with this itemf
        return add(childName, node, enableNode, showNode, replace, entry);
    }

    // it wasn't a group...
    std::cerr << "ERROR - " << parentName << " already exists as a non-group ("
              << entry->getNode()->className() << ")" << std::endl
              << " --> YOu can't added to an already-added thing that isn't a group" << std::endl;
    return false;
};

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
MainWindow::d3DisplayItem* MainWindow::findChild(const d3DisplayItem* myParent,
                                                 const std::string& name)
{
    // go through the list of rows and see if we can find an entry with "name"
    for ( int ii=0 ; ii<myParent->rowCount() ; ++ii )
    {
        d3DisplayItem* ch = static_cast<d3DisplayItem*>(myParent->child(ii));
        if ( ch && (ch->getName() == name) )
            return ch;
    }
    return nullptr;
};

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
void MainWindow::updateChildren(d3DisplayItem* item,
                                const bool& checked)
{
    item->setEnabled(checked);

    uint ii(0);
    while ( true )
    {
        d3DisplayItem* child( static_cast<d3DisplayItem*>(item->child(ii)) );
        if ( nullptr == child ) break;
        if ( Qt::Checked == item->checkState() )
            updateChildren(child, checked);
        ++ii;
    }

    if ( checked && Qt::Checked == item->checkState() )
        item->getNode()->setNodeMask(~0);
    else
        item->getNode()->setNodeMask(0);
};

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
void MainWindow::closeEvent(QCloseEvent* theEvent)
{
    QMainWindow::closeEvent(theEvent);
};

} // namespace d3
