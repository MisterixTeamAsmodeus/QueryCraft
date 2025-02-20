#include "QueryCraft/operator/moreoperator.h"

namespace QueryCraft {
namespace Operator {

std::string MoreOperator::sql() const
{
    return ">";
}

bool MoreOperator::needBracket() const
{
    return false;
}

} // namespace Operator
} // namespace QueryCraft