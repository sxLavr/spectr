
#include "Declaration.h"


DECLARE_TABLE_BEGIN( Target )
    DECLARE_ITEM( ip_v4 )
    DECLARE_ITEM( port )
DECLARE_TABLE_END()


DECLARE_TABLE_BEGIN( Local )
    DECLARE_ITEM( ip_v4 )
    DECLARE_ITEM( port )
DECLARE_TABLE_END()


DECLARE_TABLE_BEGIN( Frequency )
    DECLARE_ITEM( start )
    DECLARE_ITEM( stop )
DECLARE_TABLE_END()


DECLARE_TABLE_BEGIN( Core )
    DECLARE_ITEM( core )
    DECLARE_ITEM( connector )
    DECLARE_ITEM( acceptor )
    DECLARE_VALUE( cfg_file, "zxspectrum.json" )
    DECLARE_ITEM( local_ip_v4 )
DECLARE_TABLE_END()



#ifndef CONSTANTS_H
#define CONSTANTS_H

#endif // CONSTANTS_H
