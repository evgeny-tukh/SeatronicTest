#include <QCoreApplication>
#include <iostream>
#include <string>

#include "Runner.h"

int main(int argc, char *argv[])
{
    QCoreApplication app (argc, argv);

    std::cout << "Hello" << std::endl;

    if (argc < 2) {
        std::cout << "USAGE:\n\n\tlogsender port file [lifetime]" << std::endl;
        return 0;
    }

    int port = atoi (argv [1]);
    int lifetime = argc > 4 ? atoi (argv [3]) : 0x7FFFFFFF;
    std::string filePath = argv [2];

    std::cout << "Port " << port << "; path '" << filePath << "'" << std::endl;

    if (port < 1000 || port > 65000) {
        std::cout << "Invalid port specified." << std::endl;
    }

    Runner runner (port, filePath, lifetime);

    QObject::connect (& runner, SIGNAL (finished()), & app, SLOT (quit ()));

    runner.run ();

    return app.exec();
}
