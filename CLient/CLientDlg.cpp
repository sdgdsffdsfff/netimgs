// CLientDlg.cpp : implementation file
//

#include "StdAfx.h"
#include "CLient.h"
#include "CLientDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCLientDlg dialog
#pragma comment(lib,"Ws2_32")
UINT SendFile(void *p/*窗口指针*/)
{
    

	CString strError;
	ASSERT(p!=NULL);//
	CCLientDlg *pDlg = (CCLientDlg*)p;
	CString strIP;
	pDlg->m_cSvrIP.GetWindowText(strIP);
	sockaddr_in addr;
	addr.sin_family=AF_INET;//表示在INT上通信
	addr.sin_addr.S_un.S_addr=inet_addr(strIP.GetBuffer(0));
	addr.sin_port=htons(pDlg->m_iPort);
	int ret=0;
	int error=0;
	ret=connect(pDlg->m_hSocket,(LPSOCKADDR)&addr,sizeof(addr));//主动连接服务器
	if(ret==SOCKET_ERROR)
	{
		strError.Format("Connect Error:%d ",error=WSAGetLastError());
		AfxMessageBox(strError);
		closesocket(pDlg->m_hSocket);
		return -1;
	}

	//文件操作
	
	CString strPathName = pDlg->m_strPathName;
	CFile file;
	if(!file.Open(strPathName, CFile::modeRead | CFile::typeBinary))
	{
		AfxMessageBox("打开文件失败");
		return -1;
	}
	long lFileSize = file.GetLength();
	pDlg->m_progress.ShowWindow(SW_SHOW);
	pDlg->m_progress.SetRange32(0, lFileSize);	//设置进度条范围
	pDlg->m_progress.SetStep(1);	            //设置进度条步长
	char temp[10000];            	            //首先用来存放文件大小，后面用作发送文件缓冲区
	memset(temp,0,sizeof(temp)); 
	ltoa(lFileSize, temp, 10);                  //转化为字符
	CString str;
	str = pDlg->m_strFileName;
	str += "*";
	str += temp;
	int i=str.GetLength();
	int ret1=send(pDlg->m_hSocket, str, str.GetLength(), 0);	//发送文件的名称和大小
	if (ret1==SOCKET_ERROR)
	{
			strError.Format("Send File:%d",error=WSAGetLastError());
		    AfxMessageBox(strError);
			return -1;
	}
    
	/////////////////////////发送文件///////////////////////////////////////////
	int  iEnd =0;
	int  ilen =0;
	long iTmp =0;
	while (1)
	{
		ilen = file.Read(temp, 10000);
		if (ilen==0)
		{
			break;
		}
		iEnd = send(pDlg->m_hSocket,temp,ilen,0);
		if (iEnd==SOCKET_ERROR)
		{
			strError.Format("Send File:%d",error=WSAGetLastError());
		    AfxMessageBox(strError);
			break;
		}
		iTmp +=iEnd;
		pDlg->m_progress.SetPos(iTmp);
	
	}
	file.Close();
	/////////////////判断发送是否成功、结束处理////////////////////////////////////
	closesocket(pDlg->m_hSocket);
	if (iTmp == lFileSize)
	{
		AfxMessageBox("文件发送成功！");
		
	}
	else
	{
		strError.Format("文件发送失败");
		AfxMessageBox(strError);
	    return 0;
	}
	CString strTemp="发送文件结束";
	pDlg->m_cState.SetWindowText(strTemp);
    //shutdown();
	ExitThread(0);
	return 0;
}
CCLientDlg::CCLientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCLientDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCLientDlg)
	m_strFileName = _T("");
	m_iPort = 8080;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hSocket=NULL;
}

void CCLientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCLientDlg)
	DDX_Control(pDX, IDC_STATIC_STATE, m_cState);
	DDX_Control(pDX, IDC_PROGRESS1, m_progress);
	DDX_Control(pDX, IDC_IPADDRESS1, m_cSvrIP);
	DDX_Text(pDX, IDC_EDIT_FILE, m_strPathName);
	DDX_Text(pDX, IDC_EDIT_NPORT, m_iPort);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCLientDlg, CDialog)
	//{{AFX_MSG_MAP(CCLientDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_BROWSER, OnButtonBrowser)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCLientDlg message handlers

BOOL CCLientDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	WSADATA wsaData;
	WORD version=MAKEWORD(2,0);
	int ret=WSAStartup(version,&wsaData);
	if(ret!=0) 
	{
		MessageBox("Init  Error");
		return FALSE;
	}

	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCLientDlg::OnPaint() 
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
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCLientDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CCLientDlg::OnOK() 
{
	//发送文件。
	// TODO: Add extra validation here
	UpdateData();
	m_hSocket=NULL;
	InitSock();//初始化SOCK。BIND
    CFileFind find;
  
	if(!find.FindFile(m_strPathName))
	{
		MessageBox("文件路径无效","提醒");
		return;
	}

	AfxBeginThread(SendFile, this);
	
}

void CCLientDlg::OnButtonBrowser() 
{
	// TODO: Add your control notification handler code here
	 CFileDialog mFileDlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_ALLOWMULTISELECT,
	"JPG(*.jpg)|*.jpg|位图文件(*.bmp)|*.bmp|所有文件(*.*)|*.*", NULL/*AfxGetMainWnd()*/);
     if(mFileDlg.DoModal()==IDOK)
	 {
      m_strPathName=mFileDlg.GetPathName();
	  m_strFileName=mFileDlg.GetFileName();
	 }
	 UpdateData(false);
}
void CCLientDlg::InitSock()//
{
    

	if(m_hSocket)//如果已经创建，先关闭
	{
		closesocket(m_hSocket);
        m_hSocket=NULL;
	}
	else
	{
		m_hSocket=socket(AF_INET,SOCK_STREAM,0);
		ASSERT(m_hSocket!=NULL);//
	}


}

void CCLientDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	if(m_hSocket)
	{
		closesocket(m_hSocket);

	}
	WSACleanup();
	CDialog::OnCancel();
}
