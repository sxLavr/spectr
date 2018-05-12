#ifndef SDRRFCDOCKWIDGET_H
#define SDRRFCDOCKWIDGET_H


#include <QDockWidget>


namespace Ui {
    class SdrRfcDockWidget;
}

class SdrRfcDockWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit SdrRfcDockWidget( QWidget* parent = 0 );
    ~SdrRfcDockWidget();

public slots:
    void update_frequency_group();

private slots:
    void frequency_start_changed();
    void frequency_stop_changed();
    void frequency_span_changed();
    void frequency_center_changed();
    void channel_changed( int );
    void att_changed( int );
    void rbw_changed( int );
    void vbw_changed( int );
    void calibration_changed( int );

private:
    void update_channel();
    void update_attenuation();
    void update_rbw();
    void update_calibration();

private:
    Ui::SdrRfcDockWidget* _ui;
};

#endif // SDRRFCDOCKWIDGET_H
