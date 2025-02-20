#pragma once

#include "ioperator.h"

namespace QueryCraft {
namespace Operator {

/// Класс, представляющий оператор "="
class QUERYCRAFT_EXPORT EqualsOperator final : public IOperator
{
public:
    ~EqualsOperator() override = default;

    std::string sql() const override;

    bool needBracket() const override;
};

} // namespace Operator
} // namespace QueryCraft