#pragma once
#include "ProteinFrame.h"
class ProteinView
{
public:
	ProteinView(void);
	int ChangeView(CWnd *pDlg,ProteinFrame *protein);
	~ProteinView(void);
public: 
	double previousTime;
	double currentTime;
	float deltaTime;
	double mouseX, mouseY;
	float mouseSpeed;
	float speed;
	float FoV;
	float verticalAngle;
	float horizontalAngle;
	
};

