#pragma once

#include "ioperator.h"

namespace QueryCraft {
namespace Operator {

/// Класс, представляющий оператор "IS NOT"
class IsNotOperator final : public IOperator
{
public:
    ~IsNotOperator() override = default;

    std::string sql() const override;

    bool needBracket() const override;
};

} // namespace Operator
} // namespace QueryCraft
