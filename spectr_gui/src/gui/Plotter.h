#ifndef PLOTTER_H
#define PLOTTER_H


#include <qwt_plot.h>


class QwtPlotCurve;
class QwtPlotMarker;

class Plotter
        : public QwtPlot
{
    Q_OBJECT

public:
    explicit Plotter( QWidget* parent );

    void showData( const double *frequency, const double *amplitude, int count );

public Q_SLOTS:
    void setDamp( double damping );

private:
    void showPeak( double freq, double amplitude );
    void show3dB( double freq );

    QwtPlotCurve *d_curve1;
	QwtPlotMarker *d_marker1;
    QwtPlotMarker *d_marker2;
};

#endif // PLOTTER_H
