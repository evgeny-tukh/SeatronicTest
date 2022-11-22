#ifndef DATASTORAGE_H
#define DATASTORAGE_H

#include <QtCore>
#include <mutex>
#include "Defs.h"

class DataStorage: public QObject {
    Q_OBJECT

    public:
        DataStorage (time_t timeout = 30, QObject *parent = nullptr);

        void update (time_t _timestamp, int _rpm, Validity _validity);
        void checkUpToDate ();

        void get (time_t& _timestamp, int& _rpm, Validity& _validity) const;

    private:
        std::mutex mutex;
        time_t lastUpdate, timeout;
        int rpm;
        Validity validity;

};

#endif // DATASTORAGE_H
