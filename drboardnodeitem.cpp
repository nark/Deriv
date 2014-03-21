#include "drboardnodeitem.h"

DRBoardNodeItem::DRBoardNodeItem(DRServerConnection *connection, DRBoardNodeItem *parent) : QObject()
{
    this->_parentItem = parent;
    this->_connection = connection;
}


DRBoardNodeItem::~DRBoardNodeItem() {
    qDeleteAll(this->_childItems);
}


DRServerConnection * DRBoardNodeItem::getConnection() {
    return this->_connection;
}


DRBoardNodeItem * DRBoardNodeItem::child(int row) {
    return this->_childItems.value(row);
}


int DRBoardNodeItem::childCount() const {
    return this->_childItems.count();
}


int DRBoardNodeItem::columnCount() const {
    return 1;
}


QVariant DRBoardNodeItem::data(int column) const {
    return "<null>";
}


int DRBoardNodeItem::childNumber() const {
    if (this->_parentItem)
        return this->_parentItem->_childItems.indexOf(const_cast<DRBoardNodeItem*>(this));

    return 0;
}



void DRBoardNodeItem::clear() {
    this->_childItems.clear();
}




void DRBoardNodeItem::addChild(DRBoardNodeItem *child) {
    this->_childItems.append(child);
}



DRBoardNodeItem * DRBoardNodeItem::parent() {
    return this->_parentItem;
}

