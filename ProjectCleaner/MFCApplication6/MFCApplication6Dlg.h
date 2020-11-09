
// MFCApplication6Dlg.h: 头文件
//

#pragma once


// CMFCApplication6Dlg 对话框
class CMFCApplication6Dlg : public CDialogEx
{
// 构造
public:
	CMFCApplication6Dlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCAPPLICATION6_DIALOG };
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
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	CEdit m_edit1;
	TCHAR szPath[MAX_PATH];
	TCHAR szFilePath[MAX_PATH];
	afx_msg void OnBnClickedButton2();
	BOOL DeleteTargetFolder(LPCTSTR lpstrPath);
	BOOL isDeleteFile(LPCTSTR lpszfilepath);
	afx_msg void OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	void GetThePrivilege();
	BOOL SetPrivilege(HANDLE hToken, LPCTSTR lpszPrivilege, BOOL bEnablePrivilege);
	DWORD GetCurrentFolder(DWORD nBufferLength, LPTSTR lpBuffer);
	bool List_Find(CString str);
};
