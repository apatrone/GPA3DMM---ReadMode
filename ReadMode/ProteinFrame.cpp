#include "stdafx.h"
#include "ProteinFrame.h"

//#include <tbb/tbb.h>
#include <tbb/compat/condition_variable>
#include <tbb/compat/thread>

ProteinFrame::ProteinFrame(void)
{
	m_x=0;
	m_y=-0.5;
	m_z=-5.0;
	m_move=false;
	m_read=false;

	m_rot_x=0;
	m_rot_y=0;
	m_rot_z=0;
	m_rotation=0;
	//◊‘∂Ø–˝◊™Œ™y÷·£¨ƒ¨»œ≤ª–˝◊™
	m_rof=NULL;//∂¡»°objŒƒº˛µƒ∂‘œÛ
	flag=false;
	m_rot=false;
	m_AutoRotation=false;
	flag_threadCreated=false;
	kill_thread=false;
}

int ProteinFrame::LoadProtein(char *path){
	m_read=true;
	if(flag)
	{
		delete m_rof;
		m_rof=NULL;
	}
	m_rof=new ReadOBJFile();
	m_rof->ReadFile(path);
	flag=true;
	return 1;
}

bool MyThread(ProteinFrame *p)
{
	while(!p->kill_thread){
		::wglMakeCurrent(p->pDC->m_hDC,p->m_hRC);
		if(p->flag){
			::glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
			::glLoadIdentity();
			//::glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
			//::glDisable(GL_DEPTH_TEST);
			::glPushMatrix();
			::glTranslatef(p->m_x,p->m_y,p->m_z);
			if(p->m_AutoRotation)
				p->m_rot_y+=1;
			glRotated(p->m_rot_z, 0.0f, 0.0f, 1.0f);
			glRotated(p->m_rot_y, 0.0f, 1.0f, 0.0f);
			glRotated(p->m_rot_x, 1.0f, 0.0f, 0.0f); 

			::glColor3f(0.0f,1.0f,1.0f);
			glColorMaterial(GL_FRONT_AND_BACK,GL_DIFFUSE);// «ª∑æ≥≤ƒ÷ —’…´∏˙◊Ÿµ±«∞—’…´
			glEnable(GL_COLOR_MATERIAL);//∆Ù”√≤ƒ÷ —’…´∏˙◊Ÿµ±«∞—’…´
			p->m_rof->Draw();
			::glPopMatrix();
			//::glEnable(GL_DEPTH_TEST);
			::glFlush();
			::SwapBuffers(p->pDC->m_hDC);
		}
	}
	p->kill_thread=false;
	return 1;
}

void ProteinFrame::Draw(void)
{	
	
	if(!flag_threadCreated){
		std::thread thread(MyThread, this);
		flag_threadCreated=true;
		kill_thread=0;
	}
	else{
		kill_thread=true;
		//je peux pas faire de thread.join()ici car il est dÈclarÈ dans un if :(
		while(kill_thread){};
		std::thread thread(MyThread, this);
		kill_thread=false;
	}
	

	//define protein frame as curent context 
/*	::wglMakeCurrent(pDC->m_hDC,m_hRC);
		if(flag){
			::glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
			::glLoadIdentity();
			//::glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
			//::glDisable(GL_DEPTH_TEST);
			::glPushMatrix();
			::glTranslatef(m_x,m_y,m_z);
			if(m_AutoRotation)
				m_rot_y+=1;
			glRotated(m_rot_z, 0.0f, 0.0f, 1.0f);
			glRotated(m_rot_y, 0.0f, 1.0f, 0.0f);
			glRotated(m_rot_x, 1.0f, 0.0f, 0.0f); 

			::glColor3f(0.0f,1.0f,1.0f);
			glColorMaterial(GL_FRONT_AND_BACK,GL_DIFFUSE);// «ª∑æ≥≤ƒ÷ —’…´∏˙◊Ÿµ±«∞—’…´
			glEnable(GL_COLOR_MATERIAL);//∆Ù”√≤ƒ÷ —’…´∏˙◊Ÿµ±«∞—’…´
			m_rof->Draw();
			::glPopMatrix();
			//::glEnable(GL_DEPTH_TEST);
			::glFlush();
			::SwapBuffers(pDC->m_hDC);
		}*/
}

void ProteinFrame::LoadFrame(CWnd *pDlg)
{
// TODO: Add extra initialization here
	GLfloat ambient[]={0.5f,0.5f,0.5f,1.0f};
	GLfloat diffuse[]={1.0f,1.0f,1.0f,1.0f};
	GLfloat specular[]={1.0f,1.0f,1.0f,1.0f};
	GLfloat position[]={0.0f,0.0f,0.0f,1.0f};

	GLfloat ambient1[]={0.0f,0.1f,0.8f,1.0f};
	GLfloat diffuse1[]={0.0f,0.3f,0.6f,1.0f};
	GLfloat specular1[]={1.0f,0.0f,1.0f,1.0f};
	GLfloat shininess[]={10.0f};
	
	
	PIXELFORMATDESCRIPTOR pfd={sizeof(PIXELFORMATDESCRIPTOR),1,PFD_SUPPORT_OPENGL|PFD_DRAW_TO_WINDOW|PFD_DOUBLEBUFFER,PFD_TYPE_RGBA,32,
		0,0,0,0,0,0,0,0,0,0,0,0,0,16,0,0,PFD_MAIN_PLANE,0,0,0,0};

	
	pDC=pDlg->GetDC();

	int PixelFormat=::ChoosePixelFormat(pDC->m_hDC,&pfd);
	::SetPixelFormat(pDC->m_hDC,PixelFormat,&pfd);
	//save protein frame context 
	m_hRC=::wglCreateContext(pDC->m_hDC);
	::wglMakeCurrent(pDC->m_hDC,m_hRC); 
	
	CRect rect;
	pDlg->GetClientRect(rect);

	::glViewport(0,0,rect.Width(),rect.Height()); //÷ÿ÷√µ±«∞ ”ø⁄
	
	::glMatrixMode(GL_PROJECTION);//—°‘ÒÕ∂”∞æÿ’Û
	::glLoadIdentity(); //÷ÿ÷√Õ∂”∞æÿ’Û
	::gluPerspective(45.0,rect.Width()/rect.Height(),0.1,1000.0);
	
	::glMatrixMode(GL_MODELVIEW);   //—°‘Òƒ£–Õπ€≤Ïæÿ’Û
	::glLoadIdentity();
	
	::glShadeModel(GL_SMOOTH);  //∆Ù”√“ı”∞∆Ωª¨
	::glClearColor(0.0f,0.0f,0.0f,0.3f); //…Ë÷√∫⁄…´±≥æ∞
	
	::glClearDepth(1.0f);  //…Ë÷√…Ó∂»ª∫¥Ê
	::glEnable(GL_DEPTH_TEST); //∆Ù”√…Ó∂»≤‚ ‘
	::glDepthFunc(GL_LEQUAL);// À˘◊˜…Ó∂»≤‚ ‘¿‡–Õ
	/*::glLightfv(GL_LIGHT1,GL_AMBIENT,ambient);
	::glLightfv(GL_LIGHT1,GL_DIFFUSE,diffuse);
	::glLightfv(GL_LIGHT1,GL_SPECULAR,specular);
	::glLightfv(GL_LIGHT1,GL_POSITION,position);
	::glLightfv(GL_FRONT_AND_BACK,GL_AMBIENT,ambient1);
	::glLightfv(GL_FRONT_AND_BACK,GL_DIFFUSE,diffuse1);
	::glLightfv(GL_FRONT_AND_BACK,GL_SPECULAR,specular1);
	::glLightfv(GL_FRONT_AND_BACK,GL_POSITION,shininess);
	::glEnable(GL_LIGHTING);
	::glEnable(GL_LIGHT1);*/
	//…Ë÷√π‚’’
	glEnable(GL_LIGHTING);  
    glEnable(GL_LIGHT0);  
	
	const GLfloat lightAmbient[]  = {0.08, 0.08, 0.08, 1.0};  
	const GLfloat lightDiffuse[]  = {1.0, 1.0, 1.0, 1.0};  
	const GLfloat lightSpecular[]  = {1.0, 1.0, 1.0, 1.0};  
	const GLfloat lightPosition[] = {25, 25, 25, 0.0}; 
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);  
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);  
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);  
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition); 
	
	//::glDisable(GL_LIGHTING);
	//::glEnable(GL_LINE_SMOOTH);
	//::glEnable(GL_POLYGON_SMOOTH);
	//::glEnable(GL_BLEND);
   // ::glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	//::glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
	//::glHint(GL_POLYGON_SMOOTH_HINT,GL_NICEST);
}