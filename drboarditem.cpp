#include <QIcon>
#include <QFileInfo>
#include "drboarditem.h"
#include "drthreaditem.h"

DRBoardItem::DRBoardItem(const QString &name, const QString &path, DRServerConnection *connection, DRBoardItem *parent) :
    DRBoardNodeItem(connection, parent)
{
    this->_path = path;
    this->_name = QFileInfo(this->_path).fileName();
    this->_threadsModel = new DRThreadsListModel(connection);

    //this->boardThreads = *new QList<DRThreadItem*>();
    //this->setIcon(QIcon(":/images/Board.png"));
}




DRBoardItem::~DRBoardItem() {
    delete this->_threadsModel;
}




DRThreadsListModel * DRBoardItem::getThreadsModel() {
    return this->_threadsModel;
}




void DRBoardItem::addThread(DRThreadItem *thread) {
    this->_threadsModel->appendRow(thread);
}



void DRBoardItem::clearThreads() {
    //this->mutex.lock();
    this->_threadsModel->clear();
    //this->mutex.unlock();
}



QString DRBoardItem::path() {
    return this->_path;
}




QString DRBoardItem::name() {
    return this->_name;
}


QVariant DRBoardItem::data(int column) const {
    return this->_name;
}
