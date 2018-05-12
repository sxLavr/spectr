#ifndef FREQUENCYGROUP_H
#define FREQUENCYGROUP_H


#include <QtGlobal>
#include <QString>


class SdrRfcCore;

class FrequencyGroup
{
public:
    FrequencyGroup();

    quint64 start() const { return _start; }
    quint64 stop() const { return _stop; }
    quint64 span() const { return _span; }
    quint64 center() const { return _center; }

    QString string_start() const { return to_qstring( _start ); }
    QString string_stop() const { return to_qstring( _stop ); }
    QString string_span() const { return to_qstring( _span ); }
    QString string_center() const { return to_qstring( _center ); }

    static quint64 from_qstring( QString const& text );
    static QString to_qstring( quint64 value );

private:
    friend class SdrRfcCore;

    bool try_set_start( quint64 start );
    bool try_set_stop( quint64 stop );
    bool try_set_span( quint64 span );
    bool try_set_center( quint64 center );

    void calc_span_center();
    void calc_start_stop();

    quint64     _start;
    quint64     _stop;
    quint64     _span;
    quint64     _center;
};

#endif // FREQUENCYGROUP_H
