#ifndef TCPSOCKET_H
#define TCPSOCKET_H


#include <QObject>
#include <QTcpSocket>
#include <QMutex>
#include <QHostAddress>
#include <QList>


class TcpSocket
        : public QTcpSocket
{
    Q_OBJECT
public:
    explicit TcpSocket( QObject* parent = NULL );

    void connect_to( QHostAddress const& host, quint16 port, OpenMode openMode = ReadWrite );
    bool is_connected() const { return _connected; }

    void async_write( char const* data, size_t len );

private slots:
    void on_ready_read();
    void on_about_to_close();
    void on_socket_error( QAbstractSocket::SocketError error );
    void on_state_changed( QAbstractSocket::SocketState state );
    void on_bytes_written( qint64 size );

signals:
    void socket_connected();
    void socket_disconnected();
    void packet_received( QByteArray const& packet );

private:
    void send_packet();

    bool                _connected;
    bool                _sending;
    QMutex              _mutex_out;
    QList<QByteArray>   _buffer_out;
    QMutex              _data_sent_mutex;
    quint64             _data_sent;
};

#endif // TCPSOCKET_H
