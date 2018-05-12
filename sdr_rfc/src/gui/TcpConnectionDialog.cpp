
#include "TcpConnectionDialog.h"
#include "ui_TcpConnectionDialog.h"
#include "SdrRfcCore.h"
#include "Constants.h"


namespace
{
    QString const DefaultTargetAddress( "192.168.2.80" );
    quint16 const DefaultTargetPort = 999;
    quint16 const DefaultLocalPort = 9999;
}


TcpConnectionDialog::TcpConnectionDialog( QWidget* parent )
    : QDialog( parent )
    , _ui( new Ui::TcpConnectionDialog )
{
    _ui->setupUi( this );

    QRegExp regExp( "\\b(([01]?\\d?\\d|2[0-4]\\d|25[0-5])\\.){3}([01]?\\d?\\d|2[0-4]\\d|25[0-5])\\b" );
    _ui->DeviceIpAddress->setValidator( new QRegExpValidator( regExp, this ) );
    _ui->LocalIpAddress->setValidator( new QRegExpValidator( regExp, this ) );

    _ui->LocalIpAddress->setReadOnly( true );

    connect( _ui->DeviceIpAddress, SIGNAL( textChanged( QString ) ), this, SLOT( slot_data_changed() ) );
    connect( _ui->DevicePort, SIGNAL( valueChanged(int)), this, SLOT( slot_data_changed() ) );
    //connect( _ui->LocalIpAddress, SIGNAL( textChanged( QString ) ), this, SLOT( slot_data_changed() ) );
    connect( _ui->LocalPort, SIGNAL( valueChanged(int)), this, SLOT( slot_data_changed() ) );

    if (GET_CONNECTOR().address().isEmpty())
    {
        _ui->DeviceIpAddress->setText( DefaultTargetAddress );
        _ui->DevicePort->setValue( DefaultTargetPort );
        _ui->LocalPort->setValue( DefaultLocalPort );
    }
    else
    {
        _ui->DeviceIpAddress->setText( GET_CONNECTOR().address() );
        _ui->DevicePort->setValue( GET_CONNECTOR().port() );
    }
    _ui->LocalIpAddress->setText( GET_PROPERTIES().valueS( Core::local_ip_v4 ) );
}

TcpConnectionDialog::~TcpConnectionDialog()
{
    delete _ui;
}

void TcpConnectionDialog::slot_data_changed()
{
    GET_CONNECTOR().set_target( _ui->DeviceIpAddress->text(), _ui->DevicePort->text().toShort() );
    GET_ACCEPTOR().set_port( _ui->LocalPort->text().toShort() );
}
