#ifndef SERVER_H
#define SERVER_H

#include <cstdint>
#include <QtCore>
#include <QTcpServer>
#include <iostream>
#include <string>

class Server: public QObject {
    Q_OBJECT

    public:
        Server (
            int _port,
            std::string _server,
            std::string _dbName,
            std::string _userName,
            std::string _pw,
            std::string _filePath = "",
            QObject *_parent = nullptr
        );

        friend std::istream& operator>> (std::istream &, Server&);

    public slots:
        void run (uint32_t lifeTime = 0);

    signals:
        void finished ();

    private:
        std::string accumulator;
        bool connected;
        int port;
        std::string dbName, userName, pw, server;
        std::string filePath;
        QTcpServer socketWrapper;

        bool waitProcessIncomingConnection ();
        void checkProcessAccumulator ();
        bool readFileOnce ();

        void runAsNormalRunner (uint32_t lifeTime);
        void runAsFileRunner (uint32_t lifeTime);
};

#endif // SERVER_H
