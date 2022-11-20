#ifndef DBHOST_H
#define DBHOST_H

#include <QSqlDatabase>
#include <string>

class DbHost {
    public:
        DbHost (std::string drv, std::string dbName, std::string user = "", std::string pw = "");
        ~DbHost ();

    private:
        QSqlDatabase db;
};

#endif // DBHOST_H
