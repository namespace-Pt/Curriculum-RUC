## File Structure
```
.
│  grammer.cpp                      // 实现文法类和文法集类
│  grammer.h                        // 定义文法类和文法集类
│  lexer.cpp                        // 包装lexer
│  main.cpp                         // 主函数
│  main.exe
│  parser.cpp                       // 实现语法分析器
│  parser.h                         // 定义语法分析器类
│  README.md
│  symbol.cpp                       // 实现符号类和符号集类
│  symbol.h                         // 定义符号类和符号集类
│
├─data
│      grammers.txt                 // 给定文法
│      KeyWords.txt                 // 提前定义关键词列表
│      lex_result.out               // 词法分析器的输出结果, 中间文件
│      Operators.txt                // 提前定义的算符
│      Separators.txt               // 提前定义的分隔符
│      test.in                      // 输入字符串
│
├─docs
│      report.pdf                   // 实验报告
```

This resource really helps me a lot. It lets me know Jacobian and provides helpful insights about backpropagation. However, one thing still perplex me is the local gradient $\frac{\partial A}{\partial C}$ for the following equation: $A = B \circ (C < 0.2)$ where $\circ$ denotes Hadamard Multiplication and $A,B,C\in \mathbb{R}^{n\times m}$. I've checked some answers suggesting the equivalence between Hadamard Multiplication and Diagonal Matrix Multiplication, however they are related with vector hadamard production, and it's not what I want