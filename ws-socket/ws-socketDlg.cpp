
// ws-socketDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "ws-socket.h"
#include "ws-socketDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CwssocketDlg 对话框



CwssocketDlg::CwssocketDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_WSSOCKET_DIALOG, pParent)
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CwssocketDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CwssocketDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CwssocketDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CwssocketDlg 消息处理程序

BOOL CwssocketDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CwssocketDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CwssocketDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CwssocketDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

#include "easyws.h"
using easywsclient::WebSocket;

namespace
{

	class CWs
	{
	public:
		CWs() {
			pp = 0;
		}
		~CWs() {

			if (pp)
			{
				pp->closesk();
			}
		}
		//std::unique_ptr<WebSocket> d_ws; assert(d_ws);
		PVOID SetPtr()
		{
			pp = WebSocket::from_url("ws://192.168.77.180:9605");
			return pp;
		}

		void wssend(string& str1)
		{
			if (pp)
				pp->send(str1);
		}

		int GetState()
		{
			if (pp)
				return pp->getReadyState();
			return -1;
		}
		WebSocket *pp;
	};
}

CWs *g_pWs = 0;
extern int ws_Parse(const char* sbuf);
void __stdcall ws_RecvCallB(std::vector<uint8_t> rbuf)
{
	std::vector<uint8_t> sv = rbuf;
	/*string sjson = BV2S(sv);
	if (sjson.size() > 0)
		ws_Parse(sjson.c_str());*/
}
DWORD WINAPI ws_RecvThread(void * lp)
{
	int ntype = g_pWs->GetState();
	while (g_pWs->pp && ntype != WebSocket::CLOSED)
	{
		g_pWs->pp->poll();
		Sleep(1000);
	}
	return 0;
}



static void wsStart()
{
	if (!g_pWs)
		g_pWs = new CWs();
	if (g_pWs->SetPtr())
	{
		WriteLog("ws 建立连接成功");
		MessageBoxW(0, L"ws 建立连接成功", 0, 0);
		g_pWs->pp->SetCallbk(ws_RecvCallB);
	     CreateThread(NULL, 0, ws_RecvThread, 0, 0, NULL);
	}
}

void CwssocketDlg::OnBnClickedButton1()
{
	wsStart();
}
