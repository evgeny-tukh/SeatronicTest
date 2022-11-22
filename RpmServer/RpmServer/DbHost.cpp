#include <iostream>
#include <QSqlError>
#include <QSqlQuery>
#include "DbHost.h"
#include "DataStorage.h"

const int RPM_TYPE = 1;

DbHost::DbHost (
    std::string _drv,
    std::string _hostName,
    std::string _dbName,
    std::string _schemaName,
    std::string _tableName,
    std::string _user,
    std::string _pw,
    bool _echo,
    QObject *_parent
): QObject (_parent), schemaName (_schemaName), tableName (_tableName), query (nullptr), echo (_echo) {
    db = QSqlDatabase::addDatabase (QString::fromStdString (_drv), "Mine");
    db.setDatabaseName (QString::fromStdString (_dbName));
    db.setHostName (QString::fromStdString (_hostName));
    db.setUserName (QString::fromStdString (_user));
    db.setPassword (QString::fromStdString (_pw));

    if (echo) std::cout << "Trying to open db '" << _dbName << "' on host '" << _hostName << "' [un: " << _user << "]; pw: " << _pw << "...";
    bool result = db.open ();
    if (echo) std::cout << (result ? "ok" : "failed") << std::endl;
    if (result) {
        query = new QSqlQuery (db);
    } else {
        std::string msg = db.lastError ().text ().toStdString ();
        if (echo) std::cout << msg << std::endl;
        throw std::exception ();
    }
}

DbHost::~DbHost () {
    if (db.isOpen()) {
        QSqlDatabase::removeDatabase ("Mine");
    }
}

bool DbHost::request (time_t& timestamp, int& rpm, Validity& validity) {
    std::string queryText { "select update_time,value,validity from " };
    queryText += schemaName;
    queryText += '.';
    queryText += tableName;
    queryText += " where type=";
    queryText += std::to_string( (RPM_TYPE));
    if (echo) std::cout << queryText << std::endl;
    QSqlQuery getter (QString::fromStdString (queryText), db);
    bool result = getter.next ();
    if (result) {
        timestamp = getter.value (0).toUInt ();
        rpm = getter.value (1).toInt ();
        char valChar = (Validity) getter.value (2).toString ().front ().toLatin1 ();
        validity = (Validity) valChar;
    }
    return result;
}

void DbHost::populate (time_t timestamp, int rpm, Validity validity) {
    std::string queryText { "insert into " };
    queryText += schemaName;
    queryText += '.';
    queryText += tableName;
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
    //queryText += ';';
    if (echo) std::cout << queryText << std::endl;
    if (query) {
        std::lock_guard<std::mutex> locker (mutex);
        db.transaction ();
        if (query->exec (QString::fromStdString (queryText)/*"insert into public.current(type,update_time,validity,value) values(1,0,'O',0)"*/)) {
            db.commit ();
        } else {
            db.rollback ();
        }
    }
}
