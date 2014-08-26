// CLientDlg.h : header file
//

#if !defined(AFX_CLIENTDLG_H__F70EECB3_2FBD_48FE_A526_1FB39BC6447C__INCLUDED_)
#define AFX_CLIENTDLG_H__F70EECB3_2FBD_48FE_A526_1FB39BC6447C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CCLientDlg dialog
#include <Winsock2.h>
class CCLientDlg : public CDialog
{
// Construction
public:
	CCLientDlg(CWnd* pParent = NULL);	// standard constructor
public:
	SOCKET  m_hSocket;
	CString m_strFileName;
// Dialog Data
	//{{AFX_DATA(CCLientDlg)
	enum { IDD = IDD_CLIENT_DIALOG };
	CStatic	m_cState;
	CProgressCtrl	m_progress;
	CIPAddressCtrl	m_cSvrIP;
	CString	m_strPathName;
	int		m_iPort;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCLientDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	void  InitSock();
	

	// Generated message map functions
	//{{AFX_MSG(CCLientDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	afx_msg void OnButtonBrowser();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLIENTDLG_H__F70EECB3_2FBD_48FE_A526_1FB39BC6447C__INCLUDED_)
