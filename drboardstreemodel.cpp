#include <QtWidgets>

#include "drboardstreemodel.h"
#include "drboarditem.h"
#include "dr.h"





DRBoardsTreeModel::DRBoardsTreeModel(DRServerConnection *connection, QObject *parent)
    : QAbstractItemModel(parent)
{
    this->rootItem = new DRBoardItem("<root>", "/", connection);
}



DRBoardsTreeModel::~DRBoardsTreeModel()
{
    delete this->rootItem;
}




void DRBoardsTreeModel::clearBoards() {
    this->rootItem->clear();
}




DRBoardItem * DRBoardsTreeModel::boardAtIndex(const QModelIndex &index) {
    return this->getItem(index);
}



DRBoardItem * DRBoardsTreeModel::boardForPath(QString path) {
    QStringList comps = path.split("/");
    DRBoardItem *board, *child;

    if(comps.size() == 1)
        return this->rootItem;

    board = this->rootItem;
    child = NULL;

    for(int i = 0; i < comps.size(); i++) {
        child = this->boardWithName(comps.at(i), board);

        if(!child)
            break;

        board = child;
    }

    return board;
}




DRBoardItem * DRBoardsTreeModel::addBoard(wi_p7_message_t *message, DRBoardItem *parent) {
    QString path = WSTOQS(wi_p7_message_string_for_name(message, WI_STR("wired.board.board")));
    QFileInfo info(path);

    parent = (parent == NULL ? this->rootItem : parent);

    DRBoardItem *board = new DRBoardItem(info.fileName(), path, parent->getConnection(), parent);
    parent->addChild(board);

    return board;
}




int DRBoardsTreeModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<DRBoardItem*>(parent.internalPointer())->columnCount();
    else
        return rootItem->columnCount();
}




QVariant DRBoardsTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    DRBoardItem *item = static_cast<DRBoardItem*>(index.internalPointer());

    return item->data(index.column());
}




Qt::ItemFlags DRBoardsTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}




QVariant DRBoardsTreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}




QModelIndex DRBoardsTreeModel::index(int row, int column, const QModelIndex &parent)
            const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    DRBoardItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<DRBoardItem*>(parent.internalPointer());

    DRBoardItem *childItem = static_cast<DRBoardItem*>(parentItem->child(row));
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}




QModelIndex DRBoardsTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    DRBoardItem *childItem = static_cast<DRBoardItem*>(index.internalPointer());
    DRBoardItem *parentItem = static_cast<DRBoardItem*>(childItem->parent());

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->childNumber(), 0, parentItem);
}




int DRBoardsTreeModel::rowCount(const QModelIndex &parent) const
{
    DRBoardItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<DRBoardItem*>(parent.internalPointer());

    return parentItem->childCount();
}




bool DRBoardsTreeModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    DRBoardItem *parentItem = getItem(parent);
    bool success;

    beginInsertRows(parent, position, position + rows - 1);
    //success = parentItem->insertChildren(position, rows, rootItem->columnCount());

    endInsertRows();

    return success;
}




DRBoardItem * DRBoardsTreeModel::getItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        DRBoardItem *item = static_cast<DRBoardItem*>(index.internalPointer());
        if (item)
            return item;
    }
    return rootItem;
}




DRBoardItem * DRBoardsTreeModel::boardWithName(QString name, DRBoardItem *parent) {
    for(int i = 0; i < parent->childCount(); i++) {
        if(dynamic_cast<DRBoardItem*>(parent->child(i))->name() == name)
            return (DRBoardItem *)parent->child(i);
    }
    return NULL;
}




