#include <QDebug>
#include <QtWidgets>
#include "drfilestreemodel.h"
#include "drfileitem.h"
#include "dr.h"
#include "main.h"



DRFilesTreeModel::DRFilesTreeModel(DRServerConnection *connection, QObject *parent)
    : QAbstractItemModel(parent)
{
    this->connection = connection;
    this->loadingItem = NULL;

    wi_p7_message_t *message = wi_p7_message_with_name(WI_STR("wired.file.list_directory"), wc_spec);
    wi_p7_message_set_string_for_name(message, WI_STR("/"), WI_STR("wired.file.path"));

    rootItem = new DRFileItem(message, connection);
    this->loadItem(rootItem, NULL);
}

DRFilesTreeModel::~DRFilesTreeModel()
{
    delete this->rootItem;
    delete this->loadingItem;
}

int DRFilesTreeModel::columnCount(const QModelIndex & /* parent */) const
{
    return 3;
}

QVariant DRFilesTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();

    DRFileItem *item = getItem(index);

    return item->data(index.column());
}

Qt::ItemFlags DRFilesTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
}

DRFileItem *DRFilesTreeModel::getItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        DRFileItem *item = static_cast<DRFileItem*>(index.internalPointer());
        if (item)
            return item;
    }
    return rootItem;
}

QVariant DRFilesTreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        if(section == 0) {
            return "Name";
        } else if(section == 1) {
            return "Date";
        } else if(section == 2) {
            return "Size";
        }
    }

    return QVariant();
}

QModelIndex DRFilesTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();

    DRFileItem *parentItem = getItem(parent);

    DRFileItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

bool DRFilesTreeModel::insertColumns(int position, int columns, const QModelIndex &parent)
{
    bool success;

    beginInsertColumns(parent, position, position + columns - 1);
    success = rootItem->insertColumns(position, columns);
    endInsertColumns();

    return success;
}

bool DRFilesTreeModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    DRFileItem *parentItem = getItem(parent);
    bool success;

    beginInsertRows(parent, position, position + rows - 1);
    success = parentItem->insertChildren(position, rows, rootItem->columnCount());
    endInsertRows();

    return success;
}

QModelIndex DRFilesTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    DRFileItem *childItem = getItem(index);
    DRFileItem *parentItem = childItem->parent();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->childNumber(), 0, parentItem);
}

bool DRFilesTreeModel::removeColumns(int position, int columns, const QModelIndex &parent)
{
    bool success;

    beginRemoveColumns(parent, position, position + columns - 1);
    success = rootItem->removeColumns(position, columns);
    endRemoveColumns();

    if (rootItem->columnCount() == 0)
        removeRows(0, rowCount());

    return success;
}

bool DRFilesTreeModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    DRFileItem *parentItem = getItem(parent);
    bool success = true;

    beginRemoveRows(parent, position, position + rows - 1);
    success = parentItem->removeChildren(position, rows);
    endRemoveRows();

    return success;
}

int DRFilesTreeModel::rowCount(const QModelIndex &parent) const
{
    DRFileItem *parentItem = getItem(parent);

    return parentItem->childCount();
}


bool DRFilesTreeModel::hasChildren(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return true;

    DRFileItem *item = static_cast<DRFileItem*>(parent.internalPointer());

    if(item->getType() == DRFileDirectory ||
       item->getType() == DRFileDropBox ||
       item->getType() == DRFileUploads)
        return true;

    return false;
}


bool DRFilesTreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    return false;
//    if (role != Qt::EditRole)
//        return false;

//    DRFileItem *item = getItem(index);
//    bool result = item->setData(index.column(), value);

//    if (result)
//        emit dataChanged(index, index);

//    return result;
}

bool DRFilesTreeModel::setHeaderData(int section, Qt::Orientation orientation,
                              const QVariant &value, int role)
{
    if (role != Qt::EditRole || orientation != Qt::Horizontal)
        return false;

    bool result = rootItem->setData(section, value);

    if (result)
        emit headerDataChanged(orientation, section, section);

    return result;
}



bool DRFilesTreeModel::canFetchMore(const QModelIndex& parent) const {
    if(this->loadingItem != NULL)
        return false;

    if (!parent.isValid())
        return false;

    DRFileItem *item = static_cast<DRFileItem*>(parent.internalPointer());

    if(item->getType() == DRFileDirectory ||
       item->getType() == DRFileDropBox ||
       item->getType() == DRFileUploads)
        return true;

    return false;
}


void DRFilesTreeModel::fetchMore(const QModelIndex& parent) {
    if(this->loadingItem != NULL)
        return;

    if (!parent.isValid())
        return;

    DRFileItem *item = static_cast<DRFileItem*>(parent.internalPointer());

    this->loadItem(item, NULL);
}


void DRFilesTreeModel::loadItem(DRFileItem *item, DRFileItem *parent) {
    if(this->loadingItem != NULL)
        return;

    this->loadingItem = item;

    wi_p7_message_t *message = wi_p7_message_with_name(WI_STR("wired.file.list_directory"), wc_spec);
    wi_p7_message_set_string_for_name(message, QSTOWS(item->getPath()), WI_STR("wired.file.path"));

    this->connection->addDelegateForMessage(this, "wired.file.file_list");
    this->connection->addDelegateForMessage(this, "wired.file.file_list.done");

    //item->cleanChildren();
    //emit layoutAboutToBeChanged();
    this->connection->sendMessage(message);
}




void DRFilesTreeModel::receivedMessage(wi_p7_message_t *message, DRServerConnection *connection) {
    if(QThread::currentThread() != QApplication::instance()->thread())
        qDebug() << "is background thread";
    else
        qDebug() << "DRFilesTreeModel::receivedMessage is main thread";

    if(wi_is_equal(wi_p7_message_name(message), WI_STR("wired.file.file_list"))) {
        if(this->loadingItem == NULL)
            return;

        DRFileItem *parent = this->loadingItem;
        QModelIndex parentIndex = parent->getIndex();
        int row = parent->childNumber()+1;
        QModelIndex childIndex = createIndex(row, 0, parent);
        DRFileItem *newChild = new DRFileItem(message, this->connection, this->loadingItem, childIndex);

        bool success = false;

        qDebug() << newChild;

        beginInsertRows(parentIndex, parent->childCount()+1, parent->childCount()+2);
        success = this->loadingItem->appendChild(newChild);
        endInsertRows();
    }
    else if(wi_is_equal(wi_p7_message_name(message), WI_STR("wired.file.file_list.done"))) {

        //qDebug() << this->loadingItem->childCount();
        //emit dataChanged(this->loadingItem->getIndex(), this->loadingItem->getIndex());
        //emit rowsInserted(this->loadingItem->getIndex(), 0, 0);
        //emit layoutChanged();

        this->loadingItem = NULL;

        this->connection->removeDelegateForMessage(this, "wired.file.file_list");
        this->connection->removeDelegateForMessage(this, "wired.file.file_list.done");
//        QObject::disconnect(this->connection, SIGNAL(receivedMessage(wi_p7_message_t*,DRServerConnection*)),
//                         this, SLOT(receivedMessage(wi_p7_message_t*,DRServerConnection*)));
    }
}



void DRFilesTreeModel::receivedError(DRError *error, DRServerConnection *connection) {

}








void DRFilesTreeModel::setupModelData(const QStringList &lines, DRFileItem *parent)
{
    QList<DRFileItem*> parents;
    QList<int> indentations;
    parents << parent;
    indentations << 0;

    int number = 0;

    while (number < lines.count()) {
        int position = 0;
        while (position < lines[number].length()) {
            if (lines[number].mid(position, 1) != " ")
                break;
            ++position;
        }

        QString lineData = lines[number].mid(position).trimmed();

        if (!lineData.isEmpty()) {
            // Read the column data from the rest of the line.
            QStringList columnStrings = lineData.split("\t", QString::SkipEmptyParts);
            QVector<QVariant> columnData;
            for (int column = 0; column < columnStrings.count(); ++column)
                columnData << columnStrings[column];

            if (position > indentations.last()) {
                // The last child of the current parent is now the new parent
                // unless the current parent has no children.

                if (parents.last()->childCount() > 0) {
                    parents << parents.last()->child(parents.last()->childCount()-1);
                    indentations << position;
                }
            } else {
                while (position < indentations.last() && parents.count() > 0) {
                    parents.pop_back();
                    indentations.pop_back();
                }
            }

            // Append a new item to the current parent's list of children.
            DRFileItem *parent = parents.last();
            parent->insertChildren(parent->childCount(), 1, rootItem->columnCount());
            for (int column = 0; column < columnData.size(); ++column)
                parent->child(parent->childCount() - 1)->setData(column, columnData[column]);
        }

        ++number;
    }
}

