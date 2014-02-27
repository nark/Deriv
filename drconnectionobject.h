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
