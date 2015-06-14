#include "StdAfx.h"
#include "ProteinView.h"
#include <GL/glew.h>
ProteinView::ProteinView(void)
{
	mouseSpeed=0.005;
	speed=3.0;
	horizontalAngle=3.14;
	verticalAngle=0.0;
	FoV=45.0;
}

int ProteinView::ChangeView(CWnd *pDlg, ProteinFrame *protein){
	pDlg->EnableWindow(); 
	while(1){

	}
	return 1;
}
ProteinView::~ProteinView(void)
{
}
