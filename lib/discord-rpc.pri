LIBS += -L$$PWD/discord-rpc/lib/ -ldiscord-rpc

INCLUDEPATH += $$PWD/discord-rpc/include
DEPENDPATH += $$PWD/discord-rpc/include

discord.path = ../discord-rpc/$$TARGET
INSTALLS += discord

linux:QMAKE_LFLAGS += -no-pie
