#include "QueryCraft/operator/lessorequalsoperator.h"

namespace QueryCraft {
namespace Operator {

std::string LessOrEqualsOperator::sql() const
{
    return "<=";
}

bool LessOrEqualsOperator::needBracket() const
{
    return false;
}

} // namespace Operator
} // namespace QueryCraft