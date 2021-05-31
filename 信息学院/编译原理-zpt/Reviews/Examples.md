## 短语
给定文法$$\begin{aligned}
    E&\rightarrow T|E+T\\
    T&\rightarrow F|T*F\\
    F&\rightarrow (E)|a\\
\end{aligned}$$
- 求$a_1*a_2+a_3$的短语, 直接短语, 句柄
  - $a_1$是短语, 且是直接短语；$a_2,a_3$同理；$a_1$在最左边, 所以是句柄
    - $F\rightarrow a_1$, 且$E\rightarrow F*a_2+a_3$
  - $a_1*a_2$是短语, 不是直接短语
    - $T\xrightarrow{\boldsymbol{*}} a_1*a_2$, 且$E\rightarrow T+a_3$
  - $a_2+a_3$不是短语
    - $E\rightarrow a_2+a_3$, 且$E\nrightarrow a_1*E$