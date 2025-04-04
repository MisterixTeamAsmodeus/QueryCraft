#pragma once

#include <string>

namespace QueryCraft {
namespace Operator {

/// Интерфейс описывающий операторы используемые в логических операторах
class IOperator
{
public:
    virtual ~IOperator() = default;

    /**
     * Возвращает SQL-представление оператора равенства.
     *
     * @return Строка с SQL-представлением оператора равенства.
     */
    virtual std::string sql() const = 0;

    /**
     * Определяет, требуются ли скобки для оператора равенства.
     *
     * @return true, если скобки требуются, иначе false.
     */
    virtual bool needBracket() const = 0;
};

} // namespace Operator
} // namespace QueryCraft