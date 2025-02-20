#pragma once

#include "table.h"

#include <ostream>
#include <cstdint>

namespace QueryCraft {

struct QUERYCRAFT_EXPORT JoinColumn
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

inline std::ostream& operator<<(std::ostream& os, const JoinColumn& obj)
{
    switch(obj.joinType) {
        case JoinColumn::Type::INNER: {
            os << " INNER ";
            break;
        }
        case JoinColumn::Type::OUTER: {
            os << " OUTER ";
            break;
        }
        case JoinColumn::Type::LEFT: {
            os << " LEFT ";
            break;
        }
        case JoinColumn::Type::RIGHT: {
            os << " RIGHT ";
            break;
        }
        case JoinColumn::Type::CROSS: {
            os << " CROSS ";
            break;
        }
    }

    os << "JOIN " << obj.joinedTable.tableName() << " ON " << obj.condition.unwrap(CondionViewType::FULL_NAME);

    return os;
}

} // namespace QueryCraft