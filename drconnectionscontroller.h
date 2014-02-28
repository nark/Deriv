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



#ifndef DRCONNECTIONSCONTROLLER_H
#define DRCONNECTIONSCONTROLLER_H

#include <QObject>
#include <QMutex>
#include <QList>
#include <wired/wired.h>
#include "drconnection.h"


class DRConnectionsController : public QObject
{
    Q_OBJECT


public:
    QList<DRConnection*> *connections;


    static DRConnectionsController* instance(QObject* parent = 0);
    static void drop();


    void addConnection(DRConnection *connection);
    void removeConnection(DRConnection *connection);
    bool hasConnection(DRConnection *connection);


    DRConnection* connectionAtIndex(int index);
    DRConnection* connectionForIdentifier(QString identifier);
    DRConnection* connectionForURL(wi_url_t *url);



private:
    explicit DRConnectionsController(QObject* parent = 0);



signals:
    void connectionAdded(DRConnection *connection);
    void connectionRemoved(DRConnection *connection);



private slots:
    void userNickDidChange(QString nick);
    void userStatusDidChange(QString status);


};

#endif // DRCONNECTIONSCONTROLLER_H
