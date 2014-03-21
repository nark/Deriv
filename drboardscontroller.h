#ifndef DRBOARDSCONTROLLER_H
#define DRBOARDSCONTROLLER_H

#include <QObject>
#include <QStandardItemModel>
#include <QStandardItem>
#include "drconnectioncontroller.h"
#include "drconnection.h"
#include "drboarditem.h"
#include "drboardstreemodel.h";




class DRBoardsController : public DRConnectionController
{
    Q_OBJECT
public:
    DRBoardsTreeModel *boardsModel;
    QStandardItemModel *threadsModel;

    explicit DRBoardsController(DRServerConnection* connection);

    void connectReceiver(QObject *object);
    void disconnectReceiver(QObject *object);

    void getBoards();
    void getThreads(DRBoardItem *board);

public slots:
    void connectionSucceeded(DRServerConnection *connection);
    void connectionError(DRServerConnection *connection, DRError *error);
    void connectionClosed(DRServerConnection *connection, DRError *error = NULL);
    void receivedMessage(wi_p7_message_t *message, DRServerConnection *connection);
    void receivedError(DRError *error, DRServerConnection *connection);

private:
    void receivedBoard(wi_p7_message_t *message);
    void receivedThread(wi_p7_message_t *message);
};

#endif // DRBOARDSCONTROLLER_H
