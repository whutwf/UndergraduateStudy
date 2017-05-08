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
#include<conio.h>  
#include<stdlib.h> 
#include <Windows.h>       //GetSystemTime

using namespace std;

const int BO_NUM = 8;	//棋盘数
const int DIR_NUM = 8;	//方向数字
int DirX[]={2,1,-1,-2,-2,-1,1,2};                           //数组依次记录八个可走方向的横坐标   
int DirY[]={1,2,2,1,-1,-2,-2,-1};                           //数组依次记录八个可走方向的纵坐标   
int chessBoard[BO_NUM][BO_NUM];                                                   //定义了一个8*8的棋盘   

//初始化棋盘，将所有的格子初始化为零  
void init()                                         
{   
	for(int i = 0; i < BO_NUM; i++)   
		for(int j = 0; j < BO_NUM; j++)
		{
			chessBoard[i][j] = 0;
		}
}  

//判断位置是否可走
bool pass(int m, int n)                          
{   
	if((m>=0) && (m<BO_NUM) &&(n >= 0) && (n<BO_NUM)
							&&(chessBoard[m][n]==0)) 
	{
		return 1;      
	}
	return 0;   
} 

//计算每个方向有的出口数
int wayNum(int m, int n)                       
{
	int flag = 0;                                  
	for(int i = 0; i < DIR_NUM; i++)    
	{
		if(pass((m+DirX[i]), (n+DirY[i])))  
			flag++;    
	}
		return flag;                            
		
}   

//可走方向最少的方向,返回最小出口数的方向   
int minWay(int m, int n)                                
{                                                 
	int minWaydir = 0, minWaynumber = DIR_NUM+1, a = 0;                  //minWaydir记录最小出口数的方向，minWaynumber记录该方向的出口数
	for(int i = 0; i < DIR_NUM; i++)
	{   
		if(pass((m+DirX[i]), (n+DirY[i])))
		{
			a = wayNum((m+DirX[i]), (n+DirY[i]));  //计算该方向的出口数目
			if(a && (a<minWaynumber))  
			{                                     
				minWaynumber = a;                         
				minWaydir = i;
			}   
		}   
	}   
	 //当step=63时，所有出口均为0，记录最后一个未标记的位置
	if(minWaydir == 0)  
	{                                              
		for(int i = 0; i < DIR_NUM; i++)   
		{
			if(pass((m+DirX[i]), (n+DirY[i])))   
				return i;   
		}
	}   
	return minWaydir;                                   
} 

//搜索路径，贪心法实现，不用回溯
void findWay(int x, int y, int step)
{
	int i, j;
	i = step+1;
	for( ; i < 65; i++)
	{                 
		j=minWay(x,y);                           
		x+=DirX[j];                          
		y+=DirY[j];   
				
		chessBoard[x][y] = i;  
	} 
}

//打印棋盘信息,返回踏棋结果
int displayChess()                                   
{   
	int result = 1;//存储踏棋结果,1成功
	for(int i = 0; i < BO_NUM; i++){   
		cout << endl;
		cout << endl;
		for(int j = 0; j < BO_NUM; j++)
		{   
			cout.width(6);   
			cout << chessBoard[i][j];
			if(chessBoard[i][j] == 0)
			{
				result = 0;
			}
		}   
	}   
	cout<<endl;
	cout<<endl; 
	return result;
}   

/*****************************************主程序***********************************************/
int main()   
{   
	cout<< "-----------------马踏棋盘--------------\n";        
	int   x = 0, y = 0, step = 1, i = 0;            //step表示走的步数
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

	cout << "-----------------贪心马开始踏棋\n";

	SYSTEMTIME begTime;		//记录运行时间开始
	GetSystemTime(&begTime);

	init();                                
	chessBoard[x][y] = step;		             
	findWay(x, y, step);
	result = displayChess();

	SYSTEMTIME EndTime;
	GetSystemTime(&EndTime);		//运行时间结束
	WORD  ss = EndTime.wSecond - begTime.wSecond; 
	int  ms = EndTime.wMilliseconds - begTime.wMilliseconds;	

	if(result)
	{
		cout << "\n\n---------------马踏棋成功\n";
	}
	else
	{
		cout << "\n\n---------------马踏棋失败\n";
	}

	cout << "总共用时：" << double(ss + ms/1000.0) << "秒" << endl;
	system("pause");
	return 1;
}   
