#-------------------------------------------------
#
# Project created by QtCreator 2018-02-12T18:38:35
#
#-------------------------------------------------

TARGET = MuseWrap


TEMPLATE = lib
#TEMPLATE = app



CONFIG += static c++11

CONFIG += precompile_header
PRECOMPILED_HEADER = $$PWD/../includes/all.h

CONFIG += separate_debug_info
CONFIG += resources_big



QT +=  gui core network xml xmlpatterns printsupport concurrent svg qml quick help multimedia

QMAKE_PROJECT_DEPTH = 0 # undocumented qmake flag to force absolute paths in make files




# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

#DEFINES += MuseWrapLib

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += $$files(*.cpp, false)

HEADERS += $$files(*.h, false)
HEADERS += $$files(*.hpp, false)

HEADERS += $$PWD/../includes/all.h


RESOURCES +=  $$files(*.qrc, false)


unix {
    target.path = /usr/lib
    INSTALLS += target
}

INCLUDEPATH += $$PWD/../includes/  \
$$PWD/../includes/thirdparty

android{

QMAKE_CXXFLAGS += -std=c++11
LIBS += -L $$PWD/../Libs/android
}

unix{
LIBS += -L $$PWD/../Libs/linux
}

win32{
LIBS += -L $$PWD/../Libs/mingw
}

LIBS += -llibmscore
#LIBS += -lmstyle -ldiff_match_patch -lbww -lrtf2html  -lmidi -lfluid -leffects  -lawl -lvorbisfile -llibmscore \
#-lxmlstream -lqzip -lbeatroot -lkqoauth -lzerberus -lsynthesizer -lqtsingleapp -lofqf -laudiofile  -lsndfile \
#-lwinmm -lz -lmscore_freetype -lpthread
