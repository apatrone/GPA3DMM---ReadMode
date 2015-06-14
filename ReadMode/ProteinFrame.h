
// ReadModeDlg.h : header file
//

#pragma once

#include "ReadOBJFile.h"
#include "GL/glut.h"
#include <tbb/tbb.h>

#include "TrackBall.h"
#define ROTATE_SPEED	  0.05


// CReadModeDlg dialog
class ProteinFrame 
{
// Construction
public:
	ProteinFrame();
	int LoadProtein(char *Path);
	void Draw(void);
	void LoadFrame(CWnd *pDlg);

	// Generated message map functions
public:
	ReadOBJFile *m_rof;
	CDC*pDC;
	HGLRC m_hRC;
	float rot;
	bool flag;
	float m_x;
	float m_y;
	float m_z;
	
	float m_rot_x;
	float m_rot_y;
	float m_rot_z;
	
	bool m_move;
	bool m_rot;
	bool m_read;
	bool m_rotation;
	bool m_LDown;
	bool m_AutoRotation;//自动旋转控制
	float m_Scale;	
	bool flag_threadCreated;
	tbb::mutex lock;
	bool kill_thread;

};
