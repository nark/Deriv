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



#include <QListIterator>
#include <QDebug>
#include "drconnectionscontroller.h"
#include "drpreferenceswindow.h"
#include "main.h"



static DRConnectionsController          *m_Instance = 0;


#pragma mark -

DRConnectionsController*  DRConnectionsController::instance(QObject* parent) {
    static QMutex mutex;
    if (!m_Instance) {
        mutex.lock();
        if (!m_Instance)
            m_Instance = new DRConnectionsController(parent);
        mutex.unlock();
    }
    return m_Instance;
}




void DRConnectionsController::drop() {
    static QMutex mutex;
    mutex.lock();
    delete m_Instance;
    m_Instance = 0;
    mutex.unlock();
}





#pragma mark -

DRConnectionsController::DRConnectionsController(QObject* parent) : QObject( parent ) {
    this->connections = new QList<DRServerConnection *>();

    QObject::connect(DRPreferencesWindow::instance(), SIGNAL(userNickDidChange(QString)), this, SLOT(userNickDidChange(QString)));
    QObject::connect(DRPreferencesWindow::instance(), SIGNAL(userStatusDidChange(QString)), this, SLOT(userStatusDidChange(QString)));
}





#pragma mark -

void DRConnectionsController::addConnection(DRServerConnection *connection) {
    if(this->hasConnection(connection))
        return;

    this->connections->push_back(connection);

    emit connectionAdded(connection);
}



void DRConnectionsController::removeConnection(DRServerConnection *connection) {
    if(!this->hasConnection(connection))
        return;

    int index = this->connections->indexOf(connection);
    this->connections->removeAt(index);

    emit connectionRemoved(connection);
}



bool DRConnectionsController::hasConnection(DRServerConnection *connection) {
    DRServerConnection            *existing;
    int                     index;

    existing    = NULL;
    index       = this->connections->indexOf(connection);

    if(index >= 0) {
        existing = this->connectionAtIndex(index);

        if(existing != NULL)
            return true;
    }
    return false;
}





#pragma mark -

DRServerConnection* DRConnectionsController::connectionAtIndex(int index) {
    DRServerConnection *connection = NULL;

    if(index >= 0) {
        connection = this->connections->at(index);
    }

    return connection;
}




DRServerConnection* DRConnectionsController::connectionForIdentifier(QString identifier) {
    QListIterator<DRServerConnection*> it(*this->connections);

    while (it.hasNext()) {
        DRServerConnection *connection = it.next();
        if(connection->URLIdentifier() == identifier)
            return connection;
    }
    return NULL;
}




DRServerConnection* DRConnectionsController::connectionForURL(wi_url_t *url) {
    QListIterator<DRServerConnection*> it(*this->connections);
    QString identifier = "";

    identifier += QString(wi_string_cstring(wi_url_user(url)));
    identifier += QString("@");
    identifier += QString(wi_string_cstring(wi_url_host(url)));

    while (it.hasNext()) {
        DRServerConnection *connection = it.next();
        if(connection->URLIdentifier() == identifier)
            return connection;
    }
    return NULL;
}






#pragma mark -

void DRConnectionsController::userNickDidChange(QString nick) {
    QListIterator<DRServerConnection*> it(*this->connections);
    wi_string_t *wstring = wi_string_with_cstring(nick.toStdString().c_str());

    wi_p7_message_t *message = wi_p7_message_with_name(WI_STR("wired.user.set_nick"), wc_spec);
    wi_p7_message_set_string_for_name(message, wstring, WI_STR("wired.user.nick"));

    while (it.hasNext()) {
        DRServerConnection *connection = it.next();
        connection->sendMessage(message);
    }
}



void DRConnectionsController::userStatusDidChange(QString status) {
    QListIterator<DRServerConnection*> it(*this->connections);
    wi_string_t *wstring = wi_string_with_cstring(status.toStdString().c_str());

    wi_p7_message_t *message = wi_p7_message_with_name(WI_STR("wired.user.set_status"), wc_spec);
    wi_p7_message_set_string_for_name(message, wstring, WI_STR("wired.user.status"));

    while (it.hasNext()) {
        DRServerConnection *connection = it.next();
        connection->sendMessage(message);
    }
}



