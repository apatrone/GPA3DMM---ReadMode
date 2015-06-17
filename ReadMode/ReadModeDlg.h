
// ReadModeDlg.h : header file
//

#pragma once

#include "ReadOBJFile.h"
#include "GL/glut.h"
#include "ProteinFrame.h"
#include "ProteinView.h"
#include "TrackBall.h"
#include "afxwin.h"
#include "afxcmn.h"

#define ROTATE_SPEED	  0.05


// CReadModeDlg dialog
class CReadModeDlg : public CDialogEx
{
// Construction
public:
	CReadModeDlg(CWnd* pParent = NULL);	// standard constructor
	

// Dialog Data
	enum { IDD = IDD_READMODE_DIALOG };
	
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:

	ProteinFrame *protein1;
	ProteinFrame *protein2;
	ProteinView *protView;
	int m_move;
	afx_msg void OnBnClickedReadm();
	afx_msg void OnBnClickedMovem();
	afx_msg void OnBnClickedRoatm();
	afx_msg void OnBnClickedUpdate();
	afx_msg void OnTimer(UINT_PTR nIDEvent);


	
	
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnRotation();
	afx_msg void OnBnClickedShow2();
	afx_msg void OnBnClickedReadm2();
	afx_msg void OnBnClickedShow();
	afx_msg void OnCbnSelchangeCombo1();
	
	CPoint m_MouseL;
	CPoint m_start;
	CComboBox m_ComboMove;
	CString m_infoBox;
	CWnd *m_infobox_handle;
	CEdit m_info;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	CSliderCtrl m_SpeedSlider;
	bool m_init;
	afx_msg void OnStnClickedFrame1();
	CStatic frame1;
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnEnChangeInfo();
	CButton m_checkbox1;
	CButton m_checkbox2;
};
