#ifndef RUNNER_H
#define RUNNER_H

#include <QtCore>
#include <QTcpSocket>

#include <string>

class Runner: public QObject {
    Q_OBJECT

    public:
        Runner (int _port, std::string _destHost, std::string _filePath, int _lifetime = 0x7FFFFFFF, int _pause = 100, QObject *parent = nullptr);

    public slots:
        void run ();

    signals:
        void finished ();

    private:
        bool crcCheck;
        int port, lifetime, pause;
        std::string filePath, destHost;
};

#endif // RUNNER_H
