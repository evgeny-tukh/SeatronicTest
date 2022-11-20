#include <iostream>
#include <cstdint>
#include <QCoreApplication>
#include <QTcpServer>
#include <QTcpSocket>
#include <ctype.h>

#include <string>

#include "Server.h"
#include "Data.h"

int main(int argc, char *argv[])
{
    QCoreApplication app (argc, argv);

    std::cout << "RPM NMEA Server" << std::endl;

    uint16_t port = 0;
    time_t timeout = 0;
    bool echo = false;
    std::string server { "localhost"};
    std::string dbName { "data" };
    std::string userName, password;
    std::string inputFilePath;

    #if 0
    for (int i = 1; i < argc; ++ i) {
        auto arg = argv [i];

        if (arg [0] == '-' && (arg [1] == 'h' || arg [1] == 'H' || arg [1] == '?')) {
            std::cout << "USAGE" << std::endl << "\tRPMSERVER [-p:port] [-s:dbserver] [-u:dbusername] [-a:dbpass]" << std::endl << "or\n" << "\tRPMSERVER -f:inputfilepath" << std::endl;
            std::cout << "\n\nNote that if you don't specify database parameters so the server will write data in the file .dat" << std::endl;
            return 0;
        }

        if (*arg != '-' || arg [2] != ':') {
            std::cout << "Invalid argument: " << arg << std::endl;
            return 0;
        }

        switch (toupper (arg [1])) {
            case 'P': {
                port = atoi (arg + 3);
                if (!port) {
                    std::cout << "Invalid port specified" << std::endl;
                    return 0;
                }
                break;
            }
            case 'S': {
                server = arg + 3; break;
            }
            case 'U': {
                userName = arg + 3; break;
            }
            case 'A': {
                password = arg + 3; break;
            }
            case 'F': {
                inputFilePath = arg + 3; break;
            }
            default: {
                std::cout << "Invalid argument: " << arg << std::endl;
                return 0;
            }
        }
    }
    #endif

    Data cfg ("server.cfg");
    auto portString = cfg ["port"];
    auto timeoutString = cfg ["timeout"];
    auto echoString = cfg ["echo"];
    server = cfg ["server"];
    dbName = cfg ["dbName"];
    userName = cfg ["userName"];
    password = cfg ["password"];
    inputFilePath = cfg ["inputFilePath"];
    auto saveToFileString = cfg ["saveToFile"];
    bool saveToFile;
    try {
        port = std::stoi (portString);
    } catch (...) {
        port = 0;
    }
    try {
        echo = std::stoi (echoString) != 0;
    } catch (...) {
        echo = false;
    }
    try {
        timeout = std::stoi (timeoutString);
    } catch (...) {
        timeout = 30;
    }
    try {
        saveToFile = std::stoi (saveToFileString) != 0;
    } catch (...) {
        saveToFile = false;
    }

    std::cout << "Lifetime is " << timeout << std::endl;

    //Server srv (port, server, dbName, userName, password, timeout, echo, inputFilePath, saveToFile);
    //
    //QObject::connect (& srv, SIGNAL (finished()), & app, SLOT (quit ()));
    //
    //srv.run ();

    QTcpServer socket (& app);
    app.connect (& socket, & QTcpServer::newConnection, & app, [&socket, &app] () {
        std::cout << std::endl << "Incoming connection detected. Waiting for data..." << std::endl;
        auto incomingConection = socket.nextPendingConnection ();
        incomingConection->connect (incomingConection, & QTcpSocket::readyRead, & app, [incomingConection] () {
            auto data = incomingConection->readAll ();
            std::cout << data.data () << std::endl;
        });
        incomingConection->connect (incomingConection, & QTcpSocket::disconnected, & app, [incomingConection] () {
            std::cout << "Disconnected." << std::endl;
            incomingConection->close ();
        });
    });

    socket.listen (QHostAddress::Any, port);


    return app.exec();
}
