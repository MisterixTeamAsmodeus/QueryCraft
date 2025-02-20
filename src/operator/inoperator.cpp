#include "QueryCraft/operator/inoperator.h"

namespace QueryCraft {
namespace Operator {

std::string InOperator::sql() const
{
    return "IN";
}

bool InOperator::needBracket() const
{
    return true;
}

} // namespace Operator
} // namespace QueryCraft