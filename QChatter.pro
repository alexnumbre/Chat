QT += widgets

HEADERS       = src/window.h \
                src/DB.h \
                src/Server.h \
                src/Chat.h \
                src/dialog_account.h \
                src/dialog_connect.h

SOURCES       = src/window.cpp\
                src/actions.cpp \
                src/dialog_account.cpp \
                src/dialog_connect.cpp \
                src/main.cpp \
                src/DB.cpp \
                src/Server.cpp \
                src/Chat.cpp \
                src/menu.cpp
#! [0]
RESOURCES     =
#! [0]


unix|win32: LIBS += -lmariadb
