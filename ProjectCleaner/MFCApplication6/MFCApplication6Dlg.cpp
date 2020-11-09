
// MFCApplication6Dlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MFCApplication6.h"
#include "MFCApplication6Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCApplication6Dlg 对话框



CMFCApplication6Dlg::CMFCApplication6Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCAPPLICATION6_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplication6Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_edit1);

}

BEGIN_MESSAGE_MAP(CMFCApplication6Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCApplication6Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMFCApplication6Dlg::OnBnClickedButton2)

END_MESSAGE_MAP()


// CMFCApplication6Dlg 消息处理程序

BOOL CMFCApplication6Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	this->GetThePrivilege();


	TCHAR szPath1[MAX_PATH] = { 0 };
	GetCurrentFolder(MAX_PATH, szPath1);
	m_edit1.SetWindowText(szPath1);
	SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	
	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCApplication6Dlg::OnPaint()
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
HCURSOR CMFCApplication6Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


int CALLBACK BrowseCallBackProc(HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	if (uMsg == BFFM_INITIALIZED)//初始化选择目录
	{
		::SendMessage(hWnd, BFFM_SETSELECTION, TRUE, lpData);
	}
	return 0;
}
LPITEMIDLIST GetItemIDListFromPath(CString strPath)
{
	if (strPath.IsEmpty())
		return NULL;

	LPSHELLFOLDER pDesktopFolder = NULL;
	HRESULT hr = SHGetDesktopFolder(&pDesktopFolder);
	if (FAILED(hr))
		return NULL;

	PWCHAR pWchar = NULL;
#ifdef _UNICODE
	pWchar = (LPTSTR)(LPCTSTR)strPath;
#else
	USES_CONVERSION;
	pWchar = A2W(strPath);
#endif

	LPITEMIDLIST pItemIDList = NULL;
	hr = pDesktopFolder->ParseDisplayName(NULL, NULL, pWchar, NULL, &pItemIDList, NULL);
	pDesktopFolder->Release();
	if (FAILED(hr))
		return NULL;

	return pItemIDList;
}
void CMFCApplication6Dlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	
	BROWSEINFO mbrowsinfo = { 0 };
	mbrowsinfo.lpszTitle = _T("选择目录");//设置标题
	mbrowsinfo.ulFlags = BIF_EDITBOX;//内部编辑框
	mbrowsinfo.lpfn = BrowseCallBackProc;//回调函数
	mbrowsinfo.lParam = (LPARAM)_T("C:\\Users\\12UE\\Desktop\\测试用项目");//设置默认目录
	//mbrowsinfo.pidlRoot = GetItemIDListFromPath(_T("D:\\"));//限定选择的根目录
	mbrowsinfo.hwndOwner = m_hWnd;//如果不设置句柄就会变成非模态的对话框 设置句柄就会变成模态对话框 这么做是为了防止用户的其他操作
	ITEMIDLIST *pidl = SHBrowseForFolder(&mbrowsinfo);//因为模态对话框的阻塞机制 代码不会继续执行
	if (SHGetPathFromIDList(pidl, szPath)) {
		m_edit1.SetWindowTextW(szPath);
	}
	CoTaskMemFree(pidl);
}


void CMFCApplication6Dlg::OnBnClickedButton2()
{
	DeleteTargetFolder(szPath);
	MessageBox(TEXT("清理完成 请测试能否正常编译"), TEXT("温馨提示"), MB_OK);
}

BOOL CMFCApplication6Dlg::DeleteTargetFolder(LPCTSTR lpstrPath)
{
	CString strRootPath(lpstrPath);
	if (strRootPath[strRootPath.GetLength() - 1] != '\\')
		strRootPath = strRootPath + _T("\\");
	CFileFind mFinder;
	BOOL bFind = mFinder.FindFile(strRootPath + _T("*.*"), 0); //如果指定文件类型的话，只能在本目录中遍历
	while (bFind) {
		bFind = mFinder.FindNextFile();
		if (mFinder.IsDots()) {
			continue;
		}
		else {
			if (mFinder.IsDirectory()) {
				//目标是目录
					DeleteTargetFolder(mFinder.GetFilePath());
			}
			CString strTmp = mFinder.GetFilePath();
			if (isDeleteFile(strTmp))
			{
				//目标是文件
				BOOL bRET=DeleteFile(strTmp);
				if (!bRET)
				{
					int nErrorCode = GetLastError();
					if (nErrorCode == 5||nErrorCode==32)
					{
						SetFileAttributes(strTmp, 0);
						DeleteFile(strTmp);
					}
					else if(nErrorCode!=5||nErrorCode!=32)
					{
						TCHAR szbuffer[128] = { 0 };
						_stprintf(szbuffer, TEXT("错误代码：%d"), GetLastError());
						MessageBox(szbuffer, TEXT("错误提示"), MB_OK);
					}
				}
				
				
			}
		}
		RemoveDirectory(mFinder.GetFilePath());
	}
	mFinder.Close();
return TRUE;
}

BOOL CMFCApplication6Dlg::isDeleteFile(LPCTSTR lpszfilepath)
{

	HANDLE hFile = CreateFile(lpszfilepath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		DWORD dwFileSize = GetFileSize(hFile, NULL);
		CloseHandle(hFile);
		if (dwFileSize > 0)
		{
			CString a = PathFindExtension(lpszfilepath);
			if (List_Find(a))
			{
				return false;
			}
			else
			{
				return TRUE;
			}
		}
		else
		{
			return FALSE;
		}
	}
}




void CMFCApplication6Dlg::GetThePrivilege()
{
	HANDLE hToken;
	OpenProcessToken(GetCurrentProcess(), TOKEN_ALL_ACCESS, &hToken);
	if (!SetPrivilege(hToken, SE_DEBUG_NAME, TRUE))
	{
		CloseHandle(hToken);
		HWND MsgBoxforprivilege = 0;
		MessageBox(TEXT("请以管理员的方式启动！"), TEXT("提示"), MB_OK);

	}
}

BOOL CMFCApplication6Dlg::SetPrivilege(HANDLE hToken, LPCTSTR lpszPrivilege, BOOL bEnablePrivilege)
{
	TOKEN_PRIVILEGES tp;
	LUID luid;
	if (!LookupPrivilegeValue(NULL, lpszPrivilege, &luid))
	{
		MessageBox(TEXT("查找权限值错误"), TEXT("温馨提示"), MB_OK);
		return FALSE;
	}
	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;
	if (bEnablePrivilege)tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	else tp.Privileges[0].Attributes = 0;
	if (!AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), (PTOKEN_PRIVILEGES)NULL, (PDWORD)NULL))
	{
		MessageBox(TEXT("调整权限令牌错误"), TEXT("温馨提示"), MB_OK);
		return FALSE;
	}
	if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)
	{
		MessageBox(TEXT("令牌没有指定的权限"), TEXT("温馨提示"), MB_OK);
		return FALSE;
	}
	return TRUE;
}

DWORD CMFCApplication6Dlg::GetCurrentFolder(DWORD nBufferLength, LPTSTR lpBuffer)
{
	DWORD dwRet = 0;
	CString strPath;
	GetModuleFileName(AfxGetInstanceHandle(), strPath.GetBufferSetLength(MAX_PATH), MAX_PATH);
	strPath.ReleaseBuffer();

	int idx = strPath.ReverseFind('\\');
	if (idx >= 0) {
		strPath = strPath.Left(idx);
	}
	else {
		return 0;
	}

	dwRet = strPath.GetLength();
	_tcsncpy_s(lpBuffer, nBufferLength, strPath, dwRet);
	dwRet = (nBufferLength >= dwRet) ? dwRet : nBufferLength;

	return dwRet;
	return 0;
}

bool CMFCApplication6Dlg::List_Find(CString str)
{
	char *ReserverFileList[100] = {".cpp",".ico",".suo",".h",".dll",".lib",".sln",".vcxproj",".filters",".rc",".user",".rc2",".bmp",".txt",".doc",".html",".mp4",".png",".jpg",".jpeg",".cur",".wav",".dib",".gif",".htm",".hm" };//这里你要填写一个比较大的空间 后面的A2CW才不会越界访问
	int i = 0;
	USES_CONVERSION;
	while (ReserverFileList[i]!=nullptr)
	{
		if ((lstrcmp(str, A2CW(ReserverFileList[i])) == 0))
		{
			return true;
		}
		i++;
	}
	return false;
}


/*
if (lstrcmp(a,TEXT(".ico"))==0)
			{
				CloseHandle(hFile);
				return FALSE;
			}
			else if (a == L".suo")
			{
				CloseHandle(hFile);
				return FALSE;
			}
			else if (a == L".cpp")
			{
				CloseHandle(hFile);
				return FALSE;
			}
			else if (a == L".h")
			{
				CloseHandle(hFile);
				return FALSE;
			}
			else if (a == L".dll")
			{
				CloseHandle(hFile);
				return FALSE;
			}
			else if (a == L".lib")
			{
				CloseHandle(hFile);
				return FALSE;
			}
			else if (a == L".sln")
			{
				CloseHandle(hFile);
				return FALSE;
			}
			else if (a == L".vcxproj")
			{
				CloseHandle(hFile);
				return FALSE;
			}
			else if (a == L".filters")
			{
				CloseHandle(hFile);
				return FALSE;
			}
			else if (a == L".rc")
			{
				CloseHandle(hFile);
				return FALSE;
			}
			else if (a == L".user")
			{
				CloseHandle(hFile);
				return FALSE;
			}
			else if (a == L".rc2")
			{
				CloseHandle(hFile);
				return FALSE;
			}
			else if (a == L".bmp")
			{
				CloseHandle(hFile);
				return FALSE;
			}
			else if (a == L".txt")
			{
				CloseHandle(hFile);
				return FALSE;
			}
			else if (a == L".doc")
			{
				CloseHandle(hFile);
				return FALSE;
			}
			else if (a == L".html")
			{
				CloseHandle(hFile);
				return FALSE;
			}
			else if (a == L".mp4")
			{
				CloseHandle(hFile);
				return FALSE;
			}
			else if (a == L".png")
			{
				CloseHandle(hFile);
				return FALSE;
			}
			else if (a == L".jpg")
			{
				CloseHandle(hFile);
				return FALSE;
			}
			else if (a == L".jpeg")
			{
				CloseHandle(hFile);
				return FALSE;
			}
			else if (a == L".cur")
			{
				CloseHandle(hFile);
				return FALSE;
			}
			else if (a == L".wav")
			{
				CloseHandle(hFile);
				return FALSE;
			}
			else if (a == L".dib")
			{
				CloseHandle(hFile);
				return FALSE;
			}
			else if (a == L".gif")
			{
				CloseHandle(hFile);
				return FALSE;
			}
			else if (a == L".htm")
			{
				CloseHandle(hFile);
				return FALSE;
			}
			else if (a == L".jpe")
			{
				CloseHandle(hFile);
				return FALSE;
			}
			else if (a == L".hm")
			{
				CloseHandle(hFile);
				return FALSE;
			}
			CloseHandle(hFile);
			return FALSE;


*/