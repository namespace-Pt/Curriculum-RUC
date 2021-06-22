# 运算
### Regular
正则语言在如下运算中是闭包:
- **Union**
- **Intersection**
- **Complement**
- **Difference**
- **Reversal**
- **Star**
- **Concatenation**
- **Homomorphism**
- 某个正则语言是否为空是可以判断的
- 两个正则语言是否相等是可以判断的
### CFG
上下文无关语言在如下运算中是闭包:
- **Substitution**
  - 将CFG的每个符号换成一个新的符号, 得到的语言
- **Union**
- **Reversal**
- **Star**
- **Concatenation**
- **Homomorphism**
- 某个CFG的个数是否有限是可以判断的
- 某个CFG是否包含某个string是可以判断的
- 两个CFG是否相等是不可判断的

# Turing machine
## 基础概念
- $$M = (Q,\Sigma,\Gamma,\delta,q_0,q_{\mathsf{acc}}, q_{\mathsf{rej}})$$
  - 均为有限的集合
- 有一个无限长的tape, 上面初始写了input string, 别的格子都是空。
- 图灵机有一个读/写头, 可以读取/修改tape上的内容, 并且左移/右移一个单位
- 当读写头碰到空格时, 则证明已经整个输入串已经识别完毕
- 图灵机的状态由**tape的内容**, **读写头的位置**, 和**当前状态**唯一决定, 这个三元组是图灵机的一个**configuration**
  - 记作$uqv$, 其中$u,v$为读写头分隔的两端string, 读写头位于$v$的第一个符号上, $q$是当前state

- $\lang X\rang$代表将$X$encode为一个string

### Terminology
- $L$是图灵可识别当且仅当其中所有的字符串都能被某一个图灵机接受。 即$L$ is **Turing-recognizable:** $\Leftrightarrow$ $\exist M\in \mathsf{TM}, \forall s \in L, M(s) = accept$
- $L'$是**co-Turing-recognizable:** $\Leftrightarrow$ $L' = \overline{L}, L\in \text{TM-recognizable}$
- $L$是**decidable:**$\Leftrightarrow \exist M\in \mathsf{TM}, \forall s\in \Sigma^*, M(s) = accept/reject$并且$\forall s \in L, M(s) = accept$

## 变种
### 多带图灵机
- **多条tape的图灵机和单tape图灵机等价**
### 不确定图灵机
- 如果有一系列连贯的configurations, 使得$C_1\Rightarrow C_2\Rightarrow\cdots\Rightarrow C_k$, 并且$C_1$为输入$w$后`NTM`的起始态, $C_k$为接受态, 那么`NTM`接受$w$
- 如果所有从$C_1$开始的最终所有configuration都是reject, 那么`NTM`拒绝$w$
- **不确定图灵机和确定图灵机等价**

## Recursively Enumerable
- A is TM-recognizable $\Leftrightarrow$ A is Recursively enumerable

### Enumerator
- 本质上就是一个图灵机附带了一个打印机, 会把所有图灵机接受的字符串打印出来, 但是字符串之间**没有顺序**并且**可能重复**

## Decidability
### 总体
$$\text{regular exp} \in \text{context free} \in \text{decidable} \in \text{recursively enumerable} \in \text{all}$$

### 例子
#### Decidable
- 判断一个DFA/NFA是否能够识别某一个字符串$$A_{*FA} = \{\lang A,w\rang | \text{ $A$ is a DFA/NFA, $w$ is a string}\}$$
- 判断一个DFA/NFA是否为空$$E_{*FA} = \{\lang A\rang|\text{ $A$ is a DFA and $L(A) = \emptyset$ } \}$$
  - 判断两个DFA/NFA是否相同$\implies$判断两个DFA的异或是否为空$$EQ_{*FA} = \{\lang A,B\rang|\text{ $(L(A)\cup \bar{L(B)})\cap(\bar{L(A)} \cup L(B)) = \emptyset$ } \}$$

- 判断一个CFG/PDA能否识别某一个字符串 (**因为CFG可能生成无穷多个字符串, 因此可能loop forever, 做一个转换**)$\implies$ 判断其Chomsky Form能否识别某一个字符串(**只用遍历$2n-1$个可能的字符串**)
- 判断一个CFG/PDA是否为空$$E_{CFG}= \{\lang A\rang|\text{ $A$ is a CFG and $L(A) = \emptyset$ } \}$$

#### Undecidable
- 判断一个图灵机TM能否接受/拒绝一个字符串$$L = \{\lang A,w\rang | \text{ $A$ is a TM, $w$ is a string and $A(w) = accept/reject$}\}$$
  - 如果$A(w) = loop$那么 $L$ 就会loop
  - 假设存在一个图灵机$M$能够决定$L$, 那么在$M$的基础上设计$N$, $N$的输入是一个图灵机的编码$\lang R\rang$, 然后运行$R(\lang R\rang)$, 在$R$接受$\lang R\rang$时拒绝, 在$R$拒绝$R$时接受, 那么$$N(\lang M\rang) = \overline{M(\lang M\rang)}= \begin{cases}
    reject&\text{$M$接受$\lang M\rang$}\\
    accept&\text{$M$拒绝$\lang M\rang$}
  \end{cases}$$产生矛盾, 因此不存在$M$
- 判断一个图灵机是否为空
- 判断两个图灵机识别的语言是否相同
- 判断一个CFG是否生成$\Sigma^*$
- 判断两个CFG是否相等

#### Unecognizable
- $\overline{A_{TM}}$ 是unrecognizable的
- $EQ_{TM}$ 是unrecognizable的, $\overline{EQ_{TM}}$ 也是unrecognizable的
### 充要条件
**$L$ is decidable $\Leftrightarrow$ $L$ is TM- and co-TM- recognizable**

### Rice's Theory
对于一种图灵机的description语言$P$, 如果
- $P \ne \emptyset$ 并且 $P\ne \Sigma^*$
- $\forall M_1,M_2\in \mathsf{TM}, L(M_1) = L(M_2), \lang M_1\rang\in P \iff \lang M_2\rang\in P $

那么$P$是**undecidable**的
## Reducibility
### 定义
$$\text{$A$ reduces to $B$, then the solution for $B$ can be used to solve $A$.}$$

### 性质
- $$A\le_m B\implies \overline{A}\le_m \overline{B}$$

### Computation History
一个$M\in \mathsf{TM}$
- 对于input $x$, 一个有限的连续的configurations的序列从$C_0$到$C_k$, 其中$C_k$为accept或者reject
- 如果$M(x)=loop$, 那么$x$没有computation history

#### Linear Bounded Automata
- 只拥有有限长的纸带的TM
- $A_{*FA},A_{CFG},E_{*FA},E_{CFG}$其实都是LBA
##### Decidable
- 假设LBA有$q$个states, 纸带长$n$, 纸带的字母表共有$g$个字符, 最多有$$q*n*n^g$$个不同的configuration
- $$A_{LBA} = \{\lang A,w\rang | \text{ $A$ is a LBA, $w$ is a string}\} \text{ is decidable}$$
  - 由于$LBA$只有有限个configuration, 那么一旦某一个输入运行步数超过上限还没有halt, 那么一定进入loop, 则可以将loop分辨出来。