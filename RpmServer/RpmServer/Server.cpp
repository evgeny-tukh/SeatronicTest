#include <stdlib.h>
#include "Server.h"

#include <thread>
#include <chrono>
#include <stdio.h>
#include <sstream>
#include <fstream>
#include <qtcpsocket.h>
#include <QThread>
#include <ios>

#include "Sentence.h"
#include "Data.h"

Server::Server (
    const int _port,
    std::string _server,
    std::string _dbName,
    std::string _userName,
    std::string _pw,
    time_t _timeout,
    bool _echo,
    std::string _inputFilePath,
    const bool _saveToFile,
    QObject *_parent
):
    QObject (_parent),
    dbHost (nullptr),
    connected (false),
    saveToFile (_saveToFile),
    echo (_echo),
    port (_port),
    dbName (_dbName),
    userName (_userName),
    pw (_pw),
    server (_server),
    inputFilePath (_inputFilePath),
    socketWrapper (nullptr),
    timeout (_timeout) {
}

Server::~Server () {
    if (dbHost) delete dbHost;
}

void Server::run () {
    auto runner = QThread::create ([this] () {
        dbHost = new DbHost ("QPSQL7", dbName, userName, pw, echo);

        if (inputFilePath.empty ()) {
            runAsNormalRunner ();
        } else {
            runAsFileRunner ();
        }
        delete dbHost;
        dbHost = nullptr;
        emit finished ();
    });
    runner->start ();

    #if 0
    std::thread runner ([this] () {
        dbHost = new DbHost ("QPSQL7", dbName, userName, pw, echo);

        if (inputFilePath.empty ()) {
            runAsNormalRunner ();
        } else {
            runAsFileRunner ();
        }
        delete dbHost;
        dbHost = nullptr;
        emit finished ();
    });

    runner.detach ();
    #endif
}

void Server::runAsNormalRunner () {
    std::cout << "Waiting for first incoming connection..." << std::endl;
    time_t startTime = time (nullptr);
    while (waitProcessIncomingConnection ()) {
        while (!timeout || (time (nullptr) - startTime) < timeout) {
            if (socketWrapper->hasPendingConnections ()) {
                std::cout << "We have a connection..." << std::endl;
            }
            if (timeout) {
                printf ("%lld / %lld   \r", time (nullptr) - startTime, timeout);
            } else {
                printf ("%lld\r", time (nullptr) - startTime);
            }
            std::this_thread::sleep_for (std::chrono::milliseconds (100));
        }
        printf ("\nInterrogation finished.\nWaiting for new connection...\n");
    }
}

void Server::runAsFileRunner () {
    std::cout << "Try to read the file '" << inputFilePath << "'..." << std::endl;
    time_t startTime = time (nullptr);
    while (readFileOnce ()) {
        if (!timeout || (time (nullptr) - startTime) < timeout) {
            if (timeout) {
                printf ("%lld / %lld   \r", time (nullptr) - startTime, timeout);
            } else {
                printf ("%lld\r", time (nullptr) - startTime);
            }
            std::this_thread::sleep_for (std::chrono::milliseconds (100));
            printf ("\nLog file has been passed through.\nRewinding...\n");
        }
    }
}

bool Server::readFileOnce () {
    std::ifstream stream (inputFilePath.c_str ());
    if (stream.rdstate ()) {
        std::cout << "Unable to get file " << inputFilePath << " content:" << stream.rdstate () << std::endl;
        return false;
    }
    while (stream.rdstate () == 0) {
        std::string buffer;
        stream >> buffer;
        std::stringstream output;// (buffer.c_str ());
        output << buffer;
        output >> *this;
        std::this_thread::sleep_for (std::chrono::milliseconds (100));
    }
    return true;
}

void Server::onNewConnection () {
    std::cout << std::endl << "Incoming connection detected. Waiting for data..." << std::endl;
    auto incomingConection = socketWrapper->nextPendingConnection ();
    auto onDataAvailable = [this, incomingConection] () {
        auto data = incomingConection->readAll ();
        std::cout << data.data () << std::endl;
        std::stringstream output (data.data ());
        output >> *this;
    };
    if (incomingConection->waitForReadyRead (60000)) {
        std::cout << "Data in." << std::endl;
    } else {
        std::cout << "No data without 1 min - disconnecting" << std::endl;
    }
    #if 0
    connect (incomingConection, & QTcpSocket::readyRead, this, onDataAvailable);
    while (true) {
        auto available = incomingConection->bytesAvailable () > 0;
        std::cout << available << std::endl;
        //if (available) {
            onDataAvailable ();
        //}
        std::this_thread::sleep_for (std::chrono::microseconds (500));
    }
    #endif
}

bool Server::waitProcessIncomingConnection () {
    if (!socketWrapper) socketWrapper = new QTcpServer (nullptr);
    //QHostAddress addr ("192.168.0.136");
    //std::cout << connect (socketWrapper, SIGNAL (newConnection ()), this, SLOT (onNewConnection ())) << std::endl;
    if (socketWrapper->listen (QHostAddress::Any, port)) {
        std::cout << "Listening at port " << port << "..." << std::endl;
    } else {
        std::cout << "Unable to get listening at port " << port << ": " << socketWrapper->errorString ().toStdString () << std::endl;
        return false;
    }
    if (socketWrapper->waitForNewConnection (timeout * 1000)) {
        //std::cout << "Have a connection" << std::endl;
        onNewConnection ();
    } else {
        std::cout << "No incoming connection within specified time - stopping" << std::endl;
    }
    return true;
}

void Server::checkProcessAccumulator () {
    if (echo) std::cout << accumulator << std::endl;

    Sentence sentence (accumulator);
    if (sentence.type ().compare ("RPM") == 0 && sentence.size () >= 6) {
        auto talker = sentence.talkerID ();
        auto rpm = sentence.omitted (3) ? 0.0 : std::stof (sentence [3]);
        bool valid = sentence.omitted (5) ? false : (sentence [5].front () == 'A');
        if (echo) {
            std::cout << "RPM: " << rpm << "; validity: " << (valid ? "yes" : "no") << std::endl;
        }
        time_t timestamp = time (nullptr);
        char validity;
        if (!valid) {
            validity = 'V';
        } else if (false) {
            validity = 'O';
        } else {
            validity = 'A';
        }
        dbHost->populate (timestamp, rpm, validity);
        if (saveToFile) storeToFile (timestamp, (int) rpm, valid);
    }
}

std::istream& operator >> (std::istream& stream, Server& self) {
    while (stream.good ()) {
        char ch = 0;
        stream >> ch;
        switch (ch) {
            case '!': case '$': case '\n': case '\r': {
                if (!self.accumulator.empty ()) {
                    self.checkProcessAccumulator ();
                }
                self.accumulator.clear ();
                break;
            }
            default: {
                if (ch >= '*' && ch <= 'Z') {
                    self.accumulator += ch;
                }
            }
        }
    }
    return stream;
}

void Server::storeToFile (time_t timestamp, const int rpm, const bool validity) const {
    std::ofstream output ("server.dat", std::ios_base::out | std::ios_base::trunc);
    Data data;
    data ["rpm"] = std::to_string (rpm);
    data ["validity"] = std::to_string (validity);
    data ["timestamp"] = std::to_string (timestamp);
    output << data.serialize ();
}
