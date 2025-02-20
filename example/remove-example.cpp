#include <QueryCraft/querycraft.h>

#include <iostream>

int main()
{
    using namespace QueryCraft;

    const SqlTable table("table_name", "schema_name",
        ColumnInfo("c1"),
        ColumnInfo("c2"),
        ColumnInfo("c3"));

    std::cout << table.removeRowSql(table.column("c1") > 5
        && table.column("c2") < 7)
              << "\n";
    std::cout << table.removeRowSql() << "\n";

    return 0;
}
