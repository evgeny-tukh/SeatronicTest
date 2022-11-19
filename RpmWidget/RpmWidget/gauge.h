#ifndef GAUGE_H
#define GAUGE_H

#include <qobject.h>

class Gauge: public QObject {
    Q_OBJECT
    Q_PROPERTY(int pos READ getGaugePos WRITE setGaugePos NOTIFY posChanged)

    public:
        Gauge (int initialRpm = 0): rpm (initialRpm) {}

        void setGaugePos (int pos) {
            rpm = pos;
            emit posChanged ();
        }
        int getGaugePos () const { return rpm; }

    signals:
        void posChanged ();
    private:
        int rpm;
};

#endif // GAUGE_H
