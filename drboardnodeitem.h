#ifndef DRBOARDNODEITEM_H
#define DRBOARDNODEITEM_H

#include <QObject>
#include <QList>
#include <QVariant>
#include "drconnection.h"

class DRBoardNodeItem : public QObject
{
    Q_OBJECT
public:
    explicit DRBoardNodeItem(DRServerConnection *connection, DRBoardNodeItem *parent = 0);
    ~DRBoardNodeItem();

    DRServerConnection * getConnection();

    virtual DRBoardNodeItem *child(int row);
    virtual int childCount() const;
    virtual int columnCount() const;
    virtual QVariant data(int column) const;
    virtual DRBoardNodeItem *parent();
    virtual int childNumber() const;
    virtual void clear();
    virtual void addChild(DRBoardNodeItem *child);

private:
    QList<DRBoardNodeItem*> _childItems;
    QList<QVariant> _itemData;
    DRBoardNodeItem *_parentItem;
    DRServerConnection *_connection;
};

#endif // DRBOARDNODEITEM_H
