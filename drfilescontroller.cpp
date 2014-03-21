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


#include <QDebug>
#include <QFile>
#include "drfilescontroller.h"
#include "main.h"
#include "dr.h"


DRFilesController::DRFilesController(DRServerConnection *connection) :
    DRConnectionController(connection)
{
    this->treeModel = new DRFilesTreeModel(connection);
}


DRFilesController::~DRFilesController() {

}







void DRFilesController::connectReceiver(QObject *object) {
   QObject::connect(this, SIGNAL(filesReloaded(DRServerConnection *)), object, SLOT(filesReloaded(DRServerConnection *)));
}



void DRFilesController::disconnectReceiver(QObject *object) {
   QObject::disconnect(this, SIGNAL(filesReloaded(DRServerConnection *)), object, SLOT(filesReloaded(DRServerConnection *)));
}








void DRFilesController::connectionSucceeded(DRServerConnection *connection) {

}

void DRFilesController::connectionError(DRServerConnection *connection, DRError *error) {

}

void DRFilesController::connectionClosed(DRServerConnection *connection, DRError *error) {

}


void DRFilesController::receivedMessage(wi_p7_message_t *message, DRServerConnection *connection) {

}

void DRFilesController::receivedError(DRError *error, DRServerConnection *connection) {

}







void DRFilesController::fileReloaded(DRFileItem *fileItem, DRServerConnection *connection) {
//    qDebug() << "fileReloaded:" << fileItem->path << "[" << fileItem->childCount() << "]";
//    //emit filesReloaded(connection);

//    this->treeModel->fetching = false;

//    for(int i = 0; i < fileItem->childCount(); i++)
//        QObject::connect(fileItem->child(i), SIGNAL(fileReloaded(DRFileItem*,DRServerConnection*)),
//                         this, SLOT(fileReloaded(DRFileItem*,DRServerConnection*)));
}




void DRFilesController::dataChanged(QModelIndex index,QModelIndex index2,QVector<int> vector) {
    qDebug() << "dataChanged";
}






