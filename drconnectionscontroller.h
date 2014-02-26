#ifndef DRCONNECTIONSCONTROLLER_H
#define DRCONNECTIONSCONTROLLER_H

#include <QObject>
#include <QMutex>
#include <QList>

#include "drconnection.h"


class DRConnectionsController : public QObject
{
    Q_OBJECT


public:
    QList<DRConnection *>                   *connections;

    static DRConnectionsController*         instance(QObject* parent = 0);
    static void                             drop();

    void                                    addConnection(DRConnection *connection);
    void                                    removeConnection(DRConnection *connection);
    bool                                    hasConnection(DRConnection *connection);


private:
    explicit                                DRConnectionsController(QObject* parent = 0);
    DRConnection*                           connectionAtIndex(int index);

signals:
    void                                    connectionAdded(DRConnection *connection);
    void                                    connectionRemoved(DRConnection *connection);

public slots:

};

#endif // DRCONNECTIONSCONTROLLER_H
