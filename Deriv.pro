#-------------------------------------------------
#
# Project created by QtCreator 2013-12-29T01:48:05
#
#-------------------------------------------------

#CONFIG      -= x86_64
#CONFIG      += x86

QT          += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET =    Deriv
TEMPLATE =  app


SOURCES  += main.cpp\
            drmainwindow.cpp \
    drconnectdialog.cpp \
    drconnection.cpp \
    druser.cpp \
    drserver.cpp \
    drconnectionscontroller.cpp \
    drconnectionitem.cpp \
    drconnectioncontroller.cpp \
    drchatcontroller.cpp \
    druserscontroller.cpp \
    drtopic.cpp \
    drpreferenceswindow.cpp \
    draboutwindow.cpp \
    drconnectionobject.cpp \
    druseritemdelegate.cpp \
    dr.cpp \
    dreditconnectionwindow.cpp \
    drerror.cpp

macx: HEADERS += config/osx/config.h
unix: HEADERS += config/linux/config.h

HEADERS  += drmainwindow.h \
    drconnectdialog.h \
    drconnection.h \
    main.h \
    druser.h \
    drserver.h \
    drconnectionscontroller.h \
    drconnectionitem.h \
    drconnectioncontroller.h \
    drchatcontroller.h \
    druserscontroller.h \
    drtopic.h \
    drpreferenceswindow.h \
    draboutwindow.h \
    drconnectionobject.h \
    druseritemdelegate.h \
    dr.h \
    dreditconnectionwindow.h \
    drerror.h

FORMS    += drmainwindow.ui \
    drconnectdialog.ui \
    drpreferenceswindow.ui \
    draboutwindow.ui \
    dreditconnectionwindow.ui

macx: LIBS += -L$$PWD/libwired/osx/lib/ -L$$PWD/qtkeychain-build -lwired -lxml2 -lssl -lcrypto -lsqlite3 -lz -liconv -lqt5keychain -framework Carbon
macx: INCLUDEPATH += libwired/osx/include
macx: INCLUDEPATH += qtkeychain-build

unix: LIBS += -L$$PWD/libwired/linux/lib/ -L$$PWD/qtkeychain-build -lwired -lxml2 -lssl -lcrypto -lsqlite3 -lz -lqt5keychain
unix: INCLUDEPATH += libwired/linux/include
unix: INCLUDEPATH += qtkeychain-build

RESOURCES += \
    Resources.qrc
