
#include <QListIterator>
#include "drconnectionscontroller.h"




static DRConnectionsController          *m_Instance = 0;


#pragma mark -

DRConnectionsController*  DRConnectionsController::instance(QObject* parent) {
    static QMutex mutex;
    if (!m_Instance) {
        mutex.lock();
        if (!m_Instance)
            m_Instance = new DRConnectionsController(parent);
        mutex.unlock();
    }
    return m_Instance;
}




void DRConnectionsController::drop() {
    static QMutex mutex;
    mutex.lock();
    delete m_Instance;
    m_Instance = 0;
    mutex.unlock();
}





#pragma mark -

DRConnectionsController::DRConnectionsController(QObject* parent) : QObject( parent ) {
    this->connections = new QList<DRConnection *>();
}





#pragma mark -

void DRConnectionsController::addConnection(DRConnection *connection) {
    if(this->hasConnection(connection))
        return;

    this->connections->push_back(connection);

    emit connectionAdded(connection);
}



void DRConnectionsController::removeConnection(DRConnection *connection) {
    if(!this->hasConnection(connection))
        return;

    int index = this->connections->indexOf(connection);
    this->connections->removeAt(index);

    emit connectionRemoved(connection);
}



bool DRConnectionsController::hasConnection(DRConnection *connection) {
    DRConnection            *existing;
    int                     index;

    existing    = NULL;
    index       = this->connections->indexOf(connection);

    if(index >= 0) {
        existing = this->connectionAtIndex(index);

        if(existing != NULL)
            return true;
    }
    return false;
}





#pragma mark -

DRConnection* DRConnectionsController::connectionAtIndex(int index) {
    DRConnection *connection = NULL;

    if(index >= 0) {
        connection = this->connections->at(index);
    }

    return connection;
}




DRConnection* DRConnectionsController::connectionForIdentifier(QString identifier) {
    QListIterator<DRConnection*> it(*this->connections);

    while (it.hasNext()) {
        DRConnection *connection = it.next();
        if(connection->URLIdentifier() == identifier)
            return connection;
    }
    return NULL;
}




DRConnection* DRConnectionsController::connectionForURL(wi_url_t *url) {
    QListIterator<DRConnection*> it(*this->connections);
    QString identifier = "";

    identifier += QString(wi_string_cstring(wi_url_user(url)));
    identifier += QString("@");
    identifier += QString(wi_string_cstring(wi_url_host(url)));

    while (it.hasNext()) {
        DRConnection *connection = it.next();
        if(connection->URLIdentifier() == identifier)
            return connection;
    }
    return NULL;
}
