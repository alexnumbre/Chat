#pragma once


#include "Server.h"

#include <map>
#include <memory>
#include <string>
#include <vector>

//! Клиент базы данных
/*!
* Обеспечивает работу с БД отдельного пользователя.
*/
class Chat : public Server
{
public:
    struct Message
    {
        ID sender;
        std::string text;
    };

    typedef std::shared_ptr<Message> MSG;

    auto login(const std::string& login, const std::string& password) -> ID;
	void logout();

	auto send(const std::string& text, const ID target) -> ID;
	auto send(const std::string& text) -> ID;
	auto take() -> MSG;
	long unread();

    auto activeName() -> Single;
	auto activeID() -> ID;

private:
	ID _active {nullptr};
};

typedef std::shared_ptr<Chat> ChatLN;
