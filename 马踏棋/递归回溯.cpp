/*
**��Ŀ����̤������ʵ��
**���ߣ���ֽɡ
**ʱ�䣺2015/1/13
*/

/*
**����������ڹ��������8��8���̵�ĳ�������У���������Ĺ�������ƶ���ÿ������ֻ����һ�Σ��߱����̵�ȫ��64������
**��д�㷨������������·�ߣ��������������·�ߣ���1,2,��,64��������һ��8��8�ķ��󣬲������ 
**Ҫ��(1)�����㷨������ͼ�������㷨��ʱ�临�Ӷȡ�
**	    (2)����ʹ�������㷨
*/

#include<iostream>
#include <iomanip>
#include <Windows.h>       //GetSystemTime

using namespace std;


/*
**��Ϊ��������������5��ʱ�򣬵�����û�гɹ��������ʱ������
**ѡ��5��������
*/
const int BO_NUM = 8;	//���̴�С
const int DIR_NUM = 8;	//������

const int DIRECTION[DIR_NUM][2] = {{1, 2}, {1, -2}, {-1, 2}, {-1, -2},
									{2, 1}, {2, -1}, {-2, 1}, {-2, -2}};		//��������
int chessBoard[BO_NUM][BO_NUM];				//����

void initChess()
{
	for(int i = 0; i <= BO_NUM-1; ++i)
	{
		for(int j = 0; j <= BO_NUM-1; ++j)
		{
			chessBoard[i][j] = 0;
		}
	}
}

void displayChess()                                   
{   
	for(int i = 0; i < BO_NUM; i++){   
		cout << endl;
		cout << endl;
		for(int j = 0; j < BO_NUM; j++)
		{   
			cout.width(6);   
			cout << chessBoard[i][j];
		}   
	}   
	cout<<endl;
	cout<<endl; 
}  

//���λ���Ƿ��Ѿ��߹�
int IF_mark(int &x, int &y)
{
	if(chessBoard[x][y] == 0)
	{
		return 1;
	}
	return 0;
}

//�����һ�������ߵ�λ��
int checkNext(int &x, int &y, int step)
{
	int x_temp = -1, y_temp =-1;
	switch(step)
	{
		case 0:
			x_temp = x + DIRECTION[0][0];
			y_temp = y + DIRECTION[0][1];
			if(x_temp <= BO_NUM-1 && y_temp <= BO_NUM-1 && IF_mark(x_temp, y_temp))
			{
				x = x_temp;
				y = y_temp;
				return 1;
			}
			break;
		case 1:
			x_temp = x + DIRECTION[1][0];
			y_temp = y + DIRECTION[1][1];
			if(x_temp <= BO_NUM-1 && y_temp >= 0 && IF_mark(x_temp, y_temp))
			{
				x = x_temp;
				y = y_temp;
				return 1;
			}
			break;
		case 2:
			x_temp = x + DIRECTION[2][0];
			y_temp = y + DIRECTION[2][1];
			if(x_temp >= 0 && y_temp <= BO_NUM-1 && IF_mark(x_temp, y_temp))
			{
				x = x_temp;
				y = y_temp;
				return 1;
			}
			break;
		case 3:
			x_temp = x + DIRECTION[3][0];
			y_temp = y + DIRECTION[3][1];
			if(x_temp >= 0 && y_temp >= 0 && IF_mark(x_temp, y_temp))
			{
				x = x_temp;
				y = y_temp;
				return 1;
			}
			break;
		case 4:
			x_temp = x + DIRECTION[4][0];
			y_temp = y + DIRECTION[4][1];
			if(x_temp <= BO_NUM-1 && y_temp <= BO_NUM-1 && IF_mark(x_temp, y_temp))
			{
				x = x_temp;
				y = y_temp;
				return 1;
			}
			break;
		case 5:
			x_temp = x + DIRECTION[5][0];
			y_temp = y + DIRECTION[5][1];
			if(x_temp <= BO_NUM-1 && y_temp >= 0 && IF_mark(x_temp, y_temp))
			{
				x = x_temp;
				y = y_temp;
				return 1;
			}
			break;
		case 6:
			x_temp = x + DIRECTION[6][0];
			y_temp = y + DIRECTION[6][1];
			if(x_temp >= 0 && y_temp <= BO_NUM-1 && IF_mark(x_temp, y_temp))
			{
				x = x_temp;
				y = y_temp;
				return 1;
			}
			break;
		case 7:
			x_temp = x + DIRECTION[7][0];
			y_temp = y + DIRECTION[7][1];
			if(x_temp >= 0 && y_temp >= 0 && IF_mark(x_temp, y_temp))
			{
				x = x_temp;
				y = y_temp;
				return 1;
			}
			break;
		default:
			break;
	}
	return 0;
}

//��̤��ִ�����
int travelChess(int x, int y, int count)
{

	int x_temp = x, y_temp = y;
	int nextSign;	//��¼��һ���Ƿ����
	int trResult;	//��¼�������̽��
	if(count > BO_NUM * BO_NUM)
	{
		return 1;
	}
	for(int i = 0; i < DIR_NUM; ++i)
	{
		nextSign = checkNext(x_temp, y_temp, i);
		if(nextSign)
		{
			chessBoard[x_temp][y_temp] = count;
			trResult = travelChess(x_temp, y_temp, count+1);
			if(trResult)
			{
				return 1;
			}
			//�˷���δ�ܱ����꣬���ݵ���ʱλ��
			else
			{
				chessBoard[x_temp][y_temp] = 0;
				x_temp = x;
				y_temp = y;
			}
		}
	}
	return 0;
}
 
int main()
{ 
	cout<< "-----------------��̤����--------------\n";        
	int   x = 0, y = 0, step, i = 0;            //step��ʾ�ߵĲ���
	char ch;                             //�ж�����������Ƿ���ȷ
	int result;							//��¼��̤����

	while(1)
	{
		cout << endl << "�����������(0--7)" << endl;
		cout << "x=, y=\n";
		cin >> x >> y; 
		cout << "�����������ȷ���t/f" << endl;
		cin >> ch;
		if(ch=='t'|| ch == 'T') break;
	}

	
	cout << "------------------���ݷ�������̤�忪ʼ\n";
	SYSTEMTIME begTime;
	GetSystemTime(&begTime);
	initChess();
	chessBoard[x][y] = 1;
    step = 2;
	result=travelChess(x,y,step);  

	displayChess();

	SYSTEMTIME EndTime;
	GetSystemTime(&EndTime);
	WORD  ss = EndTime.wSecond - begTime.wSecond; 
	int  ms = EndTime.wMilliseconds - begTime.wMilliseconds;
	
	if(result)
	{
		cout << "\n\n---------------��̤��ɹ�" << endl;
	}
	else 
	{
		cout << "\n\n---------------��̤��ʧ��" << endl;
	}
	cout << "�ܹ���ʱ��" << double(ss + ms/1000.0) << "��" << endl;
	system("pause");
	return 1;
}