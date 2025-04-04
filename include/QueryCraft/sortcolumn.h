#pragma once

#include "conditiongroup.h"
#include <cstdint>

namespace QueryCraft {

struct SortColumn
{
    /**
     * @brief Типы сортировки в SQL-запросах.
     */
    enum class Type : uint8_t
    {
        /// Сортировка в порядке возрастания (ASC).
        ASC,

        /// Сортировка в порядке убывания (DESC).
        DESC
    };

    ColumnInfo column {};
    Type sortType = Type::ASC;
};

inline SortColumn randomSort();

} // namespace QueryCraft