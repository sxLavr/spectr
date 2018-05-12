
#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "ChoiceDeviceDialog.h"
#include "DockWidgetFft.h"

#include <QDir>
#include <QPluginLoader>
#include <QDebug>
#include <QDialog>
#include <QLayout>
#include <QScopedPointer>

#include <qwt_picker_machine.h>
#include <qwt_plot_picker.h>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow( parent )
    , _ui( new Ui::MainWindow )
    , _device( NULL )
{
    _ui->setupUi( this );

    {
        _plotter = new Plotter( this );
        setCentralWidget( _plotter );

        _picker = new QwtPlotPicker(
                    QwtPlot::xBottom,
                    QwtPlot::yLeft,
                    QwtPlotPicker::CrossRubberBand,
                    QwtPicker::AlwaysOn,
                    _plotter->canvas()
                    );
        _picker->setStateMachine( new QwtPickerDragPointMachine() );
        _picker->setRubberBandPen( QColor( Qt::green ) );
        _picker->setRubberBand( QwtPicker::CrossRubberBand );
        _picker->setTrackerPen( QColor( Qt::white ) );

        connect( _picker, SIGNAL( moved( QPoint const& ) ), SLOT( moved( QPoint const& ) ) );
        connect( _picker, SIGNAL( selected( QPolygon const& ) ), SLOT( selected( QPolygon const& ) ) );
    }

    {
        setCorner( Qt::TopLeftCorner, Qt::LeftDockWidgetArea );
        setCorner( Qt::TopRightCorner, Qt::RightDockWidgetArea );
        setCorner( Qt::BottomLeftCorner, Qt::BottomDockWidgetArea );
        setCorner( Qt::BottomRightCorner, Qt::RightDockWidgetArea );

        _ui->menuView->addAction( _ui->mainToolBar->toggleViewAction() );
        _ui->menuView->addSeparator();

        DockWidgetFft* dock_fft = new DockWidgetFft( this );
        addDockWidget( Qt::RightDockWidgetArea, dock_fft );
        _ui->menuView->addAction( dock_fft->toggleViewAction() );
    }

    {
        QDir plugins_dir( "./plugins" );
        foreach( QString const& plugin_name, plugins_dir.entryList( QDir::Files ) )
        {
            qDebug() << "===============================================================================";
            qDebug() << "Found plugin:" << plugin_name;

            QPluginLoader loader( plugins_dir.absoluteFilePath( plugin_name ) );
            if( loader.load() )
            {
                if ( PluginInterface* plugin = qobject_cast< PluginInterface* >( loader.instance() ) )
                {
                    qDebug() << "Plugin name: " << plugin->get_name();
                    switch (plugin->get_type())
                    {
                    case PluginInterface::Type_Device:
                        _devices.append( plugin );
                        break;
                    }
                }
            }
            else
            {
                qDebug() << "Failed to load :(";
                qDebug() << loader.errorString();
            }

            qDebug() << "";
        }
    }

    {
        // Status bar

        _lbConnection = new QLabel();
        _lbConnection->setFixedSize( 16, 16 );
        _lbConnection->setScaledContents( true );

        _leConnection = new QLineEdit();
        _leConnection->setFixedWidth( 130 );
        _leConnection->setReadOnly( true );
        _leConnection->setFrame( false );

        QWidget* w = new QWidget( this );
        w->setFixedWidth( 200 );

        QHBoxLayout* layout = new QHBoxLayout( w );
        layout->setContentsMargins( 0, 0, 0, 0 );
        layout->setAlignment( Qt::AlignLeft | Qt::AlignVCenter );

        layout->addWidget( _lbConnection );
        layout->addWidget( new QLabel( tr( "Target:" ) ) );
        layout->addWidget( _leConnection );

        statusBar()->addPermanentWidget( new QLabel() );
        statusBar()->addPermanentWidget( w, 1 );
    }
}

MainWindow::~MainWindow()
{
    delete _plotter;
    delete _ui;
}

void MainWindow::fft_data_ready( double const* frequency, double const* amplitude, int count )
{
    _plotter->showData( frequency, amplitude, count );
}

void MainWindow::connection_state_changed()
{
    show_connection_state();
}

int MainWindow::on_actionDevConfig_triggered()
{
    qDebug() << "Show device selection dialog";

    ChoiceDeviceDialog dlg;

    foreach( PluginInterface const* plugin, _devices )
    {
        dlg.add_item( plugin->get_name(), QVariant() );
    }

    if (_ui->actionStart->isChecked())
    {
        on_actionStart_triggered( false );
    }

    int result = dlg.exec();

    if (result == QDialog::Accepted)
    {
        PluginDeviceInterface* device = static_cast<PluginDeviceInterface*>( _devices[dlg.current_index()] );

        QScopedPointer<QDialog> connection_dlg( device->create_connection_dialog( this ) );
        result = connection_dlg->exec();
        if (result == QDialog::Accepted)
        {
            if (_device)
            {
                disconnect( _device->get_core(), SIGNAL(fft_data_ready(const double*,const double*,int)), this, SLOT(fft_data_ready(const double*,const double*,int)));
                disconnect( _device->get_core(), SIGNAL( connection_state_changed() ), this, SLOT( connection_state_changed() ) );
            }
            if (_device_dock.data())
            {
                removeDockWidget( _device_dock.data() );
                _ui->menuView->removeAction( _device_dock->toggleViewAction() );
            }


            _device = device;
            _device_dock.reset( _device->create_dock_widget( this ) );
            addDockWidget( Qt::RightDockWidgetArea, _device_dock.data() );
            _ui->menuView->addAction( _device_dock->toggleViewAction() );

            connect( _device->get_core(), SIGNAL( connection_state_changed() ), this, SLOT( connection_state_changed() ) );
            connect( _device->get_core(), SIGNAL(fft_data_ready(const double*,const double*,int)), this, SLOT(fft_data_ready(const double*,const double*,int)));

            _device->get_core()->connect();

            show_connection_state();
        }
    }

    return result;
}

void MainWindow::on_actionStart_triggered( bool checked )
{
    if (!_device)
        return;

    if (checked)
    {
        _device->get_core()->start();
    }
    else
    {
        _device->get_core()->stop();
    }
}

void MainWindow::moved( const QPoint &pos )
{
    QString info;
    info.sprintf( "Freq=%g, Ampl=%g",
        _plotter->invTransform( QwtPlot::xBottom, pos.x() ),
        _plotter->invTransform( QwtPlot::yLeft, pos.y() )
    );
    showInfo( info );
}

void MainWindow::selected( const QPolygon & )
{
    showInfo();
}

void MainWindow::show_connection_state()
{
    if (!_device)
        return;

    PluginCore::ConnectionState state = _device->get_core()->connection_state();

    switch (state)
    {
    case PluginCore::ConnectionState_Disconnected:
        _lbConnection->setPixmap( QPixmap( ":/images/ledRed.png" ) );
        break;

    case PluginCore::ConnectionState_Connected:
        _lbConnection->setPixmap( QPixmap( ":/images/ledGreen.png" ) );
        break;

    case PluginCore::ConnectionState_Partially:
        _lbConnection->setPixmap( QPixmap( ":/images/ledYellow.png" ) );
        break;
    }
}

void MainWindow::showInfo( QString text )
{
    if ( text == QString::null )
    {
        if ( _picker->rubberBand() )
            text = "Cursor Pos: Press left mouse button in plot region";
        else
            text = "Zoom: Press mouse button and drag";
    }

#ifndef QT_NO_STATUSBAR
    statusBar()->showMessage( text );
#endif
}
