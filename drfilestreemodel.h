#ifndef DRFILESTREEMODEL_H
#define DRFILESTREEMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <wired/wired.h>
#include "drconnection.h"

class DRFileItem;

class DRFilesTreeModel : public QAbstractItemModel, public DRMessageDelegate
{
    Q_OBJECT

public:
    DRFilesTreeModel(DRServerConnection *connection, QObject *parent = 0);
    ~DRFilesTreeModel();

    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;

    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    bool hasChildren(const QModelIndex &parent) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole);
    bool setHeaderData(int section, Qt::Orientation orientation,
                       const QVariant &value, int role = Qt::EditRole);

    bool insertColumns(int position, int columns,
                       const QModelIndex &parent = QModelIndex());
    bool removeColumns(int position, int columns,
                       const QModelIndex &parent = QModelIndex());
    bool insertRows(int position, int rows,
                    const QModelIndex &parent = QModelIndex());
    bool removeRows(int position, int rows,
                    const QModelIndex &parent = QModelIndex());

    void fetchMore(const QModelIndex& parent);
    bool canFetchMore(const QModelIndex& parent) const;

private slots:
    void receivedMessage(wi_p7_message_t *message, DRServerConnection *connection);
    void receivedError(DRError *error, DRServerConnection *connection);

private:
    void loadItem(DRFileItem *item, DRFileItem *parent);
    void setupModelData(const QStringList &lines, DRFileItem *parent);

    DRFileItem *getItem(const QModelIndex &index) const;

    DRServerConnection *connection;
    DRFileItem *rootItem;
    DRFileItem *loadingItem;
};

#endif // DRFILESTREEMODEL_H
