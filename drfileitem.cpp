/*
    Deriv is a cross-platform client for th Wired 2.0 protocol
    Copyright (C) 2014  Rafael Warnault, rw@read-write.fr

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include <QDebug>
#include <QFileInfo>
#include "drfileitem.h"
#include "drfilestreemodel.h"
#include "dr.h"
#include "main.h"
#include <QStringList>




DRFileItem::DRFileItem(wi_p7_message_t *message, DRServerConnection *connection, DRFileItem *parent, QModelIndex index)
{
    this->connection = connection;
    this->parentItem = parent;
    this->index = index;
    this->type = DRFileDirectory;

    if(message != NULL) {
        wi_string_t *string = NULL;
        wi_date_t *date = NULL;
        wi_p7_enum_t wenum;

        string = wi_p7_message_string_for_name(message, WI_STR("wired.file.path"));
        if(string != NULL) {
            this->path = WSTOQS(string);
            this->name = QFileInfo(this->path).fileName();
        }

        date = wi_p7_message_date_for_name(message, WI_STR("wired.file.creation_time"));
        if(date != NULL)
            this->creationTime = DR::wiredDateToQDateTime(date);

        wi_p7_message_get_enum_for_name(message, &wenum, WI_STR("wired.file.type"));
        this->type = (DRFileType)wenum;
    }
}


DRFileItem::~DRFileItem()
{
    qDeleteAll(childItems);
}

DRFileItem *DRFileItem::child(int number)
{
    return childItems.value(number);
}

int DRFileItem::childCount() const
{
    return childItems.count();
}

int DRFileItem::childNumber() const
{
    if (parentItem)
        return parentItem->childItems.indexOf(const_cast<DRFileItem*>(this));

    return 0;
}

int DRFileItem::columnCount() const
{
    return 3;
}

QVariant DRFileItem::data(int column) const
{
    if(column == 0)
        return this->name;

    else if (column == 1)
        return this->creationTime.toString();

    else if (column == 2)
        return this->dataSize;

    return this->name;
}


QString DRFileItem::getName() {
    return this->name;
}


QString DRFileItem::getPath() {
    return this->path;
}


QModelIndex DRFileItem::getIndex() {
    return this->index;
}


DRFileType DRFileItem::getType() {
    return this->type;
}




bool DRFileItem::appendChild(DRFileItem *item) {
    this->childItems.append(item);
    return true;
}



bool DRFileItem::cleanChildren() {
    this->childItems.clear();
}


bool DRFileItem::insertChildren(int position, int count, int columns)
{
    if (position < 0 || position > childItems.size())
        return false;

    for (int row = 0; row < count; ++row) {
//        QVector<QVariant> data(columns);
//        DRFileItem *item = new DRFileItem(data, this);
//        childItems.insert(position, item);
    }

    return true;
}

bool DRFileItem::insertColumns(int position, int columns)
{
    if (position < 0 || position > itemData.size())
        return false;

    for (int column = 0; column < columns; ++column)
        itemData.insert(position, QVariant());

    foreach (DRFileItem *child, childItems)
        child->insertColumns(position, columns);

    return true;
}

DRFileItem *DRFileItem::parent()
{
    return parentItem;
}

bool DRFileItem::removeChildren(int position, int count)
{
    if (position < 0 || position + count > childItems.size())
        return false;

    for (int row = 0; row < count; ++row)
        delete childItems.takeAt(position);

    return true;
}

bool DRFileItem::removeColumns(int position, int columns)
{
    if (position < 0 || position + columns > itemData.size())
        return false;

    for (int column = 0; column < columns; ++column)
        itemData.remove(position);

    foreach (DRFileItem *child, childItems)
        child->removeColumns(position, columns);

    return true;
}

bool DRFileItem::setData(int column, const QVariant &value)
{
    if (column < 0 || column >= itemData.size())
        return false;

    itemData[column] = value;
    return true;
}
