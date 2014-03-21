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


#ifndef DRMAINWINDOW_H
#define DRMAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QItemSelectionModel>
#include <QItemDelegate>
#include <QApplication>
#include <QDebug>
#include <QProgressBar>
#include <QLabel>
#include <QList>
#include <wired/wired.h>

#include "drconnection.h"
#include "drconnectionitem.h"
#include "drboarditem.h"
#include "druser.h"
#include "drtopic.h"
#include "drerror.h"



namespace Ui {
class DRMainWindow;
}

class DRMainWindow : public QMainWindow, public DRConnectionDelegate
{
    Q_OBJECT

public:
    explicit                DRMainWindow(QWidget *parent = 0);

    static DRMainWindow*    instance(QWidget* parent = 0);
    static void             drop();

    static void             startProgress(QString title);
    static void             stopProgress(QString title);
    ~DRMainWindow();

    void                    close();

    void                    selectConnection(DRServerConnection *connection);
    void                    setConnection(DRServerConnection *connection);

public slots:
    void                    connect();
    void                    disconnect();
    void                    removeConnection();
    void                    editConnection();
    void                    connectionInfo();
    void                    saveConnections();

    void                    connectionSucceeded(DRServerConnection *connection);
    void                    connectionError(DRServerConnection *connection, DRError* error);
    void                    connectionClosed(DRServerConnection *connection, DRError *error);

    void                    receivedError(DRError *error);
    void                    on_actionNewConnection_triggered();

private slots:
    void                    treeViewSelectionDidChange();
    void                    treeViewDoubleClicked(QModelIndex index);
    void                    treeViewContextMenu(const QPoint &point);

    void                    boardsTreeViewSelectionDidChange();
    void                    boardsTreeViewContextMenu(const QPoint &point);

    void                    connectionAdded(DRServerConnection* connection);

    void                    chatControllerReceivedChatSay(DRServerConnection*, QString string, DRUser *user);
    void                    chatControllerReceivedChatMe(DRServerConnection*, QString string, DRUser *user);
    void                    chatControllerTopicChanged(DRServerConnection*, DRTopic *topic, bool init);

    void                    usersControllerUserListLoaded(DRServerConnection*);
    void                    usersControllerUserJoined(DRServerConnection*, DRUser *user);
    void                    usersControllerUserLeave(DRServerConnection*, DRUser *user);

    void                    filesReloaded(DRServerConnection *connection);

    void                    on_chatInputEdit_returnPressed();

    void on_actionAbout_triggered();

    void on_actionPreferences_triggered();

    void on_actionPreferences_triggered(bool checked);

    void on_actionChat_triggered();

    void on_actionBoards_triggered();

    void on_actionMessages_triggered();

    void on_actionFiles_triggered();

    void on_actionTransfers_triggered();

private:
    Ui::DRMainWindow        *ui;
    DRServerConnection            *connection;

    QStandardItemModel      *treeModel;
    QStandardItem           *rootNode;
    DRConnectionItem        *connectionsNode;
    DRConnectionItem        *bookmarksNode;
    DRConnectionItem        *bonjoursNode;

    QStandardItemModel      *usersModel;
    QProgressBar            *progressBar;
    QLabel                  *progressLabel;

    QList<QString>          chatCommands;

    void                    reloadTreeView();
    void                    reloadUserList();
    void                    reloadChatView();
    void                    reloadFilesView();
    void                    reloadBoardsView();
    void                    reloadThreadsView();

    void                    loadConnections();

    void                    appendChat(QString string, DRServerConnection *connection);

    DRServerConnection*     selectedConnection();
    DRConnectionItem*       itemForConnection(DRServerConnection *connection);
    bool                    hasItemForConnection(DRServerConnection *connection);

    DRBoardItem*            selectedBoardForConnection(DRServerConnection *connection);

    void                    toolbarActionClicked();
    bool                    executeCommand(QString string, DRServerConnection *connection);
};





class DRConnectionItemDelegate : public QItemDelegate
{
public:
  DRConnectionItemDelegate()
  {}
  QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const
  {
    return QSize(option.rect.size().width(), 22);
  }
};


#endif // DRMAINWINDOW_H
