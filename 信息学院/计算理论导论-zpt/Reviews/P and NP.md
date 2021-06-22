[TOC]
# P and NP
## 变种图灵机的复杂度
- 一个$t(n)$的**多带图灵机**等价于一个$O(t(n)^2)$的**单带图灵机**
- 一个$t(n)$的**非确定图灵机**等价于一个$2^{O(t(n))}$的**确定图灵机**

## P class
$$\mathsf{P} = \bigcup_k\mathsf{TIME}(n^k)$$
- $\mathsf{TIME}(n^k)$代表能在$O(n^k)$内被一个**确定单带图灵机**计算得到结果的所有语言
  - 多带图灵机和单带相差的只是一个多项式, 所以多带其实也可以

- $A_{CFG}\in \mathsf{P}, \forall CFG\in \mathsf{P}$

## NP class
$$\mathsf{NP} = \bigcup_k\mathsf{NTIME}(n^k)\Leftrightarrow \mathsf{NP} = \{x\mid\exist y, |y|< |x|^k, \lang x, y\rang \in R\in \mathsf{P}\}$$
- $\mathsf{NTIME}(n^k)$代表能在$O(n^k)$内被一个**非确定单带图灵机**计算得到结果的所有语言
- $y$代表一个**证书certificate**
- **无法证明$L\in NP$并且$L\notin P$**
### Verifier
$$A = \{w\mid V \text{ accepts } \lang w,c\rang \text{ for some string $c$}\}$$
- $V$ 是verifier
- $\mathsf{NP}$就是拥有多项式时间verifier的语言集合
  - $T(V(c)) = O(|c|^k)$

因此, 要证明一个语言$L\in \mathsf{NP}$, 只需要构造一个多项式时间的verifier去识别$L$的一个certificate

### Examples
#### Clique
$k-clique$是一个无向图的**有$k$个节点的全连接子图**, 即该子图上每个节点之间连边
##### Problem
$$CLIQUE = \{\lang G,k\rang \mid\text{$G$ is an undirected graph with $k$-clique}\}$$
##### Proof
Give a verifier($\mathsf{TM}$) for $CLIQUE$
- 书p320

### NP-Complete
一些特殊的$L\in\mathsf{NP}$, 一旦任何$l\in L$找到了多项式时间可解的算法, 那么所有$\mathsf{NP}$问题都能用这个算法解决, 即
$$\begin{aligned}
L\in \mathsf{NPComplete}&\Leftrightarrow \begin{cases}
L\in \mathsf{NP}\\
\forall A\in \mathsf{NP}, A\le_p L
\end{cases}
\end{aligned}$$
#### Poly-time Reduction
多项式时间可计算的函数$f:\Sigma^*\rightarrow \Sigma^*$, 即有一个$M\in \mathsf{TM}$, 对于任何输入$w\in \Sigma^*$, $M$停机时纸带上得到$f(w)$, 并且$M\in \mathsf{TIME}(n^k)$

##### Theorem
- $$A\le_p B, B\in \mathsf{P}\implies A\in \mathsf{P}$$
- $$A\le_p B, A\in \mathsf{NPComplete}\implies B\in \mathsf{NPComplete}$$

#### SAT
一个布尔表达式, 变量可以取$0/1$, 变量之间通过$\wedge/\lor$连接, 其是satisfiable的代表有一组变量的取值, 使得整个表达式最终值为$1$
##### Problem
$$SAT = \{\lang\phi\rang \mid\phi\text{ is a satisfiable Boolean formula} \}$$
##### Proof
- 证明$SAT\in\mathsf{NP}$
- 证明$\forall A\in\mathsf{NP}, A\le_pSAT$
#### 3SAT
是一种SAT表达式, 称为$\mathbf{3cnf}$, 形如
$(x_1\lor x_2\lor x_3)\wedge(x_4\lor x_5\lor x_6)\wedge\cdots$
- 每个clause(即括号中的内容)包含三个变量
- clause中的变量用$\lor$连接
- clause之间用$\wedge$连接
##### Problem
$$3SAT = \{\lang\phi\rang \mid\phi\text{ is a satisfiable 3cnf formula} \}$$

#### Indipendent Set
给定一个图$G(V,E)$, Ind. Set是其中的一个顶点集$V'\in V$, 其中$\forall u,w\in V', (u,w)\notin E$

##### Problem
将寻找一个图$G$的最大Ind. Set转化为判定一个图是否含有至少$k$个顶点的Ind. Set:
$$IS = \{\lang G,k\rang\mid \text{$G$ has an IS of $size\ge k$}\}$$

##### Proof
- $\in \mathsf{NP}$
- **poly-time computable reduction**
  - 对于3cnf的每一个clause, 构造一个三角形, 顶点为clause中每个变量(带非), 不同clause中如果有相同的变量, 并且一个有非, 一个没有, 则将两者相连, 比如$x,\lnot x$ 如果出现在两个clause中, 则将两者对应的顶点相连

- **Yes to Yes**
  - 如果3cnf可满足, 那么每个clause中至少有一个变量取$x=1(True)$, 那么别的clause中, **不可能**有$\lnot x=1$
  - 因此取每个clause中取值为1的变量, 这些变量之间不可能存在边
  - 则构成$size=m$的IS, 其中$m$为clause的个数
  - 令$k=m$, 得到结果
- **No to No**
  - 如果3cnf不满足, 那么无论什么取值都不能让布尔表达式最终值为$1$。
  - 注意到IS只能在每个clause中取一个顶点, 否则就无法构成IS
  - 如果在每个clause中取某个顶点, 最终构成了$size=m$的IS, 那么把这些顶点对应的变量取$1$, 则可以满足布尔表达式, 矛盾

#### Vertex Cover
给定一个图$G(V,E)$, 一个**vertex cover** 是G的顶点集的子集$V'\in V$, 使得$\forall (u,v)\in E, u\in V' \text{ or } v\in V'$, 即每个边都要有一个端点在$V'$内。

记$|V| = n$

##### Problem
将寻找一个图的**最小顶点覆盖**转化为判定一个图是否包含至多有$k$个顶点的顶点覆盖

$$VC = \{\lang G,k\rang\mid G\text{ has a VC of size} \le k\}$$

##### Proof
- $\in \mathsf{NP}$
- **poly-time computable reduction**
  - reduce from $IS$, $IS\le_p VC$
    - 对于$G(V,E)$, 如果$V'\in V, |V'| = k$是一个$size=k$的**独立顶点集**, 那么$V-V'$是一个$size=n-k$的**顶点覆盖**
      - 否则, 如果$V-V'$不是顶点覆盖, 则$\exist e(v_1,v_2)\in E, v_1,v_2\notin V-V'$, 则一定有$v_1,v_2\in V'$, 然而独立顶点集中不能存在边$e(v_1,v_2)$, 矛盾
    - 同理, 对于$G(V,E)$, 如果$V'\in V, |V'| = k$是一个$size=k$的**顶点覆盖**, 那么$V-V'$是一个$size=n-k$的**独立顶点集**
      - 否则, 如果$V-V'$不是独立顶点集, 则$\exist e(v_1,v_2)\in E, v_1,v_2\in V-V'$, 即一定有$v_1,v_2\notin V'$, 然而顶点覆盖中必须至少包含$e$的一个端点, 矛盾
  - 因此, 对于输入$\lang G,k\rang$, 永远可以输出$\lang G,n-k\rang$

- **Yes to Yes**
  - 如果存在一个$size\ge k$的IS, 那么一定存在一个$size\le n-k$的VC
- **No to No**
  - 如果不存在$size\ge k$的IS, 即所有IS的$size=s < k$, 那么最小VC的$size= n-s> n-k$
- 需要注意, 任何一个图$G$一定有$size=1$的独立顶点集, 并且一定有$size=n-1$的顶点覆盖

#### Clique
定义[同上](#clique)
##### Problem
将寻找一个图的**最大团**的问题转化为判定一个图是否包含大小至少为$k$的团
$$CLIQUE = \{\lang G,k\rang\mid G\text{ has a clique of size}\ge k\}$$
##### Proof
- $\in \mathsf{NP}$
- **poly-time computable reduction**
  - reduce from $IS$, $IS\le_p CLIQUE$
    - 定义**对偶图**
      - 给定一个图$G(V,E)$, 其对偶图$G'(V,E')$: $\forall e\in E, e\notin E';\forall e\notin E, e\in E'$
      - 即一个图保持顶点不变, 然后把原来的边都去掉, 把原来没练边的顶点之间都连边
    - 对于一个图$G(V,E)$, 其独立顶点集为$V', |V'| = k$, $G$的对偶图$G'(V,E')$中, 有子图$G''(V',E'')$, 其中$\forall e(v_1,v_2)\in E'$, 如果$v_1,v_2\in V'$, 则$e\in E''$, $G''$中每个顶点之间都有边
    - 否则, $V'\notin IS$
  - 因此, 对于输入$\lang G,k\rang$, 永远可以输出$\lang G'',k\rang$
- **Yes to Yes**
  - 如果存在$IS$ of size$\ge k$,那么一定有$CLIQUE$ of size $\ge k$
- **No to No**
  - 如果存在$CLIQUE$ of size $\ge k$, 那么一定有$IS$ of size$\ge k$

#### Hamilton Path
给定一个**有向图**$G(V,E)$, 一条哈密尔顿路径为一条**穿过所有顶点一次且仅一次**的路径。

##### Problem
将寻找一个图的哈密尔顿路径转化为判定一个图是否包含从$s$到$t$的哈密尔顿路径

$$HAMILTON = \{\lang G,s,t\rang\mid G\text{ has a Hamilton Path from $s$ to $t$}\}$$

##### Proof
- $\in \mathsf{NP}$
- **poly-time computable reduction**
  - reduce from $3SAT$, $3SAT\le_p HAMILTON$
  - 