#ifndef DRCONNECTIONITEM_H
#define DRCONNECTIONITEM_H

#include <QStandardItem>
#include "drconnection.h"

class DRConnectionItem : public QStandardItem
{
public:
    explicit DRConnectionItem(const QString & text, DRConnection *connection);

    DRConnection            *connection;
};

#endif // DRCONNECTIONITEM_H
