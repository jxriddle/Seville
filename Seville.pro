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

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    Palace_Client.cc \
    Palace_Crypto.cc \
    Palace_Host.cc \
    Palace_Room.cc \
    Palace_Server.cc \
    Palace_User.cc \
    View_AboutDialog.cc \
    View_AppTabWidget.cc \
    View_ConnectDialog.cc \
    View_LogWidget.cc \
    View_LogWindow.cc \
    View_MainWindow.cc \
    View_ClientWidget.cc \
    QtApp_ByteArray.cc \
    Log.cc \
    Main.cc \
    Palace_NetMsg_Generic.cc \
    App_Fraction.cc \
    Palace_NetMsg_Logon.cc \
    Palace_NetMsg_Room.cc \
    App_PascalString.cc

HEADERS += \
    View_AboutDialog.h \
    View_AppTabWidget.h \
    View_ClientWidget.h \
    View_ConnectDialog.h \
    View_LogWidget.h \
    View_LogWindow.h \
    View_MainWindow.h \
    Palace_Client.h \
    Palace_Crypto.h \
    Palace_Host.h \
    Palace_Room.h \
    Palace_Server.h \
    Palace_User.h \
    QtApp_ByteArray.h \
    Common.h \
    Log.h \
    Main.h \
    Palace_NetMsg_Generic.h \
    App_Fraction.h \
    Palace_NetMsg_Logon.h \
    Palace_NetMsg_Room.h \
    App_PascalString.h

DISTFILES +=

RESOURCES += \
    assets.qrc
