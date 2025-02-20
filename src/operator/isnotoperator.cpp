#include "QueryCraft/operator/isnotoperator.h"

namespace QueryCraft {
namespace Operator {

std::string IsNotOperator::sql() const
{
    return "IS NOT";
}

bool IsNotOperator::needBracket() const
{
    return false;
}

} // namespace Operator
} // namespace QueryCraft