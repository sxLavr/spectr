#ifndef RBWGROUP_H
#define RBWGROUP_H


#include "SdrRfcProtocol.h"


class SdrRfcCore;

class RbwGroup
{
public:
    RbwGroup();

    bool is_changed();
    bool is_vbw_changed();

    quint32 rbw() const { return _rbw; }
    RBWPacket::Window window() const { return _win; }
    RBWPacket::Algorithm algorithm() const { return _algo; }
    RBWPacket::SweepMode sweep_mode() const { return _sweep_mode; }
    quint32 vbw() const { return _vbw; }

private:
    friend class SdrRfcCore;

    void set_rbw( quint32 rbw );
    void set_window( RBWPacket::Window win );
    void set_algorithm( RBWPacket::Algorithm algo );
    void set_sweet_mode( RBWPacket::SweepMode sweep_mode );
    void set_vbw( quint32 vbw );

    bool                    _changed;
    bool                    _vbw_changed;
    quint32                 _rbw;
    RBWPacket::Window       _win;
    RBWPacket::Algorithm    _algo;
    RBWPacket::SweepMode    _sweep_mode;
    quint32                 _vbw;
};

#endif // RBWGROUP_H
