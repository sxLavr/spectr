
#include "Plotter.h"

#include <qwt_math.h>
#include <qwt_scale_engine.h>
#include <qwt_symbol.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_curve.h>
#include <qwt_legend.h>
#include <qwt_text.h>
#include <qwt_plot_canvas.h>
#include <qmath.h>
#include <complex>


namespace
{

    static void logSpace( double *array, int size, double xmin, double xmax )
    {
        if ( ( xmin <= 0.0 ) || ( xmax <= 0.0 ) || ( size <= 0 ) )
            return;

        const int imax = size - 1;

        array[0] = xmin;
        array[imax] = xmax;

        const double lxmin = log( xmin );
        const double lxmax = log( xmax );
        const double lstep = ( lxmax - lxmin ) / double( imax );

        for ( int i = 1; i < imax; i++ )
            array[i] = qExp( lxmin + double( i ) * lstep );
    }

}

Plotter::Plotter( QWidget* parent )
    : QwtPlot( parent )
{
    setAutoReplot( false );

    setTitle( "erbhr" );

    QwtPlotCanvas *canvas = new QwtPlotCanvas();
    canvas->setBorderRadius( 10 );

    setCanvas( canvas );
    setCanvasBackground( QColor( "MidnightBlue" ) );

    // legend
//    QwtLegend *legend = new QwtLegend;
//    insertLegend( legend, QwtPlot::BottomLegend );

    // grid
    QwtPlotGrid *grid = new QwtPlotGrid;
    grid->enableXMin( true );
    grid->setMajorPen( Qt::white, 0, Qt::DotLine );
    grid->setMinorPen( Qt::gray, 0 , Qt::DotLine );
    grid->attach( this );

    setAxisScale( QwtPlot::yLeft, -120, 10 );

    // axes
    setAxisTitle( QwtPlot::xBottom, "Frequency" );
    setAxisTitle( QwtPlot::yLeft, "Amplitude [dB]" );

    setAxisMaxMajor( QwtPlot::xBottom, 6 );
    setAxisMaxMinor( QwtPlot::xBottom, 9 );
	//setAxisScaleEngine( QwtPlot::xBottom, new QwtLogScaleEngine );

    // curves
    d_curve1 = new QwtPlotCurve();
    d_curve1->setRenderHint( QwtPlotItem::RenderAntialiased );
    d_curve1->setPen( Qt::yellow );
//    d_curve1->setLegendAttribute( QwtPlotCurve::LegendShowLine );
    d_curve1->setYAxis( QwtPlot::yLeft );
    d_curve1->attach( this );

	// marker
    d_marker1 = new QwtPlotMarker();
    d_marker1->setValue( 0.0, 0.0 );
    d_marker1->setLineStyle( QwtPlotMarker::VLine );
    d_marker1->setLabelAlignment( Qt::AlignRight | Qt::AlignBottom );
    d_marker1->setLinePen( Qt::green, 0, Qt::DashDotLine );
    d_marker1->attach( this );

    d_marker2 = new QwtPlotMarker();
    d_marker2->setLineStyle( QwtPlotMarker::HLine );
    d_marker2->setLabelAlignment( Qt::AlignRight | Qt::AlignBottom );
    d_marker2->setLinePen( QColor( 200, 150, 0 ), 0, Qt::DashDotLine );
    d_marker2->setSymbol( new QwtSymbol( QwtSymbol::Diamond,
        QColor( Qt::yellow ), QColor( Qt::green ), QSize( 8, 8 ) ) );
    d_marker2->attach( this );

    setDamp( 0.0 );

    setAutoReplot( true );
}

void Plotter::showData( const double *frequency, const double *amplitude, int count )
{
    d_curve1->setSamples( frequency, amplitude, count );
}

void Plotter::showPeak( double freq, double amplitude )
{
    QString label;
    label.sprintf( "Peak: %.3g dB", amplitude );

    QwtText text( label );
    text.setFont( QFont( "Helvetica", 10, QFont::Bold ) );
    text.setColor( QColor( 200, 150, 0 ) );

    d_marker2->setValue( freq, amplitude );
    d_marker2->setLabel( text );
}

void Plotter::show3dB( double freq )
{
    QString label;
    label.sprintf( "-3 dB at f = %.3g", freq );

    QwtText text( label );
    text.setFont( QFont( "Helvetica", 10, QFont::Bold ) );
    text.setColor( Qt::green );

    d_marker1->setValue( freq, 0.0 );
    d_marker1->setLabel( text );
}

//
// re-calculate frequency response
//
void Plotter::setDamp( double damping )
{
    const bool doReplot = autoReplot();
    setAutoReplot( false );

    const int ArraySize = 200;

    double frequency[ArraySize];
    double amplitude[ArraySize];

    // build frequency vector with logarithmic division
    logSpace( frequency, ArraySize, 0.01, 100 );

    int i3 = 1;
    double fmax = 1;
    double amax = -1000.0;

    for ( int i = 0; i < ArraySize; i++ )
    {
        double f = frequency[i];
        const std::complex<double> g =
            std::complex<double>( 1.0 ) / std::complex<double>( 1.0 - f * f, 2.0 * damping * f );

        amplitude[i] = 20.0 * log10( qSqrt( g.real() * g.real() + g.imag() * g.imag() ) );

        if ( ( i3 <= 1 ) && ( amplitude[i] < -3.0 ) )
            i3 = i;
        if ( amplitude[i] > amax )
        {
            amax = amplitude[i];
            fmax = frequency[i];
        }

    }

    double f3 = frequency[i3] - ( frequency[i3] - frequency[i3 - 1] )
        / ( amplitude[i3] - amplitude[i3 -1] ) * ( amplitude[i3] + 3 );

    showPeak( fmax, amax );
    show3dB( f3 );
	showData( frequency, amplitude, ArraySize );

    setAutoReplot( doReplot );

    replot();
}

