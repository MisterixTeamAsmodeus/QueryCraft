#include "QueryCraft/operator/equalsoperator.h"

namespace QueryCraft {
namespace Operator {

std::string EqualsOperator::sql() const
{
    return "=";
}

bool EqualsOperator::needBracket() const
{
    return false;
}

} // namespace Operator
} // namespace QueryCraft