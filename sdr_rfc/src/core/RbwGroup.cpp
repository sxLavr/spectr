#include "RbwGroup.h"

RbwGroup::RbwGroup()
    : _changed( true )
    , _vbw_changed( true )
    , _rbw( 10000 )
    , _win( RBWPacket::Window_Hann )
    , _algo( RBWPacket::Algorithm_RBW )
    , _sweep_mode( RBWPacket::SweepMode_Normal )
    , _vbw( 100 )
{

}

bool RbwGroup::is_changed()
{
    bool result = _changed;
    _changed = false;
    return result;
}

bool RbwGroup::is_vbw_changed()
{
    bool result = _vbw_changed;
    _vbw_changed = false;
    return result;
}

void RbwGroup::set_rbw( quint32 rbw )
{
    if (rbw != _rbw)
    {
        _rbw = rbw;
        _changed = true;
    }
}

void RbwGroup::set_window( RBWPacket::Window win )
{
    if (win != _win)
    {
        _win = win;
        _changed = true;
    }
}

void RbwGroup::set_algorithm( RBWPacket::Algorithm algo )
{
    if (algo != _algo)
    {
        _algo = algo;
        _changed = true;
    }
}

void RbwGroup::set_sweet_mode( RBWPacket::SweepMode sweep_mode )
{
    if (sweep_mode != _sweep_mode)
    {
        _sweep_mode = sweep_mode;
        _changed = true;
    }
}

void RbwGroup::set_vbw( quint32 vbw )
{
    if (vbw != _vbw)
    {
        _vbw = vbw;
        _vbw_changed = true;
    }
}
