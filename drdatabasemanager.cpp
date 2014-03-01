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



#include <QMutex>
#include <QSqlQuery>
#include <QDir>
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
