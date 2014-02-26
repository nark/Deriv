#ifndef DRCHATCONTROLLER_H
#define DRCHATCONTROLLER_H

#include <wired/wired.h>

#include "drconnectioncontroller.h"
#include "drconnection.h"
#include "drtopic.h"
#include "druser.h"


class DRChatController : public DRConnectionController
{
    Q_OBJECT
public:
    DRTopic             *topic;

    explicit            DRChatController(DRConnection* connection);
                        ~DRChatController();

signals:
    void                chatControllerReceivedChatSay(DRConnection*, QString string, DRUser *user);
    void                chatControllerReceivedChatMe(DRConnection*, QString string, DRUser *user);
    void                chatControllerTopicChanged(DRConnection*, DRTopic *topic);

public slots:
    void                receivedMessage(wi_p7_message_t *message);

private slots:
    void                receivedWiredChatSay(wi_p7_message_t *message);
    void                receivedWiredChatMe(wi_p7_message_t *message);
    void                receivedWiredChatTopic(wi_p7_message_t *message);
    void                receivedWiredUserStatus(wi_p7_message_t *message);
};

#endif // DRCHATCONTROLLER_H
