#include <QResource>
#include <QDebug>
#include <QMessageBox>
#include <QStringListModel>
#include <QStandardItemModel>
#include <QListIterator>
#include <QString>
#include <QMutex>
#include "drmainwindow.h"
#include "ui_drmainwindow.h"
#include "drconnectdialog.h"
#include "drconnectionscontroller.h"
#include "drconnectionitem.h"
#include "druserscontroller.h"
#include "drchatcontroller.h"
#include "drpreferenceswindow.h"
#include "draboutwindow.h"
#include "main.h"




static DRMainWindow          *m_Instance = 0;




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





#pragma mark -

DRMainWindow::DRMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DRMainWindow)
{
    ui->setupUi(this);




    ///////////////////////////////////////////////////////
    /* USER LIST */
    ///////////////////////////////////////////////////////
    this->connection = NULL;

    //Creating a standard item model
    this->usersModel = new QStandardItemModel(this);
    //Setting the icon size
    this->ui->userList->setIconSize(QSize(40,30));
    //Setting the model
    this->ui->userList->setModel(this->usersModel);




    ///////////////////////////////////////////////////////
    /* TREE VIEW */
    ///////////////////////////////////////////////////////
    ui->treeView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->treeView->setStyleSheet(
          "background-color: rgb(222, 228, 234);"
          "selection-color: white;"
          "selection-background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 rgb(110, 165, 218), stop: 1 rgb(33, 108, 183))"
    );

    this->treeModel = new QStandardItemModel();
    this->rootNode = this->treeModel->invisibleRootItem();

    //defining a couple of items
    this->connectionsNode = new DRConnectionItem("CONNECTIONS", NULL);
    this->connectionsNode->setIcon(QIcon(":/images/WiredServer-100.png"));
    this->connectionsNode->setSelectable(false);
    this->connectionsNode->setEditable(false);

    this->bookmarksNode = new DRConnectionItem("BOOKMARKS", NULL);
    this->bookmarksNode->setIcon(QIcon(":/images/Bookmarks.tiff"));
    this->bookmarksNode->setSelectable(false);
    this->bookmarksNode->setEditable(false);

    //building up the hierarchy
    this->rootNode->appendRow(this->connectionsNode);
    this->rootNode->appendRow(this->bookmarksNode);

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



    ///////////////////////////////////////////////////////
    /* CONNECTIONS CONTROLLER SIGNALS */
    ///////////////////////////////////////////////////////
    DRConnectionsController *c = DRConnectionsController::instance();
    QObject::connect(c, SIGNAL(connectionAdded(DRConnection*)), this, SLOT(connectionAdded(DRConnection*)));
}




DRMainWindow::~DRMainWindow() {
    delete ui;

    if(connection != NULL) delete connection;

    if(treeModel != NULL) delete treeModel;
    if(usersModel != NULL) delete usersModel;
}








#pragma mark -

void DRMainWindow::setConnection(DRConnection *connection) {
    this->connection = connection;

    if(connection->connected) {
        this->setWindowTitle(QString(wi_string_cstring(this->connection->server->name)));

        QObject::connect(this->connection, SIGNAL(receivedError(wi_p7_message_t *)), this, SLOT(receivedError(wi_p7_message_t *)));

        QObject::connect(this->connection->chatController, SIGNAL(chatControllerReceivedChatMe(DRConnection*,QString,DRUser*)), this, SLOT(chatControllerReceivedChatMe(DRConnection*,QString,DRUser*)));
        QObject::connect(this->connection->chatController, SIGNAL(chatControllerReceivedChatSay(DRConnection*,QString,DRUser*)), this, SLOT(chatControllerReceivedChatSay(DRConnection*,QString,DRUser*)));
        QObject::connect(this->connection->chatController, SIGNAL(chatControllerTopicChanged(DRConnection*,DRTopic*)), this, SLOT(chatControllerTopicChanged(DRConnection*,DRTopic*)));

        QObject::connect(this->connection->usersController, SIGNAL(usersControllerUserListLoaded(DRConnection *)), this, SLOT(usersControllerUserListLoaded(DRConnection *)));
        QObject::connect(this->connection->usersController, SIGNAL(usersControllerUserJoined(DRConnection *, DRUser*)), this, SLOT(usersControllerUserJoined(DRConnection *, DRUser*)));
        QObject::connect(this->connection->usersController, SIGNAL(usersControllerUserLeave(DRConnection *, DRUser*)), this, SLOT(usersControllerUserLeave(DRConnection *, DRUser*)));
    } else {
        this->setWindowTitle(QString("Deriv Client"));
    }
}






#pragma mark -

void DRMainWindow::disconnect() {
    DRConnection *connection = NULL;

    connection = this->selectedConnection();

    if(connection == NULL)
        return;

    DRConnectionsController::instance()->removeConnection(connection);

    connection->disconnect();

    QObject::disconnect(this, SLOT(chatControllerReceivedChatMe(QString,DRUser*)));
    QObject::disconnect(this, SLOT(chatControllerReceivedChatSay(QString,DRUser*)));
    QObject::disconnect(this, SLOT(chatControllerTopicChanged(DRTopic*)));

    QObject::disconnect(this, SLOT(usersControllerUserListLoaded()));
    QObject::disconnect(this, SLOT(usersControllerUserJoined(DRUser*)));
    QObject::disconnect(this, SLOT(usersControllerUserLeave(DRUser*)));

    this->reloadTreeView();
    this->reloadUserList();

    ui->chatOutputTextEdit->clear();
    ui->topicLabel->setText("");
}





void DRMainWindow::receivedError(wi_p7_message_t *message) {
    QMessageBox msgBox;
    wi_string_t *error;

    error = wi_p7_message_string_for_name(message, WI_STR("wired.error.string"));

    msgBox.setText("Connection Error");
    msgBox.setInformativeText(QString(wi_string_cstring(error)));
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);

    msgBox.exec();
}







#pragma mark -

void DRMainWindow::chatControllerReceivedChatSay(DRConnection *connection, QString string, DRUser *user) {
    ui->chatOutputTextEdit->appendPlainText(string);
}



void DRMainWindow::chatControllerReceivedChatMe(DRConnection *connection, QString string, DRUser *user) {
    ui->chatOutputTextEdit->appendPlainText(string);
}



void DRMainWindow::chatControllerTopicChanged(DRConnection *connection, DRTopic *topic) {
    QString topicString = *topic->topic + QString(" by ") + *topic->nick + QString(" at ") + topic->time->toString();
    ui->topicLabel->setText(topicString);

    QString string = QString("<< Topic changed to: ") + *topic->topic + QString(" by ") + *topic->nick + QString(" >>");
    ui->chatOutputTextEdit->appendPlainText(string);
}



void DRMainWindow::usersControllerUserListLoaded(DRConnection *connection) {
    this->reloadUserList();
}






#pragma mark -

void DRMainWindow::usersControllerUserJoined(DRConnection *connection, DRUser *user) {
    QString nick = QString(wi_string_cstring(user->nick));
    QString string = QString("<< ") + nick + QString(" has joined >>");

    ui->chatOutputTextEdit->appendPlainText(string);
}



void DRMainWindow::usersControllerUserLeave(DRConnection *connection, DRUser *user) {
    QString nick = QString(wi_string_cstring(user->nick));
    QString string = QString("<< ") + nick + QString(" has left >>");

    ui->chatOutputTextEdit->appendPlainText(string);
}







#pragma mark -

void DRMainWindow::reloadTreeView() {
    QList<DRConnection*> *connections = DRConnectionsController::instance()->connections;
    QListIterator<DRConnection*> it(*connections);

    this->connectionsNode->removeRows(0, this->connectionsNode->rowCount());

    while(it.hasNext()) {
        DRConnection *connection = it.next();
        DRConnectionItem *item = new DRConnectionItem(QString(wi_string_cstring(connection->server->name)), connection);

        this->connectionsNode->appendRow(item);
    }

    ui->treeView->expandAll();
}



void DRMainWindow::reloadUserList() {
    DRConnection *connection = NULL;

    connection = this->selectedConnection();

    this->usersModel->clear();

    if(connection == NULL)
        return;

    if(connection->connected) {
        QVectorIterator<DRUser*> i(*connection->usersController->users);

         while (i.hasNext()) {
            DRUser *user = i.next();

            const QString base64icon = QString(wi_string_cstring(wi_data_base64(user->icon)));

            QByteArray base64Data = base64icon.toUtf8();

            QImage image;
            image.loadFromData(QByteArray::fromBase64(base64Data));

            QStandardItem* item = new QStandardItem(QIcon(QPixmap::fromImage(image)), QString(wi_string_cstring(user->nick)));
            this->usersModel->appendRow(item);
         }
    }

     ui->userList->repaint();
}






wi_data_t *DRMainWindow::getBase64DefaultUserIcon() {
    QResource resource("/images/user-icon.png");

    QFile* file = new QFile((resource.absoluteFilePath()));

    file->open(QIODevice::ReadOnly);

    const QByteArray image = file->readAll();

    wi_data_t *data = wi_data_with_bytes(image.constData(), image.size()+1);

    file->close();

    if(data == NULL || wi_data_length(data) <= 0)
        return NULL;

    return data;
}




DRConnection* DRMainWindow::selectedConnection() {
    DRConnection        *connection;
    DRConnectionItem    *item;
    QModelIndex          itemIndex;

    connection  = NULL;

    if(ui->treeView->selectionModel()->selectedIndexes().length() <= 0)
        return NULL;

    itemIndex   = ui->treeView->selectionModel()->selectedIndexes()[0];

    if(itemIndex.row() == -1)
        return NULL;

    item        = (DRConnectionItem *)this->treeModel->itemFromIndex(itemIndex);

    if(item == NULL)
        return NULL;

    connection  = item->connection;

    return connection;
}




DRConnectionItem* DRMainWindow::itemForConnection(DRConnection *connection) {
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




bool DRMainWindow::hasItemForConnection(DRConnection *connection) {
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









#pragma mark -

void DRMainWindow::treeViewSelectionDidChange() {
    DRConnection            *connection;
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
    }
}



void DRMainWindow::treeViewContextMenu(const QPoint &point) {
    qDebug() << "treeViewContextMenu";

    QStandardItem *item = this->treeModel->itemFromIndex(ui->treeView->indexAt(point));

    DRConnectionItem *connectionItem = dynamic_cast<DRConnectionItem*>(item);

    if(item && item->isSelectable()){
        if (dynamic_cast<DRConnectionItem*>(item)) {
            QMenu *menu = new QMenu;

            if(connectionItem->connection->connected)
                 menu->addAction(QString("Disconnect"), this, SLOT(disconnect()));

            menu->exec(QCursor::pos());
        }
    }
}



void DRMainWindow::connectionAdded(DRConnection* connection) {
    this->reloadTreeView();

    DRConnectionItem *item = this->itemForConnection(connection);

    if(item == NULL)
        return;

    QModelIndex itemIndex = this->treeModel->indexFromItem(item);
    ui->treeView->selectionModel()->select(itemIndex, QItemSelectionModel::Select);
}




void DRMainWindow::on_chatInputEdit_returnPressed() {
    DRConnection *connection;
    wi_p7_message_t *message;
    wi_string_t *string;

    connection = this->selectedConnection();

    if(connection == NULL)
        return;

    string = wi_string_with_cstring(ui->chatInputEdit->text().toStdString().c_str());

    message = wi_p7_message_with_name(WI_STR("wired.chat.send_say"), wc_spec);
    wi_p7_message_set_uint32_for_name(message, 1, WI_STR("wired.chat.id"));
    wi_p7_message_set_string_for_name(message, string, WI_STR("wired.chat.say"));

    connection->sendMessage(message);
    ui->chatInputEdit->setText("");
}



void DRMainWindow::on_actionNewConnection_triggered()
{
    DRConnectDialog connectDialog(this);
    connectDialog.exec();
}




void DRMainWindow::on_actionPreferences_triggered()
{
    DRPreferencesWindow::instance()->show();
}



void DRMainWindow::on_actionAbout_triggered()
{
    DRAboutWindow::instance()->show();
}
