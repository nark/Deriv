#ifndef DRBOARDSTREEMODEL_H
#define DRBOARDSTREEMODEL_H

#include <QObject>
#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include "drconnection.h"

class DRBoardItem;

class DRBoardsTreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    DRBoardsTreeModel(DRServerConnection *connection, QObject *parent = 0);
    ~DRBoardsTreeModel();

    DRBoardItem * addBoard(wi_p7_message_t *message, DRBoardItem *parent = 0);
    void clearBoards();

    DRBoardItem * boardForPath(QString path);
    DRBoardItem * boardAtIndex(const QModelIndex &index);

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    bool insertRows(int position, int rows, const QModelIndex &parent);

private:
    DRBoardItem *rootItem;

    DRBoardItem * getItem(const QModelIndex &index) const;
    DRBoardItem * boardWithName(QString name, DRBoardItem *parent);
};

#endif // DRBOARDSTREEMODEL_H
