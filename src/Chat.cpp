#include "Chat.h"

#include <memory>
#include <iostream>


auto Chat::login(const std::string& login, const std::string& password) -> ID
{
	logout();

    auto id = usersID(login);

    if (usersVerify(id, password)) {
        _active = id;
    } else {
        id = nullptr;
    };

    return id;

};


void Chat::logout() { _active = nullptr; };


auto Chat::activeName() -> Single { return userName(_active); };


auto Chat::activeID() -> ID { return  _active; };


long Chat::unread() { return recnumInbox(_active); };


auto Chat::send(const std::string& text, const ID target) -> ID
{
	ID msg = nullptr;

	if (_active) {
		msg = msgNew(_active, text);
		inboxPush(target, msg);
	};

	return msg;
};


auto Chat::send(const std::string& text) -> ID
{
	ID msg = nullptr;

	if (_active) {
		msg = msgNew(_active, text);
		auto userlist = usersID();
		if (userlist) {
            for (auto target : (*(userlist))) {
                ID ptarget = std::make_shared<std::string>(target);
                inboxPush(ptarget, msg);
            };
		} else {
		    std::cout << "# ERR: NULL userlist" << std::endl;
		;}
	};

	return msg;

};


auto Chat::take() -> MSG
{

	MSG msg{ nullptr };

	auto id = inboxPull(_active);

	if (id) {

		auto sender = msgSender(id);
		auto text = msgText(id);

		if ((sender != nullptr) && (text != nullptr)) {
			msg = std::make_shared<Message>();
			msg->sender = sender;
			msg->text = *text;
		};

	};

	return msg;
};
