#pragma once

#include <cstdint>

namespace query_craft {

/**
 * @brief Перечисление логических операторов.
 */
enum class logical_operator : uint8_t
{
    /**
     * Логический оператор "И".
     */
    and,

    /**
     * Логический оператор "ИЛИ".
     */
    or
};

} // namespace query_craft