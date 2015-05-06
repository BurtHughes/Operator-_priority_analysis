#include <iostream>

using namespace std;

#define SourceMaxLength 50
#define RULE_ROWNUM 10
#define RULE_COLNUM 10
#define BEGIN_FINISH_FLAG 108 //��ʼ,�������
#define VN 0  //���ս��

/*---------------------------------Global------------------------------------*/

int flag = 1; //���������1Ϊ��ȷ��0Ϊ����
int sourceLength = 0;//Դ���򳤶�
//������ȹ�ϵ��
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
//Դ�����ַ�����ʽ
struct sourceStackChar{
	char source[SourceMaxLength];
	int pointer;
};
//ջ�ṹ������ģ��
struct StackInt{
	int source[SourceMaxLength];
	int pointer;
};

/*------------------------------Functions-----------------------------------*/
//��ջ����������ģ��
void myPush(struct StackInt *s, int item)
{
	s->pointer++;
	s->source[s->pointer] = item;
}

//��ջ����������ģ��
int myPop(struct StackInt *s)
{
	int top = s->source[s->pointer];
	s->pointer--;
	return top;
}

//��ջ��Ԫ��
int myReadStack(struct StackInt *s)
{
	return s->source[s->pointer];
}

//����������ȹ�ϵ��ĵ�һ�У��鿴Ԫ���Ƿ����ս��,
//����ֵΪ10��������������ս��
//����ֵΪ1~9��������������ս��������ֵΪ��������ȹ�ϵ���е��к�
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

//���ڲ�������ģ���ջ��ջ��Ϊ0�ˣ������Ҫ�����������
//s1Ϊ�ַ���Դ����s2Ϊ���ֻ�Դ����
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

//��ʼ������ջ��Դ��������ջջ��ָ�뼰��ӦԪ�أ���������ģ���ջ��
//����ջ��ʼ��ѹ�뿪ʼ��ǣ��ķ���ʼ���ţ�������ջ��ָ��
//���ַ�Դ����ת��Ϊ����Դ���������ջ��������ջ��ָ��
void stackInit(struct sourceStackChar *ssc, struct StackInt *ssi, struct StackInt *ps)
{
	myPush(ps, BEGIN_FINISH_FLAG);
	int length = sourceS2I(ssc, ssi);
	ssi->pointer = length - 1;
}

//���ݷ���ջջ����һ���ս���ţ���ȡ�������к�
int getRowNumber(struct StackInt *ps)
{
	//��ȡ����ջջ������
	int top = myReadStack(ps);
	int row = check(top);
	//ջ�����ս������ֱ�ӷ������к�
	if (1 <= row && row <= 9)
	{
		return row;
	}
	//ջ�������ս�����ͷ���ջ�������һ�����ţ��˷��űض����ս����
	else
	{
		top = ps->pointer - 1;
		int row = check(top);
		return row;
	}
}

//��������ջջ�����ţ���ȡ�������к�
int getColNumber(struct StackInt *ssi)
{
	//��ȡ���봮��һ����R
	int R = myReadStack(ssi);
	int col = check(R);
	return col;
}


//��������
bool analyseSource(struct StackInt *ssi, struct StackInt *ps)
{
	while (true)
	{
		//��������ջջ�����ţ���ȡ�������к�
		int col = getColNumber(ssi);
		//����к�Ϊ10���������봮�зǷ��ַ��������ش���
		if (10 == col)
		{
			return false;
		}
		//���򣬼�������

		//��������ջջ�����ţ���ȡ�������к�
		int row = getRowNumber(ps);

		//�����ǰ�����ŵĹ�ϵ����������ȹ�ϵ��
		int relationship = priorityRelationalTable[row][col];
		//�����ϵ������(��ϵֵΪ-1��ʾ���ȹ�ϵ������)����˵��Դ�����﷨���󣬷��ش���
		if (-1 == relationship)
		{
			return false;
		}
		//���򣬹�ϵ���ڣ���������

		if (200 == relationship)//200��ʾ > �����ȹ�ϵΪ ��>�� ʱ����Լ
		{
			//�ҵ������ض���
			int relation = 0;
			int left = 0;
			int right = ps->pointer;
			while (201 != relation)
			{
				//��ȡ�к�
				int col = check(ps->source[right]);
				if (col == 10)
				{
					col = check(ps->source[right - 1]);
				}
				//��ȡ�к�
				left = right - 1;
				int row = ps->source[left];
				if (row == 10)
				{
					row = check(ps->source[--left]);
				}
				//���
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
		else//���ȹ�ϵΪ ��<�� �� ��=�� ʱ���ƽ�
		{

			int tmp = myPop(ssi);
			myPush(ps, tmp);
		}
	}
}

/*----------------------------main()----------------------------------*/
int main()
{
	struct sourceStackChar ssc;//Դ����
	struct StackInt ssi;//����ջ
	struct StackInt ps;//����ջ
	ps.pointer = -1;

	strcpy_s(ssc.source, "(a+a*a^a-a/a)+(a+a)#");
	ssc.pointer = 18;
	stackInit(&ssc, &ssi, &ps);
	bool flag = analyseSource(&ssi, &ps);
	if (flag == false)
	{
		cout << "�﷨����" << endl;
	}
	else
	{
		cout << "�﷨��ȷ" << endl;
	}
	system("pause");
	return 0;
}