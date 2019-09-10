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
        main.cc \
    ui_app_tab_widget.cc \
    pal_client.cc \
    pal_crypto.cc \
    pal_msg.cc \
    pal_room.cc \
    pal_server.cc \
    pal_user.cc \
    pal_common.cc \
    applog.cc \
    ui_connect_dialog.cc \
    ui_log_widget.cc \
    ui_main_window.cc \
    ui_about_dialog.cc \
    ui_log_window.cc \
    ui_pal_client_widget.cc

HEADERS += \
    ui_app_tab_widget.h \
    pal_client.h \
    pal_crypto.h \
    pal_msg.h \
    pal_room.h \
    pal_server.h \
    pal_user.h \
    ui_main_window.h \
    pal_common.h \
    applog.h \
    ui_connect_dialog.h \
    ui_log_widget.h \
    ui_about_dialog.h \
    main.h \
    ui_log_window.h \
    ui_pal_client_widget.h

DISTFILES +=

RESOURCES += \
    assets.qrc
