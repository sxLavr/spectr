#define DECLARE_TABLE_BEGIN(name)       namespace name {
#define DECLARE_TABLE_END()             }

#ifdef DECLARE_ITEM
#undef DECLARE_ITEM
#endif

#ifdef DECLARE_VALUE
#undef DECLARE_VALUE
#endif

#include <QString>

#ifdef DEFINITION

#define DECLARE_ITEM(item)             QString const item = #item;
#define DECLARE_VALUE(item, value)     QString const item = value;

#else

#define DECLARE_ITEM(item)             extern QString const item;
#define DECLARE_VALUE(item, value)     extern QString const item;

#endif
