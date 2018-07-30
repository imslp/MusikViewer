QT += quick
CONFIG += c++11 precompile_header

TARGET = MusikViewer

QT += gui core network xml xmlpatterns printsupport concurrent svg qml quick help multimedia  quickcontrols2 quick-private

CONFIG += separate_debug_info
CONFIG += resources_big

QMAKE_PROJECT_DEPTH = 0 # undocumented qmake flag to force absolute paths in make files

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += main.cpp \
    applicationui.cpp \
    keyreciver.cpp \
    shareutils.cpp

HEADERS += applicationui.h \
    keyreciver.h \
    shareutils.hpp

HEADERS+= $$PWD/../includes/all.h


RESOURCES +=  $$files(*.qrc, false)

DEPENDPATH += $$files(*.qml, false)

# Additional import path used to resolve QML modules in Qt Creator's code model
#QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
#QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


android{
QT += androidextras
LIBS += -L $$PWD/../Libs/android
SOURCES += $$files(androidExtra/*.c*, true)
HEADERS += $$files(androidExtra/*.h*, true)
QMAKE_CFLAGS += -std=c99

OTHER_FILES += android/src/com/museviewer/utils/QShareUtils.java \
    android/src/com/museviewer/examples/sharex/QShareActivity.java \
    android/src/com/museviewer/utils/QSharePathResolver.java

}

#unix{
#LIBS += -L $$PWD/../Libs/linux
#}

win32{
LIBS += -L $$PWD/../Libs/mingw
}

#LIBS +=  -L$$PWD -lMuseWrap
LIBS += -lMuseWrap
LIBS += -lmstyle -ldiff_match_patch -lbww -lrtf2html  -lmidi -lfluid -leffects  -lawl -lvorbisfile -llibmscore   \
-lxmlstream -lqzip -lbeatroot -lkqoauth -lzerberus -lsynthesizer -lqtsingleapp -lofqf -laudiofile

!android{
LIBS += -lsndfile -lz -lmscore_freetype -lpthread
}
#android{
#LIBS += -ljack -ljackshm -lfluidsynth -lportaudio  -lsndfile -lportmidi -lasound -lz -lfreetype  -lOpenSLES -lFLAC -lmp3lame -lvorbis -lvorbisenc -logg -llog -landroid -lEGL

#}

android{
LIBS +=  -lsndfile -lz -lfreetype  -lOpenSLES -lFLAC -lmp3lame -lvorbis -lvorbisenc -logg -landroid -lEGL

}

#unix{
#LIBS += -lporttime
#}

win32{
LIBS += -lwinmm
}

PRECOMPILED_HEADER = $$PWD/../includes/all.h
INCLUDEPATH += $$PWD/../includes


android{

DISTFILES += \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/res/xml/filepaths.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat

#ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

#contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
#    ANDROID_EXTRA_LIBS = \
#        $$PWD/../Libs/android/libsndfile.so
#}

contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
    ANDROID_EXTRA_LIBS += $$files($$PWD/../Libs/android/*.so, false)
#    deploymentLibs.files = $$files($$PWD/../Libs/android/jack/*.so, false)
#    deploymentLibs.path = /assets/jack
#    deployment.files += $$PWD/../share/*  #$$files($$PWD/../share , true)
#    deployment.path = /assets

    ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
#    INSTALLS += deployment
}

}

DISTFILES += \
    android/Saf.java
