#-------------------------------------------------
#
# Project created by QtCreator 2017-08-27T02:50:22
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Seville
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++17

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    seville/base/bytearray.cc \
    seville/base/main.cc \
    seville/base/sevilleapp.cc \
    seville/palace/cipher.cc \
    seville/palace/client.cc \
    seville/palace/host.cc \
    seville/palace/hotspot.cc \
    seville/palace/log.cc \
    seville/palace/logentry.cc \
    seville/palace/netmsg.cc \
    seville/palace/prop.cc \
    seville/palace/room.cc \
    seville/palace/server.cc \
    seville/palace/user.cc \
    seville/view/aboutdialog.cc \
    seville/view/connectdialog.cc \
    seville/view/logwidget.cc \
    seville/view/palaceclientwidget.cc \
    seville/view/tabwidget.cc \
    seville/view/mainwindow.cc

HEADERS += \
    seville/base/logcategories.h \
    seville/base/sevilleapp.h \
    seville/base/types.h \
    seville/base/bytearray.h \
    seville/palace/cipher.h \
    seville/palace/client.h \
    seville/palace/host.h \
    seville/palace/hotspot.h \
    seville/palace/log.h \
    seville/palace/logentry.h \
    seville/palace/netmsg.h \
    seville/palace/prop.h \
    seville/palace/registration.h \
    seville/palace/room.h \
    seville/palace/server.h \
    seville/palace/user.h \
    seville/view/aboutdialog.h \
    seville/view/connectdialog.h \
    seville/view/logwidget.h \
    seville/view/palaceclientwidget.h \
    seville/view/tabwidget.h \
    seville/view/mainwindow.h

DISTFILES +=

RESOURCES += \
    assets.qrc
