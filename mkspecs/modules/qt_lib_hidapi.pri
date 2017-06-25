QT_MODULE_BIN_BASE = /opt/qt/modules/usb/qhidapi/bin
QT_MODULE_INCLUDE_BASE = /opt/qt/modules/usb/qhidapi/include
QT_MODULE_IMPORT_BASE = /opt/qt/modules/usb/qhidapi/imports
QT_MODULE_QML_BASE = /opt/qt/modules/usb/qhidapi/qml
QT_MODULE_LIB_BASE = /opt/qt/modules/usb/qhidapi/lib
QT_MODULE_HOST_LIB_BASE = /opt/qt/modules/usb/qhidapi/lib
QT_MODULE_LIBEXEC_BASE = /opt/qt/modules/usbqhidapi/libexec
QT_MODULE_PLUGIN_BASE = /opt/qt/modules/usbqhidapi/plugins
include(/opt/qt/modules/usb/qhidapi/mkspecs/modules-inst/qt_lib_hidapi.pri)
QT.hidapi.priority = 1
include(/opt/qt/modules/usb/qhidapi/mkspecs/modules-inst/qt_lib_hidapi_private.pri)
QT.hidapi_private.priority = 1
