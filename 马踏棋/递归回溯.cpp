/*
**题目：马踏棋问题实现
**作者：油纸伞
**时间：2015/1/13
*/

/*
**将马随机放在国际象棋的8×8棋盘的某个方格中，马按照走棋的规则进行移动。每个方格只进入一次，走遍棋盘的全部64个方格。
**编写算法，求出马的行走路线，并按求出的行走路线，将1,2,…,64依次填入一个8×8的方阵，并输出。 
**要求：(1)画出算法的流程图，分析算法的时间复杂度。
**	    (2)至少使用两种算法
*/

#include<iostream>
#include <iomanip>
#include <Windows.h>       //GetSystemTime

using namespace std;


/*
**因为当棋盘数量大于5的时候，当处理没有成功的情况，时间骤增
**选择5来做分析
*/
const int BO_NUM = 8;	//棋盘大小
const int DIR_NUM = 8;	//方向数

const int DIRECTION[DIR_NUM][2] = {{1, 2}, {1, -2}, {-1, 2}, {-1, -2},
									{2, 1}, {2, -1}, {-2, 1}, {-2, -2}};		//方向数组
int chessBoard[BO_NUM][BO_NUM];				//棋盘

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

//检查位置是否已经走过
int IF_mark(int &x, int &y)
{
	if(chessBoard[x][y] == 0)
	{
		return 1;
	}
	return 0;
}

//检查下一步可以走的位置
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

//马踏棋执行入口
int travelChess(int x, int y, int count)
{

	int x_temp = x, y_temp = y;
	int nextSign;	//记录下一步是否可走
	int trResult;	//记录遍历棋盘结果
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
			//此方向未能遍历完，回溯到此时位置
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
	cout<< "-----------------马踏棋盘--------------\n";        
	int   x = 0, y = 0, step, i = 0;            //step表示走的步数
	char ch;                             //判断输入的坐标是否正确
	int result;							//记录马踏棋结果

	while(1)
	{
		cout << endl << "请输入横坐标(0--7)" << endl;
		cout << "x=, y=\n";
		cin >> x >> y; 
		cout << "输入的坐标正确与否t/f" << endl;
		cin >> ch;
		if(ch=='t'|| ch == 'T') break;
	}

	
	cout << "------------------回溯法分析马踏棋开始\n";
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
		cout << "\n\n---------------马踏棋成功" << endl;
	}
	else 
	{
		cout << "\n\n---------------马踏棋失败" << endl;
	}
	cout << "总共用时：" << double(ss + ms/1000.0) << "秒" << endl;
	system("pause");
	return 1;
}