#include <QueryCraft/querycraft.h>

#include <iostream>

int main()
{
    using namespace QueryCraft;

    const SqlTable table("table_name", "schema_name",
        ColumnInfo("c1"),
        ColumnInfo("c2"),
        ColumnInfo("c3"),
        ColumnInfo("c4"),
        ColumnInfo("c5"),
        ColumnInfo("c6"),
        ColumnInfo("c7"),
        ColumnInfo("c8"),
        ColumnInfo("c9"),
        ColumnInfo("c10"),
        ColumnInfo("c11"),
        ColumnInfo("c12"));

    const auto list = { "first", "second", "third", "fourth", "fifth" };
    const auto sql = table.selectRowsArgsSql(
        { { JoinColumn::Type::LEFT, static_cast<Table>(table), table.column("c1").isNull() } },
        (table.column("c1") > 5 && (table.column("c2") < 7) || table.column("c3") >= 6 || table.column("c4") <= 6 || table.column("c5") == 6 || table.column("c6") != 6) || table.column("c7").isNull() || table.column("c8").notNull() || table.column("c9").in(5, 4, 7, 8) || table.column("c11").notIn(78, 57, 57, 4, 1) || (table.column("c10").inList(list.begin(), list.end()) && table.column("c12").notInList(list.begin(), list.end())),
        { { table.column("c3"), SortColumn::Type::DESC }, { table.column("c8") } },
        10,
        45);

    std::cout << sql << "\n";

    return 0;
}
