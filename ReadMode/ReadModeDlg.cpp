
// ReadModeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "afxwin.h"
#include "ReadMode.h"
#include "ReadModeDlg.h"
#include "afxdialogex.h"
#include "TrackBall.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include "Resource.h"
#include <math.h>
#include <cstring>

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
{
	protein1=new ProteinFrame();
	protein2=new ProteinFrame();
	//protView=new ProteinView();
	//m_info.SetWindowText((LPCTSTR)"bitch");
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}
void CReadModeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_ComboMove);
	DDX_CBString(pDX, IDC_COMBO1, m_infoBox);

	DDX_Control(pDX, IDC_INFO, m_info);
	DDX_Control(pDX, IDC_SLIDER1, m_SpeedSlider);


	DDX_Control(pDX, IDC_CHECK1, m_checkbox1);
	DDX_Control(pDX, IDC_CHECK2, m_checkbox2);
	//DDX_Control(pDX, IDC_GAUSS_INF, gauss_inf);
	//DDX_Control(pDX, IDC_GAUSS_SUP, gauss_sup);
	//DDX_Control(pDX, IDC_USE_MEAN, m_use_mean1);
	//DDX_Control(pDX, IDC_USE_MEAN2, m_use_mean2);
	DDX_Control(pDX, IDC_USECURV11, m_radio_button_curvature1);
	DDX_Control(pDX, IDC_USECURV20, m_radio_button_curvature2);
	//DDX_Control(pDX, IDC_GAUSS_INF2, gauss_inf2);
	//DDX_Control(pDX, IDC_GAUSS_SUP2, gauss_sup2);
	DDX_Control(pDX, IDC_LIST1, m_uselist1);
	DDX_Control(pDX, IDC_LIST2, m_uselist2);
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
	ON_WM_CHAR()
	ON_WM_KEYUP()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &CReadModeDlg::OnCustomdrawSlider1)

	ON_BN_CLICKED(IDC_CHECK1, &CReadModeDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &CReadModeDlg::OnBnClickedCheck2)
	ON_EN_CHANGE(IDC_INFO, &CReadModeDlg::OnEnChangeInfo)
	ON_EN_KILLFOCUS(IDC_GAUSS_INF, &CReadModeDlg::OnKillfocusGaussInf)
	ON_EN_KILLFOCUS(IDC_GAUSS_SUP, &CReadModeDlg::OnKillfocusGaussSup)
	ON_EN_KILLFOCUS(IDC_GAUSS_INF2, &CReadModeDlg::OnKillfocusGaussInf2)
	ON_EN_KILLFOCUS(IDC_GAUSS_SUP2, &CReadModeDlg::OnKillfocusGaussSup2)

	ON_BN_CLICKED(IDC_USECURV11, &CReadModeDlg::OnBnClickedUsecurv11)
	ON_BN_CLICKED(IDC_USECURV12, &CReadModeDlg::OnBnClickedUsecurv12)
	ON_BN_CLICKED(IDC_USECURV13, &CReadModeDlg::OnBnClickedUsecurv13)
	ON_BN_CLICKED(IDC_USECURV14, &CReadModeDlg::OnBnClickedUsecurv14)
	ON_BN_CLICKED(IDC_USECURV15, &CReadModeDlg::OnBnClickedUsecurv15)
	ON_BN_CLICKED(IDC_USECURV20, &CReadModeDlg::OnBnClickedUsecurv20)
	ON_BN_CLICKED(IDC_USECURV21, &CReadModeDlg::OnBnClickedUsecurv21)
	ON_BN_CLICKED(IDC_USECURV22, &CReadModeDlg::OnBnClickedUsecurv22)
	ON_BN_CLICKED(IDC_USECURV23, &CReadModeDlg::OnBnClickedUsecurv23)
	ON_BN_CLICKED(IDC_USECURV24, &CReadModeDlg::OnBnClickedUsecurv24)
	ON_COMMAND(ID_POLYGONMODE_FILL, &CReadModeDlg::OnPolygonmodeFill1)
	ON_COMMAND(ID_POLYGONMODE_LINE, &CReadModeDlg::OnPolygonmodeLine1)
	ON_COMMAND(ID_POLYGONMODE_POINT, &CReadModeDlg::OnPolygonmodePoint1)
	ON_COMMAND(ID_POLYGONMODE_FILLE, &CReadModeDlg::OnPolygonmodeFill2)
	ON_COMMAND(ID_POLYGONMODE_LINE32789, &CReadModeDlg::OnPolygonmodeLine2)
	ON_COMMAND(ID_POLYGONMODE_POINT32788, &CReadModeDlg::OnPolygonmodePoint2)

	ON_COMMAND(ID_MEANCURVATURE_ALGO, &CReadModeDlg::OnMeancurvatureWithEdgeCurv)
	ON_COMMAND(ID_MEANCURVATURE_ALGO2, &CReadModeDlg::OnMeancurvatureWithoutEdgeCurv)
	ON_COMMAND(ID_MEANCURVATURE_ALGO1, &CReadModeDlg::OnMeancurvatureWithEdgeCurv2)
	ON_COMMAND(ID_MEANCURVATURE_ALGO3, &CReadModeDlg::OnMeancurvatureWithoutEdgeCurv2)
	//ON_EN_CHANGE(IDC_GAUSS_INF2, &CReadModeDlg::OnEnChangeGaussInf2)
	ON_COMMAND(ID_PROTEIN1_RESETVIEW, &CReadModeDlg::OnProtein1Resetview)
	ON_COMMAND(ID_PROTEIN2_RESETVIEW, &CReadModeDlg::OnProtein2Resetview)


	ON_LBN_SELCHANGE(IDC_LIST1, &CReadModeDlg::OnLbnSelchangeList1)
	ON_LBN_SELCHANGE(IDC_LIST2, &CReadModeDlg::OnLbnSelchangeList2)
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
	
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	m_ComboMove.SetCurSel(0);
	m_SpeedSlider.SetRange(2,120);
	m_SpeedSlider.SetPos(59);
	m_SpeedSlider.UpdateData();
	m_move=0;
	protein1->wnd=CWnd::GetDlgItem(IDC_SHOW1);
	protein2->wnd=CWnd::GetDlgItem(IDC_SHOW2);
	m_infobox_handle=GetDlgItem(IDC_INFO);
	m_gauss_inf_handle=GetDlgItem(IDC_GAUSS_INF);
	m_gauss_sup_handle=GetDlgItem(IDC_GAUSS_SUP);
	m_gauss_inf_handle2=GetDlgItem(IDC_GAUSS_INF2);
	m_gauss_sup_handle2=GetDlgItem(IDC_GAUSS_SUP2);
	::SetFocus(::GetActiveWindow());
	gauss_inferior=10;
	gauss_superior=60;
	gauss_inferior2=10;
	gauss_superior2=60;
	m_gauss_inf_handle->SetWindowTextW(_T("10"));
	m_gauss_sup_handle->SetWindowTextW(_T("60"));
	m_gauss_inf_handle2->SetWindowTextW(_T("10"));
	m_gauss_sup_handle2->SetWindowTextW(_T("60"));
	curv1=MEAN;
	curv2=MEAN;
	m_uselist1.AddString(L"Mean curvature");
	m_uselist1.AddString(L"Gaussian curvature");
	m_uselist1.AddString(L"Shape Index");
	m_uselist1.AddString(L"Salient Geometric Features");
	m_uselist1.AddString(L"None");
	m_uselist1.SetCurSel(0);
	m_uselist2.AddString(L"Mean curvature");
	m_uselist2.AddString(L"Gaussian curvature");
	m_uselist2.AddString(L"Shape Index");
	m_uselist2.AddString(L"Salient Geometric Features");
	m_uselist2.AddString(L"None");
	m_uselist2.SetCurSel(0);
//	CRect rect;
//	protein1->wnd->GetClientRect(rect);
////	trackball = new CTrackBall(rect.Width(), rect.Height(), protein1);
	m_radio_button_curvature1.SetCheck(true);
	m_radio_button_curvature2.SetCheck(true);
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
	/*if(protein1->m_read==true)
	protein1->Draw();
	if(protein2->m_read==true)
	protein2->Draw();*/
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
	//SetTimer(1,100,NULL);
	CDialogEx::OnTimer(nIDEvent);
}


//---------LOAD BUTTONS--------------------//
void CReadModeDlg::OnBnClickedReadm()
{
	// TODO: Add your control notification handler code here

	CFileDialog *pDlg=new CFileDialog(true);
	if(pDlg->DoModal()!=IDOK)
		return;
	

	if(protein1->m_rof==NULL){
		protein1->LoadProtein(CW2A(pDlg->GetPathName()));	
		protein1->Draw();
		m_init=true;
	}
	else{
		protein1->kill_thread=true;
		while(protein1->kill_thread==true){}
		protein1->LoadProtein(CW2A(pDlg->GetPathName()));		
		protein1->Draw();
	}
	if(protein1->m_rof->vn==false){  //if no normal information in file, grey out checkbox
		m_checkbox1.SetCheck(true);
		CWnd::GetDlgItem(IDC_CHECK1)->EnableWindow(false);
		m_infobox_handle->SetWindowTextW(_T("No normal information in this file"));
	}
	else m_infobox_handle->SetWindowTextW(_T(""));
	if(protein1->m_rof != NULL){
		protein1->m_rof->gauss_inf = gauss_inferior;
		protein1->m_rof->gauss_sup = gauss_superior;
		protein1->m_rof->use_curvature = curv1;
	}
	//SetTimer(1,100,NULL);
	if(protein1->m_rof && protein2->m_rof)
		ComputeGreyRelation();
	::SetFocus(::GetActiveWindow());
}
void CReadModeDlg::OnBnClickedReadm2()
{
	CFileDialog *pDlg=new CFileDialog(true);
	if(pDlg->DoModal()!=IDOK)
		return;

	if(protein2->m_rof==NULL){
		protein2->LoadProtein(CW2A(pDlg->GetPathName()));	
		protein2->Draw();
		m_init=true;
	}
	else{
		protein2->kill_thread=true;
		while(protein2->kill_thread==true){}
		protein2->LoadProtein(CW2A(pDlg->GetPathName()));		
		protein2->Draw();
	}
	if(protein2->m_rof->vn==false){  //if no normal information in file, grey out checkbox
		m_checkbox2.SetCheck(true);
		CWnd::GetDlgItem(IDC_CHECK2)->EnableWindow(false);
		m_infobox_handle->SetWindowTextW(_T("No normal information in this file"));
	}
	else m_infobox_handle->SetWindowTextW(_T(""));
	if(protein2->m_rof != NULL){
		protein2->m_rof->gauss_inf = gauss_inferior;
		protein2->m_rof->gauss_sup = gauss_superior;
		protein2->m_rof->use_curvature = curv2;
	}
	
	//SetTimer(1,100,NULL);
	::SetFocus(::GetActiveWindow());
}

//----------3 Buttons--------------------------//
void CReadModeDlg::OnBnClickedMovem()   //mobility model (???)
{
	// TODO: Add your control notification handler code here
	if (protein1->m_rof==NULL)
	{
		//AfxMessageBox(_T("请先读取物体!"));
		m_infobox_handle->SetWindowTextW(_T("Please load an object first"));
		return;
	} 
	else{
		protein1->m_move=true;
		protein1->m_AutoRotation=false;
		protein1->m_rotation=false;
	}
	::SetFocus(::GetActiveWindow());
}
void CReadModeDlg::OnBnClickedRoatm()//Rotating model
{
	// TODO: Add your control notification handler code here
	if (protein1->m_rof==NULL &&protein2->m_rof==NULL)
	{
		m_infobox_handle->SetWindowTextW(_T("Please load an object first")); //if no _T writes in chinese
		return;
	} 
	if(protein1->m_rof!=NULL && (m_move==1 || m_move==2)){
		protein1->m_rotation=true;
		protein1->m_AutoRotation=true;
		protein1->m_move=false;
	}
	if(protein2->m_rof!=NULL && (m_move==1 || m_move==3)){
		protein2->m_rotation=true;
		protein2->m_AutoRotation=true;
		protein2->m_move=false;
	}
	::SetFocus(::GetActiveWindow());
}
void CReadModeDlg::OnBnClickedUpdate()//stop button
{
	//give a stop order
	// TODO: Add your control notification handler code here
	if(m_move==1 || m_move==2){
		protein1->m_AutoRotation=false;
		protein1->m_rotation=false;
		protein1->m_move=false;
	}
	if(m_move==1 || m_move==3){
		protein2->m_AutoRotation=false;
		protein2->m_rotation=false;
		protein2->m_move=false;
	}
	//this->UpdateData(true);
	::SetFocus(::GetActiveWindow());
}
//----------MOUSE--------------------------//
void CReadModeDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CRect rect;
	protein1->wnd->GetWindowRect(rect); 
	CRect rect2; 
	protein2->wnd->GetWindowRect(rect2);
	this->ClientToScreen(&point);

	if(m_move==1){ //dual move
		if(rect.PtInRect(point) || rect2.PtInRect(point)){
			protein1->m_LDown=true;
			protein2->m_LDown=true;
			m_MouseL=point;
		}
	} 
	else if(m_move==2&& rect.PtInRect(point) ){ //move 1
		protein1->m_LDown=true;
		m_MouseL=point;
	}
	else if(m_move==3&& rect2.PtInRect(point) ){ //move 2
		protein2->m_LDown=true;
		m_MouseL=point;
	}
	
	

	//trackball->startMotion(point.x, point.y);
	//::SetFocus(::GetActiveWindow());
	CDialogEx::OnLButtonDown(nFlags, point);
}
void CReadModeDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	protein1->m_LDown=false;
	protein1->m_rot=false;
	protein2->m_LDown=false;
	protein2->m_rot=false;
	//trackball->stopMotion();
	::SetFocus(::GetActiveWindow());
	CDialogEx::OnLButtonUp(nFlags, point);
}
void CReadModeDlg::OnMouseMove(UINT nFlags, CPoint point)
{

	if (protein1->m_LDown)
	{
		if ((GetAsyncKeyState(VK_LBUTTON) & 0x8000)){
		//(GetAsyncKeyState(VK_CONTROL) & 0x8000) &&
			protein1->m_rot_x+=0.5*(m_MouseL.y-point.y);
			protein1->m_rot_y-=0.5*(m_MouseL.x-point.x);
			//protein1->Draw();
		}
	}
	else if (protein1->m_rot)
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
	if (protein2->m_LDown)
	{
		if ((GetAsyncKeyState(VK_LBUTTON) & 0x8000)){
		//(GetAsyncKeyState(VK_CONTROL) & 0x8000) &&
			protein2->m_rot_x+=0.5*(m_MouseL.y-point.y);
			protein2->m_rot_y-=0.5*(m_MouseL.x-point.x);
			//protein2->Draw();
		}
	}
	else if (protein2->m_rot)
	{

		if ((GetAsyncKeyState(VK_LBUTTON) & 0x8000))
		{
			if ((point.x - m_MouseL.x) != 0)
			{
				protein2->m_rot_y = protein2->m_rot_y + ((float)ROTATE_SPEED * (point.x - m_MouseL.x));
			}

			if ((point.y - m_MouseL.y) != 0)
			{	
				protein2->m_rot_x = protein2->m_rot_x + ((float)ROTATE_SPEED * (point.y - m_MouseL.y));
			}
			//protein2->Draw();
		}
	
	}
	if(protein1->m_LDown || protein2->m_LDown)
		m_MouseL=point;
	CDialogEx::OnMouseMove(nFlags, point);
}
BOOL CReadModeDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default
	if(protein1->m_rof!=NULL&&(m_move==1 || m_move==2) ){
		protein1->m_z+=zDelta*m_SpeedSlider.GetPos()/1000;
		//protein1->Draw();
	}
	if(protein2->m_rof!=NULL&&(m_move==1 || m_move==3)){
		protein2->m_z+=zDelta*m_SpeedSlider.GetPos()/1000;
		//protein2->Draw();
	}
	::SetFocus(::GetActiveWindow());
	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}
//--------------------------------------------//
void CReadModeDlg::OnRotation()
{
	// TODO: Add your command handler code here
	if (protein1->m_rof==NULL)
	{
		//AfxMessageBox(_T("Please load an object first!"));
		m_infobox_handle->SetWindowTextW(_T("Please load an object first"));	
		return;
	} 
	protein1->m_AutoRotation=true;
	protein1->m_move=false;
	protein1->m_rotation=false;
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
void CReadModeDlg::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	CDialogEx::OnChar(nChar, nRepCnt, nFlags);
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
	::SetFocus(::GetActiveWindow());
}
void CReadModeDlg::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	CDialogEx::OnKeyUp(nChar, nRepCnt, nFlags);
} 
void CReadModeDlg::OnEnChangeInfo()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
} //not used //not used
void CReadModeDlg::OnStnClickedFrame1()
{
// TODO: Add your control notification handler code here
}
void CReadModeDlg::OnBnClickedShow()
{
	//m_move=1;
	// TODO: Add your control notification handler code here
	::SetFocus(::GetActiveWindow());
}
void CReadModeDlg::OnBnClickedShow2()
{
	// TODO: Add your control notification handler code here
	//m_move=2;
	::SetFocus(::GetActiveWindow());
}
//---------MOVE COMBO + SPEED------------------------//
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
void CReadModeDlg::OnCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	if(protein1->m_rof !=NULL ){
		protein1->speed=m_SpeedSlider.GetPos();
	}
	if(protein2->m_rof!=NULL){
		protein2->speed=m_SpeedSlider.GetPos();
	}
	*pResult = 0;
	::SetFocus(::GetActiveWindow());
}
//--------KEY INPUT---------------------------//
BOOL CReadModeDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class

	if(pMsg->message==WM_KEYDOWN){
		if(GetFocus()!= GetDlgItem(IDC_GAUSS_INF )&& GetFocus()!= GetDlgItem(IDC_GAUSS_SUP )
			&&  GetFocus()!= GetDlgItem(IDC_GAUSS_INF2 )&& GetFocus()!= GetDlgItem(IDC_GAUSS_SUP2 )){
			if(protein1->m_rof!=NULL &&(m_move==1 || m_move==2) ){
				protein1->KeyInput(pMsg->wParam, pMsg->lParam);
			}
			if(protein2->m_rof!=NULL&& (m_move==1|| m_move==3)){
				protein2->KeyInput(pMsg->wParam,pMsg->lParam);
			}
			return true;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


//--------ESTIMATIONS SELECTION-------------//

void CReadModeDlg::OnBnClickedCheck1()
{
	// TODO: Add your control notification handler code here
	if(protein1->m_rof!=NULL){
		if(m_checkbox1.GetCheck()){
			protein1->m_useNE=true;
			protein1->m_rof->useNormalEstimation=true;
		}
		else{
			protein1->m_useNE=false;
			protein1->m_rof->useNormalEstimation=false;
		}
	}
	else{
		if(m_checkbox1.GetCheck())
			protein1->m_useNE=true;
		else
			protein1->m_useNE=false;
	}

}
void CReadModeDlg::OnBnClickedCheck2()
{
	// TODO: Add your control notification handler code here
	if(protein2->m_rof!=NULL){   //if object has already been loaded
		if(m_checkbox2.GetCheck()){
			protein2->m_useNE=true;
			protein2->m_rof->useNormalEstimation=true;
		}
		else{
			protein2->m_useNE=false;
			protein2->m_rof->useNormalEstimation=false;
		}
	}
	else{								//if object has not been loaded yes
		if(m_checkbox2.GetCheck())
			protein2->m_useNE=true;
		else
			protein2->m_useNE=false;
	}
}
void CReadModeDlg::OnKillfocusGaussInf()
{
	// TODO: Add your control notification handler code here
	CString inf;
	m_gauss_inf_handle->GetWindowTextW(inf);
	if(::StrToIntW(inf)<gauss_superior)
		gauss_inferior = ::StrToIntW(inf);
	else{
		m_infobox_handle->SetWindowTextW(_T("This value is not inferior to the sup value"));	
		gauss_inferior = gauss_superior - 1; 
		CString tmp;
		tmp.Format(_T("%d"), gauss_inferior);	
		m_gauss_inf_handle->SetWindowTextW(tmp);
	}
	if(protein1->m_rof != NULL) protein1->m_rof->gauss_inf = gauss_inferior;
	::SetFocus(::GetActiveWindow());
}
void CReadModeDlg::OnKillfocusGaussSup()
{
	// TODO: Add your control notification handler code here
	CString sup;
	m_gauss_sup_handle->GetWindowTextW(sup);
	if(gauss_inferior<::StrToIntW(sup))
		gauss_superior= ::StrToIntW(sup);
	else{
		m_infobox_handle->SetWindowTextW(_T("This value is not superior to the inf value"));	
		gauss_superior =gauss_inferior +1;
		CString tmp;
		tmp.Format(_T("%d"), gauss_superior);	
		m_gauss_sup_handle->SetWindowTextW(tmp);
	}
	if(protein1->m_rof != NULL) protein1->m_rof->gauss_sup = gauss_superior;
	::SetFocus(::GetActiveWindow());
}
void CReadModeDlg::OnKillfocusGaussInf2()
{
	// TODO: Add your control notification handler code here
	CString inf;
	m_gauss_inf_handle2->GetWindowTextW(inf);
	if(::StrToIntW(inf)<gauss_superior2)
		gauss_inferior2 = ::StrToIntW(inf);
	else{
		m_infobox_handle->SetWindowTextW(_T("This value is not inferior to the sup value"));	
		gauss_inferior2 = gauss_superior2 - 1; 
		CString tmp;
		tmp.Format(_T("%d"), gauss_inferior2);	
		m_gauss_inf_handle2->SetWindowTextW(tmp);
	}
	if(protein2->m_rof != NULL) protein2->m_rof->gauss_inf = gauss_inferior2;
	::SetFocus(::GetActiveWindow());
}
void CReadModeDlg::OnKillfocusGaussSup2()
{
		// TODO: Add your control notification handler code here
	CString sup;
	m_gauss_sup_handle2->GetWindowTextW(sup);
	if(gauss_inferior2<::StrToIntW(sup))
		gauss_superior2= ::StrToIntW(sup);
	else{
		m_infobox_handle->SetWindowTextW(_T("This value is not superior to the inf value"));	
		gauss_superior2 =gauss_inferior2 +1;
		CString tmp;
		tmp.Format(_T("%d"), gauss_superior2);	
		m_gauss_sup_handle2->SetWindowTextW(tmp);
	}
	if(protein2->m_rof != NULL) protein2->m_rof->gauss_sup = gauss_superior2;
	::SetFocus(::GetActiveWindow());

}
void CReadModeDlg::OnBnClickedUsecurv11() //use mean 1
{
	// TODO: Add your control notification handler code here
	if(protein1->m_rof!=NULL){
		protein1->m_rof->use_curvature=MEAN;
	}
	// TODO: Add your control notification handler code here
	curv1=MEAN;
}
void CReadModeDlg::OnBnClickedUsecurv12() //use gauss 1
{
	// TODO: Add your control notification handler code here
	if(protein1->m_rof!=NULL){
		protein1->m_rof->use_curvature=GAUSS;
	}
	// TODO: Add your command handler code here
	curv1=GAUSS;
}
void CReadModeDlg::OnBnClickedUsecurv13()  //use shape index 1
{
	// TODO: Add your control notification handler code here
		// TODO: Add your control notification handler code here
	if(protein1->m_rof!=NULL){
		protein1->m_rof->use_curvature=SHAPEINDEX;
	}
	// TODO: Add your command handler code here
	curv1=SHAPEINDEX;
}
void CReadModeDlg::OnBnClickedUsecurv14() //use none 1
{
	// TODO: Add your control notification handler code here
		// TODO: Add your control notification handler code here
	if(protein1->m_rof!=NULL){
		protein1->m_rof->use_curvature=NONE;
	}
	// TODO: Add your command handler code here
	curv1=NONE;
}
void CReadModeDlg::OnBnClickedUsecurv15()
{
	// TODO: Add your control notification handler code here
	if(protein1->m_rof!=NULL){
		protein1->m_rof->use_curvature=SGF;
	}
	curv1=SGF;
}
void CReadModeDlg::OnBnClickedUsecurv20() //use mean 2
{
	// TODO: Add your control notification handler code here
	if(protein2->m_rof!=NULL){
		protein2->m_rof->use_curvature=MEAN;
	}
	curv2=MEAN;
}
void CReadModeDlg::OnBnClickedUsecurv21() //use gauss 2
{
	// TODO: Add your control notification handler code here
	// TODO: Add your command handler code here
	if(protein2->m_rof!=NULL){
		protein2->m_rof->use_curvature=GAUSS;
	}
	curv2=GAUSS;
}
void CReadModeDlg::OnBnClickedUsecurv22() //use shape index 2
{
	// TODO: Add your control notification handler code here
	if(protein2->m_rof!=NULL){
		protein2->m_rof->use_curvature=SHAPEINDEX;
	}
	curv2=SHAPEINDEX;
}
void CReadModeDlg::OnBnClickedUsecurv23() //use none 2
{
	// TODO: Add your control notification handler code here
	if(protein2->m_rof!=NULL){
		protein2->m_rof->use_curvature=NONE;
	}
	curv2=NONE;
}
void CReadModeDlg::OnBnClickedUsecurv24()
{
	// TODO: Add your control notification handler code here
	if(protein2->m_rof!=NULL){
		protein2->m_rof->use_curvature=SGF;
	}
	curv2=SGF;
}

void CReadModeDlg::OnLbnSelchangeList1()
{
	// TODO: Add your control notification handler code here
	int choice=m_uselist1.GetCurSel();
	Curvature curv;
	switch(choice)
	{
		case 0:
			curv=MEAN;
			break;
		case 1:
			curv=GAUSS;
			break;
		case 2:
			curv=SHAPEINDEX;
			break;
		case 3:
			curv=SGF;
			break;
		case 4:
			curv=NONE;
			break;
	}
	if(protein1->m_rof!=NULL){
		protein1->m_rof->use_curvature=curv;
	}
	curv1=curv;	
	::SetFocus(::GetActiveWindow());
}
void CReadModeDlg::OnLbnSelchangeList2()
{
	// TODO: Add your control notification handler code here
	int choice=m_uselist2.GetCurSel();
	Curvature curv;
	switch(choice)
	{
		case 0:
			curv=MEAN;
			break;
		case 1:
			curv=GAUSS;
			break;
		case 2:
			curv=SHAPEINDEX;
			break;
		case 3:
			curv=SGF;
			break;
		case 4:
			curv=NONE;
			break;
	}
	if(protein2->m_rof!=NULL){
		protein2->m_rof->use_curvature=curv;
	}
	curv2=curv;	
	::SetFocus(::GetActiveWindow());
}

//-----ON DESTRUCT-------------------//
void CReadModeDlg::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class
	if(protein1->flag_threadCreated)
		protein1->Kill();
	if(protein2->flag_threadCreated)
		protein2->Kill();

	CDialogEx::PostNcDestroy();
}


//---------------MENU----------------//
void CReadModeDlg::OnPolygonmodeFill1()
{
	// TODO: Add your command handler code here
	protein1->polygon_mode=GL_FILL;
}
void CReadModeDlg::OnPolygonmodeLine1()
{
	// TODO: Add your command handler code here
	protein1->polygon_mode=GL_LINE;
}
void CReadModeDlg::OnPolygonmodePoint1()
{
	// TODO: Add your command handler code here
	protein1->polygon_mode=GL_POINT;
}
void CReadModeDlg::OnPolygonmodeFill2()
{
	// TODO: Add your command handler code here
	protein2->polygon_mode=GL_FILL;
}
void CReadModeDlg::OnPolygonmodeLine2()
{
	// TODO: Add your command handler code here
	protein2->polygon_mode=GL_LINE;
}
void CReadModeDlg::OnPolygonmodePoint2()
{
	// TODO: Add your command handler code here
	protein2->polygon_mode=GL_POINT;
}
void CReadModeDlg::OnMeancurvatureWithEdgeCurv()
{
	// TODO: Add your command handler code here
	if(protein1->m_rof!=NULL){
		protein1->m_rof->use_ridgeorvalley=true;
		protein1->m_rof->EstimatekGkM();
		protein1->m_rof->EstimateSGF();
		protein1->m_rof->SimilarityMeasurement();
	}
}
void CReadModeDlg::OnMeancurvatureWithoutEdgeCurv()
{
	// TODO: Add your command handler code here
	if(protein1->m_rof!=NULL){
		protein1->m_rof->use_ridgeorvalley=false;
		protein1->m_rof->EstimatekGkM();
		protein1->m_rof->EstimateSGF();
		protein1->m_rof->SimilarityMeasurement();
	}
}
void CReadModeDlg::OnMeancurvatureWithEdgeCurv2()
{
	// TODO: Add your command handler code here
	if(protein2->m_rof!=NULL){
		protein2->m_rof->use_ridgeorvalley=true;
		protein2->m_rof->EstimatekGkM();
		protein2->m_rof->EstimateSGF();
		protein2->m_rof->SimilarityMeasurement();
	}
}
void CReadModeDlg::OnMeancurvatureWithoutEdgeCurv2()
{
	// TODO: Add your command handler code here
	if(protein2->m_rof!=NULL){
		protein2->m_rof->use_ridgeorvalley=false;
		protein2->m_rof->EstimatekGkM();
		protein2->m_rof->EstimateSGF();
		protein2->m_rof->SimilarityMeasurement();
	}
}
void CReadModeDlg::OnProtein1Resetview()
{
	// TODO: Add your command handler code here
	protein1->ResetView();
}
void CReadModeDlg::OnProtein2Resetview()
{
	// TODO: Add your command handler code here
	protein2->ResetView();
}



void CReadModeDlg::ComputeGreyRelation(void)
{
	float x_row[48][48]; float y_row[48][48];
	float s_row[48]={0}; float t_row[48]={0};
	float eta_row[48]={0};
	for(int i=0; i<48; i++)  //columns
	{
		for(int j=0; j<48; j++)//rows
		{
			x_row[i][j]=protein1->m_rof->feature_matrix[i][j] - protein1->m_rof->feature_matrix[1][j];
			y_row[i][j]=protein2->m_rof->feature_matrix[i][j] - protein2->m_rof->feature_matrix[1][j];
			s_row[j]+=x_row[i][j];
			t_row[j]+=y_row[i][j];
		}
	}
	float sum_row=0;
	for(int i=0; i<48; i++)  
	{
		eta_row[i]= (1+ ::abs(s_row[i])+::abs(t_row[i]))/ ( 1 + ::abs(s_row[i])+::abs(t_row[i])+::abs(s_row[i]-t_row[i]));
		sum_row+=eta_row[i];
	}

	float x_column[48][48]; float y_column[48][48];
	float s_column[48]={0}; float t_column[48]={0};
	float eta_column[48]={0};
	for(int i=0; i<48; i++)  //columns
	{
		for(int j=0; j<48; j++)//rows
		{
			x_column[i][j]=protein1->m_rof->feature_matrix[i][j] - protein1->m_rof->feature_matrix[i][1];
			y_column[i][j]=protein2->m_rof->feature_matrix[i][j] - protein2->m_rof->feature_matrix[i][1];
			s_column[i]+=x_column[i][j];
			t_column[i]+=y_column[i][j];
		}
	}
	float sum_column=0;
	for(int i=0; i<48; i++)  
	{
		eta_column[i]= (1+ ::abs(s_column[i])+::abs(t_column[i]))/ ( 1 + ::abs(s_column[i])+::abs(t_column[i])+::abs(s_column[i]-t_column[i]));
		sum_column+=eta_column[i];
	}
	float degree= 0.5 * (sum_row + sum_column) / 48;

}