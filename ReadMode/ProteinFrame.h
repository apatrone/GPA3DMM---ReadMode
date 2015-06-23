
// ReadModeDlg.h : header file
//

#pragma once
#include <glm/glm.hpp>
#include "ReadOBJFile.h"
#include "GL/glut.h"
#include <tbb/tbb.h>

#define ROTATE_SPEED	  0.05


// CReadModeDlg dialog
class ProteinFrame 
{
// Construction
public:
	ProteinFrame();
	int LoadProtein(char *Path);
	bool Draw(void);
	void LoadFrame(CWnd *pDlg);
	void KeyInput(int wPAram, int nTimes);
	void Reset(void);
	void ResetView(void);
	bool Kill();
	// Generated message map functions
public:
	typedef enum{ FILL, POINT, LINE}PolygonMode;
	ReadOBJFile *m_rof;
	CDC*pDC;
	HGLRC m_hRC;
	float rot;
	bool flag;
	float m_x;
	float m_y;
	float m_z;
	double previousTime;
	float m_rot_x;
	float m_rot_y;
	float m_rot_z;
	bool m_init;
	bool m_move;
	bool m_rot;
	bool m_read;
	bool m_rotation;
	bool m_LDown;
	bool m_AutoRotation;
	float m_Scale;	
	bool flag_threadCreated;
	tbb::mutex lock;
	bool kill_thread;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	double currentTime;
	double deltaTime;
	float speed;
	CWnd *wnd;
	std::thread t;
	bool m_useNE; //normal estimation
	GLenum polygon_mode;

};
