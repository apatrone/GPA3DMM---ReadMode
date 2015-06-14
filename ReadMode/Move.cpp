// Move.cpp : implementation file
//

#include "stdafx.h"
#include "ReadMode.h"
#include "Move.h"
#include "afxdialogex.h"
#include "ReadModeDlg.h"


// CMove dialog

IMPLEMENT_DYNAMIC(CMove, CDialog)

CMove::CMove(CWnd* pParent /*=NULL*/)
	: CDialog(CMove::IDD, pParent)
{

	m_x1 = 0.0f;
	m_y1 = 0.0f;
	m_z1 = 0.0f;
}

CMove::~CMove()
{
}

void CMove::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDITX, m_x1);
	DDX_Text(pDX, IDC_EDITY, m_y1);
	DDX_Text(pDX, IDC_EDITZ, m_z1);
}

void CMove::OnOK()
{
	UpdateData(TRUE);
	
}
BEGIN_MESSAGE_MAP(CMove, CDialog)
END_MESSAGE_MAP()


// CMove message handlers
