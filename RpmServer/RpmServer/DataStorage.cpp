#include "DataStorage.h"

DataStorage::DataStorage (time_t _timeout, QObject *parent): QObject (parent), lastUpdate (0), timeout (_timeout), rpm (0), validity (Validity::INVALID) {}

void DataStorage::update (time_t _timestamp, int _rpm, Validity _validity) {
    std::lock_guard<std::mutex> locker (mutex);
    lastUpdate = _timestamp;
    rpm = _rpm;
    validity = _validity;
}

void DataStorage::checkUpToDate () {
    std::lock_guard<std::mutex> locker (mutex);
    if ((time (nullptr) - lastUpdate) > timeout) {
        validity = Validity::OUTDATED;
    }
}

void DataStorage::get (time_t& _lastUpdate, int& _rpm, Validity& _validity) const {
    _lastUpdate = lastUpdate;
    _rpm = rpm;
    _validity = validity;
}
