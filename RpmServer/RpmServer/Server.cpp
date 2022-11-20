#include <stdlib.h>
#include "Server.h"

#include <thread>
#include <chrono>
#include <stdio.h>
#include <sstream>
#include <fstream>
#include <qtcpsocket.h>
#include <ios>

#include "Sentence.h"
#include "Data.h"

Server::Server (
    const int _port,
    std::string _server,
    std::string _dbName,
    std::string _userName,
    std::string _pw,
    std::string _inputFilePath,
    const bool _saveToFile,
    QObject *_parent
):
    QObject (_parent),
    connected (false),
    saveToFile (_saveToFile),
    port (_port),
    dbName (_dbName),
    userName (_userName),
    pw (_pw),
    server (_server),
    inputFilePath (_inputFilePath),
    socketWrapper (this) {
}

void Server::run (uint32_t lifeTime) {
    std::thread runner ([this, lifeTime] () {
        if (inputFilePath.empty ()) {
            runAsNormalRunner (lifeTime);
        } else {
            runAsFileRunner (lifeTime);
        }
        emit finished ();
    });

    runner.detach ();
}

void Server::runAsNormalRunner (uint32_t lifeTime) {
    std::cout << "Waiting for first incoming connection..." << std::endl;
    time_t startTime = time (nullptr);
    while (waitProcessIncomingConnection ()) {
        while (!lifeTime || (time (nullptr) - startTime) < lifeTime) {
            if (lifeTime) {
                printf ("%lld / %d   \r", time (nullptr) - startTime, lifeTime);
            } else {
                printf ("%lld\r", time (nullptr) - startTime);
            }
            std::this_thread::sleep_for (std::chrono::milliseconds (100));
        }
        printf ("\nInterrogation finished.\nWaiting for new connection...\n");
    }
}

void Server::runAsFileRunner (uint32_t lifeTime) {
    std::cout << "Try to read the file '" << inputFilePath << "'..." << std::endl;
    time_t startTime = time (nullptr);
    while (readFileOnce ()) {
        if (!lifeTime || (time (nullptr) - startTime) < lifeTime) {
            if (lifeTime) {
                printf ("%lld / %d   \r", time (nullptr) - startTime, lifeTime);
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

bool Server::waitProcessIncomingConnection () {
    connect (& socketWrapper, & QTcpServer::newConnection, this, [this] () {
        printf ("Incoming connection deteced. Waiting for data...\n");

        auto incomingConection = socketWrapper.nextPendingConnection ();
        connect (incomingConection, & QTcpSocket::readyRead, this, [this, incomingConection] () {
            auto data = incomingConection->readAll ();
            std::cout << data.data () << std::endl;
            std::stringstream output (data.data ());
            output >> *this;
        });
    });
    if (!socketWrapper.listen (QHostAddress::Any, port)) {
        printf ("Unable to get listening at port %d: %s\n", port, socketWrapper.errorString ().toStdString ().c_str ()); return false;
    }
    return true;
}

void Server::checkProcessAccumulator () {
    std::cout << accumulator << std::endl;

    Sentence sentence (accumulator);
    if (sentence.type ().compare ("RPM") == 0 && sentence.size () >= 6) {
        auto talker = sentence.talkerID ();
        auto rpm = sentence.omitted (3) ? 0.0 : std::stof (sentence [3]);
        bool valid = sentence.omitted (5) ? false : (sentence [5].front () == 'A');
        std::cout << "RPM: " << rpm << "; validity: " << (valid ? "yes" : "no") << std::endl;
        if (saveToFile) storeToFile ((int) rpm, valid);
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

void Server::storeToFile (const int rpm, const bool validity) const {
    std::ofstream output ("server.dat", std::ios_base::out | std::ios_base::trunc);
    Data data;
    data ["rpm"] = std::to_string (rpm);
    data ["validity"] = std::to_string (validity);
    output << data.serialize ();
}
