#include "QueryCraft/operator/notinoperator.h"

namespace QueryCraft {
namespace Operator {

std::string NotInOperator::sql() const
{
    return "NOT IN";
}

bool NotInOperator::needBracket() const
{
    return true;
}

} // namespace Operator
} // namespace QueryCraft