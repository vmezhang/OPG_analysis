// =====================================================================================
// 
//       Filename:  grammar.cpp
//
//    Description:  
//
//        Version:  1.0
//        Created:  2014年05月06日 09时34分14秒
//       Revision:  none
//       Compiler:  g++
//
//         Author:  ZhangRui, v.me-zr@live.cn
//        Company:  Class 1107 of Computer Science and Technology
// 
// =====================================================================================

#include "grammar.h"

struct stack_char {
	char a, b, c;
};

void OperatorGrammar::input_grammar()
{
	std::cout << "请输入文法G的规则数" << std::endl;
	std::cin >> r;
	std::cout << "\n请输入文法G[s],形如(S->aS|bA)" << std::endl;
	for (int i = 0; i < r; i++) {
		// 存储文法规则
		std::cin >> grammar[i];
		// 用first_vt[i][0]与last_vt[i][0]
		// 分别表示第i个非终结符的
		// FIRSTVT与LASTVT集合的元素个数
		first_vt[i][0] = 0;
		last_vt[i][0] = 0;
	}
}

int OperatorGrammar::legal_grammar()
{
	for (int i = 0; i < r; i++) {
		// 首先判断非终结符是否合法
		if (grammar[i][0] < 'A' || grammar[i][0]> 'Z') {
			std::cout << "不是算符文法!" << std::endl;
			return -1;
		}
		// 进而判断产生式右边是否合法
		for (int j = 3; grammar[i][j] != '\0'; j++) {
			// 不能相继出现两个非终结符
			if (grammar[i][j] >= 'A' && grammar[i][j] <= 'Z') {
				if (grammar[i][j + 1] >= 'A' && grammar[i][j + 1] <= 'Z') {
					std::cout << "不是算符文法!" << std::endl;
					return -1;
				}
			}
		}
	}
	std::cout << "该文法是算符文法" << std::endl;
	return 1;
}

void OperatorGrammar::char_terminator()
{
	int k = 0;
	// 设置标志位表示终结符集合中是否有重复出现的字符
	// 0 为未出现过，1为出现过
	int flag = 0;
	for (int i = 0; i < r; i++) {
		// 扫描产生式右部
		for (int j = 3; grammar[i][j] != '\0'; j++) {
			// 找出非大写字母和'|'的字符
			if ((grammar[i][j] < 'A' || grammar[i][j] > 'Z') && grammar[i][j] != '|') {
				// 检查label中是否出现过此终结符
				for (int n = 0; n < k; n++) {
					if (grammar[i][j] == label[n]) {
						flag = 1;
						break;
					}
				}
				// flag = 0表示未出现过该终结符,则加入label集合
				if (flag == 0) {
					label[k] = grammar[i][j];
					k++;
				}
				// 恢复flag = 0
				flag = 0;
			}
		}
	}
	label[k] = '#';
	label[k + 1] = '\0';
	tchar_num = k + 1;
	std::cout << "\n该文法的非终结符集为:\n" << label << std::endl;
}

int OperatorGrammar::terminator(char c)
{
	for (int i = 0; label[i] != '\0'; i++) {
		if (c == label[i]) {
			// 是终结符返回下标
			return i;
		}
	}
	// 不是终结符返回-1
	return -1;
}

int OperatorGrammar::nterminator(char c)
{
	for (int i = 0; i < r; i++) {
		if (c == grammar[i][0]) {
			return i;
		}
	}
	// 不是非终结符返回-1
	return -1;
}

void OperatorGrammar::trans_grammar()
{
	// m为产生式的个数,n为产生式的长度
	int m = 0, n = 0;
	for (int i = 0; i < r; i++) {
		n = 0;
		for (int j = 0; grammar[i][j] != '\0'; j++) {
			if (grammar[i][j] != '|') {
				t_grammar[m][n] = grammar[i][j];
				n++;
			}
			else {
				t_grammar[m][n] = '\0';
				// 将'|'之后内容赋值给新的产生式
				m++;
				n = 3;
				// 产生式的左部赋值
				t_grammar[m][0] = grammar[i][0];
				t_grammar[m][1] = grammar[i][1];
				t_grammar[m][2] = grammar[i][2];
			}
		}
		t_grammar[m][n] = '\0';
		m++;
	}
	// 产生式个数
	tr = m;
/*
	// 输出转换后的产生式
	std::cout << "\n转换后的产生式为:\n";
	for (int i = 0; i < tr; i++) {
		std::cout << t_grammar[i] << std::endl;
	}
*/
}

void OperatorGrammar::firstvt()
{
	// 标志非终结符下标
	int k = 0;
	// 标志终结符在label中的位置
	int t = -1;
	// 存储对应值为1的符号
	std::stack<stack_char> fstack;
	stack_char sc;
	stack_char stemp;

	// 初始化数组
	for (int i = 0; i < r; i++) {
		for (int j = 0; j < tchar_num - 1; j++) {
			fflag[i][j] = 0;
		}
	}

	// 找出直接对应的firstvt关系
	for (int i = 0; i < tr; i++) {
		// 检查是否需要给k + 1
		if (i > 0) {
			if (t_grammar[i][0] != t_grammar[i - 1][0]) {
				k++;
			}
		}
		for (int j = 3; t_grammar[i][j] != '\0'; j++) {
			// 若该字符为终结符则标志为对应label下标位置为1
			t = terminator(t_grammar[i][j]);
			if (t != -1) {
				fflag[k][t] = 1;
				break;
			}
		}
	}
	
	for (int i = 0; i < r; i++) {
		for (int j = 0; j < tchar_num - 1; j++) {
			if (fflag[i][j] == 1) {
				sc.a = grammar[i][0];
				sc.b = ',';
				sc.c = label[j];
				fstack.push(sc);
			}
		}
	}
	
	while (!fstack.empty()) {
		sc =  fstack.top();
		fstack.pop();
		for (int i = 0; i < tr; i++) {
			if (t_grammar[i][3] == sc.a) {
				for (int j = 0; j < r; j++) {
					if (t_grammar[i][0] == grammar[j][0]) {
						t = terminator(sc.c);
						if (t != -1) {
							if (fflag[j][t] == 0) {
								fflag[j][t] = 1;
								stemp.a = grammar[j][0];
								stemp.b = ',';
								stemp.c = label[t];
								fstack.push(stemp);
							}
						}
					}
				}
			}	
		}
	}

	std::cout << "\nFIRSTTV集合矩阵:\n";
	for (int i = 0; i < tchar_num - 1; i++) {
		std::cout << "  " << label[i];
	}
	std::cout << std::endl;

	for (int i = 0; i < r; i++) {
		std::cout << grammar[i][0] << " ";
		int l = 0;
		for (int j = 0; j < tchar_num - 1; j++) {
			std::cout << fflag[i][j] << "  ";
			if (fflag[i][j] == 1) {
				first_vt[i][l] = label[j];
				l++;
			}
		}
		first_vt[i][l] = '\0'; 
		std::cout << "\n";
	}

	// 输出firstvt集合
	for (int i = 0; i < r; i++) {
		std::cout << "FIRSTVT[" << grammar[i][0] << "]:";
		for (int j = 0; first_vt[i][j] != '\0'; j++) {
			std::cout << first_vt[i][j] << " ";
		}
		std::cout << "\n";
	}
}

void OperatorGrammar::lastvt()
{
	// 标志非终结符下标
	int k = 0;
	// 标志终结符在label中的位置
	int t = -1;
	// 存储对应值为1的符号
	std::stack<stack_char> lstack;
	stack_char sc;
	stack_char stemp;

	// 初始化数组
	for (int i = 0; i < r; i++) {
		for (int j = 0; j < tchar_num - 1; j++) {
			lflag[i][j] = 0;
		}
	}

	// 找出直接对应的lastvt关系
	for (int i = 0; i < tr; i++) {
		// 检查是否需要给k + 1
		if (i > 0) {
			if (t_grammar[i][0] != t_grammar[i - 1][0]) {
				k++;
			}
		}
		// 找到最后一个出现的终结符
		int j = 0;
		for (; t_grammar[i][j] != '\0'; j++) {
		}
		while (j >= 3) {

			// 若该字符为终结符则标志为对应label下标位置为1
			t = terminator(t_grammar[i][j]);
			if (t != -1) {
				lflag[k][t] = 1;
				break;
			}
			j--;			
		}
	}
	
	for (int i = 0; i < r; i++) {
		for (int j = 0; j < tchar_num - 1; j++) {
			if (lflag[i][j] == 1) {
				sc.a = grammar[i][0];
				sc.b = ',';
				sc.c = label[j];
				lstack.push(sc);
			}
		}
	}
	
	while (!lstack.empty()) {
		sc =  lstack.top();
		lstack.pop();
		for (int i = 0; i < tr; i++) {
			int q = 0;
			for (; t_grammar[i][q] != '\0'; q++) {
			}
			if (t_grammar[i][q - 1] == sc.a) {
				for (int j = 0; j < r; j++) {
					if (t_grammar[i][0] == grammar[j][0]) {
						t = terminator(sc.c);
						if (t != -1) {
							if (lflag[j][t] == 0) {
								lflag[j][t] = 1;
								stemp.a = grammar[j][0];
								stemp.b = ',';
								stemp.c = label[t];
								lstack.push(stemp);
							}
						}
					}
				}
			}	
		}
	}

	std::cout << "\nLASTVT集合矩阵:\n";
	for (int i = 0; i < tchar_num - 1; i++) {
		std::cout << "  " << label[i];
	}
	std::cout << std::endl;

	for (int i = 0; i < r; i++) {
		std::cout << grammar[i][0] << " ";
		int l = 0;
		for (int j = 0; j < tchar_num - 1; j++) {
			std::cout << lflag[i][j] << "  ";
			if (lflag[i][j] == 1) {
				last_vt[i][l] = label[j];
				l++;
			}
		}
		last_vt[i][l] = '\0'; 
		std::cout << "\n";
	}

	// 输出lastvt集合
	for (int i = 0; i < r; i++) {
		std::cout << "LASTVT[" << grammar[i][0] << "]:";
		for (int j = 0; last_vt[i][j] != '\0'; j++) {
			std::cout << last_vt[i][j] << " ";
		}
		std::cout << "\n";
	}	
}

int OperatorGrammar::table()
{
	// 初始化算符优先矩阵,全初始为无优先关系('0')
	for (int i = 0; i < tchar_num; i++) {
		for (int j = 0; j < tchar_num; j++) {
			priority_array[i][j] = '0';
		}
	}
	// 将$->#E#加入到t_grammar末尾
	t_grammar[tr][0] = '$';
	t_grammar[tr][1] = '-';
	t_grammar[tr][2] = '>';
	t_grammar[tr][3] = '#';
	t_grammar[tr][4] = grammar[0][0];
	t_grammar[tr][5] = '#';
	t_grammar[tr][6] = '\0';
	tr++;
	for (int i = 0; i < tr; i++) {
		for (int j = 3; t_grammar[i][j] != '\0'; j++) {
			// 终结符与非终结符的下标参数
			int t1 = -1, t2 = -1, t3 = -1, nt = -1;
			t1 = terminator(t_grammar[i][j]);
			if (t1 != -1) {
				// aa情况
				if (t_grammar[i][j + 1] != '\0') {
					t2 = terminator(t_grammar[i][j + 1]);
					if (t2 != -1) {
						if (priority_array[t1][t2] == '0' || priority_array[t1][t2] == '=') {
							priority_array[t1][t2] = '=';
						}
						else {
							std::cout << "优先关系出现冲突，该文法不是算符优先文法!" << std::endl;
							return -1;	
						}
					}
				}
				// aAa情况
				if (t_grammar[i][j + 1] != '\0' && t_grammar[i][j + 2] != '\0') {
					t2 = terminator(t_grammar[i][j + 2]);
					if (t2 != -1) {
						if (priority_array[t1][t2] == '0' || priority_array[t1][t2] == '=') {
							priority_array[t1][t2] = '=';
						}
						else {
							std::cout << "优先关系出现冲突，该文法不是算符优先文法!" << std::endl;
							return -1;	
						}
					}			
					
				}
				// aA情况
				if (t_grammar[i][j + 1] != '\0') {
					nt = nterminator(t_grammar[i][j + 1]);
					if (nt != -1) {
						for (int h = 0; first_vt[nt][h] != '\0'; h++) {
							t3 = terminator(first_vt[nt][h]);
							if (t3 != -1) {
								if (priority_array[t1][t3] == '0' || priority_array[t1][t3] == '<') {
									priority_array[t1][t3] = '<';
								}									
								else {
									std::cout << "优先关系出现冲突，该文法不是算符优先文法!" << std::endl;
									return -1;	
								}
							}
						}
					}
				}
			}

			// Aa情况
			else {
				// 接着出现终结符
				if (t_grammar[i][j + 1] != '\0') {
					t2 = terminator(t_grammar[i][j + 1]);
					if (t2 != -1) {
						// 求出nt
						nt = nterminator(t_grammar[i][j]);
						if (nt != -1) {
							for (int h = 0; last_vt[nt][h] != '\0'; h++) {
								t3 = terminator(last_vt[nt][h]);
								if (t3 != -1) {
									if (priority_array[t3][t2] == '0' || priority_array[t3][t2] == '>') {
										priority_array[t3][t2] = '>';
									}
									else {
										std::cout << "优先关系出现冲突，该文法不是算符优先文法!" << std::endl;
										return -1;	
									}								
								}
							}
						}
					}
				}
			}
		}
	}
	
	// 将$->#E#去掉
	tr--;

	// 打印优先关系矩阵
	std::cout << "\n算符优先关系矩阵：\n ";
	for (int i = 0; i < tchar_num; i++) {
		std::cout << " " << label[i];
	}
	std::cout << "\n";
	for (int i = 0; i < tchar_num; i++) {
		std::cout << label[i] << " ";
		for (int j = 0; j < tchar_num; j++) {
			std::cout << priority_array[i][j] << " ";
		}
		std::cout << "\n";
	}
	return 1;
}

void OperatorGrammar::print_stack(int j, int k, char *s)
{
	for (int i = j; i <= k; i++) {
		std::cout << s[i];
	}
	std::cout << "\t\t";
}

int OperatorGrammar::deal()
{
	// 符号栈使用的深度
	int k = 1;
	charstack[k] = '#';
	// 存储下一个输入符号
	char a;
	int i = 0;
	int j;
	char q;
	// 存储终结符的下标
	int x, y;

	std::cout << "请输入测试句型:(以#结尾的形式)\n";
	std::cin >> sentence;
	for (sl = 0; sentence[sl] != '\0'; sl++) {
	}
	if (sentence[sl - 1] != '#') {
		std::cout << "输入句型没有结束符号!\n";
		return 0;
	}
	// 对输入串进行算符优先分析 
	while ((a = sentence[i]) != '\0') {
		// 当符号栈中深度为k的为终结符,另j = k
		if (terminator(charstack[k]) != -1) {
			j = k;	
		}
		// 不是终结符另j = k -1
		else {
			j = k - 1;
		}
		// x,y分别存储符号栈以及输入串在优先关系矩阵的行列下标
		x = terminator(charstack[j]);
		y = terminator(a);
		// 当它们都是终结符
		if (x != -1 && y != -1) {
			// 当charstack[j] > a进行归约
			if (priority_array[x][y] == '>') {
				print_stack(1, k, charstack);
				std::cout << a;
				print_stack(i + 1, sl,sentence);
				std::cout << "归约\n";
				// 找到最左素短语
				while (priority_array[x][y] != '<' && priority_array[x][y] != '=' && j > 1) {
					q = charstack[j];
					if (terminator(charstack[j - 1]) != -1) {
						j--;
					}
					else {
						j = j - 2;
					}
					x = terminator(charstack[j]);
					y = terminator(q);
				}
				if (j < 1) {
					std::cout << "\nfalse";
					return 0;
				}
				// 把charstack[j + 1]...charstack[k]归约为某个非终结符,sn为该终结符的下标
				int m, n, sn;
				for (sn = 0; sn < tr; sn++) {
					for (m = j + 1, n = 3; m <= k && t_grammar[sn][n] != '\0'; n++, m++) {
						if (charstack[m] != t_grammar[sn][n]) {
							break;
						}
					}
				}
				m--;
				// 如果符合产生式文法规则,将它归约为该非终结符
				if (m == k) {
					charstack[j + 1] = t_grammar[sn][0];
				}
				k = j + 1;
				if (k == 2 && a == '#') {
					print_stack(1, k, charstack);
					std::cout << a;
					print_stack(i + 1, sl, sentence);
					std::cout << "结束\n";
					std::cout << "输入句型符合该文法定义!\n";
					return 1;
				}
			}
			// 移进情况,charstack[j] < a || charstack[j] = a
			else {
				if (priority_array[x][y] == '<' || priority_array[x][y] == '=') {
					print_stack(1, k, charstack);
					std::cout << a;
					print_stack(i + 1, sl, sentence);
					std::cout << "移进\n";
					k++;
					charstack[k] = a;
					i++;
				}
				else {
					std::cout << "\nfalse";
					return 0;
				}
			}
		}
		// 当输入字符为非终结符移进
		else if (y == -1) {
			print_stack(1, k, charstack);
			std::cout << a;
			print_stack(i + 1, sl, sentence);
			std::cout << "移进\n";
			k++;
			charstack[k] = a;
			i++;
		}
	}
	std::cout << "\nfalse";
	return 0;
}
