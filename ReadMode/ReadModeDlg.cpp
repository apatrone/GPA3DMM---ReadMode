
// ReadModeDlg.cpp : implementation file
//

#include "stdafx.h"
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

	DDX_Control(pDX, IDC_FRAME1, frame1);
	DDX_Control(pDX, IDC_CHECK1, m_checkbox1);
	DDX_Control(pDX, IDC_CHECK2, m_checkbox2);
	DDX_Control(pDX, IDC_GAUSS_INF, gauss_inf);
	DDX_Control(pDX, IDC_GAUSS_SUP, gauss_sup);
	DDX_Control(pDX, IDC_USE_MEAN, m_use_mean1);
	DDX_Control(pDX, IDC_USE_MEAN2, m_use_mean2);
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
	ON_STN_CLICKED(IDC_FRAME1, &CReadModeDlg::OnStnClickedFrame1)
	ON_BN_CLICKED(IDC_CHECK1, &CReadModeDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &CReadModeDlg::OnBnClickedCheck2)
	ON_EN_CHANGE(IDC_INFO, &CReadModeDlg::OnEnChangeInfo)
//	ON_EN_CHANGE(IDC_GAUSS_INF, &CReadModeDlg::OnEnChangeGaussInf)
//	ON_EN_CHANGE(IDC_GAUSS_SUP, &CReadModeDlg::OnEnChangeGaussSup)
	ON_EN_KILLFOCUS(IDC_GAUSS_INF, &CReadModeDlg::OnKillfocusGaussInf)
	ON_EN_KILLFOCUS(IDC_GAUSS_SUP, &CReadModeDlg::OnKillfocusGaussSup)
	ON_EN_CHANGE(IDC_GAUSS_INF, &CReadModeDlg::OnChangeGaussInf)
	ON_BN_CLICKED(IDC_USE_MEAN, &CReadModeDlg::OnClickedUseMean)
	ON_BN_CLICKED(IDC_USE_MEAN2, &CReadModeDlg::OnClickedUseMean2)
	ON_COMMAND(IDC_USE_GAUSS, &CReadModeDlg::OnUseGauss)
	ON_COMMAND(IDC_USE_GAUSS2, &CReadModeDlg::OnUseGauss2)
	ON_BN_CLICKED(IDC_USE_NONE, &CReadModeDlg::OnClickedUseNone)
	ON_BN_CLICKED(IDC_USE_NONE2, &CReadModeDlg::OnClickedUseNone2)
	ON_BN_CLICKED(IDC_USE_SI, &CReadModeDlg::OnBnClickedUseSi)
	ON_BN_CLICKED(IDC_USE_SI2, &CReadModeDlg::OnBnClickedUseSi2)
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
	protein1->wnd=CWnd::GetDlgItem(IDC_FRAME1);
	protein2->wnd=CWnd::GetDlgItem(IDC_SHOW2);
	m_infobox_handle=GetDlgItem(IDC_INFO);
	m_gauss_inf_handle=GetDlgItem(IDC_GAUSS_INF);
	m_gauss_sup_handle=GetDlgItem(IDC_GAUSS_SUP);
	::SetFocus(::GetActiveWindow());
	gauss_inferior=10;
	gauss_superior=60;
	m_gauss_inf_handle->SetWindowTextW(_T("10"));
	m_gauss_sup_handle->SetWindowTextW(_T("60"));
	curv1=MEAN;
	curv2=MEAN;
	CRect rect;
	protein1->wnd->GetClientRect(rect);
	trackball = new CTrackBall(rect.Width(), rect.Height());
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
void CReadModeDlg::OnBnClickedReadm()
{
	// TODO: Add your control notification handler code here

	CFileDialog *pDlg=new CFileDialog(true);
	if(pDlg->DoModal()!=IDOK)
		return;
	
	
	/*CRect rect;
	protein1->wnd->GetClientRect(rect);*/
	//protein1->wnd->InvalidateRect(NULL);	
	//protein1->wnd->ValidateRect(NULL);*/*/
//	if(protein1!=NULL) protein1->;
	//protein1=new ProteinFrame();
	//if(protein1->Kill()){
	//delete protein1;
	//protein1=new ProteinFrame();
	//protein1->wnd=CWnd::GetDlgItem(IDC_SHOW1);
	//protein1->LoadFrame(protein1->wnd);
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
		//AfxMessageBox(_T("请先读取物体!"));
		m_infobox_handle->SetWindowTextW(_T("Please load an object first")); //if no _T writes in chinese
		return;
	} 
	if(protein1->m_rof!=NULL){
		protein1->m_rotation=true;
		protein1->m_AutoRotation=true;
		protein1->m_move=false;
	}
	if(protein2->m_rof!=NULL){
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
	protein1->m_AutoRotation=false;
	protein1->m_rotation=false;
	protein1->m_move=false;
	protein2->m_AutoRotation=false;
	protein2->m_rotation=false;
	protein2->m_move=false;
	//this->UpdateData(true);
	::SetFocus(::GetActiveWindow());
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
	trackball->startMotion(point.x, point.y);
	::SetFocus(::GetActiveWindow());
	CDialogEx::OnLButtonDown(nFlags, point);
}
void CReadModeDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	protein1->m_LDown=false;
	protein1->m_rot=false;
	trackball->stopMotion();
	CDialogEx::OnLButtonUp(nFlags, point);
}
void CReadModeDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	if ((GetAsyncKeyState(VK_LBUTTON) & 0x8000)){
		trackball->onMouseMotion(point.x, point.y);
	}
	else if (protein1->m_LDown)
	{
		if ((GetAsyncKeyState(VK_LBUTTON) & 0x8000)){
		//(GetAsyncKeyState(VK_CONTROL) & 0x8000) &&
			protein1->m_x+=0.5*(point.x-m_MouseL.x);
			protein1->m_y+=0.5*(m_MouseL.y-point.y);
			m_MouseL=point;
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
	
	CDialogEx::OnMouseMove(nFlags, point);
}
BOOL CReadModeDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default
	if(protein1->m_rof!=NULL&&(m_move==1 || m_move==2) ){
		protein1->m_z+=zDelta/120;
		//protein1->Draw();
	}
	if(protein2->m_rof!=NULL&&(m_move==1 || m_move==3)){
		protein2->m_z+=zDelta/120;
		//protein2->Draw();
	}
	::SetFocus(::GetActiveWindow());
	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}
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


void CReadModeDlg::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	printf("yo\n");
	CDialogEx::OnChar(nChar, nRepCnt, nFlags);
}


void CReadModeDlg::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	CDialogEx::OnKeyUp(nChar, nRepCnt, nFlags);
}


BOOL CReadModeDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class

	if(pMsg->message==WM_KEYDOWN){
		if(GetFocus()!= GetDlgItem(IDC_GAUSS_INF )&& GetFocus()!= GetDlgItem(IDC_GAUSS_SUP )){
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




void CReadModeDlg::OnCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	if(protein1->m_rof !=NULL ){
		protein1->speed=m_SpeedSlider.GetPos();
	}
	if(protein1->m_rof!=NULL){
		protein2->speed=m_SpeedSlider.GetPos();
	}
	*pResult = 0;
	::SetFocus(::GetActiveWindow());
}


void CReadModeDlg::OnStnClickedFrame1()
{
	// TODO: Add your control notification handler code here
}


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


void CReadModeDlg::OnEnChangeInfo()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


//void CReadModeDlg::OnEnChangeGaussInf()
//{
//	// TODO:  If this is a RICHEDIT control, the control will not
//	// send this notification unless you override the CDialogEx::OnInitDialog()
//	// function and call CRichEditCtrl().SetEventMask()
//	// with the ENM_CHANGE flag ORed into the mask.
//	float val=m_gauss_inf_handle->getva
//	// TODO:  Add your control notification handler code here
//}


//void CReadModeDlg::OnEnChangeGaussSup()
//{
//	// TODO:  If this is a RICHEDIT control, the control will not
//	// send this notification unless you override the CDialogEx::OnInitDialog()
//	// function and call CRichEditCtrl().SetEventMask()
//	// with the ENM_CHANGE flag ORed into the mask.

//	// TODO:  Add your control notification handler code here
//}


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
	if(protein2->m_rof != NULL) protein2->m_rof->gauss_inf = gauss_inferior;
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
	if(protein2->m_rof != NULL) protein2->m_rof->gauss_sup = gauss_superior;
	::SetFocus(::GetActiveWindow());
}


void CReadModeDlg::OnChangeGaussInf()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void CReadModeDlg::OnClickedUseMean()
{
	if(protein1->m_rof!=NULL){
		protein1->m_rof->use_curvature=MEAN;
	}
	// TODO: Add your control notification handler code here
	curv1=MEAN;
}


void CReadModeDlg::OnClickedUseMean2()
{
	// TODO: Add your control notification handler code here
	if(protein2->m_rof!=NULL){
		protein2->m_rof->use_curvature=MEAN;
	}
	curv2=MEAN;
}


void CReadModeDlg::OnUseGauss()
{
	if(protein1->m_rof!=NULL){
		protein1->m_rof->use_curvature=GAUSS;
	}
	// TODO: Add your command handler code here
	curv1=GAUSS;
}


void CReadModeDlg::OnUseGauss2()
{
	// TODO: Add your command handler code here
	if(protein2->m_rof!=NULL){
		protein2->m_rof->use_curvature=GAUSS;
	}
	curv2=GAUSS;
}


void CReadModeDlg::OnClickedUseNone()
{
	// TODO: Add your control notification handler code here
	if(protein1->m_rof!=NULL){
		protein1->m_rof->use_curvature=NONE;
	}
	// TODO: Add your command handler code here
	curv1=NONE;
}


void CReadModeDlg::OnClickedUseNone2()
{
	// TODO: Add your control notification handler code here
	if(protein2->m_rof!=NULL){
		protein2->m_rof->use_curvature=NONE;
	}
	curv2=NONE;
}


void CReadModeDlg::OnBnClickedUseSi()
{
	// TODO: Add your control notification handler code here
	if(protein1->m_rof!=NULL){
		protein1->m_rof->use_curvature=SHAPEINDEX;
	}
	// TODO: Add your command handler code here
	curv1=SHAPEINDEX;
}


void CReadModeDlg::OnBnClickedUseSi2()
{
	// TODO: Add your control notification handler code here
	if(protein2->m_rof!=NULL){
		protein2->m_rof->use_curvature=SHAPEINDEX;
	}
	curv2=SHAPEINDEX;
}
