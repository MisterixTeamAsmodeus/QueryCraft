#pragma once

#include <sstream>
#include <string>

namespace QueryCraft {
namespace Helper {

/**
 * @brief Преобразует значение в строку.
 * @tparam T Тип значения, которое необходимо преобразовать.
 * @param value Значение, которое необходимо преобразовать в строку.
 * @return Строковое представление значения.
 */
template<typename T>
std::string convertToString(T&& value)
{
    std::stringstream stream;
    stream << value;

    return stream.str();
}

} // namespace Helper
} // namespace QueryCraft