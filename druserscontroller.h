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


#ifndef DRUSERSCONTROLLER_H
#define DRUSERSCONTROLLER_H

#include <wired/wired.h>

#include "drconnectioncontroller.h"
#include "drconnection.h"
#include "druser.h"


class DRUsersController : public DRConnectionController
{
    Q_OBJECT

public:
    QVector<DRUser*>        *users;

    explicit                DRUsersController(DRConnection *connection);
                           ~DRUsersController();

    DRUser*                 userAtIndex(int index);
    DRUser*                 userWithID(wi_p7_int32_t userID);

signals:
    void usersControllerUserListLoaded(DRConnection *);
    void usersControllerUserJoined(DRConnection *, DRUser *user);
    void usersControllerUserLeave(DRConnection *, DRUser *user);

public slots:
    void receivedMessage(wi_p7_message_t *message);

private slots:
    void receivedWiredUserList(wi_p7_message_t *message);
    void receivedWiredUserJoin(wi_p7_message_t *message);
    void receivedWiredUserLeave(wi_p7_message_t *message);
    void receivedWiredUserStatus(wi_p7_message_t *message);

private:

};

#endif // DRUSERSCONTROLLER_H
