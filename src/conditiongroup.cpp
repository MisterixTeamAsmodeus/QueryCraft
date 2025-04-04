#include "QueryCraft/conditiongroup.h"

#include "QueryCraft/operator/isnotoperator.h"
#include "QueryCraft/operator/isoperator.h"

namespace QueryCraft {

ConditionGroup::Condition::Column::Column(std::string name, const Settings settings)
    : _name(std::move(name))
    , _columnSettings(settings)
{
}

bool ConditionGroup::Condition::Column::operator==(const Column& rhs) const
{
    return _name == rhs._name
        && _fullName == rhs._fullName
        && _alias == rhs._alias
        && _columnSettings == rhs._columnSettings;
}

bool ConditionGroup::Condition::Column::operator!=(const Column& rhs) const
{
    return !(*this == rhs);
}

std::string ConditionGroup::Condition::Column::name() const
{
    return _name;
}

std::string ConditionGroup::Condition::Column::fullName() const
{
    return _fullName;
}

void ConditionGroup::Condition::Column::setFullName(const std::string& fullName)
{
    _fullName = fullName;
}

std::string ConditionGroup::Condition::Column::alias() const
{
    return _alias;
}

void ConditionGroup::Condition::Column::setAlias(const std::string& alias)
{
    _alias = alias;
}

void ConditionGroup::Condition::Column::addSettings(const Settings settings)
{
    _columnSettings = settings & _columnSettings;
}

bool ConditionGroup::Condition::Column::hasSettings(const Settings settings) const
{
    return (_columnSettings & settings) == settings;
}

ConditionGroup::Condition ConditionGroup::Condition::Column::isNull() const
{
    return createCondition(std::move(std::make_shared<Operator::IsOperator>()), { nullValue() });
}

ConditionGroup::Condition ConditionGroup::Condition::Column::notNull() const
{
    return createCondition(std::move(std::make_shared<Operator::IsNotOperator>()), { nullValue() });
}

bool ConditionGroup::Condition::Column::isValid() const
{
    return !_name.empty() || !_fullName.empty() || !_alias.empty() || _columnSettings != Settings::NONE;
}

ConditionGroup::Condition ConditionGroup::Condition::Column::createCondition(std::shared_ptr<Operator::IOperator>&& conditionOperator, std::vector<std::string>&& values) const
{
    Condition condition;

    condition._conditionOperator = std::move(conditionOperator);
    condition._column = *this;
    condition._values = std::move(values);

    return std::move(condition);
}

ConditionGroup ConditionGroup::Condition::operator&&(const Condition& rhd) const
{
    ConditionGroup group;

    std::get<0>(group._node) = LogicalOperator::AND;

    group._left = std::make_shared<ConditionGroup>();
    std::get<1>(group._left->_node) = *this;

    group._right = std::make_shared<ConditionGroup>();
    std::get<1>(group._right->_node) = rhd;

    return group;
}

ConditionGroup ConditionGroup::Condition::operator&&(const ConditionGroup& rhd) const
{
    ConditionGroup group;

    std::get<0>(group._node) = LogicalOperator::AND;

    group._left = std::make_shared<ConditionGroup>();
    std::get<1>(group._left->_node) = *this;

    group._right = std::make_shared<ConditionGroup>(rhd);

    return group;
}

ConditionGroup ConditionGroup::Condition::operator||(const Condition& rhd) const
{
    ConditionGroup group;

    std::get<0>(group._node) = LogicalOperator::OR;

    group._left = std::make_shared<ConditionGroup>();
    std::get<1>(group._left->_node) = *this;

    group._right = std::make_shared<ConditionGroup>();
    std::get<1>(group._right->_node) = rhd;

    return group;
}

ConditionGroup ConditionGroup::Condition::operator||(const ConditionGroup& rhd) const
{
    ConditionGroup group;

    std::get<0>(group._node) = LogicalOperator::OR;

    group._left = std::make_shared<ConditionGroup>();
    std::get<1>(group._left->_node) = *this;

    group._right = std::make_shared<ConditionGroup>(rhd);

    return group;
}

std::string ConditionGroup::Condition::unwrap(const CondionViewType viewType) const
{
    if(_values.empty())
        return "";

    std::stringstream stream;

    switch(viewType) {
        case CondionViewType::NAME: {
            stream << _column.name();
            break;
        }
        case CondionViewType::ALIAS: {
            stream << _column.alias();
            break;
        }
        case CondionViewType::FULL_NAME: {
            stream << _column.fullName();
            break;
        }
    }

    stream << " " << _conditionOperator->sql() << " ";

    if(_conditionOperator->needBracket())
        stream << "(";

    std::for_each(_values.begin(), _values.end(), [&stream, this](const auto& value) {
        if(value != Column::nullValue())
            stream << "'";

        stream << value;

        if(value != Column::nullValue())
            stream << "'";

        if(_conditionOperator->needBracket())
            stream << ", ";
    });

    if(_conditionOperator->needBracket()) {
        stream.seekp(-2, std::stringstream::cur);
        stream << ")";
    }

    return stream.str();
}

ConditionGroup::Condition::Column ConditionGroup::Condition::column() const
{
    return _column;
}

std::shared_ptr<Operator::IOperator> ConditionGroup::Condition::conditionOperator() const
{
    return _conditionOperator;
}

std::vector<std::string> ConditionGroup::Condition::values() const
{
    return _values;
}

bool ConditionGroup::Condition::isValid() const
{
    return _conditionOperator != nullptr || !_values.empty() || _column.isValid();
}

ConditionGroup::ConditionGroup(const Condition& condition)
{
    std::get<1>(_node) = condition;
}

ConditionGroup ConditionGroup::operator&&(const ConditionGroup& rhd) const
{
    ConditionGroup group;

    std::get<0>(group._node) = LogicalOperator::AND;

    group._left = std::make_shared<ConditionGroup>(*this);

    group._right = std::make_shared<ConditionGroup>(rhd);

    return group;
}

ConditionGroup ConditionGroup::operator&&(const Condition& rhd) const
{
    ConditionGroup group;

    std::get<0>(group._node) = LogicalOperator::AND;

    group._left = std::make_shared<ConditionGroup>(*this);

    group._right = std::make_shared<ConditionGroup>();
    std::get<1>(group._right->_node) = rhd;

    return group;
}

ConditionGroup ConditionGroup::operator||(const ConditionGroup& rhd) const
{
    ConditionGroup group;

    std::get<0>(group._node) = LogicalOperator::OR;

    group._left = std::make_shared<ConditionGroup>(*this);

    group._right = std::make_shared<ConditionGroup>(rhd);

    return group;
}

ConditionGroup ConditionGroup::operator||(const Condition& rhd) const
{
    ConditionGroup group;

    std::get<0>(group._node) = LogicalOperator::OR;

    group._left = std::make_shared<ConditionGroup>(*this);

    group._right = std::make_shared<ConditionGroup>();
    std::get<1>(group._right->_node) = rhd;

    return group;
}

std::string ConditionGroup::unwrap(const CondionViewType viewType, const bool compressed) const
{
    std::stringstream stream;
    unwrapTree(this, stream, viewType, compressed);

    return stream.str();
}

bool ConditionGroup::isValid() const
{
    return !isSheet() || std::get<1>(_node).isValid();
}

bool ConditionGroup::isSheet() const
{
    return _left == _right && _left == nullptr;
}

void ConditionGroup::unwrapTree(const ConditionGroup* node, std::stringstream& stream, const CondionViewType viewType, const bool compressed)
{
    if(node->isSheet()) {
        stream << std::get<1>(node->_node).unwrap(viewType);
        return;
    }

    stream << "(";

    if(node->_left != nullptr)
        unwrapTree(node->_left.get(), stream, viewType, compressed);

    if(!compressed)
        stream << "\n";

    switch(std::get<0>(node->_node)) {
        case LogicalOperator::AND: {
            stream << " AND ";
            break;
        }
        case LogicalOperator::OR: {
            stream << " OR ";
            break;
        }
    }

    if(node->_right != nullptr)
        unwrapTree(node->_right.get(), stream, viewType, compressed);

    stream << ")";
}

ColumnInfo::Settings operator&(ColumnInfo::Settings a, ColumnInfo::Settings b)
{
    return static_cast<ColumnInfo::Settings>(static_cast<short>(a) & static_cast<short>(b));
}

} // namespace QueryCraft