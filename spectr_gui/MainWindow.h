#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Plotter.h"
#include "PluginInterface.h"
#include "PluginCore.h"

#include <QLineEdit>
#include <QLabel>
#include <QVector>


namespace Ui {
    class MainWindow;
}

class QwtPlotPicker;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void fft_data_ready( const double *frequency, const double *amplitude, int count );
    void connection_state_changed();

private slots:
    // Menu and toolbar actions
    int on_actionDevConfig_triggered();
    void on_actionStart_triggered( bool );

    // Plot picker
    void moved( const QPoint & );
    void selected( const QPolygon & );

private:
    void show_connection_state();
    void showInfo( QString text = QString::null );

    Ui::MainWindow*             _ui;
    QLineEdit*                  _leConnection;
    QLabel*                     _lbConnection;
    Plotter*                    _plotter;
    QwtPlotPicker*              _picker;
    QVector<PluginInterface*>   _devices;
    PluginDeviceInterface*      _device;
    QScopedPointer<QDockWidget> _device_dock;
};

#endif // MAINWINDOW_H
