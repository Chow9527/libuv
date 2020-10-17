
// BaiduAPIDlg.h: 头文件
//

#pragma once
#include "mHttpClient.h"
#include <string>

// CBaiduAPIDlg 对话框

class CBaiduAPIDlg : public CDialogEx
{
// 构造
public:
	CBaiduAPIDlg(CWnd* pParent = nullptr);	// 标准构造函数
	~CBaiduAPIDlg();
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BAIDUAPI_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnMessage(WPARAM wp,LPARAM lp);
	DECLARE_MESSAGE_MAP()
private:
	CEdit M_API_KEY;
	CEdit M_SECRT_KEY;
	CEdit M_IMAGE_PATH;
	mHttpClient* HttpClient;
	char* UTF82Char(const char* szU8);
	static void thread_Proc(std::string token, std::string filepath, HWND* mHand);
public:
	afx_msg void OnBnClickedButtonChoose();
	afx_msg void OnBnClickedButtonGettokenbtn();
	static CBaiduAPIDlg* pThis;
private:
	CEdit M_TOKEN;
	CEdit M_LOG;
	CButton M_NOM;
public:
	afx_msg void OnBnClickedButtonUpdatetokenbtn();
};
