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



#include "drconnectioncontroller.h"

DRConnectionController::DRConnectionController(DRServerConnection * connection) {
    this->connection = connection;
}


DRConnectionController::~DRConnectionController() {

}




void DRConnectionController::connectReceiver(QObject *object) {

}

void DRConnectionController::disconnectReceiver(QObject *object) {

}





void DRConnectionController::connectionSucceeded(DRServerConnection *connection) {

}

void DRConnectionController::connectionError(DRServerConnection *connection, DRError *error) {

}

void DRConnectionController::connectionClosed(DRServerConnection *connection, DRError *error) {

}


void DRConnectionController::receivedMessage(wi_p7_message_t *message, DRServerConnection *connection) {

}

void DRConnectionController::receivedError(DRError *error, DRServerConnection *connection) {

}
