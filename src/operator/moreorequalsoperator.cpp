#include "QueryCraft/operator/moreorequalsoperator.h"

namespace QueryCraft {
namespace Operator {

std::string MoreOrEqualsOperator::sql() const
{
    return ">=";
}

bool MoreOrEqualsOperator::needBracket() const
{
    return false;
}

} // namespace Operator
} // namespace QueryCraft