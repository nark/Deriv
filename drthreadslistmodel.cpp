#include "drthreadslistmodel.h"

DRThreadsListModel::DRThreadsListModel(DRServerConnection *connection, QObject *parent) :
    QAbstractListModel(parent)
{
    this->connection = connection;
}







void DRThreadsListModel::clear() {
    beginRemoveRows(QModelIndex(), 0, this->threadItems.count());

    this->threadItems.clear();

    endRemoveRows();
}




bool DRThreadsListModel::appendRow(DRThreadItem *item) {
    int position = this->threadItems.count()+1;

    beginInsertRows(QModelIndex(), position, 1);

    this->threadItems.insert(position, item);

    endInsertRows();
    return true;
}




bool DRThreadsListModel::appendRow(QList<DRThreadItem *> items) {
    int position = this->threadItems.count()+1;
    int size = this->threadItems.count() + this->threadItems.count();

    beginInsertRows(QModelIndex(), position, size);

    foreach(DRThreadItem *item, items) {
        this->threadItems.insert(position, item);
    }

    endInsertRows();
    return true;
}





int DRThreadsListModel::rowCount(const QModelIndex &parent) const
{
    return this->threadItems.count();
}




QVariant DRThreadsListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= this->threadItems.size())
        return QVariant();

    if (role == Qt::DisplayRole)
        return this->threadItems.at(index.row())->subject;
    else
        return QVariant();
}





QVariant DRThreadsListModel::headerData(int section, Qt::Orientation orientation,
                                     int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal)
        return QString("Column %1").arg(section);
    else
        return QString("Row %1").arg(section);
}






Qt::ItemFlags DRThreadsListModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractItemModel::flags(index);
}









bool DRThreadsListModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    beginInsertRows(QModelIndex(), position, position+rows-1);

    for (int row = 0; row < rows; ++row) {
        this->threadItems.insert(position, NULL);
    }

    endInsertRows();
    return true;

}





bool DRThreadsListModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    beginRemoveRows(QModelIndex(), position, position+rows-1);

    for (int row = 0; row < rows; ++row) {
        this->threadItems.removeAt(position);
    }

    endRemoveRows();
    return true;
}

