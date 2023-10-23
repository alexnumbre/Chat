#pragma once

#include <iostream>
#include <memory>
#include <mysql/mysql.h>
#include <string>
#include <vector>

//! База данных
/*!
* Интерфейс работы с базой данных
*/
class DB {

public:

	bool FLAG_PRINT_QUERY = false;

	typedef std::shared_ptr<std::string> Single;

	typedef std::vector<std::string> StrVec1;
	typedef std::shared_ptr<StrVec1> Multi;

	typedef std::vector<StrVec1> StrVec2;
	typedef std::shared_ptr<StrVec2> Table;

    struct Conf
    {
        std::string host = "127.0.0.1";
        std::string user = "root";
        std::string passwd = "root";
		std::string db = "test";
		unsigned int port = 3306;
		std::string unix_socket = "NULL";
		long clientflag = 0;
    };

    DB();
	DB(const DB::Conf& conf);
    ~DB();

    auto conf() -> DB::Conf;
	void setup(const DB::Conf& conf);
	void reset();

	bool status();

	bool connect();
	bool reconnect();
	void disconnect();

	auto result(MYSQL* connect) -> MYSQL_RES*;

	bool query(const char* request);
	bool query(const std::string& request);

	auto error() -> const char*;

	auto retrieve() -> Table;
	auto retrieveSingle() -> Single;

	auto respond(const std::string& request) -> Table;
	auto respondMulti(const std::string& request) -> Multi;
	auto respondSingle(const std::string& request) -> Single;

	void print(const char* request);
	void printRespond(Table respond);
	void printStatErr();

	auto lastInsertID() -> Single;

	long recnum(const char* tab);
	auto listID(const char* tab) -> Multi;

private:

	Conf _conf{};
	MYSQL* _connect{ nullptr };

};
