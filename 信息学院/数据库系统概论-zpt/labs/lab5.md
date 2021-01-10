# 实验五-查询优化实验
姓名：**张配天**\
学号：**2018202180**
## 一、实验环境
- cpu：AMD4800HS
- gpu：RTX2060MaxQ
- RDBMS：Kingbase v7.0
  
## 二、实验步骤和分析
### 1. 查看查询计划
- 建立索引前:
  - 通过执行位图索引和hash join的连接进行优化
    ![](../resources/lab5/1.png)
  - 执行时间为**0.147s**
    ![](../resources/lab5/1_23.png)
- 建立索引后:
  - 查询计划没有改变\
    ![](../resources/lab5/2.png)
  - 执行时间变快了, 为**0.138s**
    ![](../resources/lab5/2_2.png)
<br>

### 2. 优化SQL语句
- **`in`和`exists`**
  - `in`的效率:
    ![](../resources/lab5/in.png)
  - 改写后的`exists`语句:
    ```sql
    select * from orders O where exists (select * from lineitem L where L.orderkey=O.orderkey and exists (select * from part P where P.name='发电机组' and L.partkey=P.partkey));
    ```
  - `exists`的效率:
  ![](../resources/lab5/exists.png)
  - 相比之下, **exists还慢了, 和文档中预测的不一致**
<br>

- **相关子查询和不相关子查询**
  - 相关子查询:
    ![](../resources/lab5/相关.png)
  - 不相关子查询:
    ![](../resources/lab5/不相关.png)
  - **不相关子查询的效率显著提升**,  原因是
    - 相关子查询要对**每一个`O1`中的元组计算`O2`的相同`custkey`元组的平均金额**
    - 不相关子查询**仅需计算一次**
<br>

- **数据库模式规范化设计对查询性能的影响**
  - 使用`orders.totalprice`查询:
    - `sql`语句:
      ```sql
      select totalprice from lineitem, orders where orders.orderkey=lineitem.orderkey;
      ```
    - 运行时间:
    ![](../resources/lab5/lineitem.png)
  - 使用`lineitem`计算:
    - `sql`语句:
      ```sql
      select lineitem.linenumber * lineitem.extendedprice*(1-lineitem.discount)*(1+lineitem.tax) from lineitem,orders where orders.orderkey=lineitem.orderkey
      ```
    - 运行时间:
    ![](../resources/lab5/order.totalprice.png)  
  - **分析:**
  可以发现**在当前数据集上两者性能差异不大**, 但如果面临海量数据, 肯定是直接查询现有的值`totalprice`而非计算会更**高效**, 同时`TPCH`数据库符合`3NF`, 因为在`Orders`中有`totalprice`不依赖于主码`orderkey`, 而是依赖于`lineitem.discount, lineitem.tax, lineitem.extendedprice, lineitem.linenumber`, 所以不是`BCNF`, 而没有传递依赖, 因此属于`3NF`
  - **优化:**
  在经常查询的属性上**建立索引**。