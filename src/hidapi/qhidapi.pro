TARGET = QHidApi
QT = core

load(qt_module)

DEFINES += QHIDAPI_LIBRARY
#DEFINES += USE_LIBUSB

SOURCES += \
    qhidapi.cpp \
    qhiddeviceinfomodel.cpp \
    qhidapi_p.cpp \
    hexformatdelegate.cpp \
    qhiddeviceinfoview.cpp

HEADERS += \
    qhidapi.h \
    qhidapi_global.h \
    qhiddeviceinfomodel.h \
    qhiddeviceinfo.h \
    qhidapi_p.h \
    hexformatdelegate.h \
    qhiddeviceinfoview.h \
    hidapi.h

unix|win32|macx:contains(DEFINES, USE_LIBUSB) | android {
    SOURCES += libusb/hid.c
    INCLUDEPATH += /usr/include/libusb-1.0
    LIBS += -lusb-1.0
} else {
    unix: SOURCES += linux/hid.c
    win32: SOURCES += linux/hid.c
    macx: SOURCES += linux/hid.c
}
