INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

QT = core

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
