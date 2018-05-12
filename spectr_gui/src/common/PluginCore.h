#ifndef PLUGINCORE_H
#define PLUGINCORE_H


#include <QObject>


class PluginCore
		: public QObject
{
	Q_OBJECT

public:
    enum ConnectionState
    {
        ConnectionState_Disconnected,
        ConnectionState_Partially,
        ConnectionState_Connected,
    };

    virtual ~PluginCore() { }

    virtual void connect() = 0;
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual ConnectionState connection_state() const = 0;

signals:
    void connection_state_changed();
	void error_message( QString const& );
    void fft_data_ready( double const* frequency, double const* amplitude, int count );
};

#endif // PLUGINCORE_H
