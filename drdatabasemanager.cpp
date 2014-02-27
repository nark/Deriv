#include <QMutex>
#include <QSqlQuery>
#include "drdatabasemanager.h"



static DRDatabaseManager          *m_Instance = 0;



#pragma mark -

DRDatabaseManager*  DRDatabaseManager::instance(QObject* parent) {
    static QMutex mutex;
    if (!m_Instance) {
        mutex.lock();

        if (!m_Instance)
            m_Instance = new DRDatabaseManager(parent);

        mutex.unlock();
    }
    return m_Instance;
}




void DRDatabaseManager::drop() {
    static QMutex mutex;
    mutex.lock();
    delete m_Instance;
    m_Instance = 0;
    mutex.unlock();
}





#pragma mark -

DRDatabaseManager::DRDatabaseManager(QObject *parent) :
    QObject(parent)
{
}


DRDatabaseManager::~DRDatabaseManager() {

}





#pragma mark -

bool DRDatabaseManager::openDB() {
    // Find QSLite driver
    db = QSqlDatabase::addDatabase("QSQLITE");

    #ifdef Q_OS_LINUX
    // NOTE: We have to store database file into user home folder in Linux
    QString path(QDir::home().path());
    path.append(QDir::separator()).append(".deriv/database.sqlite3");
    path = QDir::toNativeSeparators(path);
    db.setDatabaseName(path);
    #else
    // NOTE: File exists in the application private folder, in Symbian Qt implementation
    db.setDatabaseName("database.sqlite3");
    #endif

    // Open databasee
    return db.open();
}

QSqlError DRDatabaseManager::lastError() {
    // If opening database has failed user can ask
    // error description by QSqlError::text()
    return db.lastError();
}

bool DRDatabaseManager::deleteDB() {
    // Close database
    db.close();

    #ifdef Q_OS_LINUX
    // NOTE: We have to store database file into user home folder in Linux
    QString path(QDir::home().path());
    path.append(QDir::separator()).append(".deriv/database.sqlite3");
    path = QDir::toNativeSeparators(path);
    return QFile::remove(path);
    #else

    // Remove created database binary file
    return QFile::remove("database.sqlite3");
    #endif
}




#pragma mark -

bool exec(QString sql) {
    QSqlQuery query;

    return query.exec(sql);
}



bool query(QString sql) {
    QSqlQuery query;

    return query.exec(sql);
}
