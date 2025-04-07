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
    std::cout << table.updateRowSql(table.column("c1") > 5 && table.column("c2") < 7) << "\n";

    table.addRowArgs(5, "874", true);
    std::cout << table.updateRowSql() << "\n";

    table.addRowArgs(5, true);
    std::cout << table.updateRowArgsSql({}, { table.column("c1"), table.column("c3") }) << "\n";
}
