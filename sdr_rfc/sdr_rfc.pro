#-------------------------------------------------
#
# Project created by QtCreator 2018-04-19T10:58:44
#
#-------------------------------------------------

QT       += core widgets network

QT       -= gui

TARGET = sdr_rfc
TEMPLATE = lib

DEFINES += SDR_RFC_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    SdrRfcPluginInterface.cpp \
    src/core/SdrRfcCore.cpp \
    src/transport/TcpAcceptor.cpp \
    src/transport/TcpConnector.cpp \
    src/transport/TcpSocket.cpp \
    src/core/Constants.cpp \
    src/core/FrequencyGroup.cpp \
    src/core/RbwGroup.cpp \
    src/core/FftData.cpp \
    src/gui/TcpConnectionDialog.cpp \
    src/gui/SdrRfcDockWidget.cpp

HEADERS += \
        sdr_rfc_global.h \ 
    SdrRfcPluginInterface.h \
    src/core/SdrRfcCore.h \
    ../spectr_gui/src/common/PluginCore.h \
    src/transport/TcpAcceptor.h \
    src/transport/TcpConnector.h \
    src/transport/TcpSocket.h \
    src/core/Constants.h \
    src/core/FrequencyGroup.h \
    src/core/RbwGroup.h \
    src/core/SdrRfcProtocol.h \
    src/core/FftData.h \
    src/gui/TcpConnectionDialog.h \
    src/gui/SdrRfcDockWidget.h

INCLUDEPATH += \
        ../spectr_gui/src/common \
        ./src/core \
        ./src/transport \
        ./src/gui

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    src/gui/TcpConnectionDialog.ui \
    src/gui/SdrRfcDockWidget.ui

DISTFILES += \
    SdrRfcPlugin.json
