#include "drconnectioncontroller.h"

DRConnectionController::DRConnectionController(DRConnection * connection, QObject *parent) :
    QObject(parent)
{
    this->connection = connection;
}


DRConnectionController::~DRConnectionController() {

}

