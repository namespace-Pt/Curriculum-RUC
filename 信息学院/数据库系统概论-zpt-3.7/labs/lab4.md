# 实验四-安全性和完整性实验
姓名：**张配天**\
学号：**2018202180**
## 一、实验环境
- cpu：AMD4800HS
- gpu：RTX2060MaxQ
- RDBMS：Kingbase v7.0
  
## 二、实验步骤
### 1.安全性实验
- 创建用户
  - 使用企业管理器创建用户（因为直接sql语句报错）
    ![](../resources/lab4/user.png)
- 4-1
  ```sql
  GRANT SELECT ON TABLE Student TO u;
  ```
- 4.2
  ```sql
  GRANT ALL PRIVILEGES ON TABLE Student,Course TO u2,u3;
  ```
- 4-3
  ```sql
  GRANT SELECT ON TABLE SC TO PUBLIC;
  ```
- 4-4
  ```sql
  GRANT UPDATE(Sno),SELECT ON TABLE Student TO u4;
  ```
- 4-5
  ```sql
  GRANT INSERT ON TABLE SC TO u5 WITH GRANT OPTION;
  ```
- 4-6
  ```sql
  GRANT INSERT ON TABLE SC TO u6 WITH GRANT OPTION;
  ```
- 4-7
  ```sql
  GRANT INSERT ON TABLE SC TO u7;
  ```
- 4-9（4-8金仓做不了，老师说不用做）
  ```sql
  REVOKE SELECT ON TABLE SC FROM PUBLIC;
  ```
- 4-10
  ```sql
  REVOKE INSERT ON TABLE SC FROM u5 CASCADE;
  ```

### 2.完整性实验
- 创建新列
  ```sql
  SET search_path TO NORTHWIND;
  ALTER TABLE Orders
  ADD TotalPrice REAL;
  ```
- 定义`UPDATE`触发器
  ```sql
    CREATE TRIGGER trig_up
  AFTER UPDATE OF quantity,discount ON Order_Details
  FOR EACH ROW
  AS BEGIN
  	UPDATE Orders
      	SET TotalPrice = NEW.unitprice * NEW.quantity * (1-NEW.discount)
     	WHERE Orders.orderid = NEW.orderid;
  END
  ```
- 定义`INSERT`触发器
  ```sql
  CREATE TRIGGER trig_in
  AFTER INSERT ON Order_Details
  FOR EACH ROW
  AS BEGIN
      UPDATE Orders
      	SET TotalPrice = NEW.unitprice * NEW.quantity * (1-NEW.discount)
     	WHERE Orders.orderid = NEW.orderid;
  END
  ```
- 定义`BEFORE INSERT`触发器
  ```sql
  CREATE TRIGGER trig_ck
  BEFORE INSERT ON Order_details
  FOR EACH ROW
  AS BEGIN
      SELECT unitsinstock AS stock FROM Products WHERE Products.productid = New.productid;
      IF (stock < New.quantity)
          THEN RAISE EXCEPTION 'failed';
      END IF;
  END;
  ```
- 最后运行
  ```sql
  UPDATE quantity FROM Order_Details SET quantity=quantity;
  ```
  实现批量更新`Orders.TotalPrice`

## 三、实验结果
### 安全性实验
- 4-1
  ![](../resources/lab4/1.png)
- 4-2
  ![](../resources/lab4/2.png)
- 4-3
  ![](../resources/lab4/3.png)
- 4-4
  ![](../resources/lab4/4.png)
- 4-5
  ![](../resources/lab4/5.png)
- 4-6
  ![](../resources/lab4/6.png)
- 4-7
  ![](../resources/lab4/7.png)
- 4-9
  ![](../resources/lab4/9.png)
- 4-10
  ![](../resources/lab4/10.png)
  
### 完整性实验
- 1
  ![](../resources/lab4/2_1.png)
- 2
  ![](../resources/lab4/2_2.png)
- 3
  ![](../resources/lab4/2_3.png)
- 运行`UPDATE`语句后成功执行更新
  ![](../resources/lab4/2_4.png)
## 三、实验问题
- 在金仓中创建触发器，使用if语句，则编译错误，课件中给的例子也编译失败![](../resources/lab4/2_5.png)