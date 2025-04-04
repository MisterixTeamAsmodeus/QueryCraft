#pragma once

#include "conditiongroup.h"

#include <unordered_map>

namespace QueryCraft {
/**
 * Класс Table представляет таблицу в базе данных и предоставляет интерфейс для работы с ее столбцами.
 */
class Table
{
public:
    Table() = default;

    /**
     * Конструктор с указанием имени таблицы и схемы.
     *
     * @param tableName Имя таблицы.
     * @param scheme    Имя схемы. По умолчанию пустая строка.
     * @param columns   Список столбцов таблицы. По умолчанию пустой список.
     */
    explicit Table(std::string tableName, std::string scheme = "", const std::initializer_list<ColumnInfo>& columns = {});

    /**
     * Конструктор с указанием имени таблицы, схемы и переменного числа столбцов.
     *
     * @param tableName Имя таблицы.
     * @param scheme    Имя схемы.
     * @param columns   Столбцы таблицы.
     */
    template<typename... Args>
    explicit Table(std::string tableName, std::string scheme, Args&&... columns)
        : _scheme(std::move(scheme))
        , _tableName(std::move(tableName))
    {
        auto columnList = { std::forward<Args>(columns)... };
        for(const ColumnInfo& column : columnList) {
            ColumnInfo tempColumn(column);
            addColumn(tempColumn);
        }
    }

    /**
     * Конструктор с указанием имени таблицы, схемы и диапазона итераторов столбцов.
     *
     * @param tableName Имя таблицы.
     * @param scheme    Имя схемы.
     * @param startIt   Итератор начала диапазона столбцов.
     * @param endIt     Итератор конца диапазона столбцов.
     */
    template<class StartColumnIt, class EndColumnIt>
    explicit Table(std::string tableName, std::string scheme, StartColumnIt&& startIt, EndColumnIt&& endIt)
        : _scheme(std::move(scheme))
        , _tableName(std::move(tableName))
    {
        std::for_each(startIt, endIt, [this](const ColumnInfo& column) {
            ColumnInfo tempColumn(column);
            addColumn(tempColumn);
        });
    }

    Table(const Table& other) = default;

    Table(Table&& other) noexcept = default;

    Table& operator=(const Table& other) = default;

    Table& operator=(Table&& other) noexcept = default;

    /**
     * Добавление столбца в таблицу.
     *
     * @param column Столбец для добавления.
     * @return Ссылка на текущую таблицу.
     */
    Table& addColumn(ColumnInfo& column);

    /**
     * Получение полного имени таблицы.
     *
     * @return Имя таблицы с названием схемы если она есть.
     */
    std::string tableName() const;

    /**
     * Получение информации о столбце по его имени.
     *
     * @param name Имя столбца.
     * @return Информация о столбце.
     */
    ColumnInfo column(const std::string& name) const;

    /**
     * Получение списка столбцов таблицы.
     *
     * @return Список столбцов.
     */
    std::vector<ColumnInfo> columns() const;

    int indexOf(const std::string& name) const;

    int indexOf(const ColumnInfo& column) const;

protected:
    /// Название схемы таблицы.
    std::string _scheme {};
    /// Название таблицы.
    std::string _tableName {};
    /// Список столбцов таблицы. Нужен для сохранения порядка при запросах.
    std::vector<ColumnInfo> _columns;
    /// Таблица столбцов. Нужна для быстрого поиска столбца по имени за O(1).
    std::unordered_map<std::string, ColumnInfo> _columnsMap {};
};

} // namespace QueryCraft