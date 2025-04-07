#include "QueryCraft/sqltable.h"

namespace {
void insert_with_escaping_character(std::stringstream& sqlStream, const std::string& value)
{
    if(value != QueryCraft::ColumnInfo::nullValue()) {
        sqlStream << "'";
    }
    // TODO добавить экранирование символов
    sqlStream << value;

    if(value != QueryCraft::ColumnInfo::nullValue()) {
        sqlStream << "'";
    }
}
} // namespace

namespace QueryCraft {

SqlTable::SqlTable(std::string table_name, std::string scheme,
    const std::initializer_list<ColumnInfo>& columns)
    : Table(std::move(table_name),
          std::move(scheme), columns)
{
}

SqlTable::SqlTable(const Table& other)
    : Table(other)
{
}

SqlTable::SqlTable(Table&& other)
    : Table(other)
{
}

SqlTable& SqlTable::addRow(const Row& row)
{
    if(row.empty())
        throw std::logic_error("Ошибка. Попытка добавить пустую строку");

    if(!rows.empty() && rows.begin()->size() != row.size())
        throw std::logic_error("Ошибка. Не совпадает размер строки с уже добавленными в таблицу");

    rows.push_back(row);
    return *this;
}

std::string SqlTable::insertRowArgsSql(const std::initializer_list<ColumnInfo>& columns)
{
    return insertRowSql(std::vector<ColumnInfo>(columns));
}

std::string SqlTable::insertRowSql(const std::vector<ColumnInfo>& columns)
{
    const auto insertColumns = columns.empty() ? _columns : columns;

    if(insertColumns.empty())
        throw std::invalid_argument("Ошибка. Отсутствует информация о колонках");

    if(rows.empty())
        throw std::invalid_argument("Ошибка. Отсутвуют строки для всатвки");

    if(rows.front().size() != insertColumns.size())
        throw std::invalid_argument("Ошибка. Не совпадает колличество колонок с размером данных");

    std::stringstream sqlStream;
    sqlStream << "INSERT INTO " << tableName() << " (";

    for(const auto& column : insertColumns) {
        sqlStream << column.name() << ", ";
    }

    sqlStream.seekp(-2, std::stringstream::cur);

    sqlStream << ") VALUES";

    for(const auto& row : rows) {
        sqlStream << " (";

        for(const auto& value : row) {
            insert_with_escaping_character(sqlStream, value);
            sqlStream << ", ";
        }

        sqlStream.seekp(-2, std::stringstream::cur);

        sqlStream << "),";
    }

    sqlStream.seekp(-1, std::stringstream::cur);

    sqlStream << ";";

    rows.clear();

    return sqlStream.str();
}

std::string SqlTable::updateRowArgsSql(const ConditionGroup& condition, const std::initializer_list<ColumnInfo>& columns)
{
    return updateRowSql(condition, std::vector<ColumnInfo>(columns));
}

std::string SqlTable::updateRowSql(const ConditionGroup& condition, const std::vector<ColumnInfo>& columns)
{
    const auto updateColumns = columns.empty() ? _columns : columns;

    if(updateColumns.empty())
        throw std::invalid_argument("Ошибка. Отсутствует информация о колонках");

    if(rows.empty())
        throw std::invalid_argument("Ошибка. Отсутвуют строки для всатвки");

    if(rows.front().size() != updateColumns.size())
        throw std::invalid_argument("Ошибка. Не совпадает колличество колонок с размером данных");

    if(rows.size() != 1)
        throw std::invalid_argument("Ошибка. В рамках запроса update можно обновить использовать только 1 строку");

    std::stringstream sqlStream;

    sqlStream << "UPDATE " << tableName() << " SET ";

    const auto& row = rows.front();
    for(int i = 0; i < updateColumns.size(); i++) {
        sqlStream << updateColumns[i].name() << " = ";

        insert_with_escaping_character(sqlStream, row[i]);

        sqlStream << ", ";
    }

    sqlStream.seekp(-2, std::stringstream::cur);

    if(condition.isValid())
        sqlStream << " WHERE " << condition.unwrap();

    sqlStream << ";";

    rows.clear();

    return sqlStream.str();
}

std::string SqlTable::removeRowSql(const ConditionGroup& condition) const
{
    std::stringstream sqlStream;

    sqlStream << "DELETE FROM " << tableName();

    if(condition.isValid())
        sqlStream << " WHERE " << condition.unwrap();

    sqlStream << ";";

    return sqlStream.str();
}

std::string SqlTable::selectRowsArgsSql(
    const std::initializer_list<JoinColumn>& joinColumns,
    const ConditionGroup& condition,
    const std::initializer_list<SortColumn>& sortColumns,
    const size_t limit,
    const size_t offset,
    const std::initializer_list<ColumnInfo>& columns) const
{
    return selectRowsSql(
        std::vector<JoinColumn>(joinColumns),
        condition,
        std::vector<SortColumn>(sortColumns),
        limit,
        offset,
        std::vector<ColumnInfo>(columns));
}

std::string SqlTable::selectRowsSql(
    const std::vector<JoinColumn>& joinColumns,
    const ConditionGroup& condition,
    const std::vector<SortColumn>& sortColumns,
    const size_t limit,
    const size_t offset,
    const std::vector<ColumnInfo>& columns) const
{
    // TODO Добавить реализацию group by, having

    const auto selectColumns = columns.empty() ? _columns : columns;

    std::stringstream sqlStream;

    sqlStream << "SELECT ";

    if(!selectColumns.empty()) {
        for(const ColumnInfo& column : selectColumns) {
            sqlStream << column.fullName() << " AS " << column.alias() << ", ";
        }
        sqlStream.seekp(-2, std::stringstream::cur);
    } else {
        sqlStream << "*";
    }

    sqlStream << " FROM " << tableName();

    for(const auto& joinColumn : joinColumns) {
        sqlStream << joinColumn << ", ";
    }
    if(!joinColumns.empty())
        sqlStream.seekp(-2, std::stringstream::cur);

    if(condition.isValid())
        sqlStream << " WHERE " << condition.unwrap(CondionViewType::FULL_NAME);

    if(!sortColumns.empty()) {
        sqlStream << " ORDER BY ";

        for(const auto& sortColumn : sortColumns) {
            sqlStream << sortColumn.column.alias();

            switch(sortColumn.sortType) {
                case SortColumn::Type::ASC:
                    sqlStream << " ASC, ";
                    break;
                case SortColumn::Type::DESC:
                    sqlStream << " DESC, ";
                    break;
            }
        }

        sqlStream.seekp(-2, std::stringstream::cur);
    }

    if(limit != 0)
        sqlStream << " LIMIT " << limit;

    if(offset != 0)
        sqlStream << " OFFSET " << offset;

    sqlStream << ";";

    return sqlStream.str();
}

} // namespace QueryCraft