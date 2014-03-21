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



#ifndef DRFILESCONTROLLER_H
#define DRFILESCONTROLLER_H

#include <QObject>
#include <wired/wired.h>
#include "drfilestreemodel.h"
#include "drfileitem.h"
#include "drconnectioncontroller.h"
#include "drconnection.h"


class DRFilesController : public DRConnectionController
{
    Q_OBJECT
public:
    explicit DRFilesController(DRServerConnection *connection);
    ~DRFilesController();

    DRFilesTreeModel        *treeModel;

    void connectReceiver(QObject *object);
    void disconnectReceiver(QObject *object);

    void reload();

signals:
    void filesReloaded(DRServerConnection *connection);

public slots:
    void connectionSucceeded(DRServerConnection *connection);
    void connectionError(DRServerConnection *connection, DRError *error);
    void connectionClosed(DRServerConnection *connection, DRError *error = NULL);
    void receivedMessage(wi_p7_message_t *message, DRServerConnection *connection);
    void receivedError(DRError *error, DRServerConnection *connection);

private slots:
    void fileReloaded(DRFileItem *fileItem, DRServerConnection *connection);
    void dataChanged(QModelIndex,QModelIndex,QVector<int>);

private:
    DRFileItem *rootNode;
};

#endif // DRFILESCONTROLLER_H
