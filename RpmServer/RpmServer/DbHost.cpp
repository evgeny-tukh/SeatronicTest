#include <iostream>
#include <QSqlError>
#include "DbHost.h"

DbHost::DbHost (std::string drv, std::string dbName, std::string user, std::string pw) {
    db = QSqlDatabase::addDatabase (QString::fromStdString (drv), "Mine");
    db.setDatabaseName (QString::fromStdString (dbName));
    db.setHostName ("localhost");
    db.setPassword (QString::fromStdString (pw));

    std::cout << "Trying to open db '" << dbName << "' on host 'localhost' [un: " << user << "; pw: " << pw << "..." << std::endl;
    bool result = db.open ();
    std::cout << (result ? "ok" : "failed") << std::endl;
    if (!result) {
        std::cout << db.lastError ().text ().toStdString () << std::endl;
    }
}

DbHost::~DbHost () {
    if (db.isOpen()) {
        QSqlDatabase::removeDatabase ("Mine");
    }
}
