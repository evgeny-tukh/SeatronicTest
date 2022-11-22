#include <fstream>
#include "Processor.h"
#include "Sentence.h"
#include "Data.h"

Processor::Processor (
        std::string _serverHost,
        std::string _dbName,
        std::string _schemaName,
        std::string _tableName,
        std::string _userName,
        std::string _pw,
        bool _echo,
        std::string _inputFilePath,
        bool _saveToFile,
        time_t _dataTimeout,
        uint32_t _checkPeriod,
        QObject *_parent
):
    QObject (_parent),
    dbHost (nullptr),
    inputFilePath (_inputFilePath),
    dbName (_dbName),
    schemaName (_schemaName),
    tableName (_tableName),
    userName (_userName),
    pw (_pw),
    serverHost (_serverHost),
    saveToFile (_saveToFile),
    echo (_echo) {
    dbHost = new DbHost ("QPSQL7", serverHost, dbName, schemaName, tableName, userName, pw, echo, this);
    storage = new DataStorage (_dataTimeout, this);
    checker = new QTimer (this);

    checker->start (_checkPeriod);

    connect (checker, & QTimer::timeout, this, [this] () {
        time_t timestamp = 0;
        int rpm = 0;
        Validity validity = Validity::INVALID;

        storage->checkUpToDate ();
        storage->get (timestamp, rpm, validity);
        dbHost->populate (timestamp, rpm, validity);
        if (saveToFile) storeToFile (timestamp, rpm, validity);
    });
}

Processor::~Processor () {
}

std::istream& operator >> (std::istream& stream, Processor& self) {
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

void Processor::checkProcessAccumulator () {
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
        Validity validity;
        if (!valid) {
            validity = Validity::INVALID;
        } else if (false) {
            validity = Validity::OUTDATED;
        } else {
            validity = Validity::VALID;
        }
        storage->update (timestamp, rpm, validity);
    }
}

void Processor::storeToFile (time_t timestamp, const int rpm, const bool validity) const {
    std::ofstream output ("server.dat", std::ios_base::out | std::ios_base::trunc);
    Data data;
    data ["rpm"] = std::to_string (rpm);
    data ["validity"] = std::to_string (validity);
    data ["timestamp"] = std::to_string (timestamp);
    output << data.serialize ();
}
