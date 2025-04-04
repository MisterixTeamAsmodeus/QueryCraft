#pragma once

#include "ioperator.h"

namespace QueryCraft {
namespace Operator {

/// Класс, представляющий оператор "NOT IN"
class NotInOperator final : public IOperator
{
public:
    ~NotInOperator() override = default;

    std::string sql() const override;

    bool needBracket() const override;
};

} // namespace Operator
} // namespace QueryCraft