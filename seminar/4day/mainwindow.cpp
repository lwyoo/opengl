#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QQuickView>
#include <QtWidgets>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QQuickView* viewer = new QQuickView();
    viewer->setFlags(Qt::FramelessWindowHint);
    viewer->setColor(QColor(Qt::transparent));
    viewer->setResizeMode(QQuickView::SizeViewToRootObject);
    QWidget *pNew = new QWidget;
    QWidget* container = QWidget::createWindowContainer(viewer, pNew);
    viewer->setSource(QUrl(QStringLiteral("qrc:///scenegraph/openglunderqml/main.qml")));

    createDockWindows();
    createStatusBar();
    createActions();
    setCentralWidget(container);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createDockWindows()
{
    QDockWidget *dock = new QDockWidget(tr("Customers"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    customerList = new QListWidget(dock);
    customerList->addItems(QStringList()
            << "Sally Hobart, Tiroli Tea, 67 Long River, Fedula");
    dock->setWidget(customerList);
    addDockWidget(Qt::RightDockWidgetArea, dock);
    //viewMenu->addAction(dock->toggleViewAction());

    dock = new QDockWidget(tr("Paragraphs"), this);
    paragraphsList = new QListWidget(dock);
    paragraphsList->addItems(QStringList()
            << "You made an overpayment (more than $5). Do you wish to "
               "buy more items, or should we return the excess to you?");
    dock->setWidget(paragraphsList);
    addDockWidget(Qt::RightDockWidgetArea, dock);
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::createActions()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    QToolBar *fileToolBar = addToolBar(tr("File"));

    const QIcon newIcon = QIcon::fromTheme("document-new", QIcon(":/images/new.png"));
    newAct = new QAction(newIcon, tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new file"));
    //connect(newAct, &QAction::triggered, this, &MainWindow::newFile);
    fileMenu->addAction(newAct);
    fileToolBar->addAction(newAct);

    const QIcon openIcon = QIcon::fromTheme("document-open", QIcon(":/images/open.png"));
    QAction *openAct = new QAction(openIcon, tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    //connect(openAct, &QAction::triggered, this, &MainWindow::open);
    fileMenu->addAction(openAct);
    fileToolBar->addAction(openAct);

    const QIcon saveIcon = QIcon::fromTheme("document-save", QIcon(":/images/save.png"));
    saveAct = new QAction(saveIcon, tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the document to disk"));
    //connect(saveAct, &QAction::triggered, this, &MainWindow::save);
    fileToolBar->addAction(saveAct);

    const QIcon saveAsIcon = QIcon::fromTheme("document-save-as");
    saveAsAct = new QAction(saveAsIcon, tr("Save &As..."), this);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("Save the document under a new name"));
    //connect(saveAsAct, &QAction::triggered, this, &MainWindow::saveAs);
    fileMenu->addAction(saveAsAct);

    fileMenu->addSeparator();

    QMenu *recentMenu = fileMenu->addMenu(tr("Recent..."));
    //connect(recentMenu, &QMenu::aboutToShow, this, &MainWindow::updateRecentFileActions);
    recentFileSubMenuAct = recentMenu->menuAction();

    for (int i = 0; i < MaxRecentFiles; ++i) {
       // recentFileActs[i] = recentMenu->addAction(QString(), this, &MainWindow::openRecentFile);
    //    recentFileActs[i]->setVisible(false);
    }

    recentFileSeparator = fileMenu->addSeparator();

    //setRecentFilesVisible(MainWindow::hasRecentFiles());

    //fileMenu->addAction(tr("Switch layout direction"), this, &MainWindow::switchLayoutDirection);

    fileMenu->addSeparator();

//! [0]
    const QIcon exitIcon = QIcon::fromTheme("application-exit");
    QAction *exitAct = fileMenu->addAction(exitIcon, tr("E&xit"), qApp, &QApplication::closeAllWindows);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit the application"));
    fileMenu->addAction(exitAct);
//! [0]

#ifndef QT_NO_CLIPBOARD
    QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));
    QToolBar *editToolBar = addToolBar(tr("Edit"));

    const QIcon cutIcon = QIcon::fromTheme("edit-cut", QIcon(":/images/cut.png"));
    cutAct = new QAction(cutIcon, tr("Cu&t"), this);
    cutAct->setShortcuts(QKeySequence::Cut);
    cutAct->setStatusTip(tr("Cut the current selection's contents to the "
                            "clipboard"));
   // connect(cutAct, &QAction::triggered, this, &MainWindow::cut);
    editMenu->addAction(cutAct);
    editToolBar->addAction(cutAct);

    const QIcon copyIcon = QIcon::fromTheme("edit-copy", QIcon(":/images/copy.png"));
    copyAct = new QAction(copyIcon, tr("&Copy"), this);
    copyAct->setShortcuts(QKeySequence::Copy);
    copyAct->setStatusTip(tr("Copy the current selection's contents to the "
                             "clipboard"));
    //connect(copyAct, &QAction::triggered, this, &MainWindow::copy);
    editMenu->addAction(copyAct);
    editToolBar->addAction(copyAct);

    const QIcon pasteIcon = QIcon::fromTheme("edit-paste", QIcon(":/images/paste.png"));
    pasteAct = new QAction(pasteIcon, tr("&Paste"), this);
    pasteAct->setShortcuts(QKeySequence::Paste);
    pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
                              "selection"));
   // connect(pasteAct, &QAction::triggered, this, &MainWindow::paste);
    editMenu->addAction(pasteAct);
    editToolBar->addAction(pasteAct);
#endif

    QMenu *serialPortMenu = menuBar()->addMenu(tr("&Device"));
    QToolBar *serialPortToolBar = addToolBar(tr("Device"));
    const QIcon connectIcon = QIcon::fromTheme("edit-cut", QIcon(":/images/connect.png"));
    connectAct = new QAction(connectIcon, tr("&DCTest"), this);
    //connectAct->setShortcuts(QKeySequence::Cut);
    connectAct->setStatusTip(tr("Add DCTest"));
    //connect(connectAct, &QAction::triggered, this, &MainWindow::openSerialPort);
    serialPortMenu->addAction(connectAct);
    serialPortToolBar->addAction(connectAct);

    const QIcon disConnectIcon = QIcon::fromTheme("edit-cut", QIcon(":/images/disconnect.png"));
    disconnectAct = new QAction(disConnectIcon, tr("&MCU"), this);
    disconnectAct->setStatusTip(tr("Add MCU"));
    //connect(disconnectAct, &QAction::triggered, this, &MainWindow::closeSerialPort);
    serialPortMenu->addAction(disconnectAct);
    serialPortToolBar->addAction(disconnectAct);

    const QIcon configureIcon = QIcon::fromTheme("edit-cut", QIcon(":/images/settings.png"));
    configureAct = new QAction(configureIcon, tr("C&onfigure"), this);
    configureAct->setStatusTip(tr("Configure Serialport"));
    //connect(configureAct, &QAction::triggered, m_settings, &SettingsDialog::show);
    //connect(configureAct, &QAction::triggered, m_DDD, &DeviceDiscoveryDialog::show);
    serialPortMenu->addAction(configureAct);
    serialPortToolBar->addAction(configureAct);

    const QIcon clearIcon = QIcon::fromTheme("edit-cut", QIcon(":/images/clear.png"));
    clearAct = new QAction(clearIcon, tr("C&lear"), this);
    clearAct->setStatusTip(tr("Clear Terminal"));
    //connect(clearAct, &QAction::triggered, m_console, &Console::clear);
    serialPortMenu->addAction(clearAct);
    serialPortToolBar->addAction(clearAct);

    windowMenu = menuBar()->addMenu(tr("&Window"));
    //connect(windowMenu, &QMenu::aboutToShow, this, &MainWindow::updateWindowMenu);

    closeAct = new QAction(tr("Cl&ose"), this);
    closeAct->setStatusTip(tr("Close the active window"));
    //connect(closeAct, &QAction::triggered,
    //        mdiArea, &QMdiArea::closeActiveSubWindow);

    closeAllAct = new QAction(tr("Close &All"), this);
    closeAllAct->setStatusTip(tr("Close all the windows"));
   // connect(closeAllAct, &QAction::triggered, mdiArea, &QMdiArea::closeAllSubWindows);

    tileAct = new QAction(tr("&Tile"), this);
    tileAct->setStatusTip(tr("Tile the windows"));
    //connect(tileAct, &QAction::triggered, mdiArea, &QMdiArea::tileSubWindows);

    cascadeAct = new QAction(tr("&Cascade"), this);
    cascadeAct->setStatusTip(tr("Cascade the windows"));
    //connect(cascadeAct, &QAction::triggered, mdiArea, &QMdiArea::cascadeSubWindows);

    nextAct = new QAction(tr("Ne&xt"), this);
    nextAct->setShortcuts(QKeySequence::NextChild);
    nextAct->setStatusTip(tr("Move the focus to the next window"));
    //connect(nextAct, &QAction::triggered, mdiArea, &QMdiArea::activateNextSubWindow);

    previousAct = new QAction(tr("Pre&vious"), this);
    previousAct->setShortcuts(QKeySequence::PreviousChild);
    previousAct->setStatusTip(tr("Move the focus to the previous "
                                 "window"));
   // connect(previousAct, &QAction::triggered, mdiArea, &QMdiArea::activatePreviousSubWindow);

    windowMenuSeparatorAct = new QAction(this);
    windowMenuSeparatorAct->setSeparator(true);

   // updateWindowMenu();

    menuBar()->addSeparator();

    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));

    //QAction *aboutAct = helpMenu->addAction(tr("&About"), this, &MainWindow::about);
   // aboutAct->setStatusTip(tr("Show the application's About box"));
}
