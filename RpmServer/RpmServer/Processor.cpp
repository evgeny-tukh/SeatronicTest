#include <fstream>
#include "Processor.h"
#include "Sentence.h"
#include "Data.h"

Processor::Processor (
        std::string _server,
        std::string _dbName,
        std::string _userName,
        std::string _pw,
        bool _echo,
        std::string _inputFilePath,
        const bool _saveToFile,
        QObject *_parent
):
    QObject (_parent),
    dbHost (nullptr),
    inputFilePath (_inputFilePath),
    dbName (_dbName),
    userName (_userName),
    pw (_pw),
    server (_server),
    saveToFile (_saveToFile),
    echo (_echo) {
    dbHost = new DbHost ("QPSQL7", dbName, userName, pw, echo);
}

Processor::~Processor () {
    if (dbHost) delete dbHost;
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

void Processor::storeToFile (time_t timestamp, const int rpm, const bool validity) const {
    std::ofstream output ("server.dat", std::ios_base::out | std::ios_base::trunc);
    Data data;
    data ["rpm"] = std::to_string (rpm);
    data ["validity"] = std::to_string (validity);
    data ["timestamp"] = std::to_string (timestamp);
    output << data.serialize ();
}
