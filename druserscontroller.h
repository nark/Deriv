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

private:

};

#endif // DRUSERSCONTROLLER_H
