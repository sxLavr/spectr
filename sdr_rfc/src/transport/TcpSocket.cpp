#include "TcpSocket.h"


TcpSocket::TcpSocket( QObject* parent/* = NULL*/ )
    : QTcpSocket( parent )
    , _connected( false )
    , _sending( false )
    , _data_sent( 0 )
{
    connect( this, SIGNAL( readyRead() ),                                  this, SLOT(on_ready_read()));
    connect( this, SIGNAL( aboutToClose() ),                               this, SLOT(on_about_to_close()));
    connect( this, SIGNAL( bytesWritten( qint64 ) ),                       this, SLOT(on_bytes_written(qint64)));
    connect( this, SIGNAL( error( QAbstractSocket::SocketError ) ),        this, SLOT(on_socket_error(QAbstractSocket::SocketError)));
    connect( this, SIGNAL( stateChanged( QAbstractSocket::SocketState ) ), this, SLOT(on_state_changed(QAbstractSocket::SocketState)));
}

void TcpSocket::connect_to( QHostAddress const& host, quint16 port, QIODevice::OpenMode openMode )
{
    QTcpSocket::connectToHost( host, port, openMode );
}

void TcpSocket::async_write( char const* data, size_t len )
{
    _mutex_out.lock();
    _buffer_out.append( QByteArray( data, len ) );
    _mutex_out.unlock();
    send_packet();
}

void TcpSocket::on_ready_read()
{
    QByteArray packet( readAll() );
    emit packet_received( packet );
    send_packet();
}

void TcpSocket::on_about_to_close()
{

}

void TcpSocket::on_socket_error( QAbstractSocket::SocketError error )
{
    switch (error)
    {
    case QAbstractSocket::ConnectionRefusedError:
    case QAbstractSocket::RemoteHostClosedError:
    case QAbstractSocket::NetworkError:
        if (_connected)
        {
            _connected = false;
            emit socket_disconnected();
            close();
        }
        break;

    case QAbstractSocket::SocketTimeoutError:
        break;

    default:
        qDebug () << QString("Socket [%1] has error: %2").arg(socketDescriptor()).arg(errorString());
    }

    qDebug () << QString("Socket [%1] has error: %2").arg(socketDescriptor()).arg(errorString());
}

void TcpSocket::on_state_changed( QAbstractSocket::SocketState state )
{
    switch (state)
    {
    case QAbstractSocket::UnconnectedState:
        if (_connected)
        {
            _connected = false;
            emit socket_disconnected();
            close();
        }
        break;

    case QAbstractSocket::ConnectedState:
        if (!_connected)
        {
            _connected = true;
            emit socket_connected();
            send_packet();
        }
        break;

    default:
        break;
    }
}

void TcpSocket::on_bytes_written( qint64 size )
{
    QMutexLocker sentLocker( &_data_sent_mutex );
    _data_sent += size;
    QMutexLocker bufferLocker( &_mutex_out );
    if (true == _buffer_out.isEmpty())
    {
        return;
    }

    quint64 const cmd_size = _buffer_out.first().size();
    if (_data_sent < cmd_size)
    {
        return;
    }

    _data_sent -= cmd_size;

    _buffer_out.removeFirst();
    bufferLocker.unlock();
    _sending = false;
    //send_packet();
}

void TcpSocket::send_packet()
{
    if (_sending || !_connected) {
        return;
    }

    QMutexLocker bufferLocker( &_mutex_out );
    if (_buffer_out.isEmpty())
    {
        return;
    }
    QByteArray& data = _buffer_out.first();

    qint64 res = QTcpSocket::write( data );
    if (res == data.size())
    {
        _sending = true;
    }
}
