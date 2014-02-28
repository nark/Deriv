/*
   Copyright 2014, Rafaël Warnault <rw@read-write.fr>

   Clementine is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   Clementine is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Clementine.  If not, see <http://www.gnu.org/licenses/>.
*/



#ifndef DRCONNECTION_H
#define DRCONNECTION_H

#include <QObject>
#include <QThread>
#include <QTimer>
#include <wired/wired.h>


#include "drserver.h"


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

    bool                        connected;

    explicit                    DRConnection(wi_url_t *url, QObject *parent = 0);
    ~DRConnection();

    QString                     URLIdentifier();

public slots:
    void                        connect();
    void                        disconnect();
    void                        sendMessage(wi_p7_message_t *message);

    void                        sendUserInfo();
    void                        joinPublicChat();

signals:
    void                        connectSucceeded();
    void                        connectError(QString err);
    void                        connectionClosed(DRConnection *connection);

    void                        receivedMessage(wi_p7_message_t *message);
    void                        receivedError(wi_p7_message_t *message);


private slots:
    void                        receiveMessagesLoop();

private:
    wi_p7_socket_t *			wc_connect(wi_url_t *);
    wi_boolean_t				wc_login(wi_p7_socket_t *, wi_url_t *);
    wi_p7_message_t *           wc_write_message_and_read_reply(wi_p7_socket_t *, wi_p7_message_t *, wi_string_t *);

};

#endif // DRCONNECTION_H
