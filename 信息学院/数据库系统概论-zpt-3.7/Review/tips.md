## SQL
- `DDL`中所有指明列名都在括号里
  - 创建视图要指明列名
- `DDL`中所有关键字后面要声明数据对象
  - ```sql
    GRANT ... ON TABLE t1 ...
    ```
  - ```sql
    CREATE TABLE t2(...)
    ```
- `DDL`中参数都是括在括号里的
  - ```sql
    CREATE TABLE A(a VARCHAR(30) PRIMARY KEY, CONSTRAINT c CHECK(a not null))
    ```
- **聚集函数不能嵌套使用**
## 关系数据理论
- `3NF`中传递依赖的结尾是一个**非主属性**
- 没有函数依赖的关系是`BCNF`

# 回做
- 第九章补充习题