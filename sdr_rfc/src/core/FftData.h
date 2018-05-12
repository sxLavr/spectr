#ifndef FFTDATA_H
#define FFTDATA_H


#include "SdrRfcProtocol.h"
#include <QByteArray>
#include <QVector>



class FftData
{
public:
    FftData();

    void parse( QByteArray const& buffer );

    bool is_ready() const;

    quint64 start() const { return _header.F_Start_MHz; }
    quint64 step() const { return _header.F_Stop_MHz; }
    QVector<double>& get_data() { return _data; }

private:
    bool            _valid;
    SdrRfcData      _header;
    QVector<double> _data;
    quint32         _points_count;
};

#endif // FFTDATA_H
