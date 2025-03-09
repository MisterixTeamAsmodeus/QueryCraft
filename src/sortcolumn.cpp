#include "QueryCraft/sortcolumn.h"

QueryCraft::SortColumn QueryCraft::randomSort()
{
    SortColumn column;
    column.column.setAlias("RANDOM ()");
    return column;
}