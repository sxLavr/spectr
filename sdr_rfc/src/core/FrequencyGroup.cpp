
#include "FrequencyGroup.h"


namespace
{
    quint64 const DefaultFreqStart = 931600000; // 931.6 VHz
    quint64 const DefaultFreqStop = 941600000;  // 941.6 VHz
    quint64 const ReceiverMinLo = 50000000;     // 50 MHz
    quint64 const ReceiverMaxLo = 7000000000;   // 6 GHz
    quint64 const SweepMaxSpan = (ReceiverMaxLo - ReceiverMinLo) - 50000000;
}


FrequencyGroup::FrequencyGroup()
    : _start( DefaultFreqStart )
    , _stop( DefaultFreqStop )
{
    calc_span_center();
}

bool FrequencyGroup::try_set_start( quint64 start )
{
    if  (start > _stop)
        return false;
    if (start < ReceiverMinLo || start >= ReceiverMaxLo)
        return false;
    if ((_stop - start) > SweepMaxSpan)
        return false;
    _start = start;
    calc_span_center();
    return true;
}

bool FrequencyGroup::try_set_stop( quint64 stop )
{
    if  (_start > stop)
        return false;
    if (stop <= ReceiverMinLo || stop >= ReceiverMaxLo)
        return false;
    if ((stop - _start) > SweepMaxSpan)
        return false;
    _stop = stop;
    calc_span_center();
    return true;
}

bool FrequencyGroup::try_set_span( quint64 span )
{
    if (span == 0 || span > SweepMaxSpan)
        return false;
    _span = span;
    calc_start_stop();
    return true;
}

bool FrequencyGroup::try_set_center( quint64 center )
{
    if ((center - _span / 2) < ReceiverMinLo)
        return false;
    if ((center + _span / 2) > ReceiverMaxLo)
        return false;
    _center = center;
    calc_start_stop();
    return true;
}

void FrequencyGroup::calc_span_center()
{
    _span = _stop - _start;
    _center = _start + _span / 2;
}

void FrequencyGroup::calc_start_stop()
{
    _start = _center - _span / 2;
    _stop = _center + _span / 2;
}

/*static*/ quint64 FrequencyGroup::from_qstring( QString const& text )
{
    quint64 value = 0;

    if (text.contains( "G" ))
        value = text.left( text.indexOf( 'G' ) ).toFloat() * 1000000000;
    else if (text.contains( "M" ))
        value = text.left( text.indexOf( 'M' ) ).toFloat() * 1000000;
    else if (text.contains( "k" ))
        value = text.left( text.indexOf( 'k' ) ).toFloat() * 1000;
    else
        value = text.left( text.indexOf( 'H' ) ).toInt();
    return value;
}

/*static*/ QString FrequencyGroup::to_qstring( quint64 value )
{
    if (value > 1e9)
    {
        return QString::number( value / 1e9 ) + "GHz";
    }
    else if (value > 1e6)
    {
        return QString::number( value / 1e6 ) + "MHz";
    }
    else if (value > 1e3)
    {
        return QString::number( value / 1e3 ) + "kHz";
    }
    else
    {
        return QString::number( value ) + "Hz";
    }
}
