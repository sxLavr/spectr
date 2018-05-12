
#include "SdrRfcDockWidget.h"
#include "ui_SdrRfcDockWidget.h"

#include "SdrRfcCore.h"


SdrRfcDockWidget::SdrRfcDockWidget( QWidget* parent ) :
    QDockWidget( parent ),
    _ui( new Ui::SdrRfcDockWidget )
{
    _ui->setupUi( this );

    {
        // GroupBox Frequency

        QRegExpValidator* validator = new QRegExpValidator( QRegExp( "^([1-9][0-9]*|0)(\\.|,{0})[0-9]{0,3}[k,M,G]{1}[Hz]{0,2}" ), this );
        _ui->leFreqStart->setValidator( validator );
        _ui->leFreqStop->setValidator( validator );
        _ui->leFreqSpan->setValidator( validator );
        _ui->leFreqCenter->setValidator( validator );

        update_frequency_group();

        connect( _ui->leFreqStart, SIGNAL( returnPressed() ), this, SLOT( frequency_start_changed() ) );
        connect( _ui->leFreqStop, SIGNAL( returnPressed() ), this, SLOT( frequency_stop_changed() ) );
        connect( _ui->leFreqSpan, SIGNAL( returnPressed() ), this, SLOT( frequency_span_changed() ) );
        connect( _ui->leFreqCenter, SIGNAL( returnPressed() ), this, SLOT( frequency_center_changed() ) );
    }

    {
        // GroupBox Channel
        update_channel();
        connect( _ui->cbChannel, SIGNAL( activated( int ) ), this, SLOT( channel_changed( int ) ) );
    }

    {
        // GroupBox Gain
        update_attenuation();
        connect( _ui->cbGainAutoAtt, SIGNAL( stateChanged( int ) ), this, SLOT( att_changed( int ) ) );
        connect( _ui->sbGainAtt, SIGNAL( valueChanged( int ) ), this, SLOT( att_changed( int ) ) );
    }

    {
        // GroupBoxRBW
        _ui->cbRbwWidth->addItem( "30 Hz", 30 );
        _ui->cbRbwWidth->addItem( "100 Hz", 100 );
        _ui->cbRbwWidth->addItem( "300 Hz", 300 );
        _ui->cbRbwWidth->addItem( "1 kHz", 1000 );
        _ui->cbRbwWidth->addItem( "10 kHz", 10000 );
        _ui->cbRbwWidth->addItem( "30 kHz", 30000 );
        _ui->cbRbwWidth->addItem( "300 kHz", 300000 );
        _ui->cbRbwWidth->addItem( "1 MHz", 1000000 );
        _ui->cbRbwWidth->addItem( "3 kHz", 3000000 );

        _ui->cbRbwWindow->addItem( "RECTANGLE", 0 );
        _ui->cbRbwWindow->addItem( "HANN", 1 );
        _ui->cbRbwWindow->addItem( "HAMMING", 4 );
        _ui->cbRbwWindow->addItem( "BLACKMAN", 5 );
        _ui->cbRbwWindow->addItem( "NUTTALL", 7 );
        _ui->cbRbwWindow->addItem( "FLATTOP", 12 );

        _ui->cbRbwVbw->addItem( "1 Hz", 1 );
        _ui->cbRbwVbw->addItem( "3 Hz", 3 );
        _ui->cbRbwVbw->addItem( "10 Hz", 10 );
        _ui->cbRbwVbw->addItem( "30 Hz", 30 );
        _ui->cbRbwVbw->addItem( "100 Hz", 100 );
        _ui->cbRbwVbw->addItem( "300 Hz", 300 );
        _ui->cbRbwVbw->addItem( "1 kHz", 1000 );
        _ui->cbRbwVbw->addItem( "10 kHz", 10000 );
        _ui->cbRbwVbw->addItem( "30 kHz", 30000 );
        _ui->cbRbwVbw->addItem( "300 kHz", 300000 );
        _ui->cbRbwVbw->addItem( "1 MHz", 1000000 );
        _ui->cbRbwVbw->addItem( "3 kHz", 3000000 );

        update_rbw();

        connect( _ui->cbRbwWindow, SIGNAL( activated( int ) ), this, SLOT( rbw_changed( int ) ));
        connect( _ui->cbRbwWidth, SIGNAL( activated( int ) ), this, SLOT( rbw_changed( int ) ));
        connect( _ui->cbRbwMode, SIGNAL( activated( int ) ), this, SLOT( rbw_changed( int ) ));
        connect( _ui->cbRbwEnable, SIGNAL( stateChanged( int ) ), this, SLOT( rbw_changed( int ) ));

        connect( _ui->cbRbwVbw, SIGNAL( activated( int ) ), this, SLOT( vbw_changed( int ) ));
    }

    {
        // GroupBox Calibration
        connect( _ui->cbLevelComp, SIGNAL( stateChanged( int ) ), this, SLOT( calibration_changed( int ) ) );
    }
}

SdrRfcDockWidget::~SdrRfcDockWidget()
{
    delete _ui;
}

void SdrRfcDockWidget::update_frequency_group()
{
    _ui->leFreqStart->setText( GET_FREQ_GROUP().string_start() );
    _ui->leFreqStop->setText( GET_FREQ_GROUP().string_stop() );
    _ui->leFreqSpan->setText( GET_FREQ_GROUP().string_span() );
    _ui->leFreqCenter->setText( GET_FREQ_GROUP().string_center() );
}

void SdrRfcDockWidget::frequency_start_changed()
{
    GET_CORE().try_set_freq_start( _ui->leFreqStart->text() );
    update_frequency_group();
}

void SdrRfcDockWidget::frequency_stop_changed()
{
    GET_CORE().try_set_freq_stop( _ui->leFreqStop->text() );
    update_frequency_group();
}

void SdrRfcDockWidget::frequency_span_changed()
{
    GET_CORE().try_set_freq_span( _ui->leFreqSpan->text() );
    update_frequency_group();
}

void SdrRfcDockWidget::frequency_center_changed()
{
    GET_CORE().try_set_freq_center( _ui->leFreqCenter->text() );
    update_frequency_group();
}

void SdrRfcDockWidget::channel_changed( int index )
{
    GET_CHANNEL_GROUP().set_channel( index );
    update_channel();
}

void SdrRfcDockWidget::att_changed(int)
{
    quint8 value = (_ui->cbGainAutoAtt->checkState() == Qt::Checked) ? -1 : _ui->sbGainAtt->value();
    GET_CORE().set_attenuation( value );
    update_attenuation();
}

void SdrRfcDockWidget::rbw_changed(int)
{
    quint32 rbw = _ui->cbRbwWidth->itemData( _ui->cbRbwWidth->currentIndex() ).toInt();
    RBWPacket::Window win = (RBWPacket::Window)_ui->cbRbwWindow->itemData( _ui->cbRbwWindow->currentIndex() ).toInt();
    RBWPacket::Algorithm algo = (_ui->cbRbwEnable->checkState() == Qt::Checked) ? RBWPacket::Algorithm_StepOfPoints : RBWPacket::Algorithm_RBW;
    RBWPacket::SweepMode sweep_mode = (RBWPacket::SweepMode)_ui->cbRbwMode->currentIndex();
    GET_CORE().set_rbw( rbw, win, algo, sweep_mode );
    update_rbw();
}

void SdrRfcDockWidget::vbw_changed(int)
{
    int index = _ui->cbRbwVbw->currentIndex();
    quint32 vbw = _ui->cbRbwVbw->itemData( index ).toInt();
    GET_CORE().set_vbw( vbw );
    update_rbw();
}

void SdrRfcDockWidget::calibration_changed(int)
{
    int enable_comp = _ui->cbLevelComp->checkState();
    GET_CORE().set_level_compesation( enable_comp );
}

void SdrRfcDockWidget::update_channel()
{
    _ui->cbChannel->setCurrentIndex( GET_CHANNEL_GROUP().channel() );
}

void SdrRfcDockWidget::update_attenuation()
{
    qint8 att = GET_GAIN_GROUP().attenuation();
    _ui->cbGainAutoAtt->setChecked( att == -1 );
    _ui->sbGainAtt->setEnabled( att != -1 );
    if (att == 1)
        _ui->sbGainAtt->setValue( 0 );
    else
        _ui->sbGainAtt->setValue( att );
}

void SdrRfcDockWidget::update_rbw()
{
    _ui->cbRbwWidth->setCurrentIndex( _ui->cbRbwWidth->findData( GET_RBW_GROUP().rbw() ) );
    _ui->cbRbwWindow->setCurrentIndex( _ui->cbRbwWindow->findData( GET_RBW_GROUP().window() ) );
    _ui->cbRbwEnable->setChecked( GET_RBW_GROUP().algorithm() );
    _ui->cbRbwMode->setCurrentIndex( GET_RBW_GROUP().sweep_mode() );
    _ui->cbRbwVbw->setCurrentIndex( _ui->cbRbwVbw->findData( GET_RBW_GROUP().vbw() ) );
}

void SdrRfcDockWidget::update_calibration()
{
    _ui->cbLevelComp->setChecked( GET_CALIBR_GROUP().level_compensation() );
    _ui->sbVcoValue->setValue( GET_CALIBR_GROUP().vco_value() );
}
