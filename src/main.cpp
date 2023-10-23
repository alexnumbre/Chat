#include <QApplication>

#include "Chat.h"
#include "window.h"

#include <memory>

auto chat() -> std::shared_ptr<Chat>;
auto reconfDB() -> DB::Conf;

int main(int argc, char *argv[])
{

    QApplication app(argc, argv);

    QCoreApplication::setOrganizationName("CPLUS-27");
    QCoreApplication::setApplicationName("Chat (Module 36 - Qt GUI)");
    QCoreApplication::setApplicationVersion("0.0.9");

    MainWindow winMain;
    winMain.show();

    auto srv = chat();
    winMain.setChatServer(srv);
    winMain.update();

    return app.exec();
}

auto chat() -> std::shared_ptr<Chat>
{
    auto chat = std::make_shared<Chat>();
    auto conf = reconfDB();

    chat->setup(conf);
    chat->usersRegist(std::string("test"), std::string("test"));

    return chat;
};

//Quick hardcode DB connection reconfiguration
auto reconfDB() -> DB::Conf
{
    DB::Conf conf;

    conf.host = "127.0.0.1";
    conf.user = "root";
    conf.passwd = "root";
    conf.db = "test";
    conf.port = 3306;
    conf.unix_socket = "NULL";
    conf.clientflag = 0;

    return conf;
};

/*! @mainpage  GUI чат

Проект курса C++ на платформе SkillFactory.
Модуль #36 Графический пользовательский интерфейс на Qt


@section Задание

Напишите ваш вариант и сдайте код в виде проекта.


@subsection Отчет

В качестве итогового решения поставленной задачи каждый участник
* прикладывает в форму ниже ссылку на репозиторий итогового проекта в GitHub
* с обязательным описанием проекта в Readme.

В Readme должна быть указана следующая информация:
- список участников команды;
- имя тимлида (по желанию);
- описание выбранной идеи решения;
- описание пользовательских типов и функций в проекте;
- пояснение, как были распределены задачи в команде (кто какую часть проекта реализовывал).



@subsection Оценка

Критерии по баллам для проекта функционального мессенджера с использованием Qt, C++, Postgres следующие:

1. Реализация GUI для серверной части приложения (от 0 до 10 баллов):
Наличие окна просмотра всех сообщений и списка пользователей (5 баллов)
Возможность отключения и бана пользователей (5 баллов)

2. Изменения и дополнения к интерфейсу клиентской части приложения (от 0 до 10 баллов):
Удобство интерфейса (5 баллов)
Дизайн интерфейса (5 баллов)

3. Удобство приложения (от 0 до 10 баллов):
Функциональность и работоспособность (5 баллов)
Производительность и стабильность работы (5 баллов)
Итого, максимальное количество баллов за проект - 30.

@section Решение


@subsection Реализация

Выбран клиент-серверный подход.

Функции сервера выполняет сервер БД MySQL - должен работать с настроенными правами доступа.
Сервер предоставляет возможности БД для хранения данных чата.

Функции клиента выполняет программа чата.
Клиент реализует подключение к серверу БД, задает структуру БД.
Для обмена сообщениями между пользователями каждый клиент создает в БД соответствующие записи.

Структура БД:
- users: таблица данных пользователей
- mesgs: таблица текстовых сообщений
- inbox* : таблицы входящих (непрочитанных) сообщений пользователй.





@section Зависимости


@subsection MySQL

Для работы программы необходим доступ к MySQL Server.
По умолчанию программа настроена на подключение к тестовой базе данных (схеме) 'test',
которая должна быть создана на сервере заранее сторонними средствами.
При успешном подключении к тестовой базе данных, программа создает свою базу данных (схему)
 и далее работает с ней.
Настройки подключения к серверу MySQL по умолчанию заданы структурой DB::Conf:
    struct Conf {
        std::string host = "127.0.0.1";
        std::string user = "root";
        std::string passwd = "root";
        std::string db = "test";
        unsigned int port = 3306;
        std::string unix_socket = "NULL";
        long clientflag = 0;
    };


@subsection Qt

Использовано Qt-5



@section Прочее


@subsection Команда

Задание выполнено самостоятельно.


@subsection Разработка

ОС - ArchLinux x64.

Среда разработки - Qt Creator.

Контроль версий - git.


@subsection КлючевыеПонятия

Chat (Чат) - клиент БД для работы с конкретным пользователем

Server (Сервер) - сервер БД для работы без привязки к конкретному пользователю

*/
