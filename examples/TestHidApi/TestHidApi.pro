#-------------------------------------------------
#
# Project created by QtCreator 2014-11-19T19:08:59
#
#-------------------------------------------------

QT       += core gui hidapi

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DEFINES *= QT_USE_QSTRINGBUILDER

TEMPLATE = app

CONFIG += debug_and_release
CONFIG += debug_and_release_target
CONFIG += build_all
CONFIG += c++11 # C++11 is the newest standard

CONFIG(debug, debug|release) {
    TARGET = TestHidApid
    DESTDIR = ../build/hidapi
    OBJECTS_DIR = $$DESTDIR/.objd
    MOC_DIR = $$DESTDIR/.mocd
    RCC_DIR = $$DESTDIR/.qrcd
    UI_DIR = $$DESTDIR/.uid
}

CONFIG(release, debug|release) {
    TARGET = TestHidApi
    DESTDIR = ../build/hidapi
    OBJECTS_DIR = $$DESTDIR/.obj
    MOC_DIR = $$DESTDIR/.moc
    RCC_DIR = $$DESTDIR/.qrc
    UI_DIR = $$DESTDIR/.ui
}

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    qhexspinbox.cpp \
    udevmonitor.cpp

HEADERS  += \
    mainwindow.h \
    qhexspinbox.h \
    udevmonitor.h

FORMS    +=

#TestHidApi.depends += qhidapi

#== qhidapi =====================================================================================
#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../lib/ -lqhidapi
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../lib/ -lqhidapid
#else:unix:CONFIG(release, debug|release): LIBS += -L$$PWD/../lib/ -lqhidapi
#else:unix:CONFIG(debug, debug|release): LIBS += -L$$PWD/../lib/ -lqhidapid

#INCLUDEPATH += $$PWD/../common/qhidapi
#DEPENDPATH += $$PWD/../common/qhidapi

#== hidraw ======================================================================================
#unix|win32: LIBS += -lhidapi-hidraw
#== udev   ======================================================================================
unix|win32: LIBS += -ludev

#unix|win32: LIBS += -lhidapi-hidraw
