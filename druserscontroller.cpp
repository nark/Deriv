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



#include <QVector>
#include <QApplication>
#include <QDebug>
#include "druserscontroller.h"
#include "drpreferenceswindow.h"



#pragma mark -

DRUsersController::DRUsersController(DRServerConnection *connection) :
    DRConnectionController(connection)
{
    this->users = new QVector<DRUser*>();

    this->connection->addDelegateForMessage(this, "wired.chat.user_list");
    this->connection->addDelegateForMessage(this, "wired.chat.user_list.done");
    this->connection->addDelegateForMessage(this, "wired.chat.user_join");
    this->connection->addDelegateForMessage(this, "wired.chat.user_leave");
    this->connection->addDelegateForMessage(this, "wired.chat.user_status");
}

DRUsersController::~DRUsersController() {

}



void DRUsersController::connectReceiver(QObject *object) {
    QObject::connect(this, SIGNAL(usersControllerUserListLoaded(DRServerConnection *)), object, SLOT(usersControllerUserListLoaded(DRServerConnection *)));
    QObject::connect(this, SIGNAL(usersControllerUserJoined(DRServerConnection *, DRUser*)), object, SLOT(usersControllerUserJoined(DRServerConnection *, DRUser*)));
    QObject::connect(this, SIGNAL(usersControllerUserLeave(DRServerConnection *, DRUser*)), object, SLOT(usersControllerUserLeave(DRServerConnection *, DRUser*)));
}


void DRUsersController::disconnectReceiver(QObject *object) {
    QObject::disconnect(this, SIGNAL(usersControllerUserListLoaded(DRServerConnection *)), object, SLOT(usersControllerUserListLoaded(DRServerConnection *)));
    QObject::disconnect(this, SIGNAL(usersControllerUserJoined(DRServerConnection *, DRUser*)), object, SLOT(usersControllerUserJoined(DRServerConnection *, DRUser*)));
    QObject::disconnect(this, SIGNAL(usersControllerUserLeave(DRServerConnection *, DRUser*)), object, SLOT(usersControllerUserLeave(DRServerConnection *, DRUser*)));
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

void DRUsersController::connectionSucceeded(DRServerConnection *connection) {

}

void DRUsersController::connectionError(DRServerConnection *connection, DRError *error) {

}

void DRUsersController::connectionClosed(DRServerConnection *connection, DRError *error) {

}


void DRUsersController::receivedMessage(wi_p7_message_t *message, DRServerConnection *connection) {
    qDebug() << wi_string_cstring(wi_p7_message_name(message));
    if(QThread::currentThread() == QCoreApplication::instance()->thread())
        qDebug() << "is On Main Thread";
    else
        qDebug() << "is Not On Main Thread";

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
    else if(wi_is_equal(wi_p7_message_name(message), WI_STR("wired.chat.user_status"))) {
        this->receivedWiredUserStatus(message);
    }
}


void DRUsersController::receivedError(DRError *error, DRServerConnection *connection) {

}







#pragma mark -

void DRUsersController::receivedWiredUserList(wi_p7_message_t *message) {
    DRUser *user;

    if(wi_is_equal(wi_p7_message_name(message), WI_STR("wired.chat.user_list"))) {
        user = new DRUser(message);
        this->users->push_back(user);
    }
    else if(wi_is_equal(wi_p7_message_name(message), WI_STR("wired.chat.user_list.done"))) {
        emit usersControllerUserListLoaded(this->connection);
    }
}




void DRUsersController::receivedWiredUserJoin(wi_p7_message_t *message) {
    DRUser *user;

    user = new DRUser(message);
    this->users->push_back(user);

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

    this->users->remove(index);

    emit usersControllerUserListLoaded(this->connection);
    emit usersControllerUserLeave(this->connection, user);
}




void DRUsersController::receivedWiredUserStatus(wi_p7_message_t *message) {
    DRUser                  *user;
    wi_p7_int32_t           userID;

    wi_p7_message_get_int32_for_name(message, &userID, WI_STR("wired.user.id"));

    if(userID < 0)
        return;

    user = this->userWithID(userID);

    if(user == NULL)
        return;

    user->setObjectWithMessage(message);

    emit usersControllerUserListLoaded(this->connection);
}
