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
#include <wired/wired.h>


#include "drserver.h"
#include "drerror.h"


class DRChatController;
class DRUsersController;

class DRConnection : public QObject
{
    Q_OBJECT
public:
    wi_url_t                    *url;
    wi_socket_t                 *socket;
    wi_p7_socket_t              *p7_socket;
    wi_p7_uint32_t               userID;

    DRServer                    *server;
    DRChatController            *chatController;
    DRUsersController           *usersController;

    QString                     serverName;
    bool                        connected;
    bool                        connecting;
    bool                        reconnecting;

    bool                        autoConnect;
    bool                        autoReconnect;

    QTimer                      *receiveTimer;

    explicit                    DRConnection(wi_url_t *url, QObject *parent = 0);
    ~DRConnection();

    QString                     URLIdentifier();
    QString                     URLPassword();

public slots:
    void                        connect(QObject *receiver);
    void                        disconnect();

    void                        sendMessage(wi_p7_message_t *message);

    void                        sendUserInfo();
    void                        joinPublicChat();

signals:    
    void                        connectionSucceeded(DRConnection *connection);
    void                        connectionError(DRConnection *connection, DRError *error);
    void                        connectionClosed(DRConnection *connection, DRError *error = NULL);

    void                        receivedMessage(wi_p7_message_t *message);
    void                        receivedError(wi_p7_message_t *message);


private slots:
    void                        connectThread();
    void                        receiveMessagesLoop(DRError **error);

private:
    wi_p7_socket_t *			connectSocket(DRError **error);
    wi_boolean_t				login(DRError **error);

    wi_p7_message_t *           writeMessageAndReadReply(wi_p7_message_t *, wi_string_t *);
    DRError*                    errorWithCode(int code, QString info = 0);
};

#endif // DRCONNECTION_H
