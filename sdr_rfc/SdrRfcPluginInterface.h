#ifndef SDR_RFC_H
#define SDR_RFC_H


#include "sdr_rfc_global.h"
#include "PluginInterface.h"


class SDR_RFCSHARED_EXPORT SdrRfcPluginInterface
        : public QObject
        , public PluginDeviceInterface
{
    Q_OBJECT
	Q_PLUGIN_METADATA( IID "ua.altron.Spectr.SdrRfcPluginInterface" FILE "SdrRfcPlugin.json" )
    Q_INTERFACES( PluginInterface )

public:
    ~SdrRfcPluginInterface();

    // PluginDeviceInterface
    virtual QString get_name() const;
    virtual QDockWidget* create_dock_widget( QWidget* parent = 0 );
    virtual PluginCore* get_core();
    virtual QDialog* create_connection_dialog( QWidget* parent );
};

#endif // SDR_RFC_H
