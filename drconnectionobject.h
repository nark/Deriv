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



#ifndef DRCONNECTIONOBJECT_H
#define DRCONNECTIONOBJECT_H

#include <QObject>
#include "drconnection.h"


class DRConnectionObject : public QObject
{
    Q_OBJECT
public:
    DRConnection *connection;

    explicit DRConnectionObject(DRConnection *connection, QObject *parent = 0);
    ~DRConnectionObject();


};

#endif // DRCONNECTIONOBJECT_H
