#include "QueryCraft/operator/likeoperator.h"

namespace QueryCraft {
namespace Operator {

std::string LikeOperator::sql() const
{
    return "LIKE";
}

bool LikeOperator::needBracket() const
{
    return false;
}

} // namespace Operator
} // namespace QueryCraft