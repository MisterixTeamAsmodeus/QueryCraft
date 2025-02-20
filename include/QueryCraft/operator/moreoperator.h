#pragma once

#include "ioperator.h"

namespace QueryCraft {
namespace Operator {

/// Класс, представляющий оператор ">"
class QUERYCRAFT_EXPORT MoreOperator final : public IOperator
{
public:
    ~MoreOperator() override = default;

    std::string sql() const override;

    bool needBracket() const override;
};

} // namespace Operator
} // namespace QueryCraft