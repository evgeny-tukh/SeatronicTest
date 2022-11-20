#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <cstdint>
#include <QtCore>
#include <iostream>
#include <string>

#include "DbHost.h"

class Processor: public QObject {
    Q_OBJECT

    public:
        Processor (
            std::string _server,
            std::string _dbName,
            std::string _userName,
            std::string _pw,
            bool _echo = false,
            std::string _inputFilePath = "",
            const bool saveToFile = false,
            QObject *_parent = nullptr
        );
        ~Processor ();

        friend std::istream& operator>> (std::istream &, Processor&);

    private:
        DbHost *dbHost;
        std::string accumulator, inputFilePath;
        std::string dbName, userName, pw, server;
        bool saveToFile, echo;

        void checkProcessAccumulator ();
        void storeToFile (time_t timestamp, const int rpm, const bool validity) const;
};

#endif // PROCESSOR_H
