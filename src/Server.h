#pragma once

#include "DB.h"

#include <vector>
#include <memory>
#include <string>

	//! Сервер базы данных
	/*!
	* Задает структуру БД и логику работы с ней.
	* Предоставляет общие для всех пользователей интерфейсы работы с БД.
	*/
class Server : public DB {


public:

    typedef Single ID;

    Server();
    ~Server();

    bool connect();

    void stop();
    bool start();
    bool drop();

    void demo();


    auto usersID(const std::string &name) -> ID;
    auto usersID() -> Multi;

    bool usersRegist(const std::string &name, const std::string &passwd);
    bool usersExists(const std::string &name);
    bool usersVerify(ID user, const std::string& passwd);

    auto userName(ID user) -> Single;
    auto userInbox(ID user) -> ID;
    long userUnread(ID user);

    auto msgNew(ID sender, const std::string &text) -> ID;
    auto msgText(ID msg) -> Single;
    auto msgSender(ID msg) -> ID;

    bool inboxPush(ID reader, ID mesg);
    auto inboxPull(ID reader) -> ID;

    void printStat();
    auto getStat() -> Single;

    bool trash();

    long recnumUsers();
    long recnumInbox(ID user);

private:

    bool initDB();
    bool createTables();

};
