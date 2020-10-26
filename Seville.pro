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
    seville/base/app.cc \
    seville/base/bytearray.cc \
    seville/base/main.cc \
    seville/palace/base/cipher.cc \
    seville/palace/base/client.cc \
    seville/palace/base/host.cc \
    seville/palace/base/logger.cc \
    seville/palace/base/logmessage.cc \
    seville/palace/base/netmsg.cc \
    seville/palace/base/prop.cc \
    seville/palace/base/room.cc \
    seville/palace/base/server.cc \
    seville/palace/base/user.cc \
    seville/palace/netmsg/httpserverlocation.cc \
    seville/palace/netmsg/logon.cc \
    seville/palace/netmsg/roomdescription.cc \
    seville/view/dialog/aboutdialog.cc \
    seville/view/dialog/connectdialog.cc \
    seville/view/widget/clientwidget.cc \
    seville/view/widget/logwidget.cc \
    seville/view/widget/tabwidget.cc \
    seville/view/window/mainwindow.cc
    seville/view/window/mainwindow.cc \

HEADERS += \
    seville/base/app.h \
    seville/base/types.h \
    seville/base/bytearray.h \
    seville/palace/base/cipher.h \
    seville/palace/base/client.h \
    seville/palace/base/host.h \
    seville/palace/base/logger.h \
    seville/palace/base/logmessage.h \
    seville/palace/base/netmsg.h \
    seville/palace/base/prop.h \
    seville/palace/base/room.h \
    seville/palace/base/server.h \
    seville/palace/base/user.h \
    seville/palace/netmsg/httpserverlocation.h \
    seville/palace/netmsg/logon.h \
    seville/palace/netmsg/roomdescription.h \
    seville/view/dialog/aboutdialog.h \
    seville/view/dialog/connectdialog.h \
    seville/view/widget/clientwidget.h \
    seville/view/widget/logwidget.h \
    seville/view/widget/tabwidget.h \
    seville/view/window/mainwindow.h

DISTFILES +=

RESOURCES += \
    assets.qrc
