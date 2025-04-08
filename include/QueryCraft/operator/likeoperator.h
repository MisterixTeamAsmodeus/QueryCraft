#pragma once

#include "ioperator.h"

namespace QueryCraft {
namespace Operator {

class LikeOperator final : public IOperator
{
public:
    std::string sql() const override;
    bool needBracket() const override;
};

} // namespace Operator
} // namespace QueryCraft