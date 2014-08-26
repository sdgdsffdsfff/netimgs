// CLient.h : main header file for the CLIENT application
//

#if !defined(AFX_CLIENT_H__DA0587B1_1578_4A01_919D_EA97B73F61FC__INCLUDED_)
#define AFX_CLIENT_H__DA0587B1_1578_4A01_919D_EA97B73F61FC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCLientApp:
// See CLient.cpp for the implementation of this class
//

class CCLientApp : public CWinApp
{
public:
	CCLientApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCLientApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CCLientApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLIENT_H__DA0587B1_1578_4A01_919D_EA97B73F61FC__INCLUDED_)
