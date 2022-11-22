#include <QtOpenGL>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <QQmlContext>
#include <QMessageBox>

#include <QtWidgets/QMessageBox>

#include "DataManagement/DataHost.h"
#include "../../RpmServer/RpmServer/Defs.h"
#include "../../RpmServer/RpmServer/DbHost.h"
#include "../../RpmServer/RpmServer/Data.h"

int main (int argc, char *argv[]) {
    #if QT_VERSION < QT_VERSION_CHECK (6, 0, 0)
    QCoreApplication::setAttribute (Qt::AA_EnableHighDpiScaling);
    #endif

    Data cfg ("client.cfg");
    std::string serverHost = cfg ["serverHost"];
    std::string dbName = cfg ["dbName"];
    std::string tableName = cfg ["tableName" ];
    std::string schemaName = cfg ["schemaName"];
    std::string userName = cfg ["userName"];
    std::string password = cfg ["password"];
    std::string checkPeriodStr = cfg ["checkPeriod"];
    auto str2int = [] (std::string source, int defValue = 0) {
        int result = defValue;
        try {
            result = std::stoi (source);
        } catch (...) {
            result = defValue;
        }
        return result;
    };
    int checkPeriod = str2int (checkPeriodStr);

    QGuiApplication app (argc, argv);
    QQuickView view;
    QQmlApplicationEngine engine;
    DbHost *host = nullptr;
    try {
        host = new DbHost ("QPSQL7", serverHost, dbName, schemaName, tableName, userName, password, false, & app);
    } catch (...) {
        QMessageBox::critical (nullptr, "Database Not Available", "Unable to connect to the database; check your settins please!");
    }

    QObject *mainQmlWnd = nullptr;

    QTimer timer (& app);
    const QUrl url (QStringLiteral("qrc:/main.qml"));
    QObject::connect (& timer, & QTimer::timeout, & app, [&host, &mainQmlWnd] () {
        time_t lastUpdate;
        int rpm = 0;
        Validity validity = Validity::INVALID;
        int validityNum;

        host->request (lastUpdate, rpm, validity);
        switch (validity) {
            case Validity::VALID: validityNum = 1; break;
            case Validity::OUTDATED: validityNum = 2; break;
            case Validity::UNRELIABLE: validityNum = 3; break;
            default: validityNum = 4;
        }
        mainQmlWnd->setProperty("rpm", rpm);
        mainQmlWnd->setProperty("validity", validityNum);
    });
    QObject::connect (
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url] (QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl) {
                QCoreApplication::exit (-1);
            }
        },
        Qt::QueuedConnection
    );
    timer.start (checkPeriod);
    engine.load (url);
    auto rootWidgets = engine.rootObjects ();
    mainQmlWnd = rootWidgets.front ();
    mainQmlWnd->setProperty("rpm", 111);
    mainQmlWnd->setProperty("validity", 3);
    return app.exec ();
}
