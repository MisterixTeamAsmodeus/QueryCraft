#include "QueryCraft/operator/lessoperator.h"

namespace QueryCraft {
namespace Operator {

std::string LessOperator::sql() const
{
    return "<";
}

bool LessOperator::needBracket() const
{
    return false;
}

} // namespace Operator
} // namespace QueryCraft