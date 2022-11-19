#include <QScopedPointer>
#include <QSqlQuery>
#include <libpq-fe.h>
//#include <qsql_psql_p.h>

#include "DataHost.h"

DataHost::DataHost (QString host, QString dbName, QString user, QString pw) {
    // Build connection string first
    //QString connStr = QString ("host=%1 dbname=%2").arg (host, dbName);
    //if (!user.isEmpty()) connStr += QString ("user=%1").arg (user);
    //if (!pw.isEmpty()) connStr += QString ("password=%1").arg (pw);

    auto fail = [] (const char *desc) {
      throw std::exception (desc);
    };

    // Choose a PS SQL driver
    //QScopedPointer<PGconn> connection (PQconnectdb (connStr.toStdString ().c_str ()));
    //QScopedPointer<QPSQLDriver> driver (new QPSQLDriver (connection.get ()));
    //
    //if (!driver) fail ("PSQL driver is not registered in the system. Pls call your administrator.");

    db = QSqlDatabase::addDatabase ("QPSQL", "MyDb");

    //db.setHostName (host);
    db.setDatabaseName (dbName);

    if (!user.isEmpty ()) db.setUserName (user);
    if (!pw.isEmpty ()) db.setPassword (pw);

    if (!db.open ()) {
        fail (
            "The database can't be opened.\n"
            "Please make sure that your server is up and running and connection parameters are correct\n"
        );
    }
}

DataHost::~DataHost () {
    if (db.isOpen ()) db.close ();
}

bool DataHost::exec (QString queryText, bool transacted) {
    QSqlQuery query (db);
    if (transacted) db.transaction ();
    bool success = query.exec (queryText);
    if (transacted) {
        if (success) {
            db.commit ();
        } else {
            db.rollback ();
        }
    }
    return success;
}
