#ifndef DBHOST_H
#define DBHOST_H

#include <QtCore>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <string>
#include <mutex>
#include "DataStorage.h"

class DbHost: public QObject {
    public:
        DbHost (
            std::string drv,
            std::string hostName,
            std::string dbName,
            std::string schemaName,
            std::string tableName,
            std::string user = "",
            std::string pw = "",
            bool _echo = false,
            QObject *_parent = nullptr
        );
        ~DbHost ();

        void populate (time_t timestamp, int rpm, Validity validity);
        bool request (time_t& timestamp, int& rpm, Validity& validity);

    private:
        std::string schemaName, tableName;
        std::mutex mutex;
        QSqlDatabase db;
        QSqlQuery *query;
        bool echo;
};

#endif // DBHOST_H
