#include <QVector>
#include <QDebug>
#include "druserscontroller.h"

#pragma mark -

DRUsersController::DRUsersController(DRConnection *connection) :
    DRConnectionController(connection)
{
    this->users = new QVector<DRUser*>();

    QObject::connect(this->connection, SIGNAL(receivedMessage(wi_p7_message_t*)),
                     this, SLOT(receivedMessage(wi_p7_message_t*)));

}

DRUsersController::~DRUsersController() {
    delete this->users, this->users = NULL;
}




#pragma mark -

DRUser* DRUsersController::userAtIndex(int index) {
    return this->users->at(index);
}



DRUser* DRUsersController::userWithID(wi_p7_int32_t userID) {
     QVectorIterator<DRUser*> i(*this->users);

     while (i.hasNext()) {
        DRUser *user = i.next();
        if(user->userID == userID)
            return user;
     }
     return NULL;
}






#pragma mark -

void DRUsersController::receivedMessage(wi_p7_message_t *message) {
    qDebug() << wi_string_cstring(wi_p7_message_name(message));

    if(wi_is_equal(wi_p7_message_name(message), WI_STR("wired.chat.user_list"))) {
        this->receivedWiredUserList(message);
    }
    else if(wi_is_equal(wi_p7_message_name(message), WI_STR("wired.chat.user_list.done"))) {
        this->receivedWiredUserList(message);
    }
    else if(wi_is_equal(wi_p7_message_name(message), WI_STR("wired.chat.user_join"))) {
        this->receivedWiredUserJoin(message);
    }
    else if(wi_is_equal(wi_p7_message_name(message), WI_STR("wired.chat.user_leave"))) {
        this->receivedWiredUserLeave(message);
    }
}






#pragma mark -

void DRUsersController::receivedWiredUserList(wi_p7_message_t *message) {
    DRUser                  *user;
    wi_p7_int32_t           userID;

    if(wi_is_equal(wi_p7_message_name(message), WI_STR("wired.chat.user_list"))) {
        user = new DRUser();

        wi_p7_message_get_int32_for_name(message, &userID, WI_STR("wired.user.id"));

        if(userID < 0)
            return;

        user->userID    = userID;
        user->nick      = wi_p7_message_string_for_name(message, WI_STR("wired.user.nick"));
        user->status    = wi_p7_message_string_for_name(message, WI_STR("wired.user.status"));
        user->icon      = wi_p7_message_data_for_name(message, WI_STR("wired.user.icon"));

        this->users->push_front(user);
    }
    else if(wi_is_equal(wi_p7_message_name(message), WI_STR("wired.chat.user_list.done"))) {
        emit usersControllerUserListLoaded(this->connection);
    }
}




void DRUsersController::receivedWiredUserJoin(wi_p7_message_t *message) {
    DRUser                  *user;
    wi_p7_int32_t           userID;

    user = new DRUser();

    wi_p7_message_get_int32_for_name(message, &userID, WI_STR("wired.user.id"));

    if(userID < 0)
        return;

    user->userID    = userID;
    user->nick      = wi_p7_message_string_for_name(message, WI_STR("wired.user.nick"));
    user->status    = wi_p7_message_string_for_name(message, WI_STR("wired.user.status"));
    user->icon      = wi_p7_message_data_for_name(message, WI_STR("wired.user.icon"));

    this->users->push_front(user);

    emit usersControllerUserJoined(this->connection, user);
    emit usersControllerUserListLoaded(this->connection);
}




void DRUsersController::receivedWiredUserLeave(wi_p7_message_t *message) {
    DRUser                  *user;
    wi_p7_int32_t           userID;

    wi_p7_message_get_int32_for_name(message, &userID, WI_STR("wired.user.id"));

    if(userID < 0)
        return;

    user = this->userWithID(userID);

    if(user == NULL)
        return;

    int index = this->users->indexOf(user);

    if(index < 0)
        return;

    this->users->removeAt(index);

    emit usersControllerUserListLoaded(this->connection);
    emit usersControllerUserLeave(this->connection, user);
}
