[TOC]
## 外模式
- 视图
  - 是一种关系
## 模式
- 基本表
  - 一个关系对应一个基本表，一个或多个基本表对应一个存储文件，一个表可以带若干索引
## 内模式
  - 存储文件的逻辑结构
  - 存储文件的物理结构对用户隐蔽
  - 索引属于该范畴

## 语句
### 创建
- 创建模式、索引
  ```sql
  -- 创建模式，一个模式下面有很多个表
  -- 把模式作为环境
  CREATE SCHEMA xxx authorization user;
  SET search_path TO 'schema name';

  -- 在table的column上创建名为xxx的索引，以降/升序
  CREATE [UNIQUE/CLUSTER] INDEX xxx ON table.column [DESC/ASC];
  ```
### 查询
- 普通查询
  ```sql
  -- DISTINCT删除相同的行，只能对输出结果做修饰
  select [DISTINCT] column_1,column_2 from table
  
  -- WHERE 子句
  -- BETWEEN AND区间
  WHERE column_3 = 'xx'
        column_3 BETWEEN 1 AND 2
  -- IN 是否在集合
  WHERE column_4 IN ('a','b','c')
  -- LIKE 匹配字符串，可以包含通配符%和_，分别表示多个随便字符和单个随便字符，如果不包含通配符，则相当于等号
  -- adb,addb,adddb
  WHERE column_5 LIKE 'a%b'
  -- adb,acb,afb
  WHERE column_5 LIKE 'a_b'
  -- 如果要查的数据本来就有%和_，需要转义
  WHERE column_5 LIKE 'a\%b' ESCAPE '\'
  -- 空值只能用IS指向，不能用=
  WHERE column_5 IS NULL
  -- AND OR NOT 逻辑运算
  WHERE column_5 IS NOT NULL

  -- ORDER BY 子句
  -- 降序/升序排列，升序中NULL最先显示，降序中NULL最后显示
  -- 不能在嵌套子查询中出现
  -- 可以有多个排序条件, 先排条件1, 1相同时排条件2
  ORDER BY column_6 [DESC/ASC]

  -- 聚集函数（跳过空值）
  -- COUNT() 统计元组（行）个数；
  -- 统计一列中值得个数；
  COUNT([DISTINCT/ALL] column_7)   
  -- SUM计算总和，AVG计算平均，MAX计算最大，MIN计算最小
  
  -- GROUP BY 将查询结果分组，如果没有分组，聚集函数作用于整个查询结果，如果分组了，那么分别作用于每一个组
  -- GROUP BY 一旦出现，那么SELECT后面的属性一定要出现在GROUP BY中
  -- 可以在嵌套查询中使用
  GROUP BY column_8 
  -- HAVING子句筛选组，WHERE不能筛选组
  -- 筛选数量大于3的组
  HAVING COUNT(*) > 3

  ```
- 连接查询
  ```sql
  -- 如果在table_2上建立column_2的索引，那么查询会变快，因为不用从头到尾遍历了
  SELECT table_1.*,table_2.* FROM table_1,table_2
  WHERE table_1.column_1 [=/各种上面说的运算] table_2.column_2

  -- 如果column_1在table_1和table_2中是唯一的，那么可以去掉表名前缀，如果不唯一需要前缀

  -- 可以自连接，但是需要给该表别名
  SELECT a.column_1,b.column_2
  FROM table_1 a, table_1 b
  WHERE a.column_2 = b.column_1

  -- 左连接：保留左边的所有元组，无论其是否符合查询条件
  SELECT table_1.column_1,table_1.column_2,table_2.column_3
  FROM table_1 LEFT OUTER JOIN table_2 ON (table_1.column_2 = table_2.column_4)

  -- 用AND进行多表连接
  ```
- 集合操作
  ```sql
  SELECT column_1 FROM table_1 WHERE column_2 > 10
  -- 并集、交集、差集
  -- UNION会自动去除重复元组，如果需要保留，则使用UNION ALL
  UNION[ALL]/INTERSECT/EXCEPT
  SELECT column_1 FROM table_1 WHERE column_3 in ('a','b')
  ```
### 更新数据
数据的增添删改可能会破坏参照完整性
- 修改表结构
  ```sql
  ALTER TABLE table_1
  -- 添加新列
  ADD column_x CHAR(10) NOT NULL,column_y
  -- 添加约束
  ADD 表级完整性约束
  -- 删除列、完整性约束
  DROP column_1
  DROP CONSTRAINT c_1 RESTRICT
  -- 改变数据类型
  ALTER column_2 REAL
  ```

- 插入元组
  ```sql
  -- 插入新的一条数据（元组）
  -- 可以指定列名，可以不指定
  -- 新建元组的column_2为空值
  INSERT
  INTO table_1(column_1,column_3)
  VALUES('zpt',15);
  -- 可以使用子查询
  -- 使用子查询时不能用VALUES，对于定值可以在select中返回
  INSERT INTO table_1
  SELECT 1,column_3,AVG(column_2)
  FROM table_2
  GROUP BY column_3;

  ```

- 修改
  ```sql
  -- 修改一列 
  UPDATE Student 
  SET column_3=22
  WHERE column_1='zpt';

  -- 修改全部
  UPDATE Student
  SET Sage = Sage+1;
  ```
- 删除
  ```sql
  -- 删除一条元组（记录）
  DELETE
  FROM table_1
  WHERE column_1='zpt';

  -- 删除table_2中所有
  DELETE
  FROM table_2;

  -- 可以嵌套查询
  -- 不能cascade
  DELETE
  FROM table_2
  WHERE column_1 IN
    (
      SELECT column_1
      FROM table_2
      WHERE colmn_1='zpt';
    )
  ```
- 空值
**属性定义中有```NOT NULL```约束条件的不能取空值，加了```UNIQUE```限制的属性不能取空值，码属性不能取空值**
  ```sql
  -- 判断是否为空
  SELECT *
  FROM table_1
  WHERE column_1 IS NOT NULL;
  ```
  空值与另一个值的算术运算结果为空值，空值与另一个值的比较结果为```UNKNOWN```，```UNKNOWN```的非还是```UNKNOWN```，```SELECT```语句不会选择结果为```UNKNOWN```的

### 视图
- 创建
  ```CREATE VIEW```语句执行时不查询，在查询VIEW时才会进行查询
  ```sql
  -- 子查询语句中是否允许ORDER BY和DISTINCT短语，取决于系统的实现
  -- 列名可以省略，如果省略，将子查询中返回的列作为结果
  -- 当目标列是聚集函数或者列表达式时、或者子查询中使用的多个表存在相同列名时，必须指定列名
  CREATE VIEW view_1(column_1,column_2,column_3)
  AS (
    <子查询语句>
  )
  -- 如果加入下面这句，对视图进行增添删改时会自动执行子查询中的WHERE来判断该条数据是否合格
  [WITH CHECK OPTION];
  -- 视图的子查询中可以带表达式或者聚集函数，成为带表达式的视图
  CREATE VIEW view_2(column_1,column_2,column_3_avg)
  AS
  SELECT column_1,column_2-10,AVG(column_3)
  FROM view_2
  GROUP BY column_8;
  ```
- 行列子集视图：从单个基本表中删除一些列，保留码，导出的视图
- 删除
  ```sql
  -- 如果带上CASCADE参数则会删除所有由当前视图导出的视图
  -- 基本表删除后，视图的定义仍然保存在DBMS中，需要显示删除
  DROP VIEW view_1 CASCADE;
  ```
- 查询
  视图定义后需要显示删除，派生表用完就被删除
  ```sql
  -- 查询视图时，先将视图定义的子查询和用户对视图的查询结合起来，转换成等价的对基本表的查询，再执行修正了的查询，这称为视图消解
  -- 非行列子集的视图查询有可能视图转换/消解不能正确进行
  SELECT * FROM view_1
  WHERE column_1 LIKE "zpt";
  ```
- 更新
  ```sql
  -- 定义了WITH CHECK OPTION的视图会自动将条件加入增删改的元组中
  -- 有一些视图没法修改/添加值，比如使用聚集函数构造的视图
  -- p137
  UPDATE view_1
  SET column_1='zpt'
  WHERE column_2='2018202180';
  ```

## Tips
### 函数
- 日期加减
  ```sql
  DATEADD()
  ```
- 小数
  ```sql
  FLOOR()
  ```
- 间隔
  ```sql
  INTERVAL()
  ```
- 获取用户
  ```sql
  USER
  ```
## 理解
- **嵌套子查询**
  - 理解为循环, 对于外层查询中的每一个元组, 进行一次内层子查询
  - `EXISTS`
    - 对于外层查询中的每一个元组, 去验证这个元组是否满足条件
    - 双重```NOT EXISTS```代表：**不要不符合条件的**，最后一层嵌套查询中是条件（正向的）