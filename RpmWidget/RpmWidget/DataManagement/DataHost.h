#ifndef DATAHOST_H
#define DATAHOST_H

#include <QtSql/QtSql>
#include <QtSql/QSqlDatabase>
#include <QDebug>

#include <libpq-fe.h>

#include "qsql_psql_p.h"

class DataHost {
    public:
        DataHost (QString host, QString dbName, QString user = "", QString pw = "");
        ~DataHost ();

        bool exec (QString query, bool transacted);

    private:
        QSqlDatabase db;
};

#endif // DATAHOST_H
