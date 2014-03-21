#-------------------------------------------------
#
# Project created by QtCreator 2013-12-29T01:48:05
#
#-------------------------------------------------

#CONFIG      -= x86_64
#CONFIG      += x86

QMAKE_CXXFLAGS_WARN_OFF -= -Wunused-parameter

QT  += core gui network sql concurrent
QT  += webkit webkitwidgets

greaterThan(QT_MAJOR_VERSION, 5): QT += widgets

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
    druseritemdelegate.cpp \
    dr.cpp \
    dreditconnectionwindow.cpp \
    drerror.cpp \
    drserverinfodialog.cpp \
    drfilescontroller.cpp \
    drp7messageobject.cpp \
    drfilestreemodel.cpp \
    drfileitem.cpp \
    drboardscontroller.cpp \
    drthreaditem.cpp \
    drboarditem.cpp \
    drboardnodeitem.cpp \
    drboardstreemodel.cpp \
    drthreadslistmodel.cpp

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
    druseritemdelegate.h \
    dr.h \
    dreditconnectionwindow.h \
    drerror.h \
    drserverinfodialog.h \
    drfilescontroller.h \
    drp7messageobject.h \
    drfilestreemodel.h \
    drfileitem.h \
    drboardscontroller.h \
    drthreaditem.h \
    drboarditem.h \
    drboardstreemodel.h \
    drboardnodeitem.h \
    drthreadslistmodel.h

FORMS    += drmainwindow.ui \
    drconnectdialog.ui \
    drpreferenceswindow.ui \
    draboutwindow.ui \
    dreditconnectionwindow.ui \
    drserverinfodialog.ui

macx: LIBS += -L$$PWD/libwired/osx/lib/ -L$$PWD/qtkeychain-build -lwired -lxml2 -lssl -lcrypto -lsqlite3 -lz -liconv -lqt5keychain -framework Carbon
macx: INCLUDEPATH += libwired/osx/include
macx: INCLUDEPATH += qtkeychain-build

unix: LIBS += -L$$PWD/libwired/linux/lib/ -L$$PWD/qtkeychain-build -lwired -lxml2 -lssl -lcrypto -lsqlite3 -lz -lqt5keychain
unix: INCLUDEPATH += libwired/linux/include
unix: INCLUDEPATH += qtkeychain-build

RESOURCES += \
    Resources.qrc
