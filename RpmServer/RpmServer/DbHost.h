#ifndef DBHOST_H
#define DBHOST_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <string>

class DbHost {
    public:
        DbHost (std::string drv, std::string dbName, std::string user = "", std::string pw = "", bool _echo = false);
        ~DbHost ();

        void populate (time_t timestamp, int rpm, char validity);

    private:
        QSqlDatabase db;
        QSqlQuery *query;
        bool echo;
};

#endif // DBHOST_H
