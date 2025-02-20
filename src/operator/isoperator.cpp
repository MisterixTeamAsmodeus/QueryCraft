#include "QueryCraft/operator/isoperator.h"

namespace QueryCraft {
namespace Operator {

std::string IsOperator::sql() const
{
    return "IS";
}

bool IsOperator::needBracket() const
{
    return false;
}

} // namespace Operator
} // namespace QueryCraft