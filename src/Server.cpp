#include "Server.h"

#include <iostream>
#include <map>
#include <memory>


Server::Server() {
    start();
};

Server::~Server() {
    stop();
};


bool Server::connect() {
    return start();
};


void Server::stop() {disconnect();};


bool Server::start() {

    bool state = DB::connect();;
    printStatErr();

    if (state) state = initDB();
    if (! state) stop();

    return state;

};


bool Server::drop() {
    return (status() ? query("DROP DATABASE IF EXISTS db_chat") : false);
};


bool Server::initDB() {

    bool state = false;

    state = query("CREATE DATABASE IF NOT EXISTS db_chat");

    if (state) state = query("USE db_chat");
    if (state) state = createTables();

    return state;

};


bool Server::createTables() {

    bool state = status();

    if (state) state = query("CREATE TABLE IF NOT EXISTS users(id INT AUTO_INCREMENT PRIMARY KEY, name VARCHAR(255) NOT NULL, passwd VARCHAR(255) NOT NULL, inbox VARCHAR(255), UNIQUE (name))");
    if (state) state = query("CREATE TABLE IF NOT EXISTS mesgs(id INT AUTO_INCREMENT PRIMARY KEY, time TIMESTAMP DEFAULT CURRENT_TIMESTAMP, sender INT, wait INT NOT NULL, text TEXT, FOREIGN KEY (sender) REFERENCES users(id))");
    if (state) state = query("CREATE TABLE IF NOT EXISTS trash(id INT PRIMARY KEY, FOREIGN KEY (id) REFERENCES mesgs(id))");

    return state;

};




bool Server::usersRegist(const std::string& name, const std::string& passwd) {

    bool state = status();

    std::string request {};
    Single id {};

    if (state) state = (! usersExists(name));

    request = ("INSERT INTO users(id, name, passwd, inbox) values(default, '" +
                name +     "', '" + passwd + "', " + "'trash'" + ")");
    if (state) state = query(request.c_str());

    if (state) id = usersID(name);

    if (id) {

        std::string inbox = "inbox_" + (*id);

        request = ("CREATE TABLE " +  inbox + " AS SELECT * FROM trash");
        state = query(request.c_str());

        if (state) request = ("UPDATE users SET inbox = '" + inbox + "' WHERE id = '" + (*id) + "'");
        state = query(request.c_str());

    };

    return state;

};


bool Server::usersExists(const std::string &name) {
    return (usersID(name) != nullptr);
};

bool Server::usersVerify(ID user, const std::string& passwd) {
    bool verified {false};
    Single pass {nullptr};
    if (user) pass = respondSingle(std::string("SELECT passwd FROM users WHERE id='" + *user + "'"));
    if (pass) verified = ((*pass) == passwd);
    return verified;
};



auto Server::usersID() -> Multi {return listID("users");};

auto Server::usersID(const std::string &name) -> Single {

    Single id {nullptr};

    if (status()) {

        auto res = respond("SELECT id FROM users WHERE name = '" + name + "'");

        if (res->size() > 0 ) {
            id = std::make_shared<std::string>();
            (*id) = ((*res)[0])[0];
        };

    };

    return id;

};


auto Server::msgNew(ID sender, const std::string &text) -> ID {

    ID mesg = nullptr;
    std::string request {};

    request = ("INSERT INTO mesgs(id, time, sender, wait, text) values(default, default, '" +
                *sender + "', 0, '" + text + "')");

    if (query(request.c_str())) mesg = lastInsertID();

    return mesg;

};



auto Server::msgText(ID msg) -> Single {return (msg ? respondSingle("SELECT text FROM mesgs WHERE id = '" + *msg + "'") : nullptr);};


auto Server::msgSender(ID msg) -> ID {return (msg ? respondSingle("SELECT sender FROM mesgs WHERE id = '" + *msg + "'") : nullptr);};



auto Server::userInbox(ID user) -> ID {
    return (user ? respondSingle("SELECT inbox FROM users WHERE id = '" + *user + "'") : nullptr);
;};


auto Server::userName(ID user) -> Single {
    return (user ? respondSingle("SELECT name FROM users WHERE id = '" + *user + "'") : nullptr);
;};


long Server::userUnread(ID user) {
    return recnumInbox(user);
};


bool Server::inboxPush(ID reader, ID mesg) {

    std::string request {};

    if (reader == nullptr) std::cout << "# reader == nullptr" << std::endl;
    if (mesg == nullptr) std::cout << "# mesg == nullptr" << std::endl;

    bool state = (reader && mesg);

    if (state) {
        auto inbox = userInbox(reader);
        state = (inbox != nullptr);
        if (state) state = query("INSERT INTO " + *inbox + "(id) values ('" + *mesg + "')");
        if (state) state = query("UPDATE mesgs SET wait += 1 WHERE id = '" + *mesg + "')");
    };

    return (state);

};


auto Server::inboxPull(ID reader) -> ID {

    ID mesg {nullptr};

    auto inbox = userInbox(reader);

    if (inbox) {
        mesg = respondSingle("SELECT id FROM " + *inbox + " ORDER BY id LIMIT 1");
        if (mesg) {
            query("UPDATE mesgs SET wait -= 1 WHERE id = '" + *mesg + "'");
            query("DELETE FROM " + *inbox + " WHERE id = '" + *mesg + "'");
        };
    };

    return mesg;
};

auto Server::getStat() -> Single
{
    auto pstr = std::make_shared<std::string>();

    std::map<std::string, long> stat;

    stat["users"] = recnum("users");
    stat["mesgs"] = recnum("mesgs");
    stat["trash"] = recnum("trash");

    std::string conn = (status() ? "OK" : "NA");

    *pstr += "# Server status [" + conn + "] : ";
    *pstr += std::to_string(stat["users"]) + " users, ";
    *pstr += std::to_string(stat["mesgs"]) + " mesgs, ";
    *pstr += std::to_string(stat["trash"]) + " trash; ";

    return pstr;
};

void Server::printStat()
{
    std::map<std::string, long> stat;

    stat["users"] = recnum("users");
    stat["mesgs"] = recnum("mesgs");
    stat["trash"] = recnum("trash");

    std::string conn = (status() ? "OK" : "NA");


    std::cout << "# Server stats [" <<  conn << "] : "  <<
         stat["users"] << " users, " <<
         stat["mesgs"] << " mesgs, " <<
         stat["trash"] << " trash; " <<
    std::endl;
};

bool Server::trash()
{
    return query("INSERT INTO trash SELECT id FROM mesgs WHERE wait<1");
};

long Server::recnumUsers()
{
    return recnum("users");
};

long Server::recnumInbox(ID user) {
    long num {-1};
    auto inbox = userInbox(user);
    if (inbox) num = recnum(inbox->c_str());
    return num;
};



