#-------------------------------------------------
#
# Project created by QtCreator 2019-08-09T20:00:47
#
#-------------------------------------------------

QT       += core gui serialport


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ComSetup
TEMPLATE = app

RC_ICONS += KS2_logo2.ico




# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

# static linking library
CONFIG +=   static \
            platform \
            release \
            no-exceptions

SOURCES += \
        main.cpp \
        mainwindow.cpp \
        properties_2.cpp \
        sendlerclass.cpp \
        serialtimer.cpp

HEADERS += \
        mainwindow.h \
        properties_2.h \
        sendlerclass.h \
        serialtimer.h

FORMS += \
        mainwindow.ui \
        properties_2.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

RESOURCES += \
    images.qrc \
    resources.qrc
