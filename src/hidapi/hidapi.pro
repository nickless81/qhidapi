TARGET = QHidApi

QMAKE_DOCS = $$PWD/doc/qhidapi.qdocconf

load(qt_module)

DEFINES += QHIDAPI_LIBRARY
DEFINES += USE_LIBUSB

CONFIG += build_hidapi_lib
include(hidapi.pri)

QMAKE_TARGET_COMPANY = "Simon Meaden"
QMAKE_TARGET_COPYRIGHT = "Copyright (C) 2014-2016 Simon Meaden <simonmeaden@smelecomp.co.uk>"
QMAKE_TARGET_DESCRIPTION = "USB HID reader/writer for Qt5"
