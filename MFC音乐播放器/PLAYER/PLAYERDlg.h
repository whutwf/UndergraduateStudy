// PLAYERDlg.h : header file
//
#include "afxwin.h"
#pragma comment(lib,"winmm.lib")
#if !defined(AFX_PLAYERDLG_H__FD807529_37CE_4A44_A90E_5D624B3E7D46__INCLUDED_)
#define AFX_PLAYERDLG_H__FD807529_37CE_4A44_A90E_5D624B3E7D46__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////


class CPLAYERDlg : public CDialog
{
// Construction
public:
	CPLAYERDlg(CWnd* pParent = NULL);	// standard constructor

	void stockMatic();
	void single();

// Dialog Data
	//{{AFX_DATA(CPLAYERDlg)
	enum { IDD = IDD_PLAYER_DIALOG };
	CSliderCtrl	m_slider;
	CSliderCtrl	m_progress;
	CButton	m_stop;
	CButton	m_sdel;
	CButton	m_pop;
	CButton	m_play;
	CButton	m_per;
	CButton	m_open;
	CButton	m_next;
	CListBox	m_list;
	CButton	m_bhelp;
	CButton	m_bexit;
	int		m_vol;
	//}}AFX_DATA


	protected:
		void  stop();

	    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

protected:

    CMenu m_Menu;
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CPLAYERDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg DWORD getinfo(DWORD item);
	afx_msg void OnOpen();
	afx_msg void OnStop();
	afx_msg void OnPlay();
	afx_msg void OnPer();
	afx_msg void OnNext();
	afx_msg void OnPop();
	afx_msg void OnSdel();
	afx_msg void OnBexit();
	afx_msg void OnBhelp();
	afx_msg void OnBmim();
	afx_msg void OnDblclkList();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg  int Index(CString aa);
	afx_msg void OnOutofmemorySlider(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCustomdrawSlider(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangevol();

	afx_msg void exit();
	afx_msg void mini();
	afx_msg void m_single();
	afx_msg void m_help();
	afx_msg void m_random();
	afx_msg void m_que();
	afx_msg void addSong();


	//afx_msg void OnSetVolume();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangefilename();
	CButton m_min;
	afx_msg void OnBnClickedModel();
	afx_msg void OnBnClickedModel2();
	afx_msg void OnBnClickedModeldesign();
	afx_msg void OnNMCustomdrawProgress(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedMim();
	CButton m_mini;
};

#endif 
