# 实验二-数据库定义实验
姓名：张配天\
学号：2018202180
## 一、实验环境
- cpu：AMD4800HS
- gpu：RTX2060mq
- RDBMS：Kingbase v7.0

## 二、实验步骤
1. 创建数据库
    ```sql
    CREATE DATABASE TPCH ENCODING='utf-8';
    ```
2. 创建和应用模式
    ```sql
    CREATE SCHEMA Sales AUTHORIZATION SYSTEM;
    set search_path TO "Sales",PUBLIC;
    ```
3. 创建基本表（前六个）
    ```sql
    CREATE TABLE Region(regionkey INT PRIMARY KEY, name VARCHAR(32), comment VARCHAR(32));
    CREATE TABLE Nation(nationkey INT PRIMARY KEY, name VARCHAR(32),regionkey INT,comment VARCHAR(32),FOREIGN KEY(regionkey) REFERENCES Region(regionkey));
    CREATE TABLE Supplier(suppkey INT PRIMARY KEY,name VARCHAR(32),address VARCHAR(32),nationkey INT,phone CHAR,acctbal REAL,comment VARCHAR(32),FOREIGN KEY(nationkey) REFERENCES Nation(nationkey));
    CREATE TABLE Part(partkey INT PRIMARY KEY, name VARCHAR(32),mfgr CHAR,brand CHAR,type VARCHAR(32),size INT,container CHAR,retailprice REAL,comment VARCHAR(32));
    CREATE TABLE PartSupp(partkey INT,suppkey INT,availqty INT,supplycost REAL,comment VARCHAR(32),PRIMARY KEY(partkey,suppkey),FOREIGN KEY(partkey) REFERENCES Part(partkey), FOREIGN KEY(suppkey) REFERENCES Supplier(suppkey));
    CREATE TABLE Customer(custkey INT PRIMARY KEY,name VARCHAR(32),address VARCHAR(32),nationkey INT,phone CHAR,acctbal REAL,mktsegment CHAR,comment VARCHAR(32),FOREIGN KEY(nationkey) REFERENCES Nation(nationkey));
    ```
    其中使用```FOREIGN KEY(xxx) REFERENCES Y(xxx)```来添加外码，同时需要保证参照完整性约束。

4. 创建基本表（后两个，带用户完整性约束及注释）
    ```sql
    CREATE TABLE Orders(orderkey INT PRIMARY KEY,custkey INT,Orderstatus CHAR,totalprice REAL,orderdate DATE,orderpriority CHAR,clerk CHAR,shippriority INT,comment VARCHAR(32),FOREIGN KEY(custkey) REFERENCES Customer(custkey));
    CREATE TABLE Lineitem(orderkey INT,partkey INT,suppkey INT,Linenumber INT,quantity REAL,extendedprice REAL,discount REAL,tax REAL,returnflag CHAR,linestatus CHAR,shipdate DATE,commitdate DATE,receiptdate DATE,shipinstruct CHAR,shipmode CHAR,comment VARCHAR(32),PRIMARY KEY(orderkey,Linenumber),FOREIGN KEY(orderkey) REFERENCES Orders(orderkey),FOREIGN KEY(partkey) REFERENCES Part(partkey),FOREIGN KEY(suppkey) REFERENCES Supplier(suppkey),CHECK(discount <= 1.00 and discount >= 0.00),CHECK(tax <= 0.08 and tax >= 0.00));
    ```
    其中使用```COMMENT```语句为特定的列添加注释（用户定义完整性约束），使用```CHECK```语句检查该属性的值是否在规定的域中（用户定义完整性约束）。
## 三、实验结果
1. 查询所有数据库，可以看到其中有创建的TPCH数据库，使用了utf-8编码
    ![](resources/kingbase1.png)
2. 查询数据库中所有数据表，**由于金仓数据库的查询分析器中有选择数据库的按钮，可以直接选中数据库如下**
    
    ![](resources/kingbase3.png)
    
    我暂时还没找到通过sql命令查询某一个数据库中的所有表，因为使用
    ```sql
    SELECT * FROM SYS_TABLES;
    ```
    查询得到的表的数据结构中可以发现没有一个表名其所属数据库的列，*如下图*

    ![](resources/kingbase4.png)

    因此我只能通过选定数据库**TPCH**后限定模式来查询该模式下的所有表，*如下图*

    ![](resources/kingbase2.png)

3. 查询8张基本表的表结构
    - Region
    
      ![](resources/表1.png)
    - Nation
    
      ![](resources/表2.png)

    - Supplier
    
      ![](resources/表3.png)

    - Part
    
      ![](resources/表4.png)

    - PartSupp
    
      ![](resources/表5.png)

    - Customer
    
      ![](resources/表6.png)

    - Orders
    
      ![](resources/表7.png)

    - Lineitem
    
      ![](resources/表8.png)

## 四、问题
1. 老师提供的*sql参考*中只有查询表级注释的语句，试了一下使用
    ```sql
    SELECT OBJ_DESCRIPTION( (SELECT oid FROM sys_class WHERE relname='table.column') ,'SYS_CLASS'); 
    ```
    是无法成功查询列级注释的，网上的答案和金仓也没有很好的兼容，想问一下**怎么查询列级注释？**
    
2. 如何查询添加进特定列的```check```的条件？