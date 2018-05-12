
#include "TcpConnector.h"
#include "Constants.h"


TcpConnector::TcpConnector( QObject* parent )
    : QObject( parent )
    , _socket( this )
{
    QObject::connect( &_socket, SIGNAL( socket_connected() ), this, SIGNAL( connected() ));
    QObject::connect( &_socket, SIGNAL( socket_disconnected() ), this, SIGNAL( disconnected() ));
    QObject::connect( &_socket, SIGNAL( packet_received( QByteArray const& ) ), this, SIGNAL( packet_received( QByteArray const& ) ));
}

TcpConnector::~TcpConnector()
{
    disconnect();
}

void TcpConnector::set_target( QString const& address, quint16 port )
{
    _address = address;
    _port = port;
}

void TcpConnector::connect_to()
{
    _socket.connectToHost( _address, _port );
}

void TcpConnector::disconnect()
{
    _socket.disconnectFromHost();
}

void TcpConnector::async_write( char const* data, size_t len )
{
    _socket.async_write( data, len );
}

void TcpConnector::read( QJsonObject const& json )
{
    if (json.contains( Target::ip_v4 ) && json[Target::ip_v4].isString())
        _address = json[Target::ip_v4].toString();

    if (json.contains( Target::port ) && json[Target::port].isDouble())
        _port = json[Target::port].toInt();
}

void TcpConnector::write( QJsonObject& json ) const
{
    json[Target::ip_v4] = _address;
    json[Target::port] = _port;
}

