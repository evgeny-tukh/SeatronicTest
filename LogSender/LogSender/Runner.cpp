#include <stdlib.h>
#include <thread>
#include <chrono>
#include <fstream>
#include <iostream>
#include <QTcpSocket>
#include <cstdint>
#include "Runner.h"

Runner::Runner (int _port, std::string _destHost, std::string _filePath, int _lifetime, QObject *_parent):
    QObject (_parent), port (_port), lifetime (_lifetime), filePath (_filePath), destHost (_destHost) {}

void Runner::run () {
    std::thread worker ([this] () {
        QTcpSocket sender (nullptr);
        if (true/*!sender.bind ()*/) {
            sender.connectToHost (destHost.c_str (), port);
            if (sender.waitForConnected ()) {
                std::cout << "Get connected. Sending data from '" << filePath << "'..." << std::endl;
                time_t start = time (nullptr);
                uint64_t totalSent = 0;
                while ((time (nullptr) - start) <= lifetime && sender.state () == QAbstractSocket::ConnectedState) {
                    std::ifstream source (filePath);
                    while (source.good ()) {
                        if (sender.state () != QAbstractSocket::ConnectedState) {
                            std::cout << "It seems that server has unexpectedly stopped the connection." << std::endl;
                            break;
                        }
                        std::string line;
                        source >> line;
                        auto sent = sender.write (line.c_str (), line.length ());
                        totalSent += sent;
                        sender.waitForBytesWritten ();
                        if (sent > 0) {
                            std::cout << totalSent << " bytes sent to the server so far.\r";
                        } else {
                            break;
                        }
                    }
                    //std::cout << "Log has been sent. Rewinding." << std::endl;
                    std::this_thread::sleep_for (std::chrono::microseconds (100));
                }
            } else {
               std::cout << "Connection error: " << sender.errorString ().toStdString () << "." << std::endl;
            }
        } else {
            std::cout << "Unable to bind the sender to any NIC (" << sender.errorString ().toStdString ().c_str () << ")" << std::endl << "Make sure that the machine is connected to LAN and network services are up and running." << std::endl;
        }

        emit finished ();
    });

    worker.detach ();
}
