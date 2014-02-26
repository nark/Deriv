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
