#include "drconnectionitem.h"

DRConnectionItem::DRConnectionItem(const QString &text, DRConnection *connection) :
    QStandardItem(text) {
    this->connection = connection;
}
