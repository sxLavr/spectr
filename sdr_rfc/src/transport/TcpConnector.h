#ifndef TCPCONNECTOR_H
#define TCPCONNECTOR_H


#include "TcpSocket.h"
#include <QObject>
#include <QJsonObject>


class TcpConnector : public QObject
{
    Q_OBJECT

public:
    explicit TcpConnector( QObject *parent = nullptr );
    virtual ~TcpConnector();

    void set_target( QString const& address, quint16 port );
    QString address() const { return _address; }
    quint16 port() const { return _port; }

    void connect_to();
    void disconnect();
    bool is_connected() const { return _socket.is_connected(); }

    void async_write( char const* data, size_t len );

    void read( QJsonObject const& json );
    void write( QJsonObject& json ) const;

signals:
    void connected();
    void disconnected();
    void packet_received( QByteArray const& packet );

public slots:

private:
    TcpSocket   _socket;
    QString     _address;
    quint16     _port;
};

#endif // TCPCONNECTOR_H
