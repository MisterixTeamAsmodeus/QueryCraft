#pragma once

#include <cstdint>

namespace QueryCraft {

/**
 * @brief Перечисление логических операторов.
 */
enum class LogicalOperator : uint8_t
{
    /**
     * Логический оператор "И".
     */
    AND,

    /**
     * Логический оператор "ИЛИ".
     */
    OR
};

} // namespace QueryCraft