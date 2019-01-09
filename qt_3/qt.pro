#-------------------------------------------------
#
# Project created by QtCreator 2018-08-28T11:19:58
#
#-------------------------------------------------

QT       += core gui
QT       += serialport
CONFIG += qwt
DEFINES += QT_DLL QWT_DLL
#LIBS += -L"D:\QT\Qt5.10.0\build-qwt-Desktop_Qt_5_10_0_MinGW_32bit-Debug\lib" -lqwtd
LIBS += -L"D:\QT\Qt5.10.0\build-qwt-Desktop_Qt_5_10_0_MinGW_32bit-Debug\lib" -lqwt
INCLUDEPATH += D:\QT\Qt5.10.0\5.10.0\mingw53_32\include\qwt
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = power
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
        mainwindow.cpp \
    newwindow.cpp \
    exit_dialog.cpp \
    name_dialog.cpp \
    warning.cpp \
    acc.cpp

RC_FILE += version.rc

HEADERS += \
        mainwindow.h \
    para.h \
    newwindow.h \
    exit_dialog.h \
    name_dialog.h \
    warning.h \
    acc.h

FORMS += \
        mainwindow.ui \
    newwindow.ui \
    exit_dialog.ui \
    name_dialog.ui \
    warning.ui \
    acc.ui
