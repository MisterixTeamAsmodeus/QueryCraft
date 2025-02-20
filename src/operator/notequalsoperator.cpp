#include "QueryCraft/operator/notequalsoperator.h"

namespace QueryCraft {
namespace Operator {

std::string NotEqualsOperator::sql() const
{
    return "<>";
}

bool NotEqualsOperator::needBracket() const
{
    return false;
}

} // namespace Operator
} // namespace QueryCraft