#ifndef TCPACCEPTOR_H
#define TCPACCEPTOR_H


#include <QTcpServer>
#include <QObject>
#include <QJsonObject>
#include <QTcpSocket>


class TcpAcceptor : public QObject
{
    Q_OBJECT

public:
    explicit TcpAcceptor( QObject* parent = nullptr );
    virtual ~TcpAcceptor();

    void set_port( quint16 port ) { _port = port; }
    quint16 port() const { return _port; }

    void run();
    void stop();

    void read( QJsonObject const& json );
    void write( QJsonObject& json ) const;

    bool is_client() const { return _client; }

signals:
    void packet_received( QByteArray const& buffer );
    void new_connection();

public slots:

private slots:
    void on_new_connection();
    void on_ready_read();

private:
    QTcpServer  _server;
    quint16     _port;
    QTcpSocket* _client;
};

#endif // TCPACCEPTOR_H
