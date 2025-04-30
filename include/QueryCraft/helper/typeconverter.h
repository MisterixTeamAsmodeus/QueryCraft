#pragma once

#include <sstream>
#include <string>

namespace query_craft {
namespace helper {

/**
 * @brief Преобразует значение в строку.
 * @tparam T Тип значения, которое необходимо преобразовать.
 * @param value Значение, которое необходимо преобразовать в строку.
 * @return Строковое представление значения.
 */
template<typename T>
std::string convert_to_string(T& value)
{
    std::stringstream stream;
    stream << value;

    return stream.str();
}

template<>
inline std::string convert_to_string(bool& value)
{
    return value ? "true" : "false";
}

} // namespace helper
} // namespace query_craft