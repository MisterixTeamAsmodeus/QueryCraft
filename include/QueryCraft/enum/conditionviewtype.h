#pragma once

#include <cstdint>

namespace query_craft {

/// @brief Перечисление для определения вида отображения названия колонки.
enum class condion_view_type : uint8_t
{
    /// Представление по имени.
    NAME,
    /// Представление по псевдониму.
    ALIAS,
    /// Полное представление.
    FULL_NAME
};

} // namespace query_craft