#include "DB.h"

#include <iostream>
#include <map>

DB::DB(){

};

DB::DB(const DB::Conf &conf)
{
    setup(conf);
};

DB::~DB() {
	disconnect();
};


auto DB::conf() -> DB::Conf { return _conf; }


void DB::setup(const DB::Conf& conf) { _conf = conf; };


bool DB::status() { return (_connect != nullptr); };


void DB::reset() {
	disconnect();
	_conf = Conf();
};

void DB::disconnect()
{
    if (_connect) {
        mysql_close(_connect);
        _connect = nullptr;
    };
};

bool DB::reconnect() {
	disconnect();
	connect();
	return status();
};


bool DB::connect() {

	if (!_connect) {

		_connect = mysql_init(0);

		if (FLAG_PRINT_QUERY) std::cout << "# DB connect database : " << _conf.db.c_str() << std::endl;

		_connect = mysql_real_connect(_connect,
			_conf.host.c_str(),
			_conf.user.c_str(),
			_conf.passwd.c_str(),
			_conf.db.c_str(),
			_conf.port,
			_conf.unix_socket.c_str(),
			_conf.clientflag);

	};

	if (_connect) {
		std::cout << "# DB connection to database : " << _connect << std::endl;
		mysql_set_character_set(_connect, "utf8");
		std::cout << "# DB database charset: " << mysql_character_set_name(_connect) << std::endl;
	}
	else {
		std::cout << "# DB connection to database " << _conf.db.c_str() << " failed: " << mysql_error(_connect) << std::endl;
	};

	return status();

};


auto DB::error() -> const char* { return mysql_error(_connect); };


auto DB::result(MYSQL* connect) -> MYSQL_RES* {return (connect ? mysql_store_result(connect) : nullptr);};


auto DB::retrieve() -> Table {

	Table respond = std::make_shared<StrVec2>();

	if (auto res = result(_connect)) {

		int columns = mysql_num_fields(res);

		std::vector<std::string> rowdat;
		rowdat.resize(columns);

		int i{ 0 };
		while (auto row = mysql_fetch_row(res)) {

			++i;

			for (int j = 0; j < columns; ++j) {
				rowdat[j] = std::string(row[j]);
			};

			respond->push_back(rowdat);

		};

		if (FLAG_PRINT_QUERY) std::cout << "# respond->size = " << respond->size() << std::endl;

		respond->resize(respond->size());

	};

	return respond;

};


auto DB::respond(const std::string& request) -> Table {
	query(request);
	return retrieve();
};


bool DB::query(const char* request) {

	if (FLAG_PRINT_QUERY) std::cout << std::endl << std::endl << "# DB> " << request << std::endl;

	bool state = (status() ? (mysql_query(_connect, request) == 0) : false);;

	if (FLAG_PRINT_QUERY) std::cout << "# DB< " << state << std::endl;

	return state;
};


bool DB::query(const std::string& request) {
	return query(request.c_str());
};


void DB::print(const char* request) {

	std::cout << std::endl << "# Print DB request: " << request << std::endl;

	if (auto out = query(request)) {

		auto res = retrieve();
		if (res->size()) {

			std::cout << "# DB respond size = " << res->size() << std::endl;
			printRespond(res);

		}
		else {
			std::cout << "# DB respond NULL" << std::endl;
		};

	}
	else {
		std::cout << "# DB query failed" << std::endl;
	};

};


void DB::printRespond(Table respond) {

	int i = 0;

	for (auto row : (*respond)) {

		std::cout << "#  " << ++i << ": ";

		for (auto col : row) {
			std::cout << " | " << col;
		}

		std::cout << std::endl;

	};
};


void DB::printStatErr() {
	std::cout << "# DB status = " << status() << " ; Err = " << error() << std::endl;
};



auto DB::respondSingle(const std::string& request) -> Single {
	query(request);
	return retrieveSingle();
};


auto DB::respondMulti(const std::string& request) -> Multi {

	Multi lst{ nullptr };

	auto table = respond(request);

	if (table) {

		lst = std::make_shared<StrVec1>();

		for (auto row : (*table)) {
			if (row.size() > 0) lst->push_back(row[0]);
		};

	};

	return lst;

};


auto DB::retrieveSingle() -> Single {

	Single value{ nullptr };

	if (auto res = result(_connect)) {

		if (mysql_num_fields(res) > 0) {
			auto row = mysql_fetch_row(res);
			value = std::make_shared<std::string>(row[0]);
		};

	};

	return value;

};


auto DB::lastInsertID() -> Single {
	return respondSingle(std::string("SELECT LAST_INSERT_ID()"));
};


long DB::recnum(const char* tab) {
	long num{ -1 };
	auto Single = respondSingle(std::string("SELECT COUNT(*) FROM " + std::string(tab)));
	if (Single) num = std::stoi(*Single);
	return num;

};


auto DB::listID(const char* tab) -> Multi {
	return respondMulti("SELECT id FROM " + std::string(tab));
};


