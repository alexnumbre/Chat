/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtWidgets>

#include "dialog_connect.h"

DialogConnect::DialogConnect(ChatLN chat)
{
    if (!chat) {
        chat = std::make_shared<Chat>();
    };

    this->chat = chat;

    setWindowTitle(tr("Database connection"));

    createFormConfDB();
    createButtonLine();

    boxDialogButtons = new QDialogButtonBox(QDialogButtonBox::Ok);
    connect(boxDialogButtons, &QDialogButtonBox::accepted, this, &DialogConnect::accept);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(formConfDB);
    mainLayout->addWidget(formButtonLine);
    mainLayout->addWidget(boxDialogButtons);
    setLayout(mainLayout);

    push(chat->conf());
    status();
};

DialogConnect::~DialogConnect(){}; // @ERR: default destructor crashed on ~DB::Conf

void DialogConnect::createFormConfDB()
{
    formConfDB = new QGroupBox(tr("Status line"));

    markHost = new QLabel(tr("Host name : "));
    editHost = new QLineEdit();

    markUser = new QLabel(tr("User name : "));
    editUser = new QLineEdit();

    markPass = new QLabel(tr("Password : "));
    editPass = new QLineEdit();

    markName = new QLabel(tr("Database : "));
    editName = new QLineEdit();

    markPort = new QLabel(tr("Port : "));
    editPort = new QSpinBox();
    editPort->setRange(1, 65535);

    markSock = new QLabel(tr("Socket : "));
    editSock = new QLineEdit();

    markFlag = new QLabel(tr("Client flag : "));
    editFlag = new QSpinBox();
    editFlag->setRange(0, 1);

    QFormLayout *layout = new QFormLayout;
    layout->addRow(markHost, editHost);
    layout->addRow(markPort, editPort);
    layout->addRow(markSock, editSock);
    layout->addRow(markFlag, editFlag);
    layout->addRow(markName, editName);
    layout->addRow(markUser, editUser);
    layout->addRow(markPass, editPass);

    formConfDB->setLayout(layout);
}

void DialogConnect::push(DB::Conf conf)
{
    editHost->setText(QString(conf.host.c_str()));
    editUser->setText(QString(conf.user.c_str()));
    editPass->setText(QString(conf.passwd.c_str()));
    editName->setText(QString(conf.db.c_str()));
    editPort->setValue(conf.port);
    editSock->setText(QString(conf.unix_socket.c_str()));
    editFlag->setValue(conf.clientflag);
};

auto DialogConnect::pull() -> DB::Conf
{
    auto conf = DB::Conf();

    conf.host = (editHost->text()).toStdString();
    conf.user = (editUser->text()).toStdString();
    conf.passwd = (editPass->text()).toStdString();
    conf.db = (editName->text()).toStdString();
    conf.port = editPort->value();
    conf.unix_socket = (editSock->text()).toStdString();
    conf.clientflag = editFlag->value();

    return conf;
};

void DialogConnect::createButtonLine()
{
    formButtonLine = new QGroupBox();

    auto *layout = new QHBoxLayout;

    butReset = new QPushButton(tr("Reset"));
    butApply = new QPushButton(tr("Apply"));

    layout->addWidget(butReset);
    layout->addWidget(butApply);

    connect(butApply, &QPushButton::clicked, this, &DialogConnect::apply);
    connect(butReset, &QPushButton::clicked, this, &DialogConnect::reset);

    formButtonLine->setLayout(layout);
};

void DialogConnect::apply()
{
    chat->disconnect();
    chat->setup(pull());
    chat->connect();
    status();
};

void DialogConnect::reset()
{
    push(DB::Conf());
    chat->disconnect();
    chat->setup(pull());
    status();
};

void DialogConnect::status()
{
    formConfDB->setTitle(QString("Connection status : %1").arg(chat->status()));
};
