
#include "TcpAcceptor.h"
#include "Constants.h"

#include <QTcpSocket>


TcpAcceptor::TcpAcceptor( QObject* parent )
    : QObject( parent )
    , _server( this )
{

}

TcpAcceptor::~TcpAcceptor()
{
    stop();
}

void TcpAcceptor::run()
{
    connect( &_server, SIGNAL( newConnection() ), this, SLOT( on_new_connection() ) );

    if (false == _server.listen( QHostAddress::Any, _port ))
    {
        qDebug() << "Can't start tcp server";
    }
}

void TcpAcceptor::stop()
{
    if (_client)
        _client->close();
    _server.close();
}

void TcpAcceptor::read( QJsonObject const& json )
{
    if (json.contains( Local::port ) && json[Local::port].isDouble())
        _port = json[Local::port].toInt();
}

void TcpAcceptor::write( QJsonObject& json ) const
{
    json[Local::port] = _port;
}

void TcpAcceptor::on_new_connection()
{
    if(_client)
        return;

    _client = _server.nextPendingConnection();
    if (!_client)
    {
        qDebug() << "Error: socket == nullptr";
        return;
    }

    qDebug() << "TcpAcceptor: is new connection: " << _client->peerAddress().toString();

    connect( _client, SIGNAL( readyRead() ), this, SLOT( on_ready_read() ) );
    emit new_connection();
}

void TcpAcceptor::on_ready_read()
{
    QByteArray in_buffer( _client->readAll() );
    emit packet_received( in_buffer );
}
