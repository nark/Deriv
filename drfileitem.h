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



#ifndef DRFILE_H
#define DRFILE_H

#include <QObject>
#include <QList>
#include <QVariant>
#include <QModelIndex>
#include <wired/wired.h>
#include "drp7messageobject.h"
#include "drconnection.h"



class DRFilesTreeModel;



enum _DRFileType {
    DRFileFile,
    DRFileDirectory,
    DRFileUploads,
    DRFileDropBox
};
typedef enum _DRFileType			DRFileType;





class DRFileItem : public QObject
{
    Q_OBJECT
public:
    explicit DRFileItem(wi_p7_message_t *message,
                        DRServerConnection *connection,
                        DRFileItem *parent = 0,
                        QModelIndex index = QModelIndex());

    ~DRFileItem();

    DRFileItem *child(int number);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;

    bool appendChild(DRFileItem *item);
    bool cleanChildren();

    bool insertChildren(int position, int count, int columns);
    bool insertColumns(int position, int columns);
    DRFileItem *parent();
    bool removeChildren(int position, int count);
    bool removeColumns(int position, int columns);
    int childNumber() const;
    bool setData(int column, const QVariant &value);

    QString getName();
    QString getPath();
    QModelIndex getIndex();
    DRFileType getType();

private:
    QList<DRFileItem*> childItems;
    QVector<QVariant> itemData;
    DRFileItem *parentItem;
    DRServerConnection *connection;
    QModelIndex index;

    QString path;
    QString name;
    QDateTime creationTime;
    uint64_t dataSize;
    DRFileType type;
};

#endif // DRFILE_H
