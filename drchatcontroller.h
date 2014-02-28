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
    QString             *chatBuffer;

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
