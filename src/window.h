#pragma once

#include <QMainWindow>
#include "Chat.h"

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QTextEdit;
class QPlainTextEdit;
class QSplitter;
class QTableWidget;
class QGroupBox;
class QMenu;
class QMenuBar;
class QDialogButtonBox;
class QTableWidgetItem;
class QCommandLinkButton;
class QLabel;
QT_END_NAMESPACE

//! Главное окно программы
/*!
	Задает интерфейс и логику работы программы.
	*/
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    void setChatServer(ChatLN chat);
    void update();

private slots:
    void about();
    void connectdb();
    void disconnectdb();
    void account();
    void send();
    void read();
    void go();
    void ontime();

private:
    void makeInterface();

    void createWorkspace();
    void createStatusBar();

    void createMenu();
    void createMenuFile();
    void createMenuDB();
    void createMenuUser();
    void createMenuHelp();

    void createBoxUsers();
    void createBoxMesgs();
    void createBoxChat();
    void createBoxEdit();
    void createBoxSend();
    void createBoxConns();
    void createBoxLogin();
    void createTimer();

    void updateUserList();
    void updateChat();

    QMenuBar *menubar;
    QMenu *menuFile;

    QSplitter *splithWorkspace;
    QSplitter *splitvMesgsManager;
    QTableWidget *tabwUsertab;
    QTextEdit *textChat;
    QPlainTextEdit *textpCompose;

    QGroupBox *boxWorkspace;
    QGroupBox *boxMesgs;
    QGroupBox *boxUsers;
    QGroupBox *boxEdit;
    QGroupBox *boxChat;
    QGroupBox *boxSend;

    QCommandLinkButton *cmdSend;

    auto rowUser(const std::string &id) -> QTableWidgetItem *;

    ChatLN chat{nullptr};
};
