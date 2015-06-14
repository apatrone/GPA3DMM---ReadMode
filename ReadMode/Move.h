#pragma once


// CMove dialog

class CMove : public CDialog
{
	DECLARE_DYNAMIC(CMove)

public:
	CMove(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMove();
	void OnOK();
// Dialog Data
	//enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	float m_x1;
	float m_y1;
	float m_z1;
};
