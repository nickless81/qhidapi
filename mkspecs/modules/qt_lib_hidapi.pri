QT_MODULE_BIN_BASE = /workspace/qt/common/qhidapi/bin
QT_MODULE_INCLUDE_BASE = /workspace/qt/common/qhidapi/include
QT_MODULE_IMPORT_BASE = /workspace/qt/common/qhidapi/imports
QT_MODULE_QML_BASE = /workspace/qt/common/qhidapi/qml
QT_MODULE_LIB_BASE = /workspace/qt/common/qhidapi/lib
QT_MODULE_HOST_LIB_BASE = /workspace/qt/common/qhidapi/lib
QT_MODULE_LIBEXEC_BASE = /workspace/qt/common/qhidapi/libexec
QT_MODULE_PLUGIN_BASE = /workspace/qt/common/qhidapi/plugins
include(/workspace/qt/common/qhidapi/mkspecs/modules-inst/qt_lib_hidapi.pri)
QT.hidapi.priority = 1
include(/workspace/qt/common/qhidapi/mkspecs/modules-inst/qt_lib_hidapi_private.pri)
QT.hidapi_private.priority = 1
