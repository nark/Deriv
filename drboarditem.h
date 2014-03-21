#ifndef DRBOARDITEM_H
#define DRBOARDITEM_H

#include <QObject>
#include <QStandardItem>
#include <QList>
#include <QMutex>
#include "drboardnodeitem.h"
#include "drconnection.h"
#include "drthreaditem.h"
#include "drthreadslistmodel.h"

class DRBoardItem : public DRBoardNodeItem
{
    Q_OBJECT
public:
    DRBoardItem(const QString &name, const QString &path, DRServerConnection *connection, DRBoardItem *parent = 0);
    ~DRBoardItem();

    DRThreadsListModel * getThreadsModel();
    void addThread(DRThreadItem *thread);
    void clearThreads();

    QString path();
    QString name();

    QVariant data(int column) const;

private:
    QString _path;
    QString _name;
    DRThreadsListModel * _threadsModel;
    mutable QMutex _mutex;
};

//Q_DECLARE_METATYPE(DRBoardItem*)

#endif // DRBOARDITEM_H
