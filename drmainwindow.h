#ifndef DRMAINWINDOW_H
#define DRMAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QItemSelectionModel>
#include <wired/wired.h>

#include "drconnection.h"
#include "drconnectionitem.h"
#include "druser.h"
#include "drtopic.h"



namespace Ui {
class DRMainWindow;
}

class DRMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    static DRMainWindow*    instance(QWidget* parent = 0);
    static void             drop();
    static wi_data_t*       getBase64DefaultUserIcon();

    explicit                DRMainWindow(DRConnection *connection, QWidget *parent = 0);
    explicit                DRMainWindow(QWidget *parent = 0);
    ~DRMainWindow();

    void                    selectConnection(DRConnection *connection);
    void                    setConnection(DRConnection *connection);

public slots:
    void                    disconnect();

    void                    receivedError(wi_p7_message_t *message);
    void                    on_actionNewConnection_triggered();


private slots:
    void                    treeViewSelectionDidChange();
    void                    treeViewContextMenu(const QPoint &point);

    void                    connectionAdded(DRConnection* connection);

    void                    chatControllerReceivedChatSay(DRConnection*, QString string, DRUser *user);
    void                    chatControllerReceivedChatMe(DRConnection*, QString string, DRUser *user);
    void                    chatControllerTopicChanged(DRConnection*, DRTopic *topic);

    void                    usersControllerUserListLoaded(DRConnection*);
    void                    usersControllerUserJoined(DRConnection*, DRUser *user);
    void                    usersControllerUserLeave(DRConnection*, DRUser *user);

    void                    on_chatInputEdit_returnPressed();


    void on_actionPreferences_triggered();

    void on_actionAbout_triggered();

private:
    Ui::DRMainWindow        *ui;
    DRConnection            *connection;

    QStandardItemModel      *treeModel;
    QStandardItem           *rootNode;
    DRConnectionItem        *connectionsNode;
    DRConnectionItem        *bookmarksNode;
    DRConnectionItem        *bonjoursNode;

    QStandardItemModel      *usersModel;

    void                    reloadTreeView();
    void                    reloadUserList();
    void                    reloadChatView();

    void                    appendChat(QString string, DRConnection *connection);

    DRConnection*           selectedConnection();
    DRConnectionItem*       itemForConnection(DRConnection *connection);
    bool                    hasItemForConnection(DRConnection *connection);
};

#endif // DRMAINWINDOW_H
