#ifndef RUNNER_H
#define RUNNER_H

#include <QtCore>
#include <QTcpSocket>

#include <string>

class Runner: public QObject {
    Q_OBJECT

    public:
        Runner (int _port, std::string _filePath, int _lifetime = 0x7FFFFFFF, QObject *parent = nullptr);

    public slots:
        void run ();

    signals:
        void finished ();

    private:
        int port, lifetime;
        std::string filePath;
};

#endif // RUNNER_H
