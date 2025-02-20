#include "QueryCraft/table.h"

namespace QueryCraft {

Table::Table(std::string tableName, std::string scheme, const std::initializer_list<ColumnInfo>& columns)
    : Table(std::move(tableName), std::move(scheme), columns.begin(), columns.end())
{
}

Table& Table::addColumn(ColumnInfo& column)
{
    std::stringstream aliasStream;
    if(!_scheme.empty())
        aliasStream << _scheme << "_";

    aliasStream << _tableName << "_" << column.name();
    column.setAlias(aliasStream.str());

    std::stringstream fullNameStream;
    fullNameStream << tableName() << "."
                   << "\"" << column.name() << "\"";
    column.setFullName(fullNameStream.str());

    if(_columnsMap.find(column.name()) != _columnsMap.end())
        throw std::logic_error("Ошибка. Дублируется название колонки");

    _columns.push_back(column);
    _columnsMap.emplace(column.name(), column);

    return *this;
}

std::string Table::tableName() const
{
    std::stringstream tableNameStream;

    if(!_scheme.empty())
        tableNameStream << "\"" << _scheme << "\""
                        << ".";

    tableNameStream << "\"" << _tableName << "\"";

    return tableNameStream.str();
}

ColumnInfo Table::column(const std::string& name) const
{
    if(_columnsMap.find(name) == _columnsMap.end())
        throw std::invalid_argument("Данной колонки нет в таблице");

    return _columnsMap.find(name)->second;
}

std::vector<ColumnInfo> Table::columns() const
{
    return _columns;
}

} // namespace QueryCraft