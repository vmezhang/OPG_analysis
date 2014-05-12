// =====================================================================================
// 
//       Filename:  opg_analysis.cpp
//
//    Description:  算符优先分析法
//
//        Version:  1.0
//        Created:  2014年05月07日 21时14分58秒
//       Revision:  none
//       Compiler:  g++
//
//         Author:  ZhangRui, v.me-zr@live.cn
//        Company:  Class 1107 of Computer Science and Technology
// 
// =====================================================================================

#include "grammar.h"

int main(int argc, char *argv[])
{
	OperatorGrammar opg;
	opg.input_grammar();
	if (opg.legal_grammar() == -1) {
		return -1;
	}
	opg.char_terminator();
	opg.trans_grammar();
	opg.firstvt();
	opg.lastvt();
	if(opg.table() == -1) {
		return -1;
	}
	std::cout << "需要进行句型分析请输入y\n";
	char c;
	std::cin >> c;
	if (c == 'y') {
		opg.deal();
	}

	return 0;
}

