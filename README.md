# QueryCraft

Библиотека QueryCraft предназначена для автоматической генерации SQL-запросов. Она предоставляет разработчикам инструменты для создания сложных запросов без необходимости написания кода вручную.

Для использования необходимо описать структуру таблицы:
``` 
sql_table table("table_name", "schema_name",
    column_info("c1"),
    column_info("c2"),
    column_info("c3"));
```

Далее у этого класса есть следующий перечень функций, который отвечает за генерацию соответствующего запроса.
```
add_row(const row& row);

insert_sql(const std::vector<column_info>& columns = {});
update_sql(const condition_group& condition = {}, const std::vector<column_info>& columns = {});
remove_sql(const condition_group& condition = {}) const;
std::string select_sql(
    const std::vector<join_column>& join_columns = {},
    const condition_group& condition = {},
    const std::vector<sort_column>& sort_columns = {},
    size_t limit = 0,
    size_t offset = 0,
    const std::vector<column_info>& columns = {}) const;
```
Функция add_row нужна для того, чтобы передать информацию о строке для вставки или обновления
