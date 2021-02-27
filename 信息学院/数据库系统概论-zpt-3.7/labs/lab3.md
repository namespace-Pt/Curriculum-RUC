# 实验三-数据库查询实验
姓名：**张配天**\
学号：**2018202180**
## 一、实验环境
- cpu：AMD4800HS
- gpu：RTX2060mq
- RDBMS：Kingbase v7.0

## 二、实验步骤
### 导入数据
```sql
SET search_path TO Sales;
COPY region FROM 'D:\\Data\\Class_data\\DB_data\\lab3\\region.csv' with csv header;
COPY nation FROM 'D:\\Data\\Class_data\\DB_data\\lab3\\nation.csv' with csv header;
COPY supplier FROM 'D:\\Data\\Class_data\\DB_data\\lab3\\supplier.csv' with csv header;
COPY part FROM 'D:\\Data\\Class_data\\DB_data\\lab3\\part.csv' with csv header;
COPY partsupp FROM 'D:\\Data\\Class_data\\DB_data\\lab3\\partsupp.csv' with csv header;
COPY customer FROM 'D:\\Data\\Class_data\\DB_data\\lab3\\customer.csv' with csv header;
COPY orders FROM 'D:\\Data\\Class_data\\DB_data\\lab3\\orders.csv' with csv header;
COPY lineitem FROM 'D:\\Data\\Class_data\\DB_data\\lab3\\lineitem.csv' with csv header;
```
### 1.(1)
使用表连接进行查询
```sql
SELECT C.* FROM Part P,Orders O,Lineitem L,Customer C
WHERE P.mfgr='海大' AND P.name='船舶模拟驾驶舱' AND L.partkey=P.partkey AND O.orderkey=L.orderkey AND C.custkey=O.custkey;
```
### 1.(2)
使用嵌套```EXISTS```进行查询，**关于双重```NOT EXISTS```的理解：不选不满足==条件==的元组**
```sql
SELECT C1.name FROM Customer C1,Orders O1
WHERE C1.name <> '张三' AND NOT EXISTS
(   SELECT * FROM Customer C2,Orders O2,Lineitem L2
    WHERE C2.name='张三' AND C2.custkey=O2.custkey AND O2.orderkey=L2.orderkey AND NOT EXISTS
    (   SELECT * FROM Lineitem L1
        WHERE O1.custkey=C1.custkey AND L1.orderkey=O1.orderkey AND L2.partkey=L1.partkey));
```
### 1.(3)
使用基于派生表的查询，因为需要查询的是**顾客的所有信息**，而如果使用```GROUP BY```子句**则```SELECT```后的所有属性都要出现在```GROUP BY```后**，我懒得打那么多列，因此先在派生表中选出对应的顾客名，再根据顾客名返还相应顾客的所有信息
```sql
SELECT C.* FROM (SELECT C.name FROM Customer C,Nation N,Orders O 
WHERE C.nationkey=N.nationkey AND N.name='中国' AND O.custkey=C.custkey
GROUP BY C.name HAVING AVG(O.totalprice) >= 10000) AS N,Customer C
WHERE C.name = N.name;
```
### 1.(4)
使用```EXCEPT```子句对集合做差，返还对应结果
```sql
SELECT P.* FROM Part P,Orders O,Customer Cz,Lineitem L
WHERE Cz.name='张三' AND Cz.custkey=O.custkey AND O.orderkey=L.orderkey AND P.partkey=L.partkey
EXCEPT
SELECT P.* FROM Part P,Orders O,Customer Cl,Lineitem L
WHERE Cl.name='李四' AND Cl.custkey=O.custkey AND O.orderkey=L.orderkey AND P.partkey=L.partkey
```
### 2.(1)
通过将相同department的课程分组，进行查询
```sql
SELECT AVG(score) AS avg_score,AVG(grade) AS avg_grade FROM Course C
GROUP BY dept;
```
### 2.(2)
**自连接查询**
```sql
SELECT C2.teacher FROM Course C1,Course C2
WHERE C1.teacher = 5 AND C1.dept=C2.dept;
```
### 2.(3)
使用下面的方法不成功，返回的值均为空，但**如果仅查询单个表（某一个分数区间）则可以正常返回，不知道为啥**
```sql
SELECT AVG(S1.score) AS score_0_10,AVG(S2.score) AS score_10_20,AVG(S3.score) AS score_20_30,AVG(S4.score) AS score_30_40,AVG(S5.score) AS score_40_50,AVG(S6.score) AS score_50_60,AVG(S7.score) AS score_60_70,AVG(S8.score) AS score_70_80,AVG(S9.score) AS score_80_90,AVG(S10.score) AS score_90_100
FROM
(SELECT * FROM Course C WHERE grade >= 0 AND grade < 10) AS S1,
(SELECT * FROM Course C WHERE grade >= 10 AND grade < 20) AS S2,
(SELECT * FROM Course C WHERE grade >= 20 AND grade < 30) AS S3,
(SELECT * FROM Course C WHERE grade >= 30 AND grade < 40) AS S4,
(SELECT * FROM Course C WHERE grade >= 40 AND grade < 50) AS S5,
(SELECT * FROM Course C WHERE grade >= 50 AND grade < 60) AS S6,
(SELECT * FROM Course C WHERE grade >= 60 AND grade < 70) AS S7,
(SELECT * FROM Course C WHERE grade >= 70 AND grade < 80) AS S8,
(SELECT * FROM Course C WHERE grade >= 80 AND grade < 90) AS S9,
(SELECT * FROM Course C WHERE grade >= 90 AND grade < 100) AS S10;
```
因此，采用另一种方法，**使用```FLOOR```将分数区间转化为分数第一位是0~9，之后直接根据此进行分组**
```sql
SELECT FLOOR(grade/10),AVG(score) FROM Courses
GROUP BY FLOOR(grade/10)
```
## 三、实验结果
- 1.(1)
![](../resources/lab3/lab3_1.png)
- 1.(2)
![](../resources/lab3/lab3_2.png)
- 1.(3)
![](../resources/lab3/lab3_3.png)
- 1.(4)
![](../resources/lab3/lab3_4.png)
- 2.(1)
![](../resources/lab3/lab3_5.png)
- 2.(2)
![](../resources/lab3/lab3_6.png)
- 2.(3)
![](../resources/lab3/lab3_7.png)

## 四、实验问题
1. 在**第二题第三问**中提出的
2. **如何更好的理解EXISTS**