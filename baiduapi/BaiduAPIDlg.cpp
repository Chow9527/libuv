
// BaiduAPIDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "BaiduAPI.h"
#include "BaiduAPIDlg.h"
#include "afxdialogex.h"
#include <string>
#include <thread>
#include <future>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBaiduAPIDlg 对话框

using namespace std;

 CBaiduAPIDlg* CBaiduAPIDlg::pThis = nullptr;

 extern "C" { FILE __iob_func[3] = { *stdin,*stdout,*stderr }; }

CBaiduAPIDlg::CBaiduAPIDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_BAIDUAPI_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}


CBaiduAPIDlg::~CBaiduAPIDlg()
{
	if (HttpClient!=nullptr)
	{
		delete HttpClient;
		HttpClient = nullptr;
	}
}

void CBaiduAPIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_API_KEY, M_API_KEY);
	DDX_Control(pDX, IDC_EDIT_SECRET_KEY, M_SECRT_KEY);
	DDX_Control(pDX, IDC_EDIT_IMAGE_PATH, M_IMAGE_PATH);
	DDX_Control(pDX, IDC_EDIT_TOKEN, M_TOKEN);
	DDX_Control(pDX, IDC_EDIT_LOG, M_LOG);
	DDX_Control(pDX, IDC_RADIO1, M_NOM);
}

BEGIN_MESSAGE_MAP(CBaiduAPIDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_CHOOSE, &CBaiduAPIDlg::OnBnClickedButtonChoose)
	ON_BN_CLICKED(IDC_BUTTON_GETTOKENBTN, &CBaiduAPIDlg::OnBnClickedButtonGettokenbtn)
	ON_MESSAGE(WM_USER + 1001,&OnMessage)
	ON_BN_CLICKED(IDC_BUTTON_UPDATETOKENBTN, &CBaiduAPIDlg::OnBnClickedButtonUpdatetokenbtn)
END_MESSAGE_MAP()


// CBaiduAPIDlg 消息处理程序

BOOL CBaiduAPIDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	M_API_KEY.SetWindowTextW(L"9Ly1um8lSakT3ldQDb1Ccbxe");
	M_SECRT_KEY.SetWindowTextW(L"g66qiKzc0xtxsUUQF0vCgtGAt1EIeCXE");
	M_NOM.SetCheck(true);
	pThis = this;
	HttpClient = new mHttpClient;
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CBaiduAPIDlg::OnPaint()
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
HCURSOR CBaiduAPIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


LRESULT CBaiduAPIDlg::OnMessage(WPARAM wp, LPARAM lp)
{
	string* str = (string*)wp;
	M_LOG.SetWindowTextW(_bstr_t(UTF82Char(str->c_str())));
	return 0;
}


char* CBaiduAPIDlg::UTF82Char(const char* szU8)
{
	int wcsLen = MultiByteToWideChar(CP_UTF8, NULL, szU8, (int)strlen(szU8), NULL, 0);
	wchar_t* wszString = new wchar_t[wcsLen + 1];
	MultiByteToWideChar(CP_UTF8, NULL, szU8, (int)strlen(szU8), wszString, wcsLen);
	wszString[wcsLen] = '\0';
	int len = WideCharToMultiByte(CP_ACP, 0, wszString, (int)wcslen(wszString), NULL, 0, NULL, NULL);
	char* c = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, wszString, (int)wcslen(wszString), c, len, NULL, NULL);
	c[len] = '\0';
	delete[] wszString;
	return c;
}



void  CBaiduAPIDlg::thread_Proc(string token,string filepath,HWND* mHand)
{
	string data_str = pThis->HttpClient->mDoPostRequest(token.c_str(), filepath.c_str());
	SendMessageA(*mHand,WM_USER+1001,(WPARAM)&data_str,0);
}

void CBaiduAPIDlg::OnBnClickedButtonChoose()
{
	// TODO: 在此添加控件通知处理程序代码
	WCHAR token[512] = { 0 };
	M_TOKEN.GetWindowTextW(token, sizeof(token));
	CFileDialog    dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("jpg文件 (*.jpg)|*.jpg|png文件 (*.png)|*.png|bmp文件 (*.bmp)|*.bmp|jpeg文件 (*.jpeg)|*.jpeg||"), NULL);
	if (dlgFile.DoModal())
	{		
		CString filename = dlgFile.GetPathName();
		if (filename.GetLength()>0)
		{
			M_IMAGE_PATH.SetWindowTextW(_bstr_t(filename));
			//使用future.get会阻塞直到获取到结果
			//future<string> datastr = async(launch::async,thread_Proc, string(_bstr_t(token)),string(_bstr_t(filename)));		
			thread th_proc(thread_Proc, string(_bstr_t(token)), string(_bstr_t(filename)), &this->m_hWnd);
			th_proc.detach();
		}
	}	
}


void CBaiduAPIDlg::OnBnClickedButtonGettokenbtn()
{
	// TODO: 在此添加控件通知处理程序代码
	WCHAR buffer_appkey[512] = { 0 };
	WCHAR buffer_seckey[512] = { 0 };
	M_API_KEY.GetWindowTextW(buffer_appkey, sizeof(buffer_appkey));
	M_SECRT_KEY.GetWindowTextW(buffer_seckey, sizeof(buffer_seckey));
	string Token= HttpClient->mGetAccessToken(string(_bstr_t(buffer_appkey)), string(_bstr_t(buffer_seckey)));
	M_TOKEN.SetWindowTextW(_bstr_t(Token.c_str()));
}


void CBaiduAPIDlg::OnBnClickedButtonUpdatetokenbtn()
{
	// TODO: 在此添加控件通知处理程序代码
}
