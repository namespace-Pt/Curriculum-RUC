[TOC]
# 词法分析
## 正规定义式
$$\begin{aligned}
  d_1&\rightarrow r_1\\
  d_2&\rightarrow r_2\\
  &\cdots\\
  d_n&\rightarrow r_n
\end{aligned}$$
  - $r_i$中不包含$d_i,d_{i+1},\cdots,d_n$
### E.g.
16进制数的正规定义式为$$\begin{aligned}
\mathsf{digit}&\rightarrow 0|1|\cdots|9|A|B|\cdots|F\\
\mathsf{hex}&\rightarrow 0\mathrm{x}\mathsf{digit}(\mathsf{digit})^*
\end{aligned}$$
## 正规式$\rightarrow DFA$
### E.g.
- ppt chapter 2-3 p21
- ppt chapter 2-3 p66
## 正规文法$\rightarrow DFA$
- 对于**右线性文法**$G = (V_N, V_T, P, S)$, 设$|V_N| = k$, 则构造的DFA一共有$k+1$个状态, 其中$k$个状态对应$\forall v\in V_N$, 剩下一个$F$是**终止态**, $S$是**初态**
  - 对于$A\rightarrow aB\in P$, 有$A\xrightarrow{a}B$
  - 对于$A\rightarrow a\in P$, 有$A\xrightarrow{a}F$

- 对于**左线性文法**$G = (V_N, V_T, P, S)$, 设$|V_N| = k$, 则构造的DFA一共有$k+1$个状态, 其中$k$个状态对应$\forall v\in V_N$, 剩下一个$R$是**初态**, $S$是**终止态**
  - 对于$A\rightarrow Ba\in P$, 有$B\xrightarrow{a}A$
  - 对于$A\rightarrow a\in P$, 有$R\xrightarrow{a} A$

## $DFA\rightarrow$正规文法
对于**DFA** $M=(V_N, \Sigma, f, S, F)$
- 转换为右线性文法
  - 如果初态符号$S\notin F$, 那么有正规文法$G=(\Sigma,V_N,S,P)$, 对于$A\xrightarrow{a}B, a\in \Sigma$且$A,B\in V_N$
    - 若$B\notin F$, 则有$A\rightarrow aB\in P$
    - 若$B\in F$, 则有$A\rightarrow aB\mid a\in P$
  - 如果初态符号$S\in F$, 那么有正规文法$G=(\Sigma,V_N\cup \{S_0\},S_0,P)$, 对于$A\xrightarrow{a}B$且$a\in \Sigma \And A,B\in V_N$
    - 若$B\notin F$, 则有$A\rightarrow aB\in P$
    - 若$B\in F$, 则有$A\rightarrow aB\mid a\in P$
    - **手动添加产生式$S_0\rightarrow S\mid \epsilon \in P$**

- 转换为左线性文法
  - 如果初态符号$S\notin F$, 那么有正规文法$G=(\Sigma,V_N,S,P)$, 对于$A\xrightarrow{a}B, a\in \Sigma$且$A,B\in V_N$
    - 若$B\notin F$, 则有$B\rightarrow Aa\in P$
    - 若$B\in F$, 则有$B\rightarrow Aa\mid a$
  - 如果初态符号$S\in F$, 那么有正规文法$G=(\Sigma,V_N\cup \{S_0\},S_0,P)$, 对于$A\xrightarrow{a}B$且$a\in \Sigma \And A,B\in V_N$
    - 若$B\notin F$, 则有$B\rightarrow Aa\in P$
    - 若$B\in F$, 则有$B\rightarrow Aa\mid a\in P$
    - **手动添加产生式$S_0\rightarrow S\mid \epsilon \in P$**

# 语法分析
## 消除左递归
1. 改写文法为$\epsilon$**无关文法**
2. 将改写后的文法中的所有非终结符排列好, 记为$A_1,\cdots, A_n$
3.
$$\begin{aligned}
\mathrm{for}\ i\ \mathrm{in}\ &\mathrm{range}(1,n):&\\
&\mathrm{for}\ j\ \mathrm{in}\ \mathrm{range}(1,i-1):&\\
\end{aligned}$$
- 将$A_i \rightarrow A_j P$改写为:
  - $A_i \rightarrow a_1 P | a_2 P | ... | a_n P$
  - 其中 $A_j \rightarrow a_1 | a_2 | ... | a_n$
  - **即将$A_j$的所有产生式代入到$A_i$中**
- 最好把开始符$S$放在末尾


4. 假设代入后$A_i$的全部产生式为$$A\implies Aa_1|Aa_2|\cdots|Aa_n|\beta_1|\beta_2|\cdots|\beta_m$$其中$\beta_j$不以$A$开头, 则消除**直接左递归**后, 有$$\begin{aligned}
A&\implies \beta_1A'|\beta_2A'|\cdots|\beta_mA'\\
A'&\implies a_1A'|a_2A'|\cdots|a_nA'|\boldsymbol{\epsilon}
\end{aligned}$$
4. **去除所有无法从开始状态推导出的非终结符的产生式**

## 基本概念
$G$是一个文法, $S$是开始符号
- **句型**
  - $S\xRightarrow{*} a$, 则$a$为句型
- **句子**
  - 仅包含终结符的句型是**句子**
- **最左推导**
  - 每次直接推导最左边的**非终结符**
- **最右推导**
  - 每次直接推导最右边的**非终结符**
- **短语**
  -  $\alpha\beta\gamma$是$G$的一个句型, 若有$S\xRightarrow{*}\alpha A\gamma$且$A\xRightarrow{+}\beta$, 则$\beta$是关于$A$的, 句型$\alpha\beta\gamma$的短语
- **直接短语**
  - $\alpha\beta\gamma$是$G$的一个句型, 若有$S\xRightarrow{*}\alpha A\gamma$且$A\xRightarrow{} \beta$, 则$\beta$是关于$A$的, 句型$\alpha\beta\gamma$的直接短语
- **句柄**
  - 最左**直接短语**
- **活前缀**
  - 一个句子的前缀, 该前缀不包含任何句柄之后的符号

### S-文法
- 每个产生式右部第一个符号为终结符
- 若某个非终结符有多个产生式, 则其每个产生式的右部第一个符号彼此不同

### 规范归约
$a$是文法$G$的一个句子, 序列$a_n,a_{n-1},\cdots,a_0$是规范归约当且仅当:
- $a_n = a$
- $a_0 = S$
- 对任何$0<i\le n$, $a_{i-1}$是从$a_i$把句柄替换为相应产生式的左部符号得到的
- 是最右推导的逆过程

## LL(1)
- S-文法的一般化, 能够写确定的从上到下的语法分析器
- L: 从左向右读
- L: 最左推导
- 1: 仅需超前搜索一个字符

### 首符号集First
- $\mathsf{First}(A) = \{a\mid A\xrightarrow{*}a\cdots,\ a\in V_T\}$
  - 如果$A\xrightarrow{*}\epsilon$, 则$\epsilon\in\mathsf{First}(A)$
- 求$\mathsf{First}(X)$的算法
  - 若$X\in V_T$, 则$\mathsf{First}(X) = \{X\}$
  - 若$X\in V_N$
    - 有产生式$X\rightarrow a\cdots,\ a\in V_T$, 则将$a$加入$\mathsf{First}(X)$
    - 有产生式$X\rightarrow Y\cdots,\ Y\in V_N$, 则将$\ \forall b\in \mathsf{First}(Y),\ b\ne \epsilon$都加入$\mathsf{First}(X)$
    - 有产生式$X\rightarrow Y_1Y_2\cdots Y_n\cdots,\ Y_i \in V_N$, 且$Y_{1\sim n-1}\xrightarrow{*}\epsilon$都成立, 则将$\forall b\in \mathsf{First}(Y_i),\ b\ne \epsilon$都加入$\mathsf{First}(X)$；特别地, 如果$Y_{1\sim n}\xrightarrow{*}\epsilon$, 则将$\epsilon$加入$\mathsf{First}(X)$
### 后随符号集Follow
- $\mathsf{Follow}(A) = \{a\mid \xrightarrow{*}\cdots Aa\cdots,\ a\in V_T\}$
  - 如果$S\xrightarrow{*} \cdots A$, 则规定$\#\in \mathsf{Follow}(A)$
- 求$\mathsf{Follow}(X)$的算法
  - 若$X$为开始符号, 则将$\#$加入$\mathsf{Follow}(X)$
  - 若有产生式$A\rightarrow PXQ$, 则把$\forall a\in \mathsf{First}(Q),\ a\ne\epsilon$加入$\mathsf{Follow}(X)$
  - 若有产生式$A\rightarrow PX$, 或者有产生式$A\rightarrow PXQ, Q\rightarrow\epsilon$, 则把$\forall a\in \mathsf{Follow}(A)$加入$\mathsf{Follow}(X)$
### 引导符号集Select
- 对于产生式$A\rightarrow PQ$, 记$\phi(A,PQ)$为其**引导符号集**, 则有$$\phi(A\rightarrow PQ) = \{a\mid a\in \mathsf{First}(P)\ \mathrm{or}\ A\rightarrow\epsilon, a\in \mathsf{Follow}(A)\}$$
- 求$\mathsf{Select}(A\rightarrow X)$的算法
  - 若$X\ne \epsilon$, 且$X\nrightarrow\epsilon$, 则$\mathsf{Select}(A\rightarrow X) = \mathsf{First}(X)$
  - 若$X\ne \epsilon$, 且$X\xrightarrow{*}\epsilon$, 则$\mathsf{Select}(A\rightarrow X) = \mathsf{First}(X)\cup\mathsf{Follow}(A)$
  - 若$X= \epsilon$, 则$\mathsf{Select}(A\rightarrow X) = \mathsf{Follow}(A)$
### E.g.
ppt chapter3 p85
### LL(1)文法定义
对于每一个非终结符, 如果其候选式多余一个, 则这些候选式的引导符号集彼此不相交

## 算符优先文法
- 由算符优先文法生成的句型不包含两个非终结符并列的情况, 即不包含$\cdots RS\cdots\  (R,S\in V_N)$的句型
### 判断优先级
如果文法不包含$P\rightarrow\epsilon$的产生式, 则对任何一对终结符$(a,b)$, 有
- $a=b$, 当且仅当有$P\rightarrow\cdots ab\cdots|P\rightarrow\cdots aQb\cdots$的产生式
- $a<b$, 当且仅当有$P\rightarrow\cdots aR\cdots$且$R\rightarrow b\cdots|R\rightarrow Qb\cdots$的产生式
- $a>b$, 当且仅当有$P\rightarrow\cdots Rb\cdots$且$R\rightarrow \cdots a|R\rightarrow \cdots aQ$的产生式

### 首符号集FirstVT
- $\mathsf{FirstVT}(P) = \{a\mid P\xrightarrow{*}a\cdots | P\xrightarrow{*}Qa\cdots,\ a\in V_T, P,Q\in V_N\}$
- 求$\mathsf{FirstVT}(P)$的算法
  - 如果有产生式$P\rightarrow a\cdots|P\rightarrow Qa\cdots$, 则$a\in \mathsf{FirstVT}(P)$
  - 如果$a\in \mathsf{FirstVT}(Q)$且有产生式$P\rightarrow Q\cdots$, 则$a\in \mathsf{FirstVT}(P)$
### 尾符号集LastVT
- $\mathsf{LastVT}(P) = \{a\mid P\xrightarrow{*}\cdots a | P\xrightarrow{*}\cdots aQ,\ a\in V_T, P,Q\in V_N\}$
- 求$\mathsf{LastVT}(P)$的算法
  - 如果有产生式$P\rightarrow \cdots a|P\rightarrow \cdots aQ$, 则$a\in \mathsf{LastVT}(P)$
  - 如果$a\in \mathsf{LastVT}(Q)$且有产生式$P\rightarrow \cdots Q$, 则$a\in \mathsf{LastVT}(P)$
### 计算优先级
- 对于形如$P\rightarrow \cdots aQ\cdots$的产生式, $\forall b\in \mathsf{FirstVT}(Q)$, 有$a<b$
- 对于形如$P\rightarrow \cdots Qb\cdots$的产生式, $\forall a\in \mathsf{LastVT}(Q)$, 有$a>b$
- 对于形如$P\rightarrow \cdots aQb\cdots$的产生式, 有$a=b$
### 定义
如果某个文法的任何一对终结符$(a,b)$只满足$a<b, a=b, a>b$中的某一关系, 则其为算符优先算法

### 素短语
- 至少包含一个终结符, 除它自身之外不再包含任何更小的素短语
- 句型最左边的素短语称为**最左素短语**
#### 重要证明
- 一个算符优先文法的句型有形式:$$N_1a_1N_2a_2\cdots N_na_nN_{n+1}a_{n+1}$$
- 该句型的最左素短语应满足$$N_ja_jN_{j+1}a_{j+1}\cdots N_ia_iN_{i+1}$$其中
  - $a_{j-1} < a_j$
  - $a_j = a_{j+1} = \cdots = a_i$
  - $a_i>a_{i+1}$

## LR(0)
### 初始化
- 将文法转换为“项目”, 即文法的右边的每个符号之间点一个点, 作为一个项目
- 将项目转换为DFA
  - 所有项目都是NFA的节点, 初始态为$S\rightarrow .\mathrm{xxx}$
  - 如果项目1是项目2的直接后继, 比如$A\rightarrow aB$中$p_1 = A\rightarrow \alpha.\beta$和$p_2 = A\rightarrow \alpha\beta.$, 那么就建立边$p_1\xrightarrow{\beta} p_2$
  - 同时, 如果$\beta$是**非终结符**, 那么还要在$p_1$和所有形如$\beta\rightarrow .\mathrm{xxx}$的项目之间连一条$\epsilon$边
  - 将NFA确定化
### 移进和归约
- **归约项目**:
  - 圆点在最右端的项目
- **接受项目**:
  - 特殊的归约项目, 其左部必须是开始符号$S$
- **移进项目**:
  - 形如$A\rightarrow a.b\gamma$的项目, 其中$b$是终结符
- **待约项目**:
  - 形如$A\rightarrow a.B\gamma$的项目, 其中$B$是非终结符

#### 项目集规范族
- $\mathsf{Closure}(I)$是项目集
- 若$A\rightarrow a.B\beta\in \mathsf{Closure}(I)$, 那么所有$B$的产生式中形如$B\rightarrow .\gamma$的项目也属于$\mathcal{I}$
- 重复执行上一步, 直到$\mathsf{Closure}(I)$不再增大

#### 状态转换函数
- $$\mathsf{Go}(\mathcal{I},X) = \mathsf{Closure}(\mathcal{J})$$其中$$\mathcal{J} = \left\{A\rightarrow aX\mathbf{.}\beta\mid A\rightarrow a\mathbf{.}X\beta \in \mathcal{I}\right\}$$
- 注意$X$其实相当于一个变量, 可以是终结符/非终结符
- 相当于找到$\mathcal{I}$中的一个产生式, 包含$aX\beta$, 然后将$aX.\beta$和$\beta\rightarrow .\gamma$加入到$\mathsf{Go}(\mathcal{I},X)$中

#### 构造项目集规范族
初始化 $\mathcal{C} = \left\{\mathsf{Closure}(S'\rightarrow .\mathrm{xxx})\right\}$
- $\forall I \in \mathcal{C}, \forall X\in G'$($G$的拓广文法), 如果 $\mathsf{{Go}(I,X)}\ne \emptyset$ 并且 $\mathsf{{Go}(I,X)}\notin \mathcal{C}$, 则将其加入$\mathcal{C}$
- 重复上述直到$\mathcal{C}$不再增大

#### 活前缀的有效项目集
- 项目$A\rightarrow \beta_1.\beta_2$对活前缀$a\beta_1$是有效的当且仅当存在规范推导$S'\xrightarrow{*}aAw\xrightarrow{*}a\beta_1\beta_2w$
从构造出的DFA的初态$S'$出发, 读出$\gamma$后抵达的状态(本质上是一个项目集)
- **在任何时候, 分析站中的活前缀$X_1X_2\cdots X_m$的有效项目集都是站定状态$S_m$所代表的集合**
- 怎么理解呢？
  - 其实在不破坏规范归约的条件下得出哪些式子能导出这个活前缀

