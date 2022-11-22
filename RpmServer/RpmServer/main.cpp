#include <iostream>
#include <sstream>
#include <cstdint>
#include <QCoreApplication>
#include <QTcpServer>
#include <QTcpSocket>
#include <ctype.h>

#include <string>

#include "Data.h"
#include "Processor.h"

int main(int argc, char *argv[])
{
    QCoreApplication app (argc, argv);

    std::cout << "RPM NMEA Server" << std::endl;

    uint16_t port = 0;
    uint32_t checkPeriod = 500;
    time_t timeout = 0, dataTimeout = 30;
    bool echo = false;
    std::string serverHost { "localhost"};
    std::string dbName { "stvesseldata" };
    std::string tableName { "current" };
    std::string schemaName { "processed_data" };
    std::string userName, password;
    std::string inputFilePath;

    Data cfg ("server.cfg");
    auto portString = cfg ["port"];
    auto timeoutString = cfg ["timeout"];
    auto dataTimeoutString = cfg ["dataTimeout"];
    auto checkPeriodString = cfg ["checkPeriod"];
    auto echoString = cfg ["echo"];
    serverHost = cfg ["serverHost"];
    dbName = cfg ["dbName"];
    tableName = cfg ["tableName"];
    schemaName = cfg ["schemaName"];
    userName = cfg ["userName"];
    password = cfg ["password"];
    inputFilePath = cfg ["inputFilePath"];
    auto saveToFileString = cfg ["saveToFile"];
    bool saveToFile;
    auto str2int = [] (std::string source, int defValue = 0) {
        int result = defValue;
        try {
            result = std::stoi (source);
        } catch (...) {
            result = defValue;
        }
        return result;
    };
    port = str2int (portString);
    echo = str2int (echoString);
    timeout = str2int (timeoutString, 30);
    dataTimeout = str2int (dataTimeoutString, 30);
    saveToFile = str2int (saveToFileString) != 0;
    checkPeriod = str2int (checkPeriodString, 500);

    std::cout << "Lifetime is " << timeout << std::endl;

    Processor processor (
        serverHost,
        dbName,
        schemaName,
        tableName,
        userName,
        password,
        echo,
        inputFilePath,
        saveToFile,
        dataTimeout,
        checkPeriod,
        & app
    );
    QTcpServer socket (& app);
    app.connect (& socket, & QTcpServer::newConnection, & app, [&socket, &app, &processor, echo] () {
        std::cout << std::endl << "Incoming connection detected. Waiting for data..." << std::endl;
        auto incomingConection = socket.nextPendingConnection ();
        incomingConection->connect (incomingConection, & QTcpSocket::readyRead, & app, [echo, incomingConection, &processor] () {
            auto data = incomingConection->readAll ();
            if (echo) std::cout << data.data () << std::endl;
            std::stringstream stream (data.data ());
            stream >> processor;
        });
        incomingConection->connect (incomingConection, & QTcpSocket::disconnected, & app, [incomingConection] () {
            std::cout << "Disconnected." << std::endl;
            incomingConection->close ();
        });
    });

    socket.listen (QHostAddress::Any, port);


    return app.exec();
}
