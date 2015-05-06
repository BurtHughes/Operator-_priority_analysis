#include <iostream>

using namespace std;

#define SourceMaxLength 50
#define RULE_ROWNUM 10
#define RULE_COLNUM 10
#define BEGIN_FINISH_FLAG 108 //开始,结束标记
#define VN 0  //非终结符

/*---------------------------------Global------------------------------------*/

int flag = 1; //分析结果，1为正确，0为错误
int sourceLength = 0;//源程序长度
//算符优先关系表
int priorityRelationalTable[RULE_ROWNUM][RULE_COLNUM] =
{
	{ -1, 100, 101, 102, 103, 104, 105, 106, 107, 108},
	{100, 200, 200, 201, 201, 201, 201, 200, 201, 200},
	{101, 200, 200, 201, 201, 201, 201, 200, 201, 200},
	{102, 200, 200, 200, 200, 201, 201, 200, 201, 200},
	{103, 200, 200, 200, 200, 201, 201, 200, 201, 200},
	{104, 200, 200, 200, 200, 200, 201, 200, 201, 200},
	{105, 201, 201, 201, 201, 201, 201, 202, 201,  -1},
	{106, 200, 200, 200, 200, 200,  -1, 200,  -1, 200},
	{107, 200, 200, 200, 200, 200,  -1, 200,  -1, 200},
	{108, 201, 201, 201, 201, 201, 201,  -1, 201, 202}
};

/*---------------------------------structs------------------------------------*/
//源程序，字符串方式
struct sourceStackChar{
	char source[SourceMaxLength];
	int pointer;
};
//栈结构，数组模拟
struct StackInt{
	int source[SourceMaxLength];
	int pointer;
};

/*------------------------------Functions-----------------------------------*/
//入栈操作，数组模拟
void myPush(struct StackInt *s, int item)
{
	s->pointer++;
	s->source[s->pointer] = item;
}

//出栈操作，数组模拟
int myPop(struct StackInt *s)
{
	int top = s->source[s->pointer];
	s->pointer--;
	return top;
}

//读栈顶元素
int myReadStack(struct StackInt *s)
{
	return s->source[s->pointer];
}

//遍历算符优先关系表的第一行，查看元素是否是终结符,
//返回值为10，代表检查对象不是终结符
//返回值为1~9，代表检查对象是终结符，返回值为在算符优先关系表中的列号
int check(int obj)
{
	int col;
	for (col = 1; col < 10; col++)
	{
		if (obj == priorityRelationalTable[0][col])
		{
			break;
		}
	}
	return col;
}

//由于采用数组模拟堆栈，栈底为0端，因此需要倒序放入数组
//s1为字符串源程序，s2为数字化源程序
int sourceS2I(struct sourceStackChar *s1, struct StackInt *s2)
{
	int i;
	int length = strlen(s1->source);
	int j = length - 1;
	for (i = 0; i <= j; i++)
	{
		char tmp = s1->source[i];
		switch (tmp)
		{
		case '+': s2->source[j--] = 100; break;
		case '-': s2->source[j--] = 101; break;
		case '*': s2->source[j--] = 102; break;
		case '/': s2->source[j--] = 103; break;
		case '^': s2->source[j--] = 104; break;
		case '(': s2->source[j--] = 105; break;
		case ')': s2->source[j--] = 106; break;
		case 'a': s2->source[j--] = 107; break;
		case '#': s2->source[j--] = 108;
		}
	}
	return length;
}

//初始化分析栈，源程序输入栈栈顶指针及相应元素（采用数组模拟堆栈）
//分析栈初始化压入开始标记，文法开始符号，并设置栈顶指针
//将字符源程序转换为数字源程序填入堆栈，并设置栈顶指针
void stackInit(struct sourceStackChar *ssc, struct StackInt *ssi, struct StackInt *ps)
{
	myPush(ps, BEGIN_FINISH_FLAG);
	int length = sourceS2I(ssc, ssi);
	ssi->pointer = length - 1;
}

//根据分析栈栈顶第一个终结符号，获取分析表行号
int getRowNumber(struct StackInt *ps)
{
	//获取分析栈栈顶符号
	int top = myReadStack(ps);
	int row = check(top);
	//栈顶是终结符，就直接返回其行号
	if (1 <= row && row <= 9)
	{
		return row;
	}
	//栈顶不是终结符，就返回栈顶下面的一个符号（此符号必定是终结符）
	else
	{
		top = ps->pointer - 1;
		int row = check(top);
		return row;
	}
}

//根据输入栈栈顶符号，获取分析表列号
int getColNumber(struct StackInt *ssi)
{
	//获取输入串下一符号R
	int R = myReadStack(ssi);
	int col = check(R);
	return col;
}


//分析过程
bool analyseSource(struct StackInt *ssi, struct StackInt *ps)
{
	while (true)
	{
		//根据输入栈栈顶符号，获取分析表列号
		int col = getColNumber(ssi);
		//如果列号为10（代表输入串有非法字符），返回错误
		if (10 == col)
		{
			return false;
		}
		//否则，继续分析

		//根据输入栈栈顶符号，获取分析表列号
		int row = getRowNumber(ps);

		//查出当前两符号的关系（查算符优先关系表）
		int relationship = priorityRelationalTable[row][col];
		//如果关系不存在(关系值为-1表示优先关系不存在)，则说明源程序语法错误，返回错误
		if (-1 == relationship)
		{
			return false;
		}
		//否则，关系存在，继续分析

		if (200 == relationship)//200表示 > ，优先关系为 ‘>’ 时，归约
		{
			//找到最左素短语
			int relation = 0;
			int left = 0;
			int right = ps->pointer;
			while (201 != relation)
			{
				//获取列号
				int col = check(ps->source[right]);
				if (col == 10)
				{
					col = check(ps->source[right - 1]);
				}
				//获取行号
				left = right - 1;
				int row = ps->source[left];
				if (row == 10)
				{
					row = check(ps->source[--left]);
				}
				//查表
				relation = priorityRelationalTable[row][col];
				right--;
			}
			ps->pointer = left;
			myPush(ps, VN);

			if (2 == ps->pointer && BEGIN_FINISH_FLAG == ssi->source[ssi->pointer])
			{
				return true;
			}
		}
		else//优先关系为 ‘<’ 和 ‘=’ 时，移进
		{

			int tmp = myPop(ssi);
			myPush(ps, tmp);
		}
	}
}

/*----------------------------main()----------------------------------*/
int main()
{
	struct sourceStackChar ssc;//源程序
	struct StackInt ssi;//输入栈
	struct StackInt ps;//分析栈
	ps.pointer = -1;

	strcpy_s(ssc.source, "(a+a*a^a-a/a)+(a+a)#");
	ssc.pointer = 18;
	stackInit(&ssc, &ssi, &ps);
	bool flag = analyseSource(&ssi, &ps);
	if (flag == false)
	{
		cout << "语法错误" << endl;
	}
	else
	{
		cout << "语法正确" << endl;
	}
	system("pause");
	return 0;
}