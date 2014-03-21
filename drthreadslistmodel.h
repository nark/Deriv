#ifndef DRTHREADSLISTMODEL_H
#define DRTHREADSLISTMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include "drconnection.h"
#include "drthreaditem.h"


class DRThreadsListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    DRThreadsListModel(DRServerConnection *connection, QObject *parent = 0);

    void clear();

    bool appendRow(DRThreadItem *item);
    bool appendRow(QList<DRThreadItem*> threadItems);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;

    bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex());
    bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex());
private:
    QList<DRThreadItem*> threadItems;
    DRServerConnection *connection;
};

#endif // DRTHREADSLISTMODEL_H
