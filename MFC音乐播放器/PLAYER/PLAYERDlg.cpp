/*
*天马星空音乐盒
*时间：2014年6月19日
*作者：王飞
*实现语言：MFC
*运行环境:vs2010
*功能：基本的播放，暂停，删除，列表添加（可多选添加），三种播放模式
*/
#pragma comment(lib,"winmm.lib")
#include "stdafx.h"
#include "PLAYER.h"
#include "PLAYERDlg.h"
#include"mmsystem.h"
#include "Digitalv.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

HWND hWnd;

static int MODEL;

const int NUM = 10; 
HICON hIcon[NUM];//加载图标类型
HBITMAP    hBmp[NUM];//加载按钮位图

const int MAXFILE = 255;

int hour = 0, minute = 0, second= 0;  


DWORD m_deviceid;
DWORD DeviceID;

DWORD voiceLength;//总时间长度
CString filename;//文件名
CString filepath;//文件路径
CString file[MAXFILE];//存放文件路径名

int fend = 0, fcur = 0;//当前文本框中文件位置，和最后位置
DWORD voiceFrom,voiceTo;//歌曲的起点和终点

BOOL m_flag,m_hidden;//允许播放，显示列表标志

MCI_OPEN_PARMS mciopenparms;//打开
MCI_PLAY_PARMS mciplayparms;//播放


 
DWORD setvolume(DWORD vol)
{
	MCI_DGV_SETAUDIO_PARMS setvolume;//这是设置音量的参数数据结构
	setvolume.dwCallback=(DWORD)hWnd; 
	setvolume.dwItem=MCI_DGV_SETAUDIO_VOLUME;//设置音量
	setvolume.dwValue=vol;//音量值是vol
	if(m_deviceid)
	{
		mciSendCommand(m_deviceid,MCI_SETAUDIO,MCI_DGV_SETAUDIO_ITEM|MCI_DGV_SETAUDIO_VALUE,
		(DWORD)(LPVOID)&setvolume);
	}
	return 0;
}

DWORD CPLAYERDlg::getinfo(DWORD item)
{//MCI接口对象的状态
MCI_STATUS_PARMS mcistatusparms;
//歌曲格式
mcistatusparms.dwCallback=(DWORD)GetSafeHwnd();
//待获取的项目
mcistatusparms.dwItem=item;
mcistatusparms.dwReturn=0;
//想多媒体设备发送指令，获取当前的状态参数
mciSendCommand(m_deviceid,MCI_STATUS,MCI_STATUS_ITEM,(DWORD)&mcistatusparms);
return mcistatusparms.dwReturn; }


class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
//	afx_msg LRESULT WM_SETVOLUME(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetvolume(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
//	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg LRESULT OnNcHitTest(CPoint point);
//	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
//	ON_MESSAGE(WM_SETVOLUME, &CAboutDlg::WM_SETVOLUME)
	//ON_MESSAGE(WM_SETVOLUME(), &CAboutDlg::OnSetvolume)
	ON_WM_HSCROLL()
//	ON_WM_LBUTTONDOWN()
ON_WM_NCHITTEST()
//ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPLAYERDlg dialog

CPLAYERDlg::CPLAYERDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPLAYERDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPLAYERDlg)
	m_vol = 0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPLAYERDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPLAYERDlg)
	DDX_Control(pDX, IDC_SLIDER, m_slider);
	DDX_Control(pDX, IDC_PROGRESS, m_progress);
	DDX_Control(pDX, IDC_STOP, m_stop);
	DDX_Control(pDX, IDC_SDEL, m_sdel);
	DDX_Control(pDX, IDC_POP, m_pop);
	DDX_Control(pDX, IDC_PLAY, m_play);
	DDX_Control(pDX, IDC_PER, m_per);
	DDX_Control(pDX, IDC_OPEN, m_open);
	DDX_Control(pDX, IDC_NEXT, m_next);
	DDX_Control(pDX, IDC_LIST, m_list);
	DDX_Control(pDX, IDC_BHELP, m_bhelp);
	DDX_Control(pDX, IDC_BEXIT, m_bexit);
	DDX_Text(pDX, IDC_vol, m_vol);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_MIM, m_mini);
}

BEGIN_MESSAGE_MAP(CPLAYERDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()

	//ON_WM_SETVOLUME()  

	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_OPEN, OnOpen)
	ON_BN_CLICKED(IDC_STOP, OnStop)
	ON_BN_CLICKED(IDC_PLAY, OnPlay)
	ON_BN_CLICKED(IDC_PER, OnPer)
	ON_BN_CLICKED(IDC_NEXT, OnNext)
	ON_BN_CLICKED(IDC_POP, OnPop)
	ON_BN_CLICKED(IDC_SDEL, OnSdel)
	ON_BN_CLICKED(IDC_BEXIT, OnBexit)
	ON_BN_CLICKED(IDC_BHELP, OnBhelp)
	ON_BN_CLICKED(IDC_BMIM, OnBmim)
	ON_LBN_SELCHANGE(IDC_LIST, OnDblclkList)
    ON_WM_TIMER()
	ON_NOTIFY(NM_OUTOFMEMORY, IDC_SLIDER, OnOutofmemorySlider)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER, OnCustomdrawSlider)
	ON_LBN_DBLCLK(IDC_LIST, OnDblclkList)
	ON_EN_CHANGE(IDC_vol, OnChangevol)
	ON_EN_CHANGE(IDC_filename, &CPLAYERDlg::OnEnChangefilename)
	ON_COMMAND(ID_EXIT, exit)
	ON_COMMAND(ID_MINI, mini)
	ON_COMMAND(ID_ADD, addSong)
	ON_COMMAND(ID_SINGLE, m_single)
	ON_COMMAND(ID_QUE, m_que)
	ON_COMMAND(ID_RANDOM, m_random)
	ON_COMMAND(ID_HELP, m_help)
	ON_BN_CLICKED(IDC_MODELDESIGN, &CPLAYERDlg::OnBnClickedModeldesign)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_PROGRESS, &CPLAYERDlg::OnNMCustomdrawProgress)
	ON_BN_CLICKED(IDC_MIM, &CPLAYERDlg::OnBnClickedMim)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPLAYERDlg message handlers

BOOL CPLAYERDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	
	

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	

	/*::MENUINFO  lpcmi;                        //：：必须写不然无法识别
    m_brush.CreateSolidBrush(RGB(255,0,0));//你的颜色
    memset(&lpcmi,0,sizeof(::LPCMENUINFO));
    lpcmi.cbSize=sizeof(MENUINFO);
    lpcmi.fMask=   MIM_BACKGROUND;   
    lpcmi.hbrBack=(HBRUSH)m_brush.operator HBRUSH();
    ::SetMenuInfo(GetMenu()->m_hMenu,&lpcmi);*/

	m_Menu.LoadMenu(IDR_MENU1);//加载菜单栏

    SetMenu(&m_Menu);//为主窗体添加菜单栏

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	m_slider.SetRange(0,1000);//滑块的移动范围0~1000
	m_slider.SetPos(500);//滑块指针的初始位置设为500代表初始音量为50
	hIcon[1]= AfxGetApp()->LoadIcon(IDI_ICON13); 
    m_play.SetIcon(hIcon[1]); 

    hIcon[2]= AfxGetApp()->LoadIcon(IDI_ICON10); 
    m_per.SetIcon(hIcon[2]); 

	hIcon[3]= AfxGetApp()->LoadIcon(IDI_ICON2); 
    m_next.SetIcon(hIcon[3]);

	
	hIcon[4]= AfxGetApp()->LoadIcon(IDI_MYLOVE); 
    m_bhelp.SetIcon(hIcon[4]);

	hBmp[1]=::LoadBitmap(AfxGetInstanceHandle(),   
    MAKEINTRESOURCE(IDB_BITMAP3));   
    m_bexit.SetBitmap(hBmp[1]);  

	/*hBmp[2]=::LoadBitmap(AfxGetInstanceHandle(),   
    MAKEINTRESOURCE(IDB_BITMAP5));   
    m_min.SetBitmap(hBmp[2]);  */

	hBmp[3]=::LoadBitmap(AfxGetInstanceHandle(),   
    MAKEINTRESOURCE(IDB_BITMAP6));   
    m_pop.SetBitmap(hBmp[3]);  
		
	
	hBmp[4]=::LoadBitmap(AfxGetInstanceHandle(),   
    MAKEINTRESOURCE(IDB_BITMAP8));   
    m_open.SetBitmap(hBmp[4]);  

   hBmp[5]=::LoadBitmap(AfxGetInstanceHandle(),   
   MAKEINTRESOURCE(IDB_BITMAP4));   
   m_sdel.SetBitmap(hBmp[5]);  

   
   hBmp[5]=::LoadBitmap(AfxGetInstanceHandle(),   
   MAKEINTRESOURCE(IDB_BITMAP9));   
   m_stop.SetBitmap(hBmp[5]);  

   hBmp[6]=::LoadBitmap(AfxGetInstanceHandle(),   
   MAKEINTRESOURCE(IDB_BITMAP4));   
   m_mini.SetBitmap(hBmp[6]);  

   	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPLAYERDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPLAYERDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this);
		CRect rc;
		GetClientRect(&rc);//获取整个区域的宽度
		CDC dcMem;
		dcMem.CreateCompatibleDC(&dc);


		CBitmap bmpBackground;
		bmpBackground.LoadBitmap(IDB_BITMAP7); //加载一个位图图像初始化位图对象

		BITMAP bitmap;       //BITMAP结构
		bmpBackground.GetBitmap(&bitmap);//用位图信息填充BITMAP结构
		CBitmap* pbmpPri = dcMem.SelectObject(&bmpBackground); //将一个对象选入设备环境中，将代替原先的对象
		dc.StretchBlt(0,0,rc.Width(), rc.Height(), &dcMem,0,0,bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);//对图像缩放
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPLAYERDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

 
void CPLAYERDlg::OnOpen() 
{  

    char szFilter[] = _T("mp3文件(*.mp3)|*.mp3|wma文件(*.wma)|*.wma|wav文件(*.wav)|*.wav|avi文件(*.avi)|*.avi所有文件(*.*)|*.*|");
	CFileDialog cfile(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT ,szFilter);

	cfile.m_ofn.Flags |=512;//可以选取多个曲目
	POSITION position = cfile.GetStartPosition(); 

	if(IDOK == cfile.DoModal())
	{
		while(position != NULL)
		{
		file[fcur] = cfile.GetNextPathName(position);
		
		//如果没有歌播放，则从当前位置播放
		if(filepath == "")
		{
		filepath = file[fcur];
		fend = fcur; //设置文件尾部位置
		stop();
		OnPlay();
		}

		//将文件名添加到列表中，并选中最后一行
		filename = cfile.GetFileName();
		m_list.AddString(file[fcur]);
	    m_list.SetCurSel(fend);
	    UpdateData(false);
		++fcur;
		}
	}
} 

void CPLAYERDlg::OnStop()
{  
	KillTimer(0);//取消计数器的显示
	voiceFrom=MCI_MAKE_HMS(0,0,0);//定位于歌曲的起始位置
	mciSendCommand(m_deviceid,MCI_CLOSE,0,NULL);//向多媒体设备发送消息，关闭多媒体设备
	m_deviceid=0;
    m_flag=true;//允许用户播放歌曲
    GetDlgItem(IDC_PLAY)-> SetWindowText( "播放");//播放按钮的显示变为播放
	hour=0;minute=0;second=0;//歌曲时间置0
}


void CPLAYERDlg::OnPlay( )
{  
	m_slider.SetPos(500);//滑块指针的初始位置设为500代表初始音量为50
	if(filepath == "" )//若目前没有曲目，打开
	{ 
		OnOpen();
	}
	else
	{
		if(m_flag && filepath != "")
		{ 
			CClientDC dc(this);
			
			dc.SetBkColor(RGB(81,54,166));
			dc.SetTextColor(RGB(215,249,9));
			dc.TextOut(40,40,filepath);
			ReleaseDC((CDC*)this);
			hIcon[1]= AfxGetApp()->LoadIcon(IDI_ICON5); 
            m_play.SetIcon(hIcon[1]); 
			m_flag=false;//播放的标志
			GetDlgItem(IDC_PLAY) -> SetWindowText( _T("暂停"));//播放按钮的显示变为暂停

			mciopenparms.lpstrElementName = filepath;//播放路径
			mciopenparms.lpstrDeviceType=NULL;//文件类型
			mciSendCommand(0,MCI_OPEN,MCI_DEVTYPE_WAVEFORM_AUDIO, (DWORD)(LPVOID)&mciopenparms);//向MCI设备发送命令消息，包含歌曲文件的类型和路径
			m_deviceid = mciopenparms.wDeviceID;//多媒体设备类型编号

			mciplayparms.dwCallback = (DWORD)GetSafeHwnd();//歌曲播放支持类型
			voiceLength = getinfo(MCI_STATUS_LENGTH);    //得到曲目长度                                                                                                                                                                                                                                          
			voiceTo = MCI_MAKE_HMS(MCI_HMS_HOUR(voiceLength), MCI_HMS_MINUTE(voiceLength), MCI_HMS_SECOND(voiceLength));//设置播放完毕某音频文件需要的时间n时n分n秒  
			mciplayparms.dwFrom = voiceFrom;
			mciplayparms.dwTo = voiceTo;
			mciSendCommand(m_deviceid, MCI_PLAY,MCI_TO|MCI_FROM, (DWORD)(LPVOID)& mciplayparms);//向多媒体设备发送播放文件命令
			SetTimer(0, 1000, NULL);//设置系统计数器，显示歌曲播放的进度
			OnTimer(0);
			
			UpdateData(false);
		}
		else
		{
			hIcon[1]= AfxGetApp()->LoadIcon(IDI_ICON13); 
            m_play.SetIcon(hIcon[1]); 
			m_flag = true;//允许播放
			GetDlgItem(IDC_PLAY) ->  SetWindowText( _T("播放"));//播放按钮的显示变为播放

			KillTimer(0);//取消计数器
	
			DWORD dwsf=getinfo(MCI_STATUS_POSITION);//暂停播放，获取当前曲目状态
			
			MCI_MSF_FRAME(dwsf);//播放音频文件暂停时，记录下目前的进度
			mciSendCommand(m_deviceid, MCI_CLOSE, 0, NULL);//向多媒体设备发送指令，停止播放
			m_deviceid = 0;
		} 
	}
}


void CPLAYERDlg::OnPer() 
{  
	if(m_list.GetCurSel() == -1)
	{
		return;
	}
	int a;
	int b;
	a = m_list.GetCount();//获取曲目数目
	b = m_list.GetCurSel();//定位当前曲目位置
	if(b == 0)
	{
		b = a;
	}
	b = (b - 1) % (a);//设置向前一步

	m_list.SetCurSel(b);
	m_list.GetText(b, filepath);//将当前信息传给filepath

	stop();
	OnPlay();
	m_next.EnableWindow(true);
	UpdateData(false);
}



void CPLAYERDlg::OnNext() 
{  
	int a;
	int b;
	a = m_list.GetCount();//获取曲目数目
	b = m_list.GetCurSel();//定位当前曲目位置
	if(b == -1)
	{
		return;
	}
	b = (b + 1) % (a);//设置向后一步

	m_list.SetCurSel(b);
	m_list.GetText(b, filepath);//将当前信息传给filepath

	stop();
	OnPlay();
	m_next.EnableWindow(true);
	UpdateData(false);

}

void CPLAYERDlg::stockMatic()
{
	int a;
	int b;
	a = m_list.GetCount();//获取曲目数目
	b = m_list.GetCurSel();//定位当前曲目位置
	if(b == -1)
	{
		return;
	}
	b =rand() % (a);//设置随机

	m_list.SetCurSel(b);
	m_list.GetText(b, filepath);//将当前信息传给filepath

	stop();
	OnPlay();
	m_next.EnableWindow(true);
	UpdateData(false);
}

void CPLAYERDlg::single()
{
	int a;
	int b;
	a = m_list.GetCount();//获取曲目数目
	b = m_list.GetCurSel();//定位当前曲目位置
	if(b == -1)
	{
		return;
	}
	b %= a;//单曲播放

	m_list.SetCurSel(b);
	m_list.GetText(b, filepath);//将当前信息传给filepath

	stop();
	OnPlay();
	m_next.EnableWindow(true);
	UpdateData(false);
}

void CPLAYERDlg::OnPop() 
{	
	//m_hidden:true表示允许隐藏列表，而false允许显示
	if(m_hidden)
	{
	  	hBmp[3]=::LoadBitmap(AfxGetInstanceHandle(),   
        MAKEINTRESOURCE(IDB_BITMAP6));   
        m_pop.SetBitmap(hBmp[3]);   
		for(int i = 220; i <= 440; ++i)
		{
			MoveWindow(320, 160, 310, i);
			m_hidden = false;
			GetDlgItem(IDC_POP)-> SetWindowText(_T( "隐藏列表"));
		}
	}
	else
	{
		hBmp[3]=::LoadBitmap(AfxGetInstanceHandle(),   
        MAKEINTRESOURCE(IDB_BITMAP5));   
        m_pop.SetBitmap(hBmp[3]);   
		
		for(int i = 440; i >= 220; --i)
		{
			MoveWindow(320, 160,310, i);
			m_hidden = true;
			GetDlgItem(IDC_POP)-> SetWindowText(_T("显示列表"));
		}
	}

}	


void CPLAYERDlg::OnSdel() 
{  
	 int temp=0,j=0, temp_2;
	temp=m_list.GetCurSel();
	if(temp < 0)
	{
		return;
	}
	j = temp;
	{
		for(j;j<fend;j++)
		{
			file[j]=file[j+1];
		} 
	fend=fend-1;
	m_list.DeleteString(temp); //在控件中，删除控件中选中条目的字符串
	temp_2 = temp - 1;
	if ((m_list.GetCount() - 1)<=-1)//这里为什么是-1？？？
	{
		KillTimer(0);
		minute = 0, hour =0, second =0;
		stop();
		filepath = "";
		m_progress.SetPos(0);
        UpdateData(false);
		return;
	}

	if(temp_2 == -1)
	{
		temp_2 = m_list.GetCount() - 1;
	}
	m_list.SetCurSel(temp_2);
    m_progress.SetPos(0);
	m_list.GetText(temp_2, filepath);//将当前信息传给filepath

	stop();
	OnPlay();

	UpdateData(false);
	}
}


void CPLAYERDlg::OnBexit() 
{ 
	stop();
    OnOK();
}



void CPLAYERDlg::OnBhelp() 
{	
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}


void CPLAYERDlg::OnBmim() 
{
	ShowWindow(SW_HIDE);
}

void CPLAYERDlg::OnDblclkList() 
{	
	int i = m_list.GetCurSel();
	m_list.GetText(i, filepath);
	CClientDC dc(this);
	dc.TextOut(40,35,"");
	UpdateData(false);
	stop();
	OnPlay();
}

void CPLAYERDlg::OnBnClickedModel()
{
	/*按钮功能有问题*/
}

void CPLAYERDlg::OnTimer(UINT nIDEvent) 
{	
	CString stime;
	second++;
	CClientDC dc(this);
	dc.SetBkColor(RGB(29,0,0));
	dc.SetTextColor(RGB(215,249,9));
    if(second==60)//设置钟表的显示
	{minute++;second=0;
	}
	if(minute==60)
	{hour++;minute=0;
	}
	if(hour==24)
	{hour=0;
	}
	stime.Format("%02d:%02d:%02d",hour,minute,second);//显示时间进度
	dc.TextOut(0,40,stime);
	m_progress.SetRange(0,voiceLength);//使进度条的范围与歌曲时间一致

	if(voiceFrom != voiceLength)//如果歌曲结束，则进度条重新置0
	{
		m_progress.SetPos(voiceFrom);
	}
	else {
		m_progress.SetPos(0);
		stop();
	}
    DWORD cdf=getinfo(MCI_STATUS_POSITION);
    voiceFrom=MCI_MAKE_MSF(MCI_MSF_MINUTE(cdf),MCI_MSF_SECOND(cdf),
    MCI_MSF_FRAME(cdf));//获取当前播放文件的信息
	UpdateData(false);
	CDialog::OnTimer(nIDEvent);
	if(getinfo(MCI_STATUS_POSITION)==voiceLength) 
	{
		switch(MODEL)
		{
		case 0:
			single();
			break;
		case 1:
			OnNext();
			break;
		case 2:
			stockMatic();
			break;
		}
	}
}




void CPLAYERDlg::OnOutofmemorySlider(NMHDR* pNMHDR, LRESULT* pResult) 
{
    for (int j = 10; j <=  90;)
	{
		m_progress.SetTic(j);
		j += 10;
	 }
	
	 if(m_list.GetCurSel() == -1)
	 {
		 return;
	 }
}


void CPLAYERDlg::OnCustomdrawSlider(NMHDR* pNMHDR, LRESULT* pResult) 
{
	
	UpdateData(true);
	m_vol=m_slider.GetPos()/10;
	setvolume(m_slider.GetPos());
	UpdateData(false);
	
	*pResult = 0;
}

void CPLAYERDlg::OnChangevol() 
{
	
	 for (int j = 10; j <=  90;)
	{
		m_progress.SetTic(j);
		j += 10;
	 }

	  DWORD m_cur = m_progress.GetPos();
	  mciplayparms.dwFrom = MCI_MAKE_HMS(MCI_HMS_HOUR(m_cur), MCI_HMS_MINUTE(m_cur), MCI_HMS_SECOND(m_cur));
	  UpdateData(false);
	
}

void CPLAYERDlg::stop() 
{
	KillTimer(0);
	voiceFrom = MCI_MAKE_HMS(0, 0, 0);//定位歌曲的起始位置
	mciSendCommand(m_deviceid, MCI_CLOSE, 0, NULL);

	m_flag = true; //允许播放

	GetDlgItem(IDC_PLAY) -> SetWindowText(_T("播放"));

	hour = 0;
	minute = 0;
	second = 0;
}



void CPLAYERDlg::OnEnChangefilename()
{
}

void CPLAYERDlg::exit()
{
	stop();
    OnOK();
}

void CPLAYERDlg::mini()
{
	OnPop();
}

void CPLAYERDlg::addSong()
{
	OnOpen();
}

void CPLAYERDlg::m_single()
{
	MODEL = 0;
	SetDlgItemText(IDC_MODELDESIGN,_T("顺序"));
}

void CPLAYERDlg::m_que()
{
	MODEL = 1;
	SetDlgItemText(IDC_MODELDESIGN,_T("随机"));
}

void CPLAYERDlg::m_random()
{
	MODEL = 2;
	SetDlgItemText(IDC_MODELDESIGN,_T("单曲"));
}

void CPLAYERDlg::m_help()
{
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}

void CPLAYERDlg::OnBnClickedModeldesign()
{
	CString strtemp;
	GetDlgItemText(IDC_MODELDESIGN,strtemp);//获取按钮状态

	if (strtemp.Compare(_T("单曲"))==0)
	{
		MODEL = 0;
		SetDlgItemText(IDC_MODELDESIGN,_T("顺序"));
	}
	else if(strtemp.Compare(_T("顺序"))==0)
	{
		MODEL = 1;
		SetDlgItemText(IDC_MODELDESIGN,_T("随机"));
	}
	else if(strtemp.Compare(_T("随机"))==0)
	{
		MODEL = 2;
		SetDlgItemText(IDC_MODELDESIGN,_T("单曲"));
	}
}

void CAboutDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CPLAYERDlg::OnNMCustomdrawProgress(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	
	*pResult = 0;
}

//最小化窗口
void CPLAYERDlg::OnBnClickedMim()
{
	CloseWindow();
}


LRESULT CAboutDlg::OnNcHitTest(CPoint pt)
{
	CRect rc;  
    GetClientRect(&rc);  
    ClientToScreen(&rc);  
	 return rc.PtInRect(pt) ? HTCAPTION : CDialog::OnNcHitTest(pt); 
}
