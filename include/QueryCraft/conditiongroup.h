#pragma once

#include "enum/conditionviewtype.h"
#include "enum/logicaloperator.h"
#include "helper/typeconverter.h"
#include "operator/equalsoperator.h"
#include "operator/inoperator.h"
#include "operator/lessoperator.h"
#include "operator/lessorequalsoperator.h"
#include "operator/moreoperator.h"
#include "operator/moreorequalsoperator.h"
#include "operator/notequalsoperator.h"
#include "operator/notinoperator.h"
#include "querycraft_global.h"

#include <algorithm>
#include <cstdint>
#include <memory>
#include <tuple>
#include <vector>

namespace QueryCraft {

/// Структура ConditionGroup предоставляет возможность создания сложных условий с использованием логических операторов
/// (И, ИЛИ) и поддерживает создание условий для столбцов таблицы.
struct QUERYCRAFT_EXPORT ConditionGroup
{
    /// Cтруктура представляющая отдельное условие.
    struct QUERYCRAFT_EXPORT Condition
    {
        /// Определяет структуру для хранения информации о столбце таблицы.
        struct QUERYCRAFT_EXPORT Column
        {

            /**
             * @brief Перечисление настроек столбца в базе данных.
             */
            enum class Settings : uint8_t
            {
                /**
                 * Нет настроек.
                 */
                NONE = 0,

                /**
                 * Столбец является первичным ключом.
                 */
                PRIMARY_KEY = 1,

                /**
                 * Столбец не может содержать NULL-значения.
                 */
                NOT_NULL = 2
            };

            /**
             * Возвращает константу, представляющую пустое значение для столбца.
             * @return Строка "NULL".
             */
            constexpr static auto nullValue() { return "NULL"; }

            Column() = default;

            /**
             * Конструктор с одним параметром. Инициализирует столбец с указанным именем и настройками по умолчанию.
             * @param name Имя столбца.
             * @param settings Настройки столбца.
             */
            explicit Column(std::string name, Settings settings = Settings::NONE);

            Column(const Column& other) = default;

            Column(Column&& other) noexcept = default;

            /**
             * Возвращает имя столбца.
             * @return Имя столбца.
             */
            std::string name() const;

            /**
             * Возвращает полное имя столбца.
             * @return Полное имя столбца.
             */
            std::string fullName() const;

            /**
             * Устанавливает полное имя столбца.
             * @param fullName Полное имя столбца.
             */
            void setFullName(const std::string& fullName);

            /**
             * Возвращает псевдоним столбца.
             * @return Псевдоним столбца.
             */
            std::string alias() const;

            /**
             * Устанавливает псевдоним столбца.
             * @param alias Псевдоним столбца.
             */
            void setAlias(const std::string& alias);

            /**
             * Добавляет настройку к столбцу.
             * @param settings Настройка, которую нужно добавить.
             */
            void addSettings(Settings settings);

            /**
             * Проверяет, имеет ли столбец указанную настройку.
             * @param settings Настройка, которую нужно проверить.
             * @return true, если столбец имеет указанную настройку, иначе false.
             */
            bool hasSettings(Settings settings) const;

            Column& operator=(const Column& other) = default;

            Column& operator=(Column&& other) noexcept = default;

            bool operator==(const Column& rhs) const;

            bool operator!=(const Column& rhs) const;

            /**
             * Возвращает условие, проверяющее, является ли значение столбца пустым.
             * @return Условие, проверяющее, является ли значение столбца пустым.
             */
            Condition isNull() const;

            /**
             * Возвращает условие, проверяющее, является ли значение столбца не пустым.
             * @return Условие, проверяющее, является ли значение столбца не пустым.
             */
            Condition notNull() const;

            /**
             * Возвращает условие, проверяющее, принадлежит ли значение столбца к указанным значениям.
             * @tparam Args Типы аргументов, которые могут быть преобразованы в строки.
             * @param args Значения, к которым должно принадлежать значение столбца.
             * @return Условие, проверяющее, принадлежит ли значение столбца к указанным значениям.
             */
            template<typename... Args>
            Condition in(Args&&... args) const
            {
                std::vector<std::string> values;
                for(const auto& arg : { std::forward<Args>(args)... })
                    values.emplace_back(Helper::convertToString(arg));

                return createCondition(std::make_shared<Operator::InOperator>(), std::move(values));
            }

            /**
             * Возвращает условие, проверяющее, принадлежит ли значение столбца к значениям, содержащимся в указанном диапазоне.
             * @tparam StartIt Тип итератора начала диапазона.
             * @tparam EndIt Тип итератора конца диапазона.
             * @param startIt Итератор начала диапазона.
             * @param endIt Итератор конца диапазона.
             * @return Условие, проверяющее, принадлежит ли значение столбца к значениям, содержащимся в указанном диапазоне.
             */
            template<class StartIt, class EndIt>
            Condition inList(StartIt&& startIt, EndIt&& endIt) const
            {
                std::vector<std::string> values;

                std::for_each(startIt, endIt, [&values](const auto& arg) {
                    values.emplace_back(Helper::convertToString(arg));
                });

                return createCondition(std::make_shared<Operator::InOperator>(), std::move(values));
            }

            /**
             * Возвращает условие, проверяющее, не принадлежит ли значение столбца к указанным значениям.
             * @tparam Args Типы аргументов, которые могут быть преобразованы в строки.
             * @param args Значения, к которым не должно принадлежать значение столбца.
             * @return Условие, проверяющее, не принадлежит ли значение столбца к указанным значениям.
             */
            template<typename... Args>
            Condition notIn(Args&&... args) const
            {
                std::vector<std::string> values;
                for(const auto& arg : { std::forward<Args>(args)... })
                    values.emplace_back(Helper::convertToString(arg));

                return createCondition(std::make_shared<Operator::NotInOperator>(), std::move(values));
            }

            /**
             * Возвращает условие, проверяющее, не принадлежит ли значение столбца к значениям, содержащимся в указанном диапазоне.
             * @tparam StartIt Тип итератора начала диапазона.
             * @tparam EndIt Тип итератора конца диапазона.
             * @param startIt Итератор начала диапазона.
             * @param endIt Итератор конца диапазона.
             * @return Условие, проверяющее, не принадлежит ли значение столбца к значениям, содержащимся в указанном диапазоне.
             */
            template<class StartIt, class EndIt>
            Condition notInList(StartIt&& startIt, EndIt&& endIt) const
            {
                std::vector<std::string> values;

                std::for_each(startIt, endIt, [&values](const auto& arg) {
                    values.emplace_back(Helper::convertToString(arg));
                });

                return createCondition(std::make_shared<Operator::NotInOperator>(), std::move(values));
            }

            /**
             * Возвращает условие, проверяющее, равно ли значение столбца указанному значению.
             * @tparam T Тип значения, которое может быть преобразовано в строку.
             * @param value Значение, к которому должно быть равно значение столбца.
             * @return Условие, проверяющее, равно ли значение столбца указанному значению.
             */
            template<typename T>
            Condition operator==(const T& value) const
            {
                return createCondition(std::make_shared<Operator::EqualsOperator>(), { Helper::convertToString(value) });
            }

            /**
             * Возвращает условие, проверяющее, не равно ли значение столбца указанному значению.
             * @tparam T Тип значения, которое может быть преобразовано в строку.
             * @param value Значение, к которому не должно быть равно значение столбца.
             * @return Условие, проверяющее, не равно ли значение столбца указанному значению.
             */
            template<typename T>
            Condition operator!=(const T& value) const
            {
                return createCondition(std::make_shared<Operator::NotEqualsOperator>(), { Helper::convertToString(value) });
            }

            /**
             * Возвращает условие, проверяющее, меньше ли значение столбца указанному значению.
             * @tparam T Тип значения, которое может быть преобразовано в строку.
             * @param value Значение, к которому должно быть меньше значение столбца.
             * @return Условие, проверяющее, меньше ли значение столбца указанному значению.
             */
            template<typename T>
            Condition operator<(const T& value) const
            {
                return createCondition(std::make_shared<Operator::LessOperator>(), { Helper::convertToString(value) });
            }

            /**
             * Возвращает условие, проверяющее, меньше или равно ли значение столбца указанному значению.
             * @tparam T Тип значения, которое может быть преобразовано в строку.
             * @param value Значение, к которому должно быть меньше или равно значение столбца.
             * @return Условие, проверяющее, меньше или равно ли значение столбца указанному значению.
             */
            template<typename T>
            Condition operator<=(const T& value) const
            {
                return createCondition(std::make_shared<Operator::LessOrEqualsOperator>(), { Helper::convertToString(value) });
            }

            /**
             * Возвращает условие, проверяющее, больше ли значение столбца указанному значению.
             * @tparam T Тип значения, которое может быть преобразовано в строку.
             * @param value Значение, к которому должно быть больше значение столбца.
             * @return Условие, проверяющее, больше ли значение столбца указанному значению.
             */
            template<typename T>
            Condition operator>(const T& value) const
            {
                return createCondition(std::make_shared<Operator::MoreOperator>(), { Helper::convertToString(value) });
            }

            /**
             * Возвращает условие, проверяющее, больше или равно ли значение столбца указанному значению.
             * @tparam T Тип значения, которое может быть преобразовано в строку.
             * @param value Значение, к которому должно быть больше или равно значение столбца.
             * @return Условие, проверяющее, больше или равно ли значение столбца указанному значению.
             */
            template<typename T>
            Condition operator>=(const T& value) const
            {
                return createCondition(std::make_shared<Operator::MoreOrEqualsOperator>(), { Helper::convertToString(value) });
            }

            /**
             * Проверяет, является ли условие валидным.
             * @return true, если была создана колонка, иначе false.
             */
            bool isValid() const;

        private:
            /**
             * Создает и возвращает условие, основанное на указанном операторе и значениях.
             * @param conditionOperator Указатель на интерфейс оператора условия.
             * @param values Значения, которые будут использованы в условии.
             * @return Объект условия.
             */
            Condition createCondition(std::shared_ptr<Operator::IOperator>&& conditionOperator, std::vector<std::string>&& values) const;

        private:
            /// Имя столбца.
            std::string _name;

            /// Полное имя столбца.
            std::string _fullName;

            /// Псевдоним столбца.
            std::string _alias;

            /// Настройки столбца.
            Settings _columnSettings = Settings::NONE;
        };

        /**
         * Оператор логического "И" для объединения текущего условия с другим условием.
         * @param rhd Другое условие.
         * @return Объект ConditionGroup, представляющий объединенное условие.
         */
        ConditionGroup operator&&(const Condition& rhd) const;

        /**
         * Оператор логического "И" для объединения текущего условия с другим условием.
         * @param rhd Другое условие.
         * @return Объект ConditionGroup, представляющий объединенное условие.
         */
        ConditionGroup operator&&(const ConditionGroup& rhd) const;

        /**
         * Оператор логического "ИЛИ" для объединения текущего условия с другим условием.
         * @param rhd Другое условие.
         * @return Объект ConditionGroup, представляющий объединенное условие.
         */
        ConditionGroup operator||(const Condition& rhd) const;

        /**
         * Оператор логического "ИЛИ" для объединения текущего условия с другим условием.
         * @param rhd Другое условие.
         * @return Объект ConditionGroup, представляющий объединенное условие.
         */
        ConditionGroup operator||(const ConditionGroup& rhd) const;

        /**
         * Возвращает строковое представление текущего условия.
         * @param viewType Настройки для отображения названия колонки.
         * @return Строковое представление текущего условия.
         */
        std::string unwrap(CondionViewType viewType = CondionViewType::NAME) const;

        /**
         * Возвращает информацию о столбце текущего условия.
         * @return Объект ColumnInfo, содержащий информацию о столбце.
         */
        Column column() const;

        /**
         * Возвращает указатель на интерфейс оператора условия текущего условия.
         * @return Указатель на интерфейс оператора условия.
         */
        std::shared_ptr<Operator::IOperator> conditionOperator() const;

        /**
         * Возвращает значения текущего условия.
         * @return Вектор строк, содержащий значения текущего условия.
         */
        std::vector<std::string> values() const;

        /**
         * Проверяет, является ли текущее условие валидным.
         * @return true, если было создано условие, иначе false.
         */
        bool isValid() const;

    private:
        std::shared_ptr<Operator::IOperator> _conditionOperator {};
        Column _column {};
        std::vector<std::string> _values {};
    };

    ConditionGroup() = default;

    /**
     * Конструктор, который создает объект ConditionGroup на основе указанного условия.
     * @param condition Условие, которое будет использоваться для создания объекта ConditionGroup.
     */
    ConditionGroup(const Condition& condition);

    ConditionGroup(const ConditionGroup& other) = default;

    ConditionGroup(ConditionGroup&& other) noexcept = default;

    ConditionGroup& operator=(const ConditionGroup& other) = default;

    ConditionGroup& operator=(ConditionGroup&& other) noexcept = default;

    /**
     * Оператор логического "И" для объединения текущего условия с другим условием.
     * @param rhd Другое условие.
     * @return Объект ConditionGroup, представляющий объединенное условие.
     */
    ConditionGroup operator&&(const ConditionGroup& rhd) const;

    /**
     * Оператор логического "И" для объединения текущего условия с другим условием.
     * @param rhd Другое условие.
     * @return Объект ConditionGroup, представляющий объединенное условие.
     */
    ConditionGroup operator&&(const Condition& rhd) const;

    /**
     * Оператор логического "ИЛИ" для объединения текущего условия с другим условием.
     * @param rhd Другое условие.
     * @return Объект ConditionGroup, представляющий объединенное условие.
     */
    ConditionGroup operator||(const ConditionGroup& rhd) const;

    /**
     * Оператор логического "ИЛИ" для объединения текущего условия с другим условием.
     * @param rhd Другое условие.
     * @return Объект ConditionGroup, представляющий объединенное условие.
     */
    ConditionGroup operator||(const Condition& rhd) const;

    /**
     * Возвращает строковое представление текущего условия.
     * @param viewType Настройки для отображения названия колонки.
     * @param compressed Сжать выходную строку, если это возможно.
     * @return Строковое представление текущего условия.
     */
    std::string unwrap(CondionViewType viewType = CondionViewType::NAME, bool compressed = true) const;

    /**
     * Проверяет, является ли текущее условие валидным.
     * @return true, если было создано условие, иначе false.
     */
    bool isValid() const;

private:
    /**
     * Проверяет, является ли текущее условие листом (не содержит дочерних условий).
     * @return true, если текущее условие является листом, иначе false.
     */
    bool isSheet() const;

    /**
     * Рекурсивно обходит дерево условий и создает строковое представление.
     * @param node Указатель на текущее условие.
     * @param stream Строковый поток, куда будут добавляться условия.
     * @param viewType Настройки для отображения названия колонки.
     * @param compressed Сжать выходную строку.
     */
    static void unwrapTree(const ConditionGroup* node, std::stringstream& stream, CondionViewType viewType, bool compressed);

private:
    /**
     * Поле, содержащее кортеж, который хранит логический оператор и условие.
     * Используется для построения дерева условий в группе условий.
     */
    std::tuple<LogicalOperator, Condition> _node {};

    /**
     * Указатель на левое поддерево группы условий.
     * Используется для представления сложных условий с использованием логических операторов.
     */
    std::shared_ptr<ConditionGroup> _left {};

    /**
     * Указатель на правое поддерево группы условий.
     * Используется для представления сложных условий с использованием логических операторов.
     */
    std::shared_ptr<ConditionGroup> _right {};
};

using ColumnInfo = ConditionGroup::Condition::Column;
using ConditionInfo = ConditionGroup::Condition;

using ColumnSettings = ConditionGroup::Condition::Column::Settings;

/**
 * @brief Оператор "и" для перечисления ColumnInfo::Settings.
 *
 * @param a Первый операнд.
 * @param b Второй операнд.
 *
 * @return Результат операции "и" для двух перечислений ColumnInfo::Settings.
 */
QUERYCRAFT_EXPORT inline ColumnInfo::Settings operator&(ColumnInfo::Settings a, ColumnInfo::Settings b);

} // namespace QueryCraft
