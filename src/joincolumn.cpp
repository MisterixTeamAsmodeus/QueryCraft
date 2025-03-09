#include "QueryCraft/joincolumn.h"

std::ostream& QueryCraft::operator<<(std::ostream& os, const JoinColumn& obj)
{
    switch(obj.joinType) {
        case JoinColumn::Type::INNER: {
            os << " INNER ";
            break;
        }
        case JoinColumn::Type::OUTER: {
            os << " OUTER ";
            break;
        }
        case JoinColumn::Type::LEFT: {
            os << " LEFT ";
            break;
        }
        case JoinColumn::Type::RIGHT: {
            os << " RIGHT ";
            break;
        }
        case JoinColumn::Type::CROSS: {
            os << " CROSS ";
            break;
        }
    }

    os << "JOIN " << obj.joinedTable.tableName() << " ON " << obj.condition.unwrap(CondionViewType::FULL_NAME);

    return os;
}