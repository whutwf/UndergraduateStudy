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
#include<conio.h>  
#include<stdlib.h> 
#include <Windows.h>       //GetSystemTime

using namespace std;

const int BO_NUM = 8;	//������
const int DIR_NUM = 8;	//��������
int DirX[]={2,1,-1,-2,-2,-1,1,2};                           //�������μ�¼�˸����߷���ĺ�����   
int DirY[]={1,2,2,1,-1,-2,-2,-1};                           //�������μ�¼�˸����߷����������   
int chessBoard[BO_NUM][BO_NUM];                                                   //������һ��8*8������   

//��ʼ�����̣������еĸ��ӳ�ʼ��Ϊ��  
void init()                                         
{   
	for(int i = 0; i < BO_NUM; i++)   
		for(int j = 0; j < BO_NUM; j++)
		{
			chessBoard[i][j] = 0;
		}
}  

//�ж�λ���Ƿ����
bool pass(int m, int n)                          
{   
	if((m>=0) && (m<BO_NUM) &&(n >= 0) && (n<BO_NUM)
							&&(chessBoard[m][n]==0)) 
	{
		return 1;      
	}
	return 0;   
} 

//����ÿ�������еĳ�����
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

//���߷������ٵķ���,������С�������ķ���   
int minWay(int m, int n)                                
{                                                 
	int minWaydir = 0, minWaynumber = DIR_NUM+1, a = 0;                  //minWaydir��¼��С�������ķ���minWaynumber��¼�÷���ĳ�����
	for(int i = 0; i < DIR_NUM; i++)
	{   
		if(pass((m+DirX[i]), (n+DirY[i])))
		{
			a = wayNum((m+DirX[i]), (n+DirY[i]));  //����÷���ĳ�����Ŀ
			if(a && (a<minWaynumber))  
			{                                     
				minWaynumber = a;                         
				minWaydir = i;
			}   
		}   
	}   
	 //��step=63ʱ�����г��ھ�Ϊ0����¼���һ��δ��ǵ�λ��
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

//����·����̰�ķ�ʵ�֣����û���
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

//��ӡ������Ϣ,����̤����
int displayChess()                                   
{   
	int result = 1;//�洢̤����,1�ɹ�
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

/*****************************************������***********************************************/
int main()   
{   
	cout<< "-----------------��̤����--------------\n";        
	int   x = 0, y = 0, step = 1, i = 0;            //step��ʾ�ߵĲ���
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

	cout << "-----------------̰����ʼ̤��\n";

	SYSTEMTIME begTime;		//��¼����ʱ�俪ʼ
	GetSystemTime(&begTime);

	init();                                
	chessBoard[x][y] = step;		             
	findWay(x, y, step);
	result = displayChess();

	SYSTEMTIME EndTime;
	GetSystemTime(&EndTime);		//����ʱ�����
	WORD  ss = EndTime.wSecond - begTime.wSecond; 
	int  ms = EndTime.wMilliseconds - begTime.wMilliseconds;	

	if(result)
	{
		cout << "\n\n---------------��̤��ɹ�\n";
	}
	else
	{
		cout << "\n\n---------------��̤��ʧ��\n";
	}

	cout << "�ܹ���ʱ��" << double(ss + ms/1000.0) << "��" << endl;
	system("pause");
	return 1;
}   
