// =====================================================================================
// 
//       Filename:  grammar.h
//
//    Description:  语法分析器-算符优先分析法头文件
//
//        Version:  1.0
//        Created:  2014年05月06日 08时40分35秒
//       Revision:  none
//       Compiler:  g++
//
//         Author:  ZhangRui, v.me-zr@live.cn
//        Company:  Class 1107 of Computer Science and Technology
// 
// =====================================================================================

#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <iostream>
#include <stack>

// 终结符最大个数
#define MAX_CHAR 20 
// 产生式最大个数
#define N 10
// 产生式最大长度
#define M 30
// 测试句型最大长度
#define S_MAX 50

// 算符优先文法类
class OperatorGrammar {
	public:
		// 输入文法
		void input_grammar();
		// 判断是否为算符文法
		int legal_grammar();
		// 求文法中的终结符集合
		void char_terminator();
		// 求转换后的文法(将文法中的'|'去掉)
		void trans_grammar();
		// 求非终结符c的FIRSTVT集
		void firstvt();
		// 求非终结符c的LASTVT集
		void lastvt();
		// 文法优先关系表,是算符优先文法返回1，否则返回-1
		int table();
		// 处理输入句型
		int deal();

	private:
		// 算符优先关系矩阵
		char priority_array[MAX_CHAR][MAX_CHAR];
		// 文法终结符集合
		char label[MAX_CHAR];
		// 模拟符号栈
		char charstack[S_MAX];
		// 存储测试句型
		char sentence[S_MAX];
		// 测试句型长度
		int sl;
		// 存储文法规则
		char grammar[N][M];
		// 文法规则个数
		int r;
		// 存储转换后的文法
		char t_grammar[N][M];
		// 转换后的文法规则个数
		int tr;
		// 终结符个数
		int tchar_num;
		// 标志第i个非终结符对应的FIRSTVT集合
		int fflag[N][MAX_CHAR];
		// 标志第i个非终结符对应的LASTVT集合
		int lflag[N][MAX_CHAR];
		// 每个非终结符的 FIRSTVT集合
		char first_vt[N][MAX_CHAR];
		// 每个非终结符的LASTVT集合
		char last_vt[N][MAX_CHAR];

		// 判断c字符是否为终结符,若是返回下标
		int terminator(char c);
		// 返回非终结符的下标
		int nterminator(char c);
		// 打印符号栈s
		void print_stack(int j, int k, char *s);
};

#endif // GRAMMAR_H
