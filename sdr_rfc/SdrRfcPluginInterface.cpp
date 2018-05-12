
#include "SdrRfcPluginInterface.h"
#include "SdrRfcDockWidget.h"
#include "SdrRfcCore.h"
#include "TcpConnectionDialog.h"


SdrRfcPluginInterface::~SdrRfcPluginInterface()
{

}

/*virtual*/ QString SdrRfcPluginInterface::get_name() const
{
    return "SDR RFC";
}

/*virtual*/ QDockWidget* SdrRfcPluginInterface::create_dock_widget( QWidget* parent )
{
    return new SdrRfcDockWidget( parent );
}

/*virtual*/ PluginCore* SdrRfcPluginInterface::get_core()
{
    return &GET_CORE();
}

QDialog* SdrRfcPluginInterface::create_connection_dialog( QWidget* parent )
{
    return new TcpConnectionDialog( parent );
}

//Q_EXPORT_PLUGIN2( PluginInterface, SdrRfcPluginInterface )
