#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <cstdint>
#include <QtCore>
#include <QTimer>
#include <iostream>
#include <string>

#include "DbHost.h"
#include "DataStorage.h"

class Processor: public QObject {
    Q_OBJECT

    public:
        Processor (
            std::string _serverHost,
            std::string _dbName,
            std::string _schemaName,
            std::string _tableName,
            std::string _userName,
            std::string _pw,
            bool _echo = false,
            std::string _inputFilePath = "",
            bool saveToFile = false,
            time_t dataTimeout = 30,
            uint32_t _checkPeriod = 500,
            QObject *_parent = nullptr
        );
        ~Processor ();

        friend std::istream& operator>> (std::istream &, Processor&);

    private:
        QTimer *checker;
        DbHost *dbHost;
        DataStorage *storage;
        std::string accumulator, inputFilePath;
        std::string dbName, schemaName, tableName, userName, pw, serverHost;
        bool saveToFile, echo;

        void checkProcessAccumulator ();
        void storeToFile (time_t timestamp, const int rpm, const bool validity) const;
};

#endif // PROCESSOR_H
