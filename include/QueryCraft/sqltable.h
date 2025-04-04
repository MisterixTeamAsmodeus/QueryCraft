#pragma once

#include "helper/tuplehelper.h"
#include "joincolumn.h"
#include "sortcolumn.h"
#include "table.h"

namespace QueryCraft {

/// Класс, представляющий таблицу SQL.
/// Расширяет класс Table и добавляет функциональность для генерации sql запросов.
class SqlTable : public Table
{
public:
    /// Тип, представляющий строку таблицы.
    /// Каждая строка представляется в виде вектора значений столбцов.
    using Row = std::vector<std::string>;

public:
    SqlTable() = default;

    /**
     * Конструктор с указанием имени таблицы и схемы.
     *
     * @param table_name Имя таблицы.
     * @param scheme     Имя схемы. По умолчанию пустая строка.
     * @param columns    Список столбцов таблицы. По умолчанию пустой список.
     */

    explicit SqlTable(std::string table_name, std::string scheme = "", const std::initializer_list<ColumnInfo>& columns = {});

    /**
     * Конструктор с указанием имени таблицы, схемы и переменного числа столбцов.
     *
     * @param tableName Имя таблицы.
     * @param scheme    Имя схемы.
     * @param columns   Столбцы таблицы.
     */
    template<typename... Args>
    explicit SqlTable(std::string tableName, std::string scheme, Args&&... columns)
        : Table(std::move(tableName),
              std::move(scheme),
              std::forward<Args>(columns)...)
    {
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
    explicit SqlTable(std::string tableName, std::string scheme, StartColumnIt&& startIt, EndColumnIt&& endIt)
        : SqlTable(std::move(tableName),
              std::move(scheme),
              std::forward<StartColumnIt>(startIt),
              std::forward<EndColumnIt>(endIt))
    {
    }

    explicit SqlTable(const Table& other);

    explicit SqlTable(Table&& other);

    SqlTable(const SqlTable& other) = default;

    SqlTable(SqlTable&& other) noexcept = default;

    SqlTable& operator=(const SqlTable& other) = default;

    SqlTable& operator=(SqlTable&& other) noexcept = default;

    /**
     * Добавление строки в таблицу.
     *
     * @param row Строка для добавления.
     * @return Ссылка на текущую таблицу.
     */
    SqlTable& addRow(const Row& row);

    /**
     * Добавление строки в таблицу с использованием переменного числа аргументов.
     *
     * @param args Значения для добавления в качестве столбцов.
     * @return Ссылка на текущую таблицу.
     * @note Использует метод convertToString (по умолчанию использует оператор << для преобразования в строку),
     * для изменения поведения нужно переопределить метод для своего типа
     */
    template<typename... Args>
    SqlTable& addRow(Args&&... args)
    {
        auto values = std::make_tuple<Args...>(std::forward<Args>(args)...);

        Row row;

        Helper::for_each(values, [&row](auto&& value) {
            row.emplace_back(Helper::convertToString(value));
        });

        if(!rows.empty() && rows.begin()->size() != row.size())
            throw std::logic_error("Ошибка. Не совпадает размер строки с уже добавленными в таблицу");

        rows.push_back(row);
        return *this;
    }

    /**
     * Генерация SQL-запроса для вставки строки в таблицу.
     *
     * @param columns Столбцы для вставки. По умолчанию все столбцы.
     * @return SQL-запрос для вставки.
     * @note Очищает добавленные строки
     * @note Перегрузка для передачи параметров через initializer_list, так как бывают ситуации когда он не может не явно кастануть к vector
     */
    std::string insertRowArgsSql(const std::initializer_list<ColumnInfo>& columns);

    /**
     * Генерация SQL-запроса для вставки строки в таблицу.
     *
     * @param columns Столбцы для вставки. По умолчанию все столбцы.
     * @return SQL-запрос для вставки.
     * @note Очищает добавленные строки
     */
    std::string insertRowSql(const std::vector<ColumnInfo>& columns = {});

    /**
     * Генерация SQL-запроса для обновления строки в таблице.
     *
     * @param condition Условие для выбора строки.
     * @param columns   Столбцы для обновления. По умолчанию все столбцы.
     * @return SQL-запрос для обновления.
     * @note Очищает добавленные строки
     * @note Перегрузка для передачи параметров через initializer_list, так как бывают ситуации когда он не может не явно кастануть к vector
     */
    std::string updateRowArgsSql(const ConditionGroup& condition = {}, const std::initializer_list<ColumnInfo>& columns = {});

    /**
     * Генерация SQL-запроса для обновления строки в таблице.
     *
     * @param condition Условие для выбора строки.
     * @param columns   Столбцы для обновления. По умолчанию все столбцы.
     * @return SQL-запрос для обновления.
     * @note Очищает добавленные строки
     */
    std::string updateRowSql(const ConditionGroup& condition = {}, const std::vector<ColumnInfo>& columns = {});

    /**
     * Генерация SQL-запроса для удаления строки из таблицы.
     *
     * @param condition Условие для выбора строки.
     * @return SQL-запрос для удаления.
     */
    std::string removeRowSql(const ConditionGroup& condition = {}) const;

    /**
     * Генерация SQL-запроса для выборки строк из таблицы.
     *
     * @param joinColumns   Информация о join соединениях
     * @param condition     Условие для выбора строк.
     * @param sortColumns   Информация о колонках необходимых для сортировок
     * @param limit         Лимит выборки.
     * @param offset        Смещение выборки.
     * @param columns       Столбцы для выборки. По умолчанию все столбцы.
     * @return SQL-запрос для выборки.
     * @note Перегрузка для передачи параметров через initializer_list, так как бывают ситуации когда он не может не явно кастануть к vector
     */
    std::string selectRowsArgsSql(
        const std::initializer_list<JoinColumn>& joinColumns = {},
        const ConditionGroup& condition = {},
        const std::initializer_list<SortColumn>& sortColumns = {},
        size_t limit = 0,
        size_t offset = 0,
        const std::initializer_list<ColumnInfo>& columns = {}) const;

    /**
     * Генерация SQL-запроса для выборки строк из таблицы.
     *
     * @param joinColumns   Информация о join соединениях
     * @param condition     Условие для выбора строк.
     * @param sortColumns   Информация о колонках необходимых для сортировок
     * @param limit         Лимит выборки.
     * @param offset        Смещение выборки.
     * @param columns       Столбцы для выборки. По умолчанию все столбцы.
     * @return SQL-запрос для выборки.
     */
    std::string selectRowsSql(
        const std::vector<JoinColumn>& joinColumns = {},
        const ConditionGroup& condition = {},
        const std::vector<SortColumn>& sortColumns = {},
        size_t limit = 0,
        size_t offset = 0,
        const std::vector<ColumnInfo>& columns = {}) const;

private:
    /// Вектор, содержащий строки таблицы.
    /// Каждая строка представляется в виде вектора значений столбцов.
    std::vector<Row> rows;
};

} // namespace QueryCraft