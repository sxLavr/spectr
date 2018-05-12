
#include "SdrRfcCore.h"
#include "FrequencyGroup.h"

#include <QString>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QNetworkInterface>

#include "Constants.h"


void SdrRfcCore::save()
{
    QFile save_file( Core::cfg_file );

    if (!save_file.open( QIODevice::WriteOnly ))
    {
        qWarning( "Couldn't open save file." );
        return;
    }

    QJsonObject object;
    write( object );
    QJsonDocument save_doc( object );
    save_file.write( save_doc.toJson() );
}

void SdrRfcCore::load()
{
    QFile load_file( Core::cfg_file );

    if (!load_file.open( QIODevice::ReadOnly ))
    {
        qWarning("Couldn't open save file.");
        return;
    }

    QByteArray save_data = load_file.readAll();

    QJsonDocument load_doc( QJsonDocument::fromJson( save_data ) );

    read( load_doc.object() );
}

void SdrRfcCore::connect()
{
    _connector.connect_to();
}

void SdrRfcCore::start()
{
    _acceptor.run();

    request_calibration_managment();
    request_vco_calibration();
    send_open_data_channel();
    send_target_mode_pkt();
    send_attenuation_pkt();
    send_rbw_pkt();
    send_vbw_pkt();
    send_frequency_pkt();
}

void SdrRfcCore::stop()
{
    _acceptor.stop();
    _connector.disconnect();
}

PluginCore::ConnectionState SdrRfcCore::connection_state() const
{
    if (_connector.is_connected())
    {
        if (_acceptor.is_client())
        {
            return ConnectionState_Connected;
        }
        else
        {
            return ConnectionState_Partially;
        }
    }
    return ConnectionState_Disconnected;
}

void SdrRfcCore::try_set_freq_start( QString const& start )
{
    if (_freq_group.try_set_start( FrequencyGroup::from_qstring( start ) ))
    {
        send_frequency_pkt();
    }
}

void SdrRfcCore::try_set_freq_stop( QString const& stop )
{
    if (_freq_group.try_set_stop( FrequencyGroup::from_qstring( stop ) ))
    {
        send_frequency_pkt();
    }
}

void SdrRfcCore::try_set_freq_span( QString const& span )
{
    if (_freq_group.try_set_span( FrequencyGroup::from_qstring( span ) ))
    {
        GET_CORE().send_frequency_pkt();
    }
}

void SdrRfcCore::try_set_freq_center( QString const& center )
{
    if (_freq_group.try_set_center( FrequencyGroup::from_qstring( center ) ))
    {
        GET_CORE().send_frequency_pkt();
    }
}

void SdrRfcCore::try_set_freq_center( quint64 center )
{
    if (_freq_group.try_set_center( center ))
    {
        GET_CORE().send_frequency_pkt();
    }
}

void SdrRfcCore::request_calibration_managment()
{
    qDebug() << "Request CalibrationManagementPacket";
    SdrRfcPacket<CalibrationManagementPacket> calibration_pkt( false );
    _connector.async_write( calibration_pkt.raw.constData(), calibration_pkt.size );
}

void SdrRfcCore::request_vco_calibration()
{
    qDebug() << "Request VcoCalibrationPacket";
    SdrRfcPacket<VcoCalibrationPacket> vco_calibration_pkt( false );
    _connector.async_write( vco_calibration_pkt.raw.constData(), vco_calibration_pkt.size );
}

void SdrRfcCore::send_open_data_channel()
{
    qDebug() << "Send OpenDataChannelPacket";
    SdrRfcPacket<OpenDataChannelPacket> open_data_channel_pkt( true );
    open_data_channel_pkt->payload.type = OpenDataChannelPacket::Type_Spectrogram;
    QHostAddress local_ipv4( _properties[Core::local_ip_v4].toString() );
    open_data_channel_pkt->payload.ipv4 = local_ipv4.toIPv4Address();
    open_data_channel_pkt->payload.port = _acceptor.port();

    _connector.async_write( open_data_channel_pkt.raw.constData(), open_data_channel_pkt.size );
}

void SdrRfcCore::send_target_mode_pkt()
{
    qDebug() << "Send SetTargetModePacket( mode = TargetModePacket::Mode_Spectr" << ", channel =" << _channel << ")";
    SdrRfcPacket<TargetModePacket> set_target_mode_pkt( true );
    set_target_mode_pkt->payload.mode = TargetModePacket::Mode_Spectr;
    set_target_mode_pkt->payload.channel = _channel;
    _connector.async_write( set_target_mode_pkt.raw.constData(), set_target_mode_pkt.size );
}

void SdrRfcCore::send_attenuation_pkt()
{
    qDebug() << "Send AttenuationPacket( att =" << _attenuation << ")";
    SdrRfcPacket<AttenuationPacket> att_pkt( true );
    att_pkt->payload.att = _attenuation;
    _connector.async_write( att_pkt.raw.constData(), att_pkt.size );
}

void SdrRfcCore::send_frequency_pkt()
{
    qDebug() << "Send FrequencyPacket( start =" << FrequencyGroup::to_qstring( _freq_group.start() )
             << ", stop =" << FrequencyGroup::to_qstring( _freq_group.stop() )
             << ")";
    SdrRfcPacket<FrequencyPacket> freq_pkt( true );
    freq_pkt->payload.start = _freq_group.start();
    freq_pkt->payload.stop = _freq_group.stop();
    _connector.async_write( freq_pkt.raw.constData(), freq_pkt.size );
}

void SdrRfcCore::send_rbw_pkt()
{
    qDebug() << "Send RBWPacket( rbw =" << _rbw_group.rbw()
             << ", win =" << _rbw_group.window()
             << ", algo =" << _rbw_group.algorithm()
             << ", sweep =" << _rbw_group.sweep_mode()
             << ")";
    SdrRfcPacket<RBWPacket> rbw_pkt( true );
    rbw_pkt->payload.rbw = _rbw_group.rbw();
    rbw_pkt->payload.window = _rbw_group.window();
    rbw_pkt->payload.algorithm = _rbw_group.algorithm();
    rbw_pkt->payload.sweep_mode = _rbw_group.sweep_mode();
    _connector.async_write( rbw_pkt.raw.constData(), rbw_pkt.size );
}

void SdrRfcCore::send_vbw_pkt()
{
    qDebug() << "Send VBWPacket( vbw =" << _rbw_group.vbw() << ", time = 100 " << ")";
    SdrRfcPacket<VBWPacket> vbw_pkt( true );
    vbw_pkt->payload.vbw = _rbw_group.vbw();
    vbw_pkt->payload.time = 100;
    _connector.async_write( vbw_pkt.raw.constData(), vbw_pkt.size );
}

void SdrRfcCore::send_calibration_managment_pkt()
{
    qDebug() << "Send CalibrationManagementPacket( level =" << _level_compensation << ")";
    SdrRfcPacket<CalibrationManagementPacket> calibration_pkt( true );
    calibration_pkt->payload.level_compensation = _level_compensation;
    _connector.async_write( calibration_pkt.raw.constData(), calibration_pkt.size );
}

void SdrRfcCore::set_channel( int ch )
{
    if (ch == 0 && _channel == TargetModePacket::Channel_RX2)
    {
        _channel = TargetModePacket::Channel_RX1;
        send_target_mode_pkt();
    }
    else if (ch == 1 && _channel == TargetModePacket::Channel_RX1)
    {
        _channel = TargetModePacket::Channel_RX2;
        send_target_mode_pkt();
    }
}

void SdrRfcCore::set_attenuation( qint8 value )
{
    if (_attenuation != value)
    {
        _attenuation = value;
        send_attenuation_pkt();
    }
}

void SdrRfcCore::set_rbw( quint32 rbw, RBWPacket::Window win, RBWPacket::Algorithm algo, RBWPacket::SweepMode sweep )
{
    _rbw_group.set_rbw( rbw );
    _rbw_group.set_window( win );
    _rbw_group.set_algorithm( algo );
    _rbw_group.set_sweet_mode( sweep );
    if (_rbw_group.is_changed())
    {
        send_rbw_pkt();
    }
}

void SdrRfcCore::set_vbw( quint32 vbw )
{
    _rbw_group.set_vbw( vbw );
    if (_rbw_group.is_vbw_changed())
    {
        send_vbw_pkt();
    }
}

void SdrRfcCore::set_level_compesation( quint32 level )
{
    CalibrationManagementPacket::LevelCompensation level_compensation =
            (level == 0) ?
                CalibrationManagementPacket::LevelCompensation_On
              : CalibrationManagementPacket::LevelCompensation_Off;
    if (_level_compensation != level_compensation)
    {
        _level_compensation = level_compensation;
        send_calibration_managment_pkt();
    }
}

void SdrRfcCore::on_cmd_packet_received( QByteArray const& packet )
{
    SdrRfcHeader const* header = reinterpret_cast<SdrRfcHeader const*>( packet.data() );

    //QString cmd_in_hex = QString("%1").arg( header->cmd, 0, 16 );
    //qDebug() << "SdrRfcCore: received cmd  " << cmd_in_hex;

    qDebug( "SdrRfcCore: received cmd = %#x, status = %#x", header->cmd, header->status );

    if (header->status)
    {
        emit response_error_received( header->status );
        return;
    }

    switch (header->cmd)
    {
    case Command_CalibrationManagement:
        if (header->len)
        {
            SdrRfcPacket<CalibrationManagementPacket> sdr_rfc_pkt( packet );
            _level_compensation = sdr_rfc_pkt->payload.level_compensation;
        }
        break;

    case Command_VcoCalibration:
        if (header->len)
        {
            SdrRfcPacket<VcoCalibrationPacket> sdr_rfc_pkt( packet );
            _vco_value = sdr_rfc_pkt->payload.value_mv;
        }
        break;

    case Command_OpenDataChannel:
    case Command_TargetMode:
    case Command_Attenuation:
    case Command_RBW:
    case Command_VBW:
    case Command_Frequency:
        break;

    case Command_BusyReport:
        {
            SdrRfcPacket<BusyReportPacket> sdr_rfc_pkt( packet );
            emit busy_report_received( sdr_rfc_pkt->payload.ipv4 );
        }
        break;

    default:
        QString cmd_in_hex = QString("%1").arg( header->cmd, 0, 16 );
        qDebug() << "Unknown command: " << cmd_in_hex;
        break;
    }

    //emit next_start_cmd( header->cmd );
    if (header->len)
    {
        emit response_received( header->cmd );
    }
}

void SdrRfcCore::on_data_received( QByteArray const& buffer )
{
    _fft_data.parse( buffer );
    if (_fft_data.is_ready())
    {
        int const size = _fft_data.get_data().size();
        QVector<double> freq;
        freq.resize( size );
        freq[0] = _fft_data.start();
        for (int i = 1; i < size; ++i)
        {
            freq[i] = freq[i - 1] + _fft_data.step();
        }
        emit fft_data_ready( freq.data(), _fft_data.get_data().data(), size );
    }
}

SdrRfcCore::SdrRfcCore()
    : _channel( TargetModePacket::Channel_RX1 )
    , _attenuation( 50 )
{
    QObject::connect( &_connector, SIGNAL( connected() ), this, SIGNAL( connection_state_changed() ) );
    QObject::connect( &_connector, SIGNAL( disconnected() ), this, SIGNAL( connection_state_changed() ) );
    QObject::connect( &_connector, SIGNAL( packet_received( QByteArray const& ) ), this, SLOT( on_cmd_packet_received( QByteArray const& ) ) );
    QObject::connect( &_acceptor, SIGNAL( new_connection() ), this, SIGNAL( connection_state_changed() ) );
    QObject::connect( &_acceptor, SIGNAL( packet_received( QByteArray const& ) ), this, SLOT( on_data_received( QByteArray const& ) ) );

    foreach (QHostAddress const& address, QNetworkInterface::allAddresses())
    {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost))
        {
            QString const& str_addr( address.toString() );
            _properties.insert( Core::local_ip_v4, str_addr );
             break;
        }
    }
}

SdrRfcCore::~SdrRfcCore()
{

}

void SdrRfcCore::read( QJsonObject const& json )
{
    if (json.contains( Core::core ) && json[Core::core].isObject())
        _properties.load( json[Core::core].toObject() );

    if (json.contains( Core::connector ) && json[Core::connector].isObject())
        _connector.read(json[Core::connector].toObject());

    if (json.contains( Core::acceptor ) && json[Core::acceptor].isObject())
        _acceptor.read(json[Core::acceptor].toObject());
}

void SdrRfcCore::write( QJsonObject& json )
{
    {
        QJsonObject core_object;
        _connector.write( core_object );
        json[Core::core] = core_object;
    }

    {
        QJsonObject connector_object;
        _connector.write( connector_object );
        json[Core::connector] = connector_object;
    }

    {
        QJsonObject acceptor_object;
        _acceptor.write( acceptor_object );
        json[Core::acceptor] = acceptor_object;
    }
}
