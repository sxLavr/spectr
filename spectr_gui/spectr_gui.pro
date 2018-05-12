#-------------------------------------------------
#
# Project created by QtCreator 2018-04-19T10:39:47
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = spectr_gui
TEMPLATE = app

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
        main.cpp \
        MainWindow.cpp \
    src/gui/Plotter.cpp \
    src/gui/ChoiceDeviceDialog.cpp \
    src/gui/DockWidgetFft.cpp

HEADERS += \
        MainWindow.h \
    src/common/PluginInterface.h \
    src/common/PluginCore.h \
    src/gui/Plotter.h \
    src/common/Declaration.h \
    src/common/Properties.h \
    src/gui/ChoiceDeviceDialog.h \
    src/gui/DockWidgetFft.h

FORMS += \
        MainWindow.ui \
    src/gui/ChoiceDeviceDialog.ui \
    src/gui/DockWidgetFft.ui

INCLUDEPATH += \
        src/common \
        src/gui \
        /usr/local/qwt-6.1.3/include

LIBS += \
        -L/usr/local/qwt-6.1.3/lib -lqwt

RESOURCES += \
    resources/spectr_gui.qrc
