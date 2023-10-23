
#include <QtWidgets>

#include "dialog_account.h"

DialogAccount::DialogAccount(ChatLN chat)
{
    this->chat = chat;

    setWindowTitle(tr("Account specification"));

    createFormAccount();
    createButtonLine();

    boxDialogButtons = new QDialogButtonBox(QDialogButtonBox::Ok);
    connect(boxDialogButtons, &QDialogButtonBox::accepted, this, &DialogAccount::accept);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(formAccount);
    layout->addWidget(formButtons);
    layout->addWidget(boxDialogButtons);
    setLayout(layout);

    status();
};

DialogAccount::~DialogAccount(){}; // @ERR: default destructor crashed on ~DB::Conf

void DialogAccount::createFormAccount()
{
    formAccount = new QGroupBox(tr("Active user: <unknown>"));

    markName = new QLabel(tr("Username : "));
    editName = new QLineEdit();

    markPass = new QLabel(tr("Password : "));
    editPass = new QLineEdit();

    QFormLayout *layout = new QFormLayout;

    layout->addRow(markName, editName);
    layout->addRow(markPass, editPass);

    formAccount->setLayout(layout);
}

void DialogAccount::createButtonLine()
{
    formButtons = new QGroupBox();

    auto *layout = new QHBoxLayout;

    butLogout = new QPushButton(tr("Logout"));
    butRegist = new QPushButton(tr("Register"));
    butLogin = new QPushButton(tr("Login"));

    layout->addWidget(butLogout);
    layout->addWidget(butRegist);
    layout->addWidget(butLogin);

    connect(butLogout, &QPushButton::clicked, this, &DialogAccount::logout);
    connect(butRegist, &QPushButton::clicked, this, &DialogAccount::regist);
    connect(butLogin, &QPushButton::clicked, this, &DialogAccount::login);

    formButtons->setLayout(layout);
};

void DialogAccount::logout()
{
    if (chat) {
        chat->logout();
    };

    status();
};

void DialogAccount::login()
{
    if (chat) {
        auto username = (editName->text()).toStdString();

        if (chat->usersExists(username)) {
            if (chat->login(username, (editPass->text()).toStdString())) {
                QMessageBox::information(this,
                                         tr("Login success"),
                                         tr("User '%1' login!").arg(username.c_str()));
            } else {
                QMessageBox::warning(this,
                                     tr("Login failure"),
                                     tr("User '%1' password is wrong!!").arg(username.c_str()));
            };
        } else {
            QMessageBox::warning(this,
                                 tr("Login failure"),
                                 tr("User '%1' isn't registered!").arg(username.c_str()));
        };
    };
    status();
};

void DialogAccount::regist()
{
    if (chat) {
        auto username = (editName->text()).toStdString();

        if (!chat->usersExists(username)) {
            if (chat->usersRegist(username, (editPass->text()).toStdString())) {
                QMessageBox::information(this,
                                         tr("Registration success"),
                                         tr("User '%1' is registered!").arg(username.c_str()));
            } else {
                QMessageBox::warning(this,
                                     tr("Registration failure"),
                                     tr("User '%1' registration rejected!").arg(username.c_str()));
            };
        } else {
            QMessageBox::warning(this,
                                 tr("Registration failure"),
                                 tr("User '%1' is already registered!").arg(username.c_str()));
        };
    };
    status();
};

bool DialogAccount::status()
{
    bool flag{false};

    if (!chat) {
        QMessageBox::warning(this, tr("Account failure"), tr("Chat client isn't running!"));
    } else if (!(chat->status())) {
        QMessageBox::warning(this, tr("Account failure"), tr("Database is disconnected!"));
    } else {
        clarifyUserStatus();
    };

    return flag;
};

void DialogAccount::clarifyUserStatus()
{
    if (chat) {
        auto username = chat->activeName();
        if (username) {
            formAccount->setTitle(QString("Active user : %1").arg(username->c_str()));
        } else {
            formAccount->setTitle("No active user");
        };
    } else {
        formAccount->setTitle("...");
    }
}
