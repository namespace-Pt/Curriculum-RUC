# 数据库备份和恢复实验
姓名：**张配天**\
学号：**2018202180**
## 一、实验环境
- cpu：AMD4800HS
- gpu：RTX2060MaxQ
- RDBMS：Kingbase v7.0

## 二、实验结果及分析
### 备份
- 逻辑备份整个数据库
  - 备份为二进制文件
  ![](../resources/lab6/c1.png)
  - 备份为SQL文件
  ![](../resources/lab6/c1_.png)
- 逻辑备份多个表
  ![](../resources/lab6/c2.png)
- 逻辑备份数据库中指定的模式
  ![](../resources/lab6/c3.png)
- 利用`COPY TO`命令备份数据
  ![](../resources/lab6/1.png)
- 联机全系统物理备份
  ![](../resources/lab6/2.png)
  ![](../resources/lab6/3.png)
- 脱机全系统物理备份
  ![](../resources/lab6/c4.png)
- 联机物理增量备份
  ![](../resources/lab6/c5.png)
- 脱机物理增量备份
  ![](../resources/lab6/c6.png)
- 最终生成的所有备份文件
  ![](../resources/lab6/c8.png)
  
### 恢复
- 逻辑恢复整个数据库
  ![](../resources/lab6/d4.png)
- 逻辑恢复多个表
  ![](../resources/lab6/d5.png)
- 逻辑恢复指定模式
  ![](../resources/lab6/d6.png)
- 利用`COPY FROM`命令会辅助具
  ![](../resources/lab6/7.png)
- 脱机物理全系统恢复
  - 清空`data`目录后恢复
  
  ![](../resources/lab6/d1.png)
- 联机/脱机物理增量恢复
  ![](../resources/lab6/d2.png)
- 使用归档的日志和尾日志文件恢复数据到新位置
  ![](../resources/lab6/d3.png)
### 分析
- `log_archive_dest`中的路径是**双斜杠**
- `DBMS`可以自动识别全备份和增量备份, 应该是因为`CTL/sys_datafile`文件
- 个人认为逻辑备份方便一点, 但是逻辑备份只能保存快照, 要实现全系统备份只能物理, 而物理备份中联机的由于可以在查询分析器中进行, 因此还是比较方便的
- 恢复数据时的`warning`猜想是因为数据库中已经有这些表了, 遂删除表/模式后重新进行, 发现还是有`warning`, 具体原因尚不清楚
  
## 三、实验总结
- 学会了数据库的备份和恢复, 这是数据库的重要一部分, 可以改进大作业