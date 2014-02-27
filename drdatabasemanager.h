#ifndef DRDATABASEMANAGER_H
#define DRDATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QFile>

//class DRMessage;
//class DRPrivateMessage;
//class DRBroadcastMessage;
//class DRChatMessage;
//class DREventMessage;

class DRDatabaseManager : public QObject
    {
    public:
        static DRDatabaseManager* instance(QObject* parent = 0);
        static void drop();

        DRDatabaseManager(QObject *parent = 0);
        ~DRDatabaseManager();

    public:



        bool openDB();
        bool deleteDB();
        QSqlError lastError();

        bool exec(QString sql);
        bool query(QString sql);

    private:
        QSqlDatabase db;
};

#endif // DRDATABASEMANAGER_H
