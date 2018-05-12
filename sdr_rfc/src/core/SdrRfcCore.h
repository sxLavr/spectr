#ifndef SDRRFCCORE_H
#define SDRRFCCORE_H


#include "Properties.h"
#include "TcpConnector.h"
#include "TcpAcceptor.h"
#include "SdrRfcProtocol.h"
#include "FftData.h"
#include "FrequencyGroup.h"
#include "RbwGroup.h"
#include "PluginCore.h"

#include <QObject>
#include <QJsonObject>


#define GET_CORE()                  SdrRfcCore::Instance()
#define GET_CONNECTOR()             GET_CORE().get_connector()
#define GET_ACCEPTOR()              GET_CORE().get_acceptor()
#define GET_PROPERTIES()            GET_CORE().get_properties()

#define GET_CHANNEL_GROUP()         GET_CORE()
#define GET_GAIN_GROUP()            GET_CORE()
#define GET_RBW_GROUP()             GET_CORE().get_rbw_group()
#define GET_FREQ_GROUP()            GET_CORE().get_freq_group()
#define GET_CALIBR_GROUP()          GET_CORE()


class SdrRfcCore
        : public PluginCore
{
    Q_OBJECT

public:
    static SdrRfcCore& Instance()
    {
        static SdrRfcCore _instance;
        return _instance;
    }

    // PluginCore
    virtual void connect();
    virtual void start();
    virtual void stop();
    virtual ConnectionState connection_state() const;

    Properties& get_properties() { return _properties; }
    TcpConnector& get_connector() { return _connector; }
    TcpAcceptor& get_acceptor() { return _acceptor; }
    FrequencyGroup& get_freq_group() { return _freq_group; }
    RbwGroup& get_rbw_group() { return _rbw_group; }

    void save();
    void load();

    void try_set_freq_start( QString const& start );
    void try_set_freq_stop( QString const& stop );
    void try_set_freq_span( QString const& span );
    void try_set_freq_center( QString const& center );
    void try_set_freq_center( quint64 center );

    // getters
    quint32 channel() const { return _channel; }
    qint8 attenuation() const { return _attenuation; }
    bool level_compensation() const { return !_level_compensation; }
    int vco_value() const { return _vco_value; }

    // setters
    void set_channel( int ch );
    void set_attenuation( qint8 value );
    void set_rbw( quint32 rbw, RBWPacket::Window win, RBWPacket::Algorithm algo, RBWPacket::SweepMode sweep );
    void set_vbw( quint32 vbw );
    void set_level_compesation( quint32 level );

public slots:
    void on_cmd_packet_received( QByteArray const& packet );
    void on_data_received( QByteArray const& buffer );

signals:
    void response_error_received( qint32 status );
    void response_received( Command cmd );
    void next_start_cmd( Command cmd );
    void busy_report_received( quint32 );

private:
    SdrRfcCore();
    virtual ~SdrRfcCore();

    void read( QJsonObject const& json );
    void write( QJsonObject& json );

    void request_calibration_managment();
    void request_vco_calibration();

    void send_open_data_channel();

    void send_target_mode_pkt();
    void send_attenuation_pkt();
    void send_frequency_pkt();
    void send_rbw_pkt();
    void send_vbw_pkt();
    void send_calibration_managment_pkt();

    Properties      _properties;
    TcpConnector    _connector;
    TcpAcceptor     _acceptor;

    FftData         _fft_data;

    TargetModePacket::Channel                       _channel;
    qint8                                           _attenuation;
    RbwGroup                                        _rbw_group;
    FrequencyGroup                                  _freq_group;
    CalibrationManagementPacket::LevelCompensation  _level_compensation;
    quint32                                         _vco_value;
};


#endif // SDRRFCCORE_H
