#ifndef SERVER_H
#define SERVER_H

#include <cstdint>
#include <QtCore>
#include <QTcpServer>
#include <iostream>
#include <string>

#include "DbHost.h"

class Server: public QObject {
    Q_OBJECT

    public:
        Server (
            const int _port,
            std::string _server,
            std::string _dbName,
            std::string _userName,
            std::string _pw,
            time_t _timeout = 30,
            bool _echo = false,
            std::string _inputFilePath = "",
            const bool saveToFile = false,
            QObject *_parent = nullptr
        );
        ~Server ();

        friend std::istream& operator>> (std::istream &, Server&);

    public slots:
        void run ();

    private slots:
        void onNewConnection ();

    signals:
        void finished ();

    private:
        DbHost *dbHost;
        std::string accumulator;
        bool connected, saveToFile, echo;
        int port;
        std::string dbName, userName, pw, server;
        std::string inputFilePath;
        QTcpServer *socketWrapper;
        time_t timeout;

        bool waitProcessIncomingConnection ();
        void checkProcessAccumulator ();
        bool readFileOnce ();

        void runAsNormalRunner ();
        void runAsFileRunner ();

        void storeToFile (time_t timestamp, const int rpm, const bool validity) const;
};

#endif // SERVER_H
