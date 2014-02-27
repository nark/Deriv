#include "drconnectionobject.h"

#pragma mark -

DRConnectionObject::DRConnectionObject(DRConnection *connection, QObject *parent) :
    QObject(parent)
{
    this->connection = connection;
}


DRConnectionObject::~DRConnectionObject() {

}



#pragma mark -
