TARGET = QHidApi

QT = core
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

load(qt_module)

DEFINES += QHIDAPI_LIBRARY
DEFINES += USE_LIBUSB

include(qhidapi.pri)

unix|win32|macx:contains(DEFINES, USE_LIBUSB) | android {
    SOURCES += libusb/hid.c
    INCLUDEPATH += /usr/include/libusb-1.0
    LIBS += -lusb-1.0
} else {
    unix: SOURCES += linux/hid.c
    win32: SOURCES += linux/hid.c
    macx: SOURCES += linux/hid.c
}
