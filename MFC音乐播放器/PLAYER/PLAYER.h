// PLAYER.h : main header file for the PLAYER application
//

#if !defined(AFX_PLAYER_H__CEE30956_762D_4142_BF6B_98E792AA1E9B__INCLUDED_)
#define AFX_PLAYER_H__CEE30956_762D_4142_BF6B_98E792AA1E9B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CPLAYERApp:
// See PLAYER.cpp for the implementation of this class
//

class CPLAYERApp : public CWinApp
{
public:
	CPLAYERApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPLAYERApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CPLAYERApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLAYER_H__CEE30956_762D_4142_BF6B_98E792AA1E9B__INCLUDED_)
