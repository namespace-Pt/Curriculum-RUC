# 补充实验-数据库管理系统的安装与功能对比
姓名：张配天\
学号：2018202180
## 一、实验环境
- cpu：AMD4800HS
- gpu：RTX2060mq
- RDBMS：Kingbase v7.0
## 二、实验步骤
1. **安装**
   1. SQL SERVER
       - 安装[SQL SERVER](https://www.microsoft.com/zh-cn/sql-server/sql-server-downloads#)
       - 参照官方文档，安装[SSMS](https://docs.microsoft.com/zh-cn/sql/ssms/download-sql-server-management-studio-ssms?view=sql-server-2017)
       - 成功连接数据库
   2. Kingbase
       - 参照老师发的word安装成功
       - 值得注意的是如果直接开启查询分析器，需要输入的用户名是```SYSTEM```而不是最开始定义的自己的用户名。
2. **对应功能**
   1. 数据定义功能
      - SQL SERVER
        ![](resources/e3.png)
      - Kingbase
        如实验二；
   2. 数据组织、存储和管理
      - SQL SERVER
        ![](resources/sql数据组织和存储.png)
      - Kingbase
        ![](resources/kingbase组织存储.png)
   3. 数据操纵（*在1中都有涉及*）
   4. 数据库的事务管理和运行管理
      - SQL SERVER
        ![](resources/sql管理.png)
      - Kingbase
        ![](resources/kingbase事务管理.jpg)
    
   5. 数据库的建立和维护
      - SQL SERVER
        ![](resources/sql建立和维护.png)
      - Kingbase
        ![](resources/e6.png)
   6. 其他功能
      - SQL SERVER
        ![](resources/sql其他.png)
      - Kingbase
        ![](resources/kingbase其他.png)
## 三、实验结果及分析
- 从上不难看出SQL SERVER和Kingbase的界面、功能都有一定程度的差异，但是都实现了书中定义的数据库管理系统的主要功能，因此都是成熟的数据库管理系统。
- 除了界面不同，功能位置不同之外，我还注意到不同的数据库系统的SQL语句有不同之处，比如mysql中就定义模式时无法运行```AUTHORIZATION root```语句，*如下图*：
    ![](resources/1.png)
  有很多语句好像都不太一样，网上搜的很多SQL语句金仓都运行不了，汗。。。

## 四、实验心得
感觉自己目前对于数据库管理系统的了解还停留在数据定义、数据操纵层面，还没有深入到事务管理、运行管理、维护等复杂功能的程度，还需继续学习。