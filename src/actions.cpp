#include <QtWidgets>

#include "dialog_account.h"
#include "dialog_connect.h"
#include "window.h"

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Application"),
                       tr("Skill Factory CPLUS-27 Module 36 project"));
}

void MainWindow::connectdb()
{
    DialogConnect dia(chat);
    dia.exec();
}

void MainWindow::disconnectdb()
{
    chat->logout();
    chat->disconnect();
}

void MainWindow::account()
{
    DialogAccount dia(chat);
    dia.exec();
    auto user = chat->activeName();
    if (user) {
        boxUsers->setTitle(tr("Active user: %1").arg(user->c_str()));
    }
}

void MainWindow::go()
{
    if (!chat->status()) {
        connectdb();
    };

    if (chat->status()) {
        if (!chat->activeID()) {
            account();
        }
    }

    if (chat->activeID()) {
        send();
    };

    update();
}

void MainWindow::send()
{
    auto text = textpCompose->toPlainText();
    if (text.size() < 1) {
        statusBar()->showMessage(tr("Status: nothing to send (empty message)"), 3000);
        return;
    };

    auto selection = tabwUsertab->selectedItems();
    if (selection.size() < 1) {
        statusBar()->showMessage(tr("Status: nobody there (empty userlist selection)"), 3000);
        return;
    }

    for (auto item : selection) {
        auto id = tabwUsertab->item(item->row(), 0);
        Chat::ID target = std::make_shared<std::string>(id->text().toStdString());
        auto msg = chat->send(text.toStdString(), target);
        QString to = "\n***\n>> ";
        to += item->text();
        textChat->append(to);
        textChat->append(text);
        statusBar()->showMessage(tr("Send message [%1] to '%2'").arg(msg->c_str(), item->text()),
                                 3000);
    }

    textpCompose->clear();
};

void MainWindow::read()
{
    for (int i = 0; i < chat->unread(); ++i) {
        auto msg = chat->take();
        if (msg) {
            auto id = msg->sender;
            auto sender = chat->userName(id);
            QString from = "\n***\n<< ";
            from += sender->c_str();
            textChat->append(from);
            textChat->append(msg->text.c_str());
        }
    }
}

void MainWindow::update()
{
    boxWorkspace->setTitle(chat->getStat()->c_str());
    updateUserList();
    updateChat();
}

void MainWindow::updateUserList()
{

    auto usersID = chat->usersID();

    auto row = tabwUsertab->rowCount();

    for (auto userid : (*usersID)) {
        if (rowUser(userid) == nullptr) {
            auto username = chat->userName(std::make_shared<std::string>(userid));
            if (username) {
                tabwUsertab->insertRow(row);
                auto *cellID = new QTableWidgetItem(tr(userid.c_str()));
                auto *cellName = new QTableWidgetItem(tr(username->c_str()));
                tabwUsertab->setItem(row, 0, cellID);
                tabwUsertab->setItem(row, 1, cellName);
                ++row;
            };
        };
    };

    tabwUsertab->resizeColumnsToContents();
};

void MainWindow::updateChat()
{
    read();
};

auto MainWindow::rowUser(const std::string &id) -> QTableWidgetItem *
{
    QTableWidgetItem *item{nullptr};

    for (long i = 0; i < tabwUsertab->rowCount(); ++i) {
        item = tabwUsertab->item(i, 0);
        if (item) {
            if (item->text().toStdString() == id) {
                break;
            } else {
                item = nullptr;
            }
        };
    };

    return item;
};

void MainWindow::ontime()
{
    update();
};
