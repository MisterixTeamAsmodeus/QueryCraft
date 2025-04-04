#pragma once

#include "ioperator.h"

namespace QueryCraft {
namespace Operator {

/// Класс, представляющий оператор "IN"
class InOperator final : public IOperator
{
public:
    ~InOperator() override = default;

    std::string sql() const override;

    bool needBracket() const override;
};

} // namespace Operator
} // namespace QueryCraft