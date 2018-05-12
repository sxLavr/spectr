
#include "FftData.h"
#include <QtMath>


FftData::FftData()
    : _valid( false )
    , _points_count( 0 )
{

}

void FftData::parse( QByteArray const& buffer )
{
    if (is_ready())
    {
        _valid = false;
        _points_count = 0;
    }

    char const* ptr = buffer.data();
    char const* end = buffer.end();

    if (!_valid)
    {
        SdrRfcData const* header = reinterpret_cast<SdrRfcData const*>( buffer.data() );

        if (header->Indx != 0)
            return;

        if (header->Ref != 0)
            return;

        if (header->TotalPoints != header->PointsCount)
            return;

        _valid = true;
        memcpy( &_header, header, sizeof( SdrRfcData ) );
        ptr += sizeof( SdrRfcData );

        _data.resize( header->PointsCount );
    }

    float const* point = reinterpret_cast<float const*>( ptr );
    float const* end_point = reinterpret_cast< float const*>( end );

    while (point < end_point && _points_count < _header.PointsCount)
    {
        _data[_points_count++] = 20 * log10( *point );
        point++;
    }
}

bool FftData::is_ready() const
{
    return _valid && _header.PointsCount == _points_count;
}
