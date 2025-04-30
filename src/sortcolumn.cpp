#include "QueryCraft/sortcolumn.h"

query_craft::sort_column query_craft:: random_sort()
{
    sort_column column;
    column.column.set_alias("RANDOM ()");
    return column;
}