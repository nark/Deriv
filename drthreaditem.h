#ifndef DRTHREADITEM_H
#define DRTHREADITEM_H

#include <QObject>
#include <QStandardItem>
#include <wired/wired.h>
#include "drconnection.h"
#include "drboardnodeitem.h"


class DRBoardItem;


class DRThreadItem : public DRBoardNodeItem
{
    Q_OBJECT
public:
    DRThreadItem(wi_p7_message_t *message, DRBoardItem *board, DRServerConnection *connection);

    DRServerConnection *connection;
    DRBoardItem *board;

    QString threadID;
    QString subject;
    QString nick;
};

Q_DECLARE_METATYPE(DRThreadItem*)

#endif // DRTHREADITEM_H
