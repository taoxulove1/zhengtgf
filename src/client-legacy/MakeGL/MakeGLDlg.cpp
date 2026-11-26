// MakeGLDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MakeGL.h"
#include "MakeGLDlg.h"
#include ".\makegldlg.h"

#include "..\LibPack\QMakeGL.h"
#include "..\LibPack\QCanvas.h"
#include <string>
#include <vector>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CMakeGLDlg 对话框



CMakeGLDlg::CMakeGLDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMakeGLDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMakeGLDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMakeGLDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_EMPTY, OnBnClickedEmpty)
	ON_BN_CLICKED(IDC_ADDDIR, OnBnClickedAdddir)
	ON_BN_CLICKED(IDC_CONFIRM, OnBnClickedConfirm)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_ADDDIRLIST, OnBnClickedAdddirlist)
END_MESSAGE_MAP()


// CMakeGLDlg 消息处理程序

BOOL CMakeGLDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将\“关于...\”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	CComboBox* tpBlendList = (CComboBox*)GetDlgItem( IDC_BLEND );	 
	tpBlendList->AddString("Blend_Null");
	tpBlendList->AddString("Blend_Add");
	tpBlendList->AddString("Blend_Add2");
	tpBlendList->AddString("Blend_AlphaAdd");
	tpBlendList->AddString("Blend_AlphaAdd2");
	tpBlendList->AddString("Blend_Bright");
	tpBlendList->AddString("Blend_IgnoreTexelColor");
	tpBlendList->AddString("Blend_ColorAdd");
	tpBlendList->AddString("Blend_AlphaAdd3");
	tpBlendList->AddString("Blend_AlphaSub");
	tpBlendList->AddString("Blend_Monochrome");
	tpBlendList->AddString("Blend_AlphaAdd4");
	tpBlendList->AddString("Blend_MulAlpha");
	tpBlendList->AddString("Blend_MulInvAlpha");
	tpBlendList->AddString("Blend_Add3");
	tpBlendList->AddString("Blend_MulInvColor");
	tpBlendList->AddString("Blend_AlphaAdd5");
	tpBlendList->AddString("Blend_MulColor");
	tpBlendList->SetCurSel( 0 );

	CEdit* tpSpeed = (CEdit*)GetDlgItem( IDC_SPEED );
	tpSpeed->SetWindowText( "200" );

	CEdit* tpColor = (CEdit*)GetDlgItem( IDC_COLOR );
	tpColor->SetWindowText( "0xffffffff" );

	CComboBox* tpPlayType = (CComboBox*)GetDlgItem( IDC_PLAYTYPE );
	tpPlayType->AddString("AlphaType_None");
	tpPlayType->AddString("AlphaType_AllBlack");
	tpPlayType->AddString("AlphaType_AllWhite");
	tpPlayType->AddString("AlphaType_OneOnly");
	tpPlayType->SetCurSel( 0 );

	GetQCanvas( )->Init( m_hWnd );	
	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}

void CMakeGLDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMakeGLDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
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
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
HCURSOR CMakeGLDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMakeGLDlg::OnBnClickedEmpty()
{
	CListBox* tpDirList = (CListBox*)GetDlgItem( IDC_DIRLIST );
	for( int i = tpDirList->GetCount( ); i >= 0 ; i -- )
	{
		tpDirList->DeleteString( i );
	}
}

void CMakeGLDlg::OnBnClickedAdddir()
{
	BROWSEINFO  tBrowseInfo;   
	ZeroMemory(&tBrowseInfo, sizeof(tBrowseInfo));   
	tBrowseInfo.hwndOwner	= m_hWnd;   	  
	tBrowseInfo.lpszTitle	= _T("所选择路径:");   
	tBrowseInfo.ulFlags		= BIF_RETURNONLYFSDIRS;           
		
	TCHAR tPath[MAX_PATH] = { 0 }; 
	// 用来保存返回信息的IDList，使用SHGetPathFromIDList函数转换为字符串   
	LPITEMIDLIST tpItemIDlist =  SHBrowseForFolder( &tBrowseInfo );   //显示选择对话框   
	if( tpItemIDlist != NULL )   
	{   
		SHGetPathFromIDList( tpItemIDlist, tPath );//把项目标识列表转化成目录
		CListBox* tpDirList = (CListBox*)GetDlgItem( IDC_DIRLIST );
		tpDirList->AddString( tPath );
	}
}

// ============================================================================
void AdjustDir( char* pDirName )
{
	size_t tLength = ::strlen( pDirName );
	if( pDirName[ tLength - 1] != '\\' )	
	{
		pDirName[ tLength ]			= '\\';
		pDirName[ tLength + 1 ]		= 0;
	}
}


void CMakeGLDlg::OnBnClickedConfirm()
{	
	CListBox* tpDirList = (CListBox*)GetDlgItem( IDC_DIRLIST );
	if( tpDirList->GetCount( ) == 0 ) return;

	CString tDstDir;
	CEdit* tpDstDirWnd = (CEdit*)GetDlgItem( IDC_DSTDIR );
	tpDstDirWnd->GetWindowText( tDstDir );

	for( int i = 0; i < tpDirList->GetCount( ); i ++ )
	{
		char tDirName[MAX_PATH] = { 0 };
		tpDirList->GetText( i, tDirName );

		MakeGL( tDirName, 0, 0, 0 );
		if( !tDstDir.IsEmpty( ) )
		{
			AdjustDir( tDirName );
			size_t tIdx = ::strlen(tDirName) - 1;
			tDirName[tIdx] = 0;
			strcat( tDirName, ".pak" );

			char* tp1 = ::strrchr( tDirName, '\\' );			

			char tNewName[MAX_PATH] = { 0 };
			::strcpy( tNewName, tDstDir.GetBuffer( ) );
			::strcat( tNewName, tp1 );
			::CopyFile( tDirName, tNewName, false );
		}
	}

	

	MessageBox( "打包完成!", "信息" );
}

void CMakeGLDlg::OnBnClickedButton1()
{
	BROWSEINFO  tBrowseInfo;   
	ZeroMemory(&tBrowseInfo, sizeof(tBrowseInfo));   
	tBrowseInfo.hwndOwner	= m_hWnd;   	  
	tBrowseInfo.lpszTitle	= _T("所选择路径:");   
	tBrowseInfo.ulFlags		= BIF_RETURNONLYFSDIRS;           
		
	TCHAR tPath[MAX_PATH] = { 0 }; 
	// 用来保存返回信息的IDList，使用SHGetPathFromIDList函数转换为字符串   
	LPITEMIDLIST tpItemIDlist =  SHBrowseForFolder( &tBrowseInfo );   //显示选择对话框   
	if( tpItemIDlist != NULL )   
	{   
		SHGetPathFromIDList( tpItemIDlist, tPath );//把项目标识列表转化成目录
		CEdit* tpDstDir = (CEdit*)GetDlgItem( IDC_DSTDIR );
		tpDstDir->SetWindowText( tPath );
	}
}


// ============================================================================
void GetDirList( std::vector<std::string>& rOutDirList,
				const char* pDir,
				bool vIsAllPath )
{	
	char tPath[MAX_PATH];
	::strcpy( tPath, pDir );
	AdjustDir( tPath );
	::strcat( tPath, "*.*" );

	WIN32_FIND_DATA tFindData;	
	HANDLE hFind = FindFirstFile( tPath, &tFindData );
	if( hFind == INVALID_HANDLE_VALUE ) return;	
	
	do
	{
		if( (tFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
			(tFindData.cFileName[0] != '.') )
		{
			if( vIsAllPath )
			{
				char tAllPath[MAX_PATH] = { 0 };
				::strcpy( tAllPath, pDir );
				AdjustDir( tAllPath );
				::strcat( tAllPath, tFindData.cFileName );

				rOutDirList.push_back( tAllPath );
			}
			else
			{
				rOutDirList.push_back( tFindData.cFileName );
			}
		}

	}while( FindNextFile( hFind, &tFindData ) );
}

void CMakeGLDlg::OnBnClickedAdddirlist()
{
	BROWSEINFO  tBrowseInfo;   
	ZeroMemory(&tBrowseInfo, sizeof(tBrowseInfo));   
	tBrowseInfo.hwndOwner	= m_hWnd;   	  
	tBrowseInfo.lpszTitle	= _T("所选择目录:");   
	tBrowseInfo.ulFlags		= BIF_RETURNONLYFSDIRS;           
		
	TCHAR tPath[MAX_PATH] = { 0 }; 
	// 用来保存返回信息的IDList，使用SHGetPathFromIDList函数转换为字符串   
	LPITEMIDLIST tpItemIDlist =  SHBrowseForFolder( &tBrowseInfo );   //显示选择对话框   
	if( tpItemIDlist != NULL )   
	{   
		SHGetPathFromIDList( tpItemIDlist, tPath );//把项目标识列表转化成目录
		CListBox* tpDirList = (CListBox*)GetDlgItem( IDC_DIRLIST );

		std::vector<std::string> tDirList;
		GetDirList( tDirList, tPath, true );
		for( size_t i = 0; i < tDirList.size( ); i ++ )
		{
			tpDirList->AddString( tDirList[i].c_str( ) );
		}		
	}
}
