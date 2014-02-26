#ifndef DRCONNECTIONCONTROLLER_H
#define DRCONNECTIONCONTROLLER_H

#include "drconnection.h"
#include <QObject>


class DRConnectionController : public QObject
{
    Q_OBJECT
public:
    DRConnectionController(DRConnection* connection, QObject *parent = 0);
    ~DRConnectionController();
protected:
    DRConnection* connection;

signals:

public slots:

};

#endif // DRCONNECTIONCONTROLLER_H
