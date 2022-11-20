#include <iostream>
#include <QSqlError>
#include <QSqlQuery>
#include "DbHost.h"

DbHost::DbHost (std::string drv, std::string dbName, std::string user, std::string pw, bool _echo): query (nullptr), echo (_echo) {
    db = QSqlDatabase::addDatabase (QString::fromStdString (drv), "Mine");
    db.setDatabaseName (QString::fromStdString (dbName));
    db.setHostName ("localhost");
    db.setPassword (QString::fromStdString (pw));

    std::cout << "Trying to open db '" << dbName << "' on host 'localhost' [un: " << user << "; pw: " << pw << "..." << std::endl;
    bool result = db.open ();
    std::cout << (result ? "ok" : "failed") << std::endl;
    if (result) {
        query = new QSqlQuery (db);
    } else {
        std::cout << db.lastError ().text ().toStdString () << std::endl;
    }
}

DbHost::~DbHost () {
    if (query) delete query;
    if (db.isOpen()) {
        QSqlDatabase::removeDatabase ("Mine");
    }
}

void DbHost::populate (time_t timestamp, int rpm, char validity) {
    static const char *TBL_NAME = "processed_data.current";
    static const int RPM_TYPE = 1;
    std::string queryText { "insert into " };
    queryText += TBL_NAME;
    queryText += "(type,update_time,validity,value) values(";
    queryText += std::to_string( (RPM_TYPE));
    queryText += ',';
    queryText += std::to_string (timestamp);
    queryText += ",'";
    queryText += validity;
    queryText += "',";
    queryText += std::to_string (rpm);
    queryText += ") on conflict (type) do update set update_time=";
    queryText += std::to_string (timestamp);
    queryText += ",validity='";
    queryText += validity;
    queryText += "',value=";
    queryText += std::to_string (rpm);
    queryText += ';';
    if (echo) std::cout << queryText << std::endl;
    if (query) {
        db.transaction ();
        if (query->exec (QString::fromStdString (queryText))) {
            db.commit ();
        } else {
            db.rollback ();
        }
    }
}
