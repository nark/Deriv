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


#ifndef DRDATABASEMANAGER_H
#define DRDATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QFile>

//class DRMessage;
//class DRPrivateMessage;
//class DRBroadcastMessage;
//class DRChatMessage;
//class DREventMessage;

class DRDatabaseManager : public QObject
    {
    public:
        static DRDatabaseManager* instance(QObject* parent = 0);
        static void drop();

        DRDatabaseManager(QObject *parent = 0);
        ~DRDatabaseManager();

    public:



        bool openDB();
        bool deleteDB();
        QSqlError lastError();

        bool exec(QString sql);
        bool query(QString sql);

    private:
        QSqlDatabase db;
};

#endif // DRDATABASEMANAGER_H
