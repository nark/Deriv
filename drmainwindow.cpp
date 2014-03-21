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



#include <QResource>
#include <QDebug>
#include <QMessageBox>
#include <QStringListModel>
#include <QStandardItemModel>
#include <QListIterator>
#include <QString>
#include <QMutex>
#include <QProgressBar>
#include <qtkeychain/keychain.h>
#include "drmainwindow.h"
#include "ui_drmainwindow.h"
#include "drconnectdialog.h"
#include "drconnectionscontroller.h"
#include "drconnectionitem.h"
#include "druserscontroller.h"
#include "drchatcontroller.h"
#include "drfilescontroller.h"
#include "drboardscontroller.h"
#include "drpreferenceswindow.h"
#include "draboutwindow.h"
#include "druseritemdelegate.h"
#include "dreditconnectionwindow.h"
#include "drserverinfodialog.h"
#include "main.h"
#include "dr.h"


using namespace QKeychain;


static DRMainWindow         *m_Instance = 0;
static QAction              *selectedAction = NULL;




#pragma mark -

DRMainWindow*  DRMainWindow::instance(QWidget* parent) {
    static QMutex mutex;
    if (!m_Instance) {
        mutex.lock();

        if (!m_Instance)
            m_Instance = new DRMainWindow(parent);

        mutex.unlock();
    }
    return m_Instance;
}




void DRMainWindow::drop() {
    static QMutex mutex;
    mutex.lock();
    delete m_Instance;
    m_Instance = 0;
    mutex.unlock();
}





void DRMainWindow::startProgress(QString title) {
    m_Instance->progressBar->setHidden(false);

    if(title != NULL)
        m_Instance->progressLabel->setText(title);
    else
        m_Instance->progressLabel->setText("");
}




void DRMainWindow::stopProgress(QString title) {
    m_Instance->progressBar->setHidden(true);

    if(title != NULL)
        m_Instance->progressLabel->setText(title);
    else
        m_Instance->progressLabel->setText("");
}



#pragma mark -

DRMainWindow::DRMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DRMainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle(QString("Deriv Client"));
    QMainWindow::setUnifiedTitleAndToolBarOnMac(true);

    ui->tabWidget->tabBar()->hide();

    // populate chat commands
    this->chatCommands.push_front("/help");
    this->chatCommands.push_front("/me");
    //this->chatCommands.push_front("/exec");
    this->chatCommands.push_front("/nick");
    this->chatCommands.push_front("/status");
    //this->chatCommands.push_front("/stats");
    this->chatCommands.push_front("/clear");
    this->chatCommands.push_front("/topic");
    this->chatCommands.push_front("/broadcast");
    //this->chatCommands.push_front("/ping");
    this->chatCommands.push_front("/afk");
    //this->chatCommands.push_front("/img");
    //this->chatCommands.push_front("/html");
    //this->chatCommands.push_front("/itunes");
    //this->chatCommands.push_front("/youtube");
    //this->chatCommands.push_front("/utube");


    ///////////////////////////////////////////////////////
    /* BOARDS TREE */
    ///////////////////////////////////////////////////////
    ui->boardsTreeView->header()->hide();
    //ui->boardsTreeView->setIndentation(8);
    ui->boardsTreeView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->boardsTreeView->setStyleSheet(
          "background-color: rgb(222, 228, 234);"
          "selection-color: white;"
          "selection-background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 rgb(110, 165, 218), stop: 1 rgb(33, 108, 183))"
    );



    ///////////////////////////////////////////////////////
    /* USER LIST */
    ///////////////////////////////////////////////////////
    this->connection = NULL;

    //Creating a standard item model
    this->usersModel = new QStandardItemModel(this);
    //Setting the icon size
    this->ui->userList->setIconSize(QSize(40,30));
    this->ui->userList->setAlternatingRowColors(true);
    //Setting the model
    this->ui->userList->setModel(this->usersModel);
    this->ui->userList->setItemDelegate(new DRUserItemDelegate());



    ///////////////////////////////////////////////////////
    /* TREE VIEW */
    ///////////////////////////////////////////////////////
    ui->treeView->setIndentation(8);
    ui->treeView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->treeView->setItemDelegate(new DRConnectionItemDelegate());
    ui->treeView->setStyleSheet(
          "background-color: rgb(222, 228, 234);"
          "selection-color: white;"
          "selection-background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 rgb(110, 165, 218), stop: 1 rgb(33, 108, 183))"
    );

    this->treeModel = new QStandardItemModel();
    this->rootNode = this->treeModel->invisibleRootItem();

    //defining a couple of items
    this->connectionsNode = new DRConnectionItem("CONNECTIONS", NULL);
    this->connectionsNode->setSelectable(false);
    this->connectionsNode->setEditable(false);
    this->connectionsNode->setFont(QFont("Lucida Grande", 11, 500));
    this->connectionsNode->setForeground(Qt::darkGray);

    this->bookmarksNode = new DRConnectionItem("BOOKMARKS", NULL);
    this->bookmarksNode->setSelectable(false);
    this->bookmarksNode->setEditable(false);

    //building up the hierarchy
    this->rootNode->appendRow(this->connectionsNode);
    //this->rootNode->appendRow(this->bookmarksNode);

    //register the model
    ui->treeView->setModel(this->treeModel);
    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->treeView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->treeView->setHeaderHidden(true);
    ui->treeView->expandAll();

    // selection changed slot
    QObject::connect(ui->treeView->selectionModel(),
                     SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
                     this,
                     SLOT(treeViewSelectionDidChange()));

    QObject::connect(ui->treeView,
                     SIGNAL(customContextMenuRequested(const QPoint &)),
                     this,
                     SLOT(treeViewContextMenu(const QPoint &)));

    QObject::connect(ui->treeView,
                     SIGNAL(doubleClicked(QModelIndex)),
                     this,
                     SLOT(treeViewDoubleClicked(QModelIndex)));







    ///////////////////////////////////////////////////////
    /* CONNECTIONS CONTROLLER SIGNALS */
    ///////////////////////////////////////////////////////
    DRConnectionsController *c = DRConnectionsController::instance();
    QObject::connect(c, SIGNAL(connectionAdded(DRServerConnection*)), this, SLOT(connectionAdded(DRServerConnection*)));




    ///////////////////////////////////////////////////////
    /* LOAD CONNECTIONS */
    ///////////////////////////////////////////////////////
    this->loadConnections();
    this->reloadTreeView();

    // add the progress controls
    this->progressBar = new QProgressBar();
    this->progressBar->setMaximumWidth(60);
    this->progressBar->setValue(-1);
    this->progressBar->setMaximum(0);
    this->progressBar->setMinimum(0);
    this->progressBar->setHidden(true);

    this->progressLabel = new QLabel();
    this->progressLabel->setText("");

    this->ui->statusBar->addWidget(this->progressBar);
    this->ui->statusBar->addWidget(this->progressLabel);

    selectedAction = ui->actionChat;
    this->toolbarActionClicked();
}




DRMainWindow::~DRMainWindow() {
    delete ui;

    if(connection != NULL) delete connection;

    if(treeModel != NULL) delete treeModel;
    if(usersModel != NULL) delete usersModel;
}





void DRMainWindow::close() {
   this->saveConnections();

    QMainWindow::close();
}





#pragma mark -

void DRMainWindow::selectConnection(DRServerConnection *connection) {
    DRConnectionItem *item = this->itemForConnection(connection);

    if(item == NULL)
        return;

    QModelIndex itemIndex = this->treeModel->indexFromItem(item);

    if(itemIndex.row() == -1)
        return;

    ui->treeView->selectionModel()->clear();
    ui->treeView->selectionModel()->select(itemIndex, QItemSelectionModel::Select);

    this->setConnection(connection);
    this->reloadUserList();
    this->reloadChatView();
    this->reloadBoardsView();
    this->reloadFilesView();
}





void DRMainWindow::setConnection(DRServerConnection *connection) {
    if(this->connection != NULL) {
        if(this->connection->chatController != NULL)
            this->connection->chatController->disconnectReceiver(this);

        if(this->connection->usersController != NULL)
            this->connection->usersController->disconnectReceiver(this);

        if(this->connection->filesController != NULL)
            this->connection->filesController->disconnectReceiver(this);

        if(this->connection->boardsController != NULL)
            this->connection->boardsController->disconnectReceiver(this);
    }

    this->connection = connection;

    if(connection->connected) {
        ui->topicLabel->setText("");

        this->connection->chatController->connectReceiver(this);
        this->connection->usersController->connectReceiver(this);
        this->connection->filesController->connectReceiver(this);
        this->connection->boardsController->connectReceiver(this);

        ///////////////////////////////////////////////////////
        /* MODELS */
        ///////////////////////////////////////////////////////
        ui->filesTreeView->setModel(connection->filesController->treeModel);
        //ui->filesTreeView->repaint();

        ui->boardsTreeView->setModel(connection->boardsController->boardsModel);

        QObject::connect(ui->boardsTreeView->selectionModel(),
                         SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
                         this,
                         SLOT(boardsTreeViewSelectionDidChange()));
    }
}







#pragma mark -

void DRMainWindow::connect() {
    DRServerConnection *connection;

    connection = this->selectedConnection();

    if(connection == NULL)
        return;

    if(connection->connected == false && connection->connecting == false) {
        DRMainWindow::startProgress(QString("Connecting to %1").arg(connection->URLIdentifier()));

        connection->addDelegateForConnection(this);
        connection->connect(this);
    }
}




void DRMainWindow::disconnect() {
    DRServerConnection *connection = NULL;

    connection = this->selectedConnection();

    if(connection == NULL)
        return;

    if(connection->connected)
        connection->disconnect();

    this->reloadTreeView();
    this->reloadUserList();

    ui->chatOutputTextEdit->clear();
    ui->topicLabel->setText("");
}




void DRMainWindow::removeConnection() {
    DRServerConnection *connection = NULL;

    connection = this->selectedConnection();

    if(connection == NULL)
        return;

    if(connection->connected)
        connection->disconnect();

    DRConnectionsController::instance()->removeConnection(connection);

    this->saveConnections();

    this->reloadTreeView();
    this->reloadUserList();

    ui->chatOutputTextEdit->clear();
    ui->topicLabel->setText("");
}




void DRMainWindow::editConnection() {
    DRServerConnection *connection = NULL;

    connection = this->selectedConnection();

    if(connection == NULL)
        return;

    // TODO: release !!!
    DREditConnectionWindow *editConnectionWindow = new DREditConnectionWindow(this);
    editConnectionWindow->setConnection(connection);
    editConnectionWindow->show();
}



void DRMainWindow::connectionInfo() {
    DRServerConnection *connection = NULL;

    connection = this->selectedConnection();

    if(connection == NULL)
        return;

    DRServerInfoDialog *infoDialog = new DRServerInfoDialog(this);
    infoDialog->setConnection(connection);
    infoDialog->show();
}



void DRMainWindow::receivedError(DRError *error) {
    DR::showError(error, this);
}





#pragma mark -

void DRMainWindow::chatControllerReceivedChatSay(DRServerConnection *connection, QString string, DRUser *user) {
    //qDebug() << "chatControllerReceivedChatSay";
    this->appendChat(string, connection);
}



void DRMainWindow::chatControllerReceivedChatMe(DRServerConnection *connection, QString string, DRUser *user) {
    this->appendChat(string, connection);
}



void DRMainWindow::chatControllerTopicChanged(DRServerConnection *connection, DRTopic *topic, bool init) {
    DRServerConnection *selectedConnection = this->selectedConnection();

    if(connection == selectedConnection)
        ui->topicLabel->setText(topic->formattedTopicByUserWithDate());

    if(!init) {
        QString string = QString("<< Topic changed to: ") + topic->formattedTopicByUser() + QString(" >>");
        this->appendChat(string, connection);
    }
}



void DRMainWindow::usersControllerUserListLoaded(DRServerConnection *connection) {
    //qDebug() << "usersControllerUserListLoaded";
    this->reloadUserList();
}






#pragma mark -

void DRMainWindow::usersControllerUserJoined(DRServerConnection *connection, DRUser *user) {
    QString string = QString("<< ") + user->nick + QString(" has joined >>");

    this->appendChat(string, connection);
}



void DRMainWindow::usersControllerUserLeave(DRServerConnection *connection, DRUser *user) {
    QString string = QString("<< ") + user->nick + QString(" has left >>");

    this->appendChat(string, connection);
}







void DRMainWindow::filesReloaded(DRServerConnection *connection) {
    qDebug() << "DRMainWindow::filesReloaded";
//    ui->filesTreeView->setItemsExpandable(true);
//    ui->filesTreeView->repaint();
}








#pragma mark -

void DRMainWindow::reloadTreeView() {
    QList<DRServerConnection*> *connections = DRConnectionsController::instance()->connections;
    QListIterator<DRServerConnection*> it(*connections);

    this->connectionsNode->removeRows(0, this->connectionsNode->rowCount());

    while(it.hasNext()) {
        DRServerConnection *connection = it.next();
        DRConnectionItem *item = NULL;

        if(!connection->serverName.isNull() && connection->serverName.length() > 0) {
            item = new DRConnectionItem(connection->serverName, connection);
        } else {
            item = new DRConnectionItem(connection->URLIdentifier(), connection);
        }
        if(connection->connected) {
            item->setIcon(QIcon(":/images/user-icon.png"));
        } else {
            item->setIcon(QIcon(":/images/WiredServer.png"));
        }
        item->setEditable(false);
        this->connectionsNode->appendRow(item);
    }

    ui->treeView->expandAll();
}



void DRMainWindow::reloadUserList() {
    DRServerConnection *connection = NULL;

    connection = this->selectedConnection();

    this->usersModel->clear();

    if(connection == NULL)
        return;

    if(connection->connected) {
        QVectorIterator<DRUser*> i(*connection->usersController->users);

         while (i.hasNext()) {
            DRUser *user = i.next();
            QStandardItem *item = new QStandardItem();
            item->setData(user->nick, DRUserItemDelegate::headerTextRole);
            item->setData(user->status, DRUserItemDelegate::subHeaderTextrole);
            item->setData(user->icon, DRUserItemDelegate::IconRole);
            item->setData(user->isIdle, DRUserItemDelegate::IdleRole);
            item->setEditable(false);
            this->usersModel->appendRow(item);
         }
    }

     ui->userList->repaint();
}





void DRMainWindow::reloadChatView() {
    DRServerConnection *connection = NULL;

    connection = this->selectedConnection();

    if(connection == NULL) {
        ui->chatOutputTextEdit->setPlainText("");
        return;
    }

    DRChatController *chatController = connection->chatController;

    if(connection->connected) {
        ui->chatOutputTextEdit->setPlainText(*chatController->chatBuffer);

        if(chatController->topic != NULL) {
            ui->topicLabel->setText(chatController->topic->formattedTopicByUserWithDate());
            return;
        }
    }
    ui->topicLabel->setText("");
}





void DRMainWindow::reloadFilesView() {
    DRServerConnection *connection = NULL;

    connection = this->selectedConnection();

    if(connection == NULL) {

        return;
    }

    if(connection->filesController != NULL)
        ui->filesTreeView->setModel(connection->filesController->treeModel);
}





void DRMainWindow::reloadBoardsView() {
    DRServerConnection *connection = NULL;

    connection = this->selectedConnection();

    if(connection == NULL) {
        return;
    }

    if(connection->boardsController != NULL) {
        ui->boardsTreeView->setModel(connection->boardsController->boardsModel);
        ui->boardsTreeView->expandAll();

        this->reloadThreadsView();
    }
}




void DRMainWindow::reloadThreadsView() {
    DRServerConnection *connection = NULL;

    connection = this->selectedConnection();

    if(connection == NULL)
        return;

    if(connection->boardsController != NULL) {
        // clear the model
        connection->boardsController->threadsModel->clear();

        qDebug() << connection->URLIdentifier();

        // get selected board
        DRBoardItem *board = this->selectedBoardForConnection(connection);

        if(board == NULL)
            return;

        qDebug() << board->path();

        // set the board model to the view
        ui->threadsListView->setModel(board->getThreadsModel());
    }
}





void DRMainWindow::appendChat(QString string, DRServerConnection *connection) {
    DRServerConnection *selectedConnection = this->selectedConnection();

    if(connection == selectedConnection) {
        ui->chatOutputTextEdit->appendPlainText(string);
    }

    connection->chatController->chatBuffer->append(string+"\n");
}








DRServerConnection* DRMainWindow::selectedConnection() {
    DRServerConnection        *connection;
    DRConnectionItem    *item;
    QModelIndex          itemIndex;

    connection  = NULL;

    if(ui->treeView->selectionModel()->selectedIndexes().length() <= 0)
        return NULL;

    itemIndex = ui->treeView->selectionModel()->selectedIndexes()[0];

    if(itemIndex.row() == -1)
        return NULL;

    item = (DRConnectionItem *)this->treeModel->itemFromIndex(itemIndex);

    if(item == NULL)
        return NULL;

    connection = item->connection;

    return connection;
}




DRConnectionItem* DRMainWindow::itemForConnection(DRServerConnection *connection) {
    if(this->connectionsNode->hasChildren()) {
        for(int i = 0; i < this->connectionsNode->rowCount(); i++) {
            DRConnectionItem *ci = (DRConnectionItem *)this->connectionsNode->child(i);
            if(ci->connection == connection) {
                return ci;
            }
        }
    }
    return NULL;
}




bool DRMainWindow::hasItemForConnection(DRServerConnection *connection) {
    if(this->connectionsNode->hasChildren()) {
        for(int i = 0; i < this->connectionsNode->rowCount(); i++) {
            DRConnectionItem *ci = (DRConnectionItem *)this->connectionsNode->child(i);
            if(ci->connection == connection) {
                return true;
            }
        }
    }
    return false;
}









DRBoardItem* DRMainWindow::selectedBoardForConnection(DRServerConnection *connection) {
    DRBoardItem *item;
    QModelIndex itemIndex;

    if(connection == NULL || !connection->connected)
        return NULL;

    item = NULL;

    if(ui->boardsTreeView->selectionModel()->selectedIndexes().length() <= 0)
        return NULL;

    itemIndex = ui->boardsTreeView->selectionModel()->selectedIndexes()[0];

    if(itemIndex.row() == -1)
        return NULL;

    item = (DRBoardItem *)connection->boardsController->boardsModel->boardAtIndex(itemIndex);

    return item;
}










void DRMainWindow::connectionSucceeded(DRServerConnection *connection) {
    DRMainWindow::stopProgress(NULL);

    DRConnectionsController::instance()->addConnection(this->connection);

    this->reloadTreeView();
    this->reloadChatView();
    this->reloadUserList();
    this->reloadBoardsView();
    this->reloadFilesView();

    DRConnectionItem *item = this->itemForConnection(connection);

    if(item == NULL)
        return;

    // selected connection in the tree
    QModelIndex itemIndex = this->treeModel->indexFromItem(item);

    if(itemIndex.row() == -1)
        return;

    ui->treeView->selectionModel()->select(itemIndex, QItemSelectionModel::Select);

    // make connection persistent
    this->saveConnections();

    // send user info (nick, status, icon)
    connection->sendUserInfo();

    // join the public chat
    connection->joinPublicChat();
}



void DRMainWindow::connectionError(DRServerConnection *connection, DRError *error) {
    DRMainWindow::stopProgress(NULL);

    DR::showError(error, this);
}



void DRMainWindow::connectionClosed(DRServerConnection *connection, DRError *error) {
    connection->connected = false;

    //QObject::disconnect(connection, SIGNAL(receivedError(DRError *)), this, SLOT(receivedError(DRError *)));

    if(connection->chatController != NULL)
        connection->chatController->disconnectReceiver(this);

    if(connection->usersController != NULL)
        connection->usersController->disconnectReceiver(this);

    if(connection->filesController != NULL)
        connection->filesController->disconnectReceiver(this);

    if(this->connection->boardsController != NULL)
        this->connection->boardsController->disconnectReceiver(this);

    this->reloadTreeView();
    this->reloadChatView();
    this->reloadUserList();

    // TODO: only handle certain domains ?
//    if(error != NULL)
//        DR::showError(error, this);
}







void DRMainWindow::connectionAdded(DRServerConnection* connection) {
    this->reloadTreeView();

    DRConnectionItem *item = this->itemForConnection(connection);

    if(item == NULL)
        return;

    QModelIndex itemIndex = this->treeModel->indexFromItem(item);

    if(itemIndex.row() == -1)
        return;

    ui->treeView->selectionModel()->select(itemIndex, QItemSelectionModel::Select);

    this->saveConnections();
}










#pragma mark -

void DRMainWindow::treeViewSelectionDidChange() {
    DRServerConnection            *connection;
    DRConnectionItem        *item;
    QModelIndex             itemIndex;

    item        = NULL;
    connection  = NULL;

    if(ui->treeView->selectionModel()->selectedIndexes().length() <= 0)
        return;

    itemIndex   = ui->treeView->selectionModel()->selectedIndexes()[0];

    if(itemIndex.row() == -1)
        return;

    item        = (DRConnectionItem *)this->treeModel->itemFromIndex(itemIndex);
    connection  = item->connection;

    if(item->connection != NULL) {
        this->setConnection(connection);

        this->reloadUserList();
        this->reloadChatView();
        this->reloadFilesView();
        this->reloadBoardsView();
    }
}





void DRMainWindow::treeViewDoubleClicked(QModelIndex index) {
    this->connect();
}




void DRMainWindow::treeViewContextMenu(const QPoint &point) {
    QStandardItem *item = this->treeModel->itemFromIndex(ui->treeView->indexAt(point));

    DRConnectionItem *connectionItem = dynamic_cast<DRConnectionItem*>(item);

    if(item && item->isSelectable()){
        if (dynamic_cast<DRConnectionItem*>(item)) {
            QMenu *menu = new QMenu;

            if(connectionItem->connection->connected) {
                menu->addAction(QString("Disconnect"), this, SLOT(disconnect()));
                menu->addAction(QString("Get Info"), this, SLOT(connectionInfo()));
            } else {
                menu->addAction(QString("Connect"), this, SLOT(connect()));
            }

            menu->addSeparator();
            menu->addAction(QString("Edit"), this, SLOT(editConnection()));
            menu->addAction(QString("Remove"), this, SLOT(removeConnection()));

            menu->exec(QCursor::pos());
        }
    }
}




void DRMainWindow::boardsTreeViewSelectionDidChange() {
    qDebug() << "boardsTreeViewSelectionDidChange";

    DRServerConnection *connection;
    DRBoardItem *item;
    QModelIndex itemIndex;

    connection  = this->selectedConnection();

    if(connection == NULL)
        return;

    item = this->selectedBoardForConnection(connection);

    this->reloadThreadsView();

    //qDebug() << item->getThreads().size();
}



void DRMainWindow::boardsTreeViewContextMenu(const QPoint &point) {

}







void DRMainWindow::loadConnections() {
    QList<QString> connectionIdentifiers;
    QList<QString> connectionNames;
    QList<bool> connectionAutoConnects;
    QList<bool> connectionAutoReconnects;
    int size = DRPreferencesWindow::instance()->settings->beginReadArray(DRConnectionIdentifiers);

    for(int i=0; i < size; i++) {
        DRPreferencesWindow::instance()->settings->setArrayIndex(i);
        connectionIdentifiers.push_front(DRPreferencesWindow::instance()->settings->value("identifier").toString());
        connectionNames.push_front(DRPreferencesWindow::instance()->settings->value("name").toString());
        connectionAutoConnects.push_front(DRPreferencesWindow::instance()->settings->value("autoConnect").toBool());
        connectionAutoReconnects.push_front(DRPreferencesWindow::instance()->settings->value("autoReconnect").toBool());
    }

    DRPreferencesWindow::instance()->settings->endArray();

    for(int i = 0; i < connectionIdentifiers.length(); i++) {
        QString identifier = connectionIdentifiers.at(i);
        QString name = connectionNames.at(i);

        QString urlString = QString("wiredp7://") + identifier;
        wi_url_t *url = wi_url_with_string(wi_string_with_cstring(urlString.toStdString().c_str()));

        DRServerConnection *newConnection = new DRServerConnection(url);
        newConnection->serverName = name;
        newConnection->autoConnect = connectionAutoConnects.at(i);
        newConnection->autoReconnect = connectionAutoReconnects.at(i);
        DRConnectionsController::instance()->addConnection(newConnection);
    }
}




void DRMainWindow::saveConnections() {
    DRPreferencesWindow::instance()->settings->remove(DRConnectionIdentifiers);

    DRPreferencesWindow::instance()->settings->beginWriteArray(DRConnectionIdentifiers);

    for(int i = 0; i< DRConnectionsController::instance()->connections->length(); i++){
        DRServerConnection *connection = DRConnectionsController::instance()->connectionAtIndex(i);
        DRPreferencesWindow::instance()->settings->setArrayIndex(i);
        DRPreferencesWindow::instance()->settings->setValue("identifier", connection->URLIdentifier());
        DRPreferencesWindow::instance()->settings->setValue("name", connection->serverName);
        DRPreferencesWindow::instance()->settings->setValue("autoConnect", connection->autoConnect);
        DRPreferencesWindow::instance()->settings->setValue("autoReconnect", connection->autoReconnect);
    }

    DRPreferencesWindow::instance()->settings->endArray();
    DRPreferencesWindow::instance()->settings->sync();
}





bool DRMainWindow::executeCommand(QString string, DRServerConnection *connection) {
    wi_p7_message_t *message = NULL;
    QString command = NULL;
    QString argument = NULL;

    if(string.length() == 0)
        return false;

    QStringList words = string.split(" ");
    QString helpString = "Help for Deriv chat commands"
            "\n\n"
            " /help \t Print this help message\n"
            " /me \t Send an action chat message\n"
            " /nick \t Set your nickname on this chat\n"
            " /status \t Set your status on this chat\n"
            " /topic \t Set the chat topic\n"
            " /broadcast \t Send a broadcast message\n"
            " /afk \t Go away from keyboard\n";

    if(words.size() == 1) {
       command = string;
    } else {
        command = words.at(0);
        argument = string.mid(command.length()+1, (string.length()-command.length())-1);
    }

    if(command == NULL)
        return false;

    if(command == "/me" && argument.length() > 0) {
        message = wi_p7_message_with_name(WI_STR("wired.chat.send_me"), wc_spec);

        wi_p7_message_set_uint32_for_name(message, 1, WI_STR("wired.chat.id"));
        wi_p7_message_set_string_for_name(message, QSTOWS(argument), WI_STR("wired.chat.me"));
        connection->sendMessage(message);

        return true;
    }
    else if((command == "/nick" || command == "/n") && argument.length() > 0) {
        message = wi_p7_message_with_name(WI_STR("wired.user.set_nick"), wc_spec);

        wi_p7_message_set_string_for_name(message, QSTOWS(argument), WI_STR("wired.user.nick"));
        connection->sendMessage(message);

        return true;
    }
    else if(command == "/status" && argument.length() > 0) {
        message = wi_p7_message_with_name(WI_STR("wired.user.set_status"), wc_spec);

        wi_p7_message_set_string_for_name(message, QSTOWS(argument), WI_STR("wired.user.status"));
        connection->sendMessage(message);

        return true;
    }
    else if(command == "/topic" && argument.length() > 0) {
        message = wi_p7_message_with_name(WI_STR("wired.chat.set_topic"), wc_spec);

        wi_p7_message_set_uint32_for_name(message, 1, WI_STR("wired.chat.id"));
        wi_p7_message_set_string_for_name(message, QSTOWS(argument), WI_STR("wired.chat.topic.topic"));
        connection->sendMessage(message);

        return true;
    }
    else if(command == "/broadcast" && argument.length() > 0) {
        message = wi_p7_message_with_name(WI_STR("wired.message.send_broadcast"), wc_spec);

        wi_p7_message_set_uint32_for_name(message, 1, WI_STR("wired.chat.id"));
        wi_p7_message_set_string_for_name(message, QSTOWS(argument), WI_STR("wired.message.broadcast"));
        connection->sendMessage(message);

        return true;
    }
    else if(command == "/afk" && argument.length() > 0) {
        message = wi_p7_message_with_name(WI_STR("wired.user.set_idle"), wc_spec);

        wi_p7_message_set_uint32_for_name(message, 1, WI_STR("wired.chat.id"));
        wi_p7_message_set_bool_for_name(message, true, WI_STR("wired.user.idle"));
        connection->sendMessage(message);

        return true;
    }
    else {
        this->appendChat(helpString, connection);
        return false;
    }


    return false;
}





void DRMainWindow::toolbarActionClicked() {
    for(int i = 0; i < this->ui->mainToolBar->actions().size(); i++) {
        QAction *action = this->ui->mainToolBar->actions().at(i);

        if(action->isChecked() && (selectedAction == NULL || action->text() != selectedAction->text())) {
            selectedAction = action;
            break;
        }
    }

    for(int i = 0; i < this->ui->mainToolBar->actions().size(); i++) {
        QAction *action = this->ui->mainToolBar->actions().at(i);
        action->setChecked(false);
    }

    selectedAction->setChecked(true);

    for(int i = 0; i < this->ui->tabWidget->count(); i++) {
        if(this->ui->tabWidget->tabText(i) == selectedAction->text())
            ui->tabWidget->setCurrentIndex(i);
    }
}




void DRMainWindow::on_chatInputEdit_returnPressed() {
    DRServerConnection *connection;
    wi_p7_message_t *message;
    wi_string_t *string;
    QString qstring;
    bool runCommand = false;

    connection = this->selectedConnection();

    if(connection == NULL)
        return;

    qstring = ui->chatInputEdit->text();

    if(qstring.length() == 0)
        return;

    if(qstring.at(0) == '/')
        runCommand = this->executeCommand(qstring, connection);

    if(!runCommand) {
        string = wi_string_with_cstring(qstring.toStdString().c_str());

        message = wi_p7_message_with_name(WI_STR("wired.chat.send_say"), wc_spec);
        wi_p7_message_set_uint32_for_name(message, 1, WI_STR("wired.chat.id"));
        wi_p7_message_set_string_for_name(message, string, WI_STR("wired.chat.say"));

        connection->sendMessage(message);
    }
    ui->chatInputEdit->setText("");
}



void DRMainWindow::on_actionNewConnection_triggered()
{
    DRConnectDialog connectDialog(this);
    connectDialog.exec();
}




void DRMainWindow::on_actionPreferences_triggered()
{
    qDebug() << "on_actionPreferences_triggered";
    DRPreferencesWindow::instance()->show();
}



void DRMainWindow::on_actionAbout_triggered()
{
    DRAboutWindow::instance()->show();
}

void DRMainWindow::on_actionPreferences_triggered(bool checked)
{
    qDebug() << "on_actionPreferences_triggered(checked)";
    DRPreferencesWindow::instance()->show();
}

void DRMainWindow::on_actionChat_triggered() {
    this->toolbarActionClicked();
}

void DRMainWindow::on_actionBoards_triggered()
{
    this->toolbarActionClicked();
}

void DRMainWindow::on_actionMessages_triggered()
{
    this->toolbarActionClicked();
}

void DRMainWindow::on_actionFiles_triggered()
{
    this->toolbarActionClicked();
}

void DRMainWindow::on_actionTransfers_triggered()
{
    this->toolbarActionClicked();
}
