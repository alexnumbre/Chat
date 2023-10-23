
#include <QtWidgets>

#include "window.h"


MainWindow::MainWindow()
{
    makeInterface();
}

void MainWindow::setChatServer(ChatLN chat)
{
    this->chat = chat;
};

void MainWindow::makeInterface()
{
    createMenu();
    createStatusBar();
    createWorkspace();
    setCentralWidget(boxWorkspace);
    chat = std::make_shared<Chat>();
    createTimer();
};

void MainWindow::createWorkspace()
{

    boxWorkspace = new QGroupBox("Workspace");
    auto *layout = new QVBoxLayout;

    createBoxUsers();
    createBoxMesgs();

    splithWorkspace = new QSplitter();
    splithWorkspace->setOrientation(Qt::Horizontal);
    splithWorkspace->addWidget(boxUsers);
    splithWorkspace->addWidget(boxMesgs);

    layout->addWidget(splithWorkspace);

    boxWorkspace->setLayout(layout);


}


void MainWindow::createBoxUsers() {

    boxUsers = new QGroupBox("Users");
    auto *layout = new QVBoxLayout;

    tabwUsertab = new QTableWidget;
    auto header = QStringList() << tr("id") << tr("userlist:");

    tabwUsertab->setColumnCount(2);
    tabwUsertab->setHorizontalHeaderLabels(header);
    tabwUsertab->setShowGrid(false);
    tabwUsertab->setSelectionMode(QAbstractItemView::MultiSelection);
    tabwUsertab->setSelectionBehavior(QAbstractItemView::SelectRows);
    tabwUsertab->setEditTriggers(QAbstractItemView::NoEditTriggers);

    tabwUsertab->hideColumn(0);
    tabwUsertab->horizontalHeader()->setStretchLastSection(true);

    layout->addWidget(tabwUsertab);

    boxUsers->setLayout(layout);
}

void MainWindow::createBoxMesgs() {

    boxMesgs = new QGroupBox("Messages");
    auto *layout = new QVBoxLayout;

    createBoxChat();
    createBoxEdit();

    splitvMesgsManager = new QSplitter(Qt::Vertical);
    splitvMesgsManager->addWidget(boxChat);
    splitvMesgsManager->addWidget(boxEdit);

    layout->addWidget(splitvMesgsManager);

    boxMesgs->setLayout(layout);
}


void MainWindow::createBoxChat() {
    boxChat = new QGroupBox("Chat");
    auto *layout = new QVBoxLayout;

    textChat = new QTextEdit();

    layout->addWidget(textChat);

    boxChat->setLayout(layout);
};


void MainWindow::createBoxEdit() {
    boxEdit = new QGroupBox("Edit");
    auto *layout = new QVBoxLayout;

    createBoxSend();
    textpCompose = new QPlainTextEdit();
    textpCompose->insertPlainText("Hi!");

    layout->addWidget(textpCompose);
    layout->addWidget(boxSend);

    boxEdit->setLayout(layout);
};

void MainWindow::createBoxSend()
{
    boxSend = new QGroupBox("Send");
    cmdSend = new QCommandLinkButton();

    connect(cmdSend, SIGNAL(clicked()), this, SLOT(go()));

    auto *layout = new QHBoxLayout;
    layout->addWidget(cmdSend);

    boxSend->setLayout(layout);
};

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Status: start"), 5000);
}

void MainWindow::createTimer()
{
    auto timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(ontime()));
    timer->start(3000);
}
