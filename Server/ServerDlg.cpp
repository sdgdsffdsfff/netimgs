// ServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Server.h"
#include "ServerDlg.h"
#include "Picture.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#pragma comment(lib,"Ws2_32")
CServerDlg *pDlg=NULL;
/////////////////////////////////////////////////////////////////////////////
// CServerDlg dialog



UINT ListenThread(void *p)
{
   	
    //准备接受请求
	
	while(1)
	{
		if(!pDlg->bAppend)
		{
			AfxEndThread(0);
			return 0;
		}
		CString strError;
		int error;
		ASSERT(pDlg!=NULL);
		
		SOCKET s=accept(pDlg->m_hSocket,NULL,NULL);
		if(s==SOCKET_ERROR)
		{
			if(pDlg->bAppend)
			{
			 strError.Format("Accept Error:%d ",error=WSAGetLastError());
			 AfxMessageBox(strError);
			 closesocket(pDlg->m_hSocket);
			 return -1;
			}
			else
			{
				break;
			}
		}
		pDlg->RevFile(s);
	}
	return 0;
}



CServerDlg::CServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CServerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CServerDlg)
	m_iPort = 4000;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hSocket=0;

	pBuffer=NULL;
	m_strFilePath="";
	m_iSize=0;
    bAppend=true;

}
CServerDlg::~CServerDlg()
{
	if(pBuffer!=NULL)
	{
	
		delete []pBuffer;
		pBuffer=NULL;
	
	}
}

void CServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CServerDlg)
	DDX_Control(pDX, IDOK, m_cOk);
	DDX_Control(pDX, ID_STOP, m_cStop);
	DDX_Control(pDX, IDC_PROGRESS1, m_progress);
	DDX_Control(pDX, IDC_STATIC_STATE, m_cState);
	DDX_Control(pDX, IDC_STATIC_IMAGE, m_cImage);
	DDX_Text(pDX, IDC_EDIT_PORT, m_iPort);
	DDV_MinMaxInt(pDX, m_iPort, 1024, 9999);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CServerDlg, CDialog)
	//{{AFX_MSG_MAP(CServerDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ID_STOP, OnStop)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServerDlg message handlers

BOOL CServerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);
	WSADATA wsaData;
	WORD version=MAKEWORD(2,0);// Set small icon
	int ret=WSAStartup(version,&wsaData);
	if(ret!=0) 
	{
		MessageBox("Init  Error");
		return FALSE;
	}
	((CButton*)GetDlgItem(IDC_RADIO_BUF))->SetCheck(1);
	// TODO: Add extra initialization here
	pDlg=this;
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CServerDlg::OnPaint() 
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
		ShowPIC();
		ShowPIC(pBuffer,m_iSize);
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CServerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}
void CServerDlg::InitSock()//
{
    

	if(m_hSocket)//如果已经创建，先关闭
	{
		closesocket(m_hSocket);
        m_hSocket=NULL;
	}
	else
	{
		m_hSocket=socket(AF_INET,SOCK_STREAM,0);
		ASSERT(m_hSocket);//
	}

    CString strError;
	m_hSocket;
	sockaddr_in addr;
	addr.sin_family=AF_INET;//表示在INT上通信
	addr.sin_addr.S_un.S_addr=INADDR_ANY;
	addr.sin_port=htons(m_iPort); 
	int ret=0;
	int error=0;
	ret=bind(m_hSocket,(LPSOCKADDR)&addr,sizeof(addr));//绑定
	if(ret==SOCKET_ERROR)
	{
		strError.Format("Bind Error:%d ",error=WSAGetLastError());
		AfxMessageBox(strError);
		closesocket(m_hSocket);
		return;
	}
	ret=listen(m_hSocket,10);//监听
	if(ret==SOCKET_ERROR)
	{
		strError.Format("Listen Error:%d ",error=WSAGetLastError());
		AfxMessageBox(strError);
		closesocket(pDlg->m_hSocket);
		return ;
	}
	else
	{
		m_cState.SetWindowText("服务端已经启动,等待连接中...");
	}
	AfxBeginThread(ListenThread,this);	
    

}

void CServerDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
    bAppend=false;

	if(m_hSocket)
	{
		closesocket(m_hSocket);
       
	}

	WSACleanup();

	CDialog::OnCancel();

}

void CServerDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	bAppend=true;
	InitSock();
	m_cStop.EnableWindow(true);
	m_cOk.EnableWindow(false);
	
}
bool CServerDlg::GetCurSelMode()
{
	if(IsDlgButtonChecked(IDC_RADIO_BUF)) 
	{
		return true;
	}
	else
	{
        return false;
	}
}
void CServerDlg::ShowPIC()
{
	if(m_strFilePath=="") return;
	CDC *pDC=m_cImage.GetDC();
	ASSERT(pDC!=NULL);
	CPicture pic;
	pic.Load(m_strFilePath);
	CRect rect;
	m_cImage.GetClientRect(&rect);
	rect.top+=15;
	rect.left+=5;
	rect.right-=5;
	rect.bottom-=5;
	pic.Render(pDC,rect);

}
void CServerDlg::ShowPIC(char* buf,int iSize)
{

	if(iSize==0||buf==NULL) return;
 	ASSERT(buf!=NULL);
	CDC *pDC=m_cImage.GetDC();
	ASSERT(pDC!=NULL);
	CPicture pic;
	pic.Load(buf,iSize);
	CRect rect;
	m_cImage.GetClientRect(&rect);
	rect.top+=15;
	rect.left+=5;
	rect.right-=5;
	rect.bottom-=5;

	pic.Render(pDC,rect);

}
void CServerDlg::RevFile(const SOCKET& s)
{

	char buffer[1024];
	char temp[1024];
	memset(buffer,0,sizeof(buffer));
	memset(temp,0,sizeof(temp));
	int rcv = recv(s,buffer, 1024, 0);
	for (int i=0; buffer[i]!='*'; i++)
	{
		temp[i] = buffer[i];
	}
	
	temp[i++] = '\0';
	CString strFileName = temp;//得到文件名
	strcpy(temp, buffer + i);
	long lFileSize = atol(temp);//文件大小；
	m_progress.SetRange32(0, lFileSize);	//设置进度条范围
	m_progress.SetStep(1);	//设置进度条步长
	m_progress.ShowWindow(SW_SHOW);
	m_cState.SetWindowText("文件正在接收....");
	if(GetCurSelMode())  //选择了BUFFER模式
	{

        if(pBuffer!=NULL)
		{
		
			delete [] pBuffer;
			pBuffer=NULL;
		}
		pBuffer=new char[lFileSize+1];
		pBuffer[0]='\0';
		char* pBuf=pBuffer;
		long iTemp = 0;
		while (1)
		{
			rcv = recv(s, buffer, 1024, 0);
			if (rcv <= 0)
			{
				break;
			}
			for (int i=0; i<=rcv; i++)
			{
				pBuf[i+iTemp] = buffer[i];
			}
			iTemp+=rcv;	
			m_progress.SetPos(iTemp);
		}
       	if (iTemp!= lFileSize)
		{
			AfxMessageBox("文件接收失败！");
			return ;
		}
       
		ShowPIC( pBuf, lFileSize);
		m_iSize =lFileSize;
	}
	else
	{
		m_strFilePath="c:\\"+strFileName;
		//进度条操作//	
		//接受并保存文件。
		CFile file(m_strFilePath, CFile::modeCreate | CFile::modeWrite);
		long iTemp = 0;
		while (1)
		{
			rcv = recv(s, buffer, 1024, 0);
			if (rcv == 0)
			{
				break;
			}
			file.Write(buffer, rcv);
			iTemp += rcv;
			m_progress.ShowWindow(SW_SHOW);
			m_progress.SetPos(iTemp);
		}
		file.Close();
		if (iTemp != lFileSize)
		{
			AfxMessageBox("文件接收失败！");
		}
	    ShowPIC();
	}
	m_cState.SetWindowText("文件接收成功！");
    shutdown(s,SD_BOTH);
	closesocket(s);

}

void CServerDlg::OnStop() 
{
	// TODO: Add your control notification handler code here
	 bAppend=false;
	  
	// shutdown(m_hSocket,SD_BOTH);

	 closesocket(m_hSocket);
	 m_hSocket=0;
	m_cStop.EnableWindow(false);
	m_cOk.EnableWindow();

}

BOOL CServerDlg::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class

	return CDialog::DestroyWindow();
}
