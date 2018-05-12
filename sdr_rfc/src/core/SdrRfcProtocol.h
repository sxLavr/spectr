#ifndef SDRRFCPROTOCOL_H
#define SDRRFCPROTOCOL_H


#include <QtGlobal>
#include <QByteArray>


enum Command : quint16
{
    Command_OpenDataChannel             = 0x8000,
    Command_BusyReport                  = 0x9000,
    Command_TargetMode                  = 0x0080,
    Command_Frequency                   = 0x0103,
    Command_RBW                         = 0x0101,
    Command_VBW                         = 0x0100,
    Command_Attenuation                 = 0x0102,
    Command_VcoCalibration              = 0x0091,
    Command_LoadingCalibrationTable     = 0x0093,
    Command_CalibrationStatusRequest    = 0x0094,
    Command_CalibrationManagement       = 0x0095,
    Command_Zero                        = 0x0000,
};


#pragma pack( 1 )


struct SdrRfcHeader
{
    Command     cmd;
    quint16     flags;
    qint32      status;     // 0 for request, error code for response
    quint32     len;        // payload size
};


struct OpenDataChannelPacket
{
    enum Type : quint16
    {
        Type_Spectrogram    = 0,
        Type_Reserved       = 1,
    };

    explicit OpenDataChannelPacket( bool set )
    {
        header.cmd = Command_OpenDataChannel;
        header.flags = 0x00;
        header.status = 0;
        header.len = set ? sizeof( Payload ) : 0;
    }

    SdrRfcHeader header;
    struct Payload
    {
        Type        type;
        quint16     port;   // TCP port of local machine
        quint32     ipv4;   // TCP address of local machine
    } payload;
};


struct TargetModePacket
{
    enum Mode : quint32
    {
        Mode_Idle       = 0,
        Mode_Spectr     = 1,
        Mode_Reserved   = 2,
    };

    enum Channel : quint32
    {
        Channel_RX1 = 0,
        Channel_RX2 = 1,
    };

    explicit TargetModePacket( bool set )
    {
        header.cmd = Command_TargetMode;
        header.flags = 0x01;
        header.status = 0;
        header.len = set ? sizeof( Payload ) : 0;
    }

    SdrRfcHeader header;
    struct Payload
    {
        Mode    mode;
        Channel channel;
    } payload;
};


struct AttenuationPacket
{
    explicit AttenuationPacket( bool set )
    {
        header.cmd = Command_Attenuation;
        header.flags = 0x01;
        header.status = 0;
        header.len = set ? sizeof( Payload ) : 0;
    }

    SdrRfcHeader header;
    struct Payload
    {
        qint8  att;
    } payload;
};


struct FrequencyPacket
{
    explicit FrequencyPacket( bool set )
    {
        header.cmd = Command_Frequency;
        header.flags = 0x01;
        header.status = 0;
        header.len = set ? sizeof( Payload ) : 0;
    }

    SdrRfcHeader header;
    struct Payload
    {
        quint64 start;
        quint64 stop;
    } payload;
};


struct RBWPacket
{
    enum Window : quint32
    {
        Window_Rectangle    = 0,
        Window_Hann         = 1,
        Window_Hamming      = 4,
        Window_Blackman     = 5,
        Window_NuttAll      = 7,
        Window_FlatTop      = 12,
    };

    enum Algorithm : quint8
    {
        Algorithm_RBW           = 0,
        Algorithm_StepOfPoints  = 1,
    };

    enum SweepMode : quint8
    {
        SweepMode_Normal    = 0,
        SweepMode_Patented  = 1,
        SweepMode_Brief     = 2,
    };

    explicit RBWPacket( bool set )
    {
        header.cmd = Command_RBW;
        header.flags = 0x01;
        header.status = 0;
        header.len = set ? sizeof( Payload ) : 0;
    }

    SdrRfcHeader header;
    struct Payload
    {
        quint32     rbw;
        Window      window;
        Algorithm   algorithm;
        SweepMode   sweep_mode;
    } payload;
};


struct VBWPacket
{
    explicit VBWPacket( bool set )
    {
        header.cmd = Command_VBW;
        header.flags = 0x01;
        header.status = 0;
        header.len = set ? sizeof( Payload ) : 0;
    }

    SdrRfcHeader header;
    struct Payload
    {
        quint32     vbw;
        quint32     time;
        quint32     averaging;
    } payload;
};


struct VcoCalibrationPacket
{
    explicit VcoCalibrationPacket( bool set )
    {
        header.cmd = Command_VcoCalibration;
        header.flags = 0x01;
        header.status = 0;
        header.len = set ? sizeof( Payload ) : 0;
    }

    SdrRfcHeader header;
    struct Payload
    {
        quint32 value_mv;
    } payload;
};


struct CalibrationManagementPacket
{
    enum LevelCompensation : quint32
    {
        LevelCompensation_On    = 0,
        LevelCompensation_Off   = 1,
    };

    explicit CalibrationManagementPacket( bool set )
    {
        header.cmd = Command_CalibrationManagement;
        header.flags = 0x01;
        header.status = 0;
        header.len = set ? sizeof( Payload ) : 0;
    }

    SdrRfcHeader header;
    struct Payload
    {
        LevelCompensation level_compensation;
    } payload;
};


struct BusyReportPacket
{
    SdrRfcHeader header;
    struct Payload
    {
        quint32     ipv4;   // TCP address of machine
    } payload;
};


template <typename T>
struct SdrRfcPacket
{
    typedef T Type;

    explicit SdrRfcPacket( bool set )
        : raw( 64, 0 )
        , size( set ? sizeof( Type ) : sizeof( SdrRfcHeader ) )
    {
        new (raw.data()) Type( set );
    }

    explicit SdrRfcPacket( QByteArray const& packet )
        : raw( packet )
        , size( packet.size() )
    {

    }

    Type* operator->()
    {
        return reinterpret_cast<Type*>( raw.data() );
    }

    QByteArray raw;
    size_t const size;
};


struct SdrRfcData
{
    quint64     TimeMarker;   //valid only indx == 0
    quint64     F_Start_MHz;  //valid only indx == 0
    quint64     F_Stop_MHz;   //valid only indx == 0
    quint32     Sweep_N;
    quint32     TotalPoints;
    quint32     Indx;
    quint32     PointsCount;
    qint8       Ref;
    qint8       Att;
    qint8       Det;
    qint8       Reserved;
    float       ValueList[];
};

#pragma pack()


#endif // SDRRFCPROTOCOL_H
