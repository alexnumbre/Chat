#include <QtWidgets>

#include "window.h"


void MainWindow::createMenu()
{
    createMenuFile();
    menuBar()->addSeparator();
    createMenuDB();
    menuBar()->addSeparator();
    createMenuUser();
    menuBar()->addSeparator();
    createMenuHelp();

}

void MainWindow::createMenuFile() {

    auto menuFile = menuBar()->addMenu(tr("&File"));

    const QIcon iconExit = QIcon::fromTheme("application-exit");
    QAction *actExit = menuFile->addAction(iconExit, tr("E&xit"), this, &QWidget::close);
    actExit->setShortcuts(QKeySequence::Quit);
    actExit->setStatusTip(tr("Exit the application"));
};

void MainWindow::createMenuDB()
{
    auto *menuDB = menuBar()->addMenu(tr("&DB"));

    QAction *actConnect = menuDB->addAction(tr("&Connect"), this, &MainWindow::connectdb);
    actConnect->setStatusTip(tr("Establish database connection"));

    QAction *actDisconnect = menuDB->addAction(tr("&Disconnect"), this, &MainWindow::disconnectdb);
    actDisconnect->setStatusTip(tr("Drop database connection"));
};

void MainWindow::createMenuUser()
{
    auto *menuUser = menuBar()->addMenu(tr("&User"));

    QAction *actAccount = menuUser->addAction(tr("&Account"), this, &MainWindow::account);
    actAccount->setStatusTip(tr("User account"));
};

void MainWindow::createMenuHelp() {

    auto *menuHelp = menuBar()->addMenu(tr("&Help"));

    QAction *actAbout = menuHelp->addAction(tr("&About"), this, &MainWindow::about);
    actAbout->setStatusTip(tr("Show some info about application"));
};
