#ifndef PLUGININTERFACE_H
#define PLUGININTERFACE_H


#include "PluginCore.h"

#include <QDockWidget>
#include <QtPlugin>


class PluginInterface
{
public:
    enum Type
    {
        Type_Device
    };

    virtual ~PluginInterface() { }

    virtual Type get_type() const = 0;
    virtual QString get_name() const = 0;
    virtual QDockWidget* create_dock_widget( QWidget* parent = 0 ) = 0;
};


class PluginDeviceInterface
        : public PluginInterface
{
public:
    virtual ~PluginDeviceInterface() {}

    // PluginInterface
    virtual Type get_type() const { return Type_Device; }

    virtual PluginCore* get_core() = 0;
    virtual QDialog* create_connection_dialog( QWidget* parent ) = 0;
};


#define PluginInterface_iid "ua.altron.Spectr.PluginInterface/1.0"
Q_DECLARE_INTERFACE( PluginInterface, PluginInterface_iid )


#endif // PLUGININTERFACE_H
