#pragma once

#include <cstdint>

namespace QueryCraft {

/// @brief Перечисление для определения вида отображения названия колонки.
enum class CondionViewType : uint8_t
{
    /// Представление по имени.
    NAME,
    /// Представление по псевдониму.
    ALIAS,
    /// Полное представление.
    FULL_NAME
};

} // namespace QueryCraft