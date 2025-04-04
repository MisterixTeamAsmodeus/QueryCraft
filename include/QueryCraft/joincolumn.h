#pragma once

#include "table.h"

#include <cstdint>

namespace QueryCraft {

struct JoinColumn
{

    /**
     * @brief Типы соединений в SQL-запросах.
     */
    enum class Type : uint8_t
    {
        /// Внутреннее соединение (INNER JOIN). Выбираются только те записи, которые имеют совпадения в обеих таблицах.
        INNER,

        /// Внешнее соединение (OUTER JOIN). Выбираются все записи из обеих таблиц, даже если в одной из них нет совпадений.
        OUTER,

        /// Левое внешнее соединение (LEFT JOIN). Выбираются все записи из левой таблицы и совпадающие записи из правой таблицы.
        LEFT,

        /// Правое внешнее соединение (RIGHT JOIN). Выбираются все записи из правой таблицы и совпадающие записи из левой таблицы.
        RIGHT,

        /// Перекрестное соединение (CROSS JOIN). Выбираются все возможные комбинации записей из двух таблиц.
        CROSS
    };

    Type joinType = Type::INNER;
    Table joinedTable {};
    ConditionGroup condition {};
};

std::ostream& operator<<(std::ostream& os, const JoinColumn& obj);

} // namespace QueryCraft