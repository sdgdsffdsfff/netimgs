// ServerDlg.h : header file
//

#if !defined(AFX_SERVERDLG_H__19A3DA0D_E2A1_4AC7_9276_4870A80B6872__INCLUDED_)
#define AFX_SERVERDLG_H__19A3DA0D_E2A1_4AC7_9276_4870A80B6872__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CServerDlg dialog
#include <Winsock2.h>
class CServerDlg : public CDialog
{
// Construction
public:
	CServerDlg(CWnd* pParent = NULL);	// standard constructor
	~CServerDlg();
	void    InitSock();
	bool    GetCurSelMode();
	void    ShowPIC();
	void    ShowPIC(char* buf,int iSize);
	void    RevFile(const SOCKET& s);
	
public:
    SOCKET  m_hSocket; 
	CString m_strFilePath;
	char*   pBuffer;
	int     m_iSize;

	bool    bAppend;
	

// Dialog Data
	//{{AFX_DATA(CServerDlg)
	enum { IDD = IDD_SERVER_DIALOG };
	CButton	m_cOk;
	CButton	m_cStop;
	CProgressCtrl	m_progress;
	CStatic	m_cState;
	CButton	m_cImage;
	int		m_iPort;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CServerDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CServerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnStop();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVERDLG_H__19A3DA0D_E2A1_4AC7_9276_4870A80B6872__INCLUDED_)
