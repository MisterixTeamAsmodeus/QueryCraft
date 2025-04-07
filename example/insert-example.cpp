#include <QueryCraft/querycraft.h>

#include <iostream>

int main()
{
    using namespace QueryCraft;

    SqlTable table("table_name", "schema_name",
        ColumnInfo("c1"),
        ColumnInfo("c2"),
        ColumnInfo("c3"));

    table.addRowArgs(5, "124", true);
    table.addRowArgs(5, "874", true);
    std::cout << table.insertRowSql() << "\n";

    table.addRowArgs(5, true);
    std::cout << table.insertRowSql({ table.column("c1"), table.column("c3") }) << "\n";
}
