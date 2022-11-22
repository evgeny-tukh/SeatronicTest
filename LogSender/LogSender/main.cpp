#include <QCoreApplication>
#include <iostream>
#include <string>

#include "Runner.h"

int main(int argc, char *argv[])
{
    QCoreApplication app (argc, argv);

    std::cout << "Hello" << std::endl;

    if (argc < 2) {
        std::cout << "USAGE:\n\n\tlogsender port file [host [lifetime [pause]]]" << std::endl;
        return 0;
    }

    int port = atoi (argv [1]);
    std::string destHost = (argc > 3) ? argv [3] : "localhost";
    int lifetime = (argc > 4) ? atoi (argv [4]) : 0x7FFFFFFF;
    int pause = (argc > 5) ? atoi (argv [5]) : 100;
    std::string filePath = argv [2];

    std::cout << "Port " << port << "; path '" << filePath << "'; host " << destHost << std::endl;

    if (port < 1000 || port > 65000) {
        std::cout << "Invalid port specified." << std::endl;
    }

    Runner runner (port, destHost, filePath, lifetime, pause);

    QObject::connect (& runner, SIGNAL (finished()), & app, SLOT (quit ()));

    runner.run ();

    return app.exec();
}
