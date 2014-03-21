/*
    Deriv is a cross-platform client for th Wired 2.0 protocol
    Copyright (C) 2014  Rafael Warnault, rw@read-write.fr

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/



#ifndef DRCONNECTION_H
#define DRCONNECTION_H

#include <QObject>
#include <QThread>
#include <QTimer>
#include <QMutex>
#include <QList>
#include <QHash>
#include <wired/wired.h>


#include "drserver.h"
#include "drerror.h"




class DRServerConnection;
class DRChatController;
class DRUsersController;
class DRFilesController;
class DRBoardsController;




/**
 * @brief The DRConnectionDelegate interface exposes methods for connection delegates
 */
class DRConnectionDelegate
{
public:
    Q_INVOKABLE virtual void connectionSucceeded(DRServerConnection *connection) = 0;
    Q_INVOKABLE virtual void connectionError(DRServerConnection *connection, DRError *error) = 0;
    Q_INVOKABLE virtual void connectionClosed(DRServerConnection *connection, DRError *error = NULL) = 0;
};
#define DRConnectionDelegate_ID "fr.read-write.DRConnectionDelegate"
Q_DECLARE_INTERFACE(DRConnectionDelegate, DRConnectionDelegate_ID)






class DRMessageDelegate
{
public:
    Q_INVOKABLE virtual void receivedMessage(wi_p7_message_t *message, DRServerConnection *connection) = 0;
    Q_INVOKABLE virtual void receivedError(DRError *error, DRServerConnection *connection) = 0;
};
#define DRMessageDelegate_ID "fr.read-write.DRMessageDelegate"
Q_DECLARE_INTERFACE(DRMessageDelegate, DRMessageDelegate_ID)








class DRServerConnection : public QObject
{
    Q_OBJECT
public:
    wi_url_t *url;
    wi_socket_t *socket;
    wi_p7_socket_t *p7_socket;
    wi_p7_uint32_t userID;

    DRServer *server;
    DRChatController *chatController;
    DRBoardsController *boardsController;
    DRUsersController *usersController;
    DRFilesController *filesController;

    QString serverName;
    bool connected;
    bool connecting;
    bool reconnecting;

    bool autoConnect;
    bool autoReconnect;

    QList<DRConnectionDelegate *> connectionsdelegates;
    QHash<QString, QList<DRMessageDelegate *> > messagesdelegates;

    explicit DRServerConnection(wi_url_t *url, QObject *parent = 0);
    ~DRServerConnection();

    void setURL(wi_url_t *url);
    QString URLIdentifier();
    QString URLPassword();
    QString URL();

    void addDelegateForConnection(DRConnectionDelegate *delegate);
    void removeDelegateForConnection(DRConnectionDelegate *delegate);

    void addDelegateForMessage(DRMessageDelegate *delegate, QString message);
    void removeDelegateForMessage(DRMessageDelegate *delegate, QString message);

public slots:
    void connect(QObject *receiver);
    void disconnect();

    bool sendMessage(wi_p7_message_t *message);
    void sendUserInfo();
    void joinPublicChat();


protected slots:
    void initSubControllers();
    void connectThread();
    void receiveMessagesThread(DRError **error);

private:
    mutable QMutex delegateMutex;
    mutable QMutex readMutex;

    wi_p7_socket_t * connectSocket(DRError **error);
    wi_boolean_t login(DRError **error);

    wi_p7_message_t * writeMessageAndReadReply(wi_p7_message_t *, wi_string_t *);
    DRError* errorWithCode(int code, QString info = 0);

    void notifyDelegatesConnectionSucceeded();
    void notifyDelegatesConnectionError(DRError *error);
    void notifyDelegatesConnectionClosed(DRError *error = NULL);
    void notifyDelegatesReceivedMessage(wi_p7_message_t *message);
    void notifyDelegatesReceivedError(wi_p7_message_t *message, DRError *error);
};


Q_DECLARE_METATYPE(DRServerConnection*)
Q_DECLARE_METATYPE(DRError*)
Q_DECLARE_OPAQUE_POINTER(wi_p7_message_t*)

#endif // DRCONNECTION_H
