
// ReadModeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ReadMode.h"
#include "ReadModeDlg.h"
#include "afxdialogex.h"

#include <GL/glew.h>
#include <GL/glut.h>
#include "Resource.h"
#include <math.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu);//add
};
CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}
BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_WM_MENUSELECT()
END_MESSAGE_MAP()


// CReadModeDlg dialog

CReadModeDlg::CReadModeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CReadModeDlg::IDD, pParent)
	, m_infoBox(_T("YOYO"))
{
	protein1=new ProteinFrame();
	protein2=new ProteinFrame();
	protView=new ProteinView();
	//m_info.SetWindowText((LPCTSTR)"bitch");
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}
void CReadModeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_ComboMove);
	DDX_CBString(pDX, IDC_COMBO1, m_infoBox);

	DDX_Control(pDX, IDC_INFO, m_info);
}
BEGIN_MESSAGE_MAP(CReadModeDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_READM, &CReadModeDlg::OnBnClickedReadm)
	ON_BN_CLICKED(IDC_MOVEM, &CReadModeDlg::OnBnClickedMovem)
	ON_BN_CLICKED(IDC_ROATM, &CReadModeDlg::OnBnClickedRoatm)
	ON_BN_CLICKED(IDC_UPDATE, &CReadModeDlg::OnBnClickedUpdate)
	
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
//	ON_WM_SIZE()
	ON_COMMAND(IDC_ROTATION, &CReadModeDlg::OnRotation)
	ON_BN_CLICKED(IDC_SHOW2, &CReadModeDlg::OnBnClickedShow2)
	ON_BN_CLICKED(IDC_READM2, &CReadModeDlg::OnBnClickedReadm2)
	ON_BN_CLICKED(IDC_SHOW1, &CReadModeDlg::OnBnClickedShow)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CReadModeDlg::OnCbnSelchangeCombo1)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CReadModeDlg message handlers

BOOL CReadModeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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
		m_infobox_handle=GetDlgItem(IDC_INFO);
		
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	m_ComboMove.SetCurSel(0);
	m_move=0;
	protein1->LoadFrame(GetDlgItem(IDC_SHOW1));
	protein2->LoadFrame(GetDlgItem(IDC_SHOW2));
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}
void CReadModeDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CReadModeDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

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
		CDialogEx::OnPaint();
	}
}
// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CReadModeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CReadModeDlg::OnTimer(UINT_PTR nIDEvent)
{	
	
	// TODO: Add your message handler code here and/or call default
	if(protein1->m_read==true)
	{
		protein1->Draw();

	}
	if(protein2->m_read==true)
	{
		protein2->Draw();

	}
	//setTimer(1,100,NULL);
	CDialogEx::OnTimer(nIDEvent);
}
void CReadModeDlg::OnBnClickedReadm()
{
	// TODO: Add your control notification handler code here
	CFileDialog *pDlg=new CFileDialog(true);
	if(pDlg->DoModal()!=IDOK)
		return;
	protein1->LoadProtein(CW2A(pDlg->GetPathName()));
	protein1->Draw();
	m_infobox_handle->SetWindowTextW(_T(""));
	//setTimer(1,100,NULL);
}
void CReadModeDlg::OnBnClickedMovem()
{
	// TODO: Add your control notification handler code here
	if (protein1->m_read==false)
	{
		//AfxMessageBox(_T("请先读取物体!"));
		m_infobox_handle->SetWindowTextW(_T("Please load an object first"));
		return;
	} 

	protein1->m_move=true;
	protein1->m_AutoRotation=false;
	protein1->m_rotation=false;

}
void CReadModeDlg::OnBnClickedRoatm()
{
	// TODO: Add your control notification handler code here
	if (protein1->m_read==false)
	{
		//AfxMessageBox(_T("请先读取物体!"));
		m_infobox_handle->SetWindowTextW(_T("Please load an object first")); //if no _T writes in chinese
		return;
	} 
	
	protein1->m_rotation=true;
	protein1->m_AutoRotation=true;
	protein1->m_move=false;
	
}
void CReadModeDlg::OnBnClickedUpdate()
{
	//give a stop order
	// TODO: Add your control notification handler code here
	protein1->m_AutoRotation=false;
	protein1->m_rotation=false;
	protein1->m_move=false;
	//this->UpdateData(true);
}
void CReadModeDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (protein1->m_move)
	{
		protein1->m_LDown=true;
		m_MouseL=point;
	}
	if (protein1->m_rotation)
	{
		protein1->m_rot=true;
		m_MouseL=point;
	}
	
	CDialogEx::OnLButtonDown(nFlags, point);
}
void CReadModeDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	protein1->m_LDown=false;
	protein1->m_rot=false;
	CDialogEx::OnLButtonUp(nFlags, point);
}
void CReadModeDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	if (protein1->m_LDown)
	{
		if ((GetAsyncKeyState(VK_LBUTTON) & 0x8000))//(GetAsyncKeyState(VK_CONTROL) & 0x8000) &&
		{
			protein1->m_x+=0.5*(point.x-m_MouseL.x);
			protein1->m_y+=0.5*(m_MouseL.y-point.y);
			m_MouseL=point;
			//protein1->Draw();
		}
	}
	if (protein1->m_rot)
	{

		if ((GetAsyncKeyState(VK_LBUTTON) & 0x8000))
		{
			if ((point.x - m_MouseL.x) != 0)
			{
				protein1->m_rot_y = protein1->m_rot_y + ((float)ROTATE_SPEED * (point.x - m_MouseL.x));
			}

			if ((point.y - m_MouseL.y) != 0)
			{	
				protein1->m_rot_x = protein1->m_rot_x + ((float)ROTATE_SPEED * (point.y - m_MouseL.y));
			}
			//protein1->Draw();
		}
	}
	CDialogEx::OnMouseMove(nFlags, point);
}
BOOL CReadModeDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default
	if(protein1->m_read=true&&(m_move==1 || m_move==2) ){
		protein1->m_z+=zDelta/120;
		//protein1->Draw();
	}
	if(protein2->m_read=true&&(m_move==1 || m_move==3)){
		protein2->m_z+=zDelta/120;
		//protein2->Draw();
	}
	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}
void CReadModeDlg::OnRotation()
{
	// TODO: Add your command handler code here
	if (protein1->m_read==false)
	{
		//AfxMessageBox(_T("Please load an object first!"));
		m_infobox_handle->SetWindowTextW(_T("Please load an object first"));	
		return;
	} 
	protein1->m_AutoRotation=true;
	protein1->m_move=false;
	protein1->m_rotation=false;

}



void CAboutDlg::OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu)
{
	CDialogEx::OnMenuSelect(nItemID, nFlags, hSysMenu);

	// TODO: Add your message handler code here
	TCHAR szFullText[256];
	CString cstStatusText;
	//分隔符项其命令ID为0
	if(nItemID!=0)
	{
		//菜单命令的ID而非菜单项的索引
		AfxLoadString(nItemID,szFullText);
		AfxExtractSubString(cstStatusText,szFullText,0,'\n');
		//根据菜单项的提示文字设置状态栏信息
		//m_wndStatusBar.SetPaneText(0,cstStatusText);
	}
}

void CReadModeDlg::OnBnClickedShow()
{
	m_move=1;
	// TODO: Add your control notification handler code here
}
void CReadModeDlg::OnBnClickedShow2()
{
	// TODO: Add your control notification handler code here
	m_move=2;
}


void CReadModeDlg::OnBnClickedReadm2()
{
	// TODO: Add your control notification handler code here
	CFileDialog *pDlg=new CFileDialog(true);
	if(pDlg->DoModal()!=IDOK)
		return;
	protein2->LoadProtein(CW2A(pDlg->GetPathName()));
	protein2->Draw();
	//setTimer(1,100,NULL);
	m_infobox_handle->SetWindowTextW(_T(""));

}


void CReadModeDlg::OnCbnSelchangeCombo1()
{
	// TODO: Add your control notification handler code here
	int choice=m_ComboMove.GetCurSel();
	switch(choice){
	case 1://move both views
		m_move=1;
		break;
	case 2://move protein1 view
		//protView->ChangeView(GetDlgItem(IDC_SHOW1),protein1);
		m_move=2;
		break;
	case 3://move protein2 view
		m_move=3;
		break;
	default: 
		m_move=0;
		break;
	}
	//Set focus back on main window
	::SetFocus(::GetActiveWindow());
}


HBRUSH CReadModeDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
	UINT id = pWnd->GetDlgCtrlID();

	if (id == IDC_INFO)
		pDC->SetTextColor(RGB(255,0,0));
	// TODO:  Return a different brush if the default is not desired
	return hbr;
}
