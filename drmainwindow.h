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
#include <wired/wired.h>

#include "drconnection.h"
#include "drconnectionitem.h"
#include "druser.h"
#include "drtopic.h"
#include "drerror.h"



namespace Ui {
class DRMainWindow;
}

class DRMainWindow : public QMainWindow
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

    void                    selectConnection(DRConnection *connection);
    void                    setConnection(DRConnection *connection);

public slots:
    void                    disconnect();
    void                    removeConnection();
    void                    editConnection();

    void                    connectionSucceeded(DRConnection *connection);
    void                    connectionError(DRConnection *connection, DRError* error);
    void                    connectionClosed(DRConnection *connection, DRError *error);

    void                    receivedError(wi_p7_message_t *message);
    void                    on_actionNewConnection_triggered();

private slots:
    void                    treeViewSelectionDidChange();
    void                    treeViewDoubleClicked(QModelIndex index);
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
    QProgressBar            *progressBar;
    QLabel                  *progressLabel;

    void                    reloadTreeView();
    void                    reloadUserList();
    void                    reloadChatView();

    void                    loadConnections();
    void                    saveConnections();

    void                    appendChat(QString string, DRConnection *connection);

    DRConnection*           selectedConnection();
    DRConnectionItem*       itemForConnection(DRConnection *connection);
    bool                    hasItemForConnection(DRConnection *connection);
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
