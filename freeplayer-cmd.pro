TEMPLATE = app
MOC_DIR = tmp/moc
UI_DIR = tmp/uic
OBJECTS_DIR = tmp/objs
DESTDIR = bin
TARGET = freeplayer-cmd
INCLUDEPATH = include tmp/uic /usr/include/qt3
LIBS += -lvlc

HEADERS += include/VlcEngine.h \
           include/VlcEngineConfig.h \
           include/CommandLineIface.h

SOURCES += src/VlcEngine.cpp \
           src/VlcEngineConfig.cpp \
           src/CommandLineIface.cpp \
           src/Main.cpp
