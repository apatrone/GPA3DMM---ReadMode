#include "stdafx.h"
#include "ProteinFrame.h"
#include "objloader.hpp"
//#include <tbb/tbb.h>
#include <tbb/compat/condition_variable>
#include <tbb/compat/thread>
#include <GL/glew.h>
#include "shader.hpp"
#include "controls.hpp"

ProteinFrame::ProteinFrame(void)
{
	m_x=0;
	m_y=-0.5;
	m_z=-10.0;
	m_move=false;
	m_read=false;
	m_init=false;
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
	speed=60;
	m_useNE=false;
}

int ProteinFrame::LoadProtein(char *path){
	m_read=true;
	/*if(flag)
	{
		delete m_rof;
		m_rof=NULL;
	}*/
	if(flag_threadCreated==true){
		kill_thread=true;
		while(kill_thread){};
		Reset();
	}
	m_rof=new ReadOBJFile(m_useNE);
	m_rof->ReadFile(path);
	//int res = loadOBJ(path, vertices, uvs, normals);

	flag=true;
	return 1;
}

void* MyThread(ProteinFrame *p)
{	

/*	GLfloat ambient[]={0.5f,0.5f,0.5f,1.0f};
	GLfloat diffuse[]={1.0f,1.0f,1.0f,1.0f};
	GLfloat specular[]={1.0f,1.0f,1.0f,1.0f};
	GLfloat position[]={0.0f,0.0f,0.0f,1.0f};

	GLfloat ambient1[]={0.0f,0.1f,0.8f,1.0f};
	GLfloat diffuse1[]={0.0f,0.3f,0.6f,1.0f};
	GLfloat specular1[]={1.0f,0.0f,1.0f,1.0f};
	GLfloat shininess[]={10.0f};
	
	
	PIXELFORMATDESCRIPTOR pfd={sizeof(PIXELFORMATDESCRIPTOR),1,PFD_SUPPORT_OPENGL|PFD_DRAW_TO_WINDOW|PFD_DOUBLEBUFFER,PFD_TYPE_RGBA,32,
	0,0,0,0,0,0,0,0,0,0,0,0,0,16,0,0,PFD_MAIN_PLANE,0,0,0,0};

	
	p->pDC=p->wnd->GetDC();

	int PixelFormat=::ChoosePixelFormat(p->pDC->m_hDC,&pfd);
	::SetPixelFormat(p->pDC->m_hDC,PixelFormat,&pfd);
	//save protein frame context 
	p->m_hRC=::wglCreateContext(p->pDC->m_hDC);
	::wglMakeCurrent(p->pDC->m_hDC,p->m_hRC); 
	
	CRect rect;
	p->wnd->GetClientRect(rect);

	::glViewport(0,0,rect.Width(),rect.Height()); //÷ÿ÷√µ±«∞ ”ø⁄
	::glEnable(GL_CULL_FACE);
	::glMatrixMode(GL_PROJECTION);//—°‘ÒÕ∂”∞æÿ’Û
	::glLoadIdentity(); //÷ÿ÷√Õ∂”∞æÿ’Û
	::gluPerspective(45.0,rect.Width()/rect.Height(),0.1,1000.0);
	
	::glMatrixMode(GL_MODELVIEW);   //—°‘Òƒ£–Õπ€≤Ïæÿ’Û
	::glLoadIdentity();
	
	::glShadeModel(GL_SMOOTH);  //∆Ù”√“ı”∞∆Ωª¨
	::glClearColor(1.0f,1.0f,1.0f,0.3f); //…Ë÷√∫⁄…´±≥æ∞
	
	::glClearDepth(1.0f);  //…Ë÷√…Ó∂»ª∫¥Ê
	::glEnable(GL_DEPTH_TEST); //∆Ù”√…Ó∂»≤‚ ‘
	::glDepthFunc(GL_LESS);// À˘◊˜…Ó∂»≤‚ ‘¿‡–Õ
	/*::glLightfv(GL_LIGHT1,GL_AMBIENT,ambient);
	::glLightfv(GL_LIGHT1,GL_DIFFUSE,diffuse);
	::glLightfv(GL_LIGHT1,GL_SPECULAR,specular);
	::glLightfv(GL_LIGHT1,GL_POSITION,position);
	::glLightfv(GL_FRONT_AND_BACK,GL_AMBIENT,ambient1);
	::glLightfv(GL_FRONT_AND_BACK,GL_DIFFUSE,diffuse1);
	::glLightfv(GL_FRONT_AND_BACK,GL_SPECULAR,specular1);
	::glLightfv(GL_FRONT_AND_BACK,GL_POSITION,shininess);
	::glEnable(GL_LIGHTING);
	::glEnable(GL_LIGHT1);
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
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition); */
	
	
	p->LoadFrame(p->wnd);///////////////////////////////////////////////////////////////////

	while(p->kill_thread==false){
		::wglMakeCurrent(p->pDC->m_hDC,p->m_hRC);
	
		//if(p->flag){
			glDrawBuffer(GL_FRONT_AND_BACK);
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
			::glEnable(GL_DEPTH_TEST);
			::glFlush();//::glFinish();
			::SwapBuffers(p->pDC->m_hDC);
			glDrawBuffer(GL_BACK);
	//	}		
	}
	p->kill_thread=false;
	p->flag_threadCreated=false;
/*	::wglMakeCurrent(p->pDC->m_hDC,p->m_hRC);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
	GLuint programID = LoadShaders( "StandardShading.vertexshader", "StandardShading.fragmentshader" );

	GLuint MatrixID = glGetUniformLocation(programID, "MVP");
	GLuint ViewMatrixID = glGetUniformLocation(programID, "V");
	GLuint ModelMatrixID = glGetUniformLocation(programID, "M");


		// Compute the MVP matrix from keyboard and mouse input
		computeMatricesFromInputs();
		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix = getViewMatrix();
		glm::mat4 ModelMatrix = glm::mat4(1.0);

		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);




	// Load it into a VBO

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, p->vertices.size() * sizeof(glm::vec3), &p->vertices[0], GL_STATIC_DRAW);

	GLuint uvbuffer;
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, p->uvs.size() * sizeof(glm::vec2), &p->uvs[0], GL_STATIC_DRAW);

	GLuint normalbuffer;
	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, p->normals.size() * sizeof(glm::vec3), &p->normals[0], GL_STATIC_DRAW);

	// Get a handle for our "LightPosition" uniform
	glUseProgram(programID);
	GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");

	do{

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

		::glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		::glLoadIdentity();
		//::glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
		//::glDisable(GL_DEPTH_TEST);
		::glPushMatrix();


		glm::vec3 lightPos = glm::vec3(4,4,4);
		glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);


		// 1rst attribute buffer : p->vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// 2nd attribute buffer : p->uvs
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(
			1,                                // attribute
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// 3rd attribute buffer : p->normals
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
		glVertexAttribPointer(
			2,                                // attribute
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// Draw the triangles !
		glDrawArrays(GL_TRIANGLES, 0, p->vertices.size() );

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		::glColor3f(0.0f,1.0f,1.0f);
		glColorMaterial(GL_FRONT_AND_BACK,GL_DIFFUSE);// «ª∑æ≥≤ƒ÷ —’…´∏˙◊Ÿµ±«∞—’…´
		glEnable(GL_COLOR_MATERIAL);//∆Ù”√≤ƒ÷ —’…´∏˙◊Ÿµ±«∞—’…´
		
		::glPopMatrix();
		//::glEnable(GL_DEPTH_TEST);
		::glFlush();
		::SwapBuffers(p->pDC->m_hDC);
	} // Check if the ESC key was pressed or the window was closed
	while( 1);

	// Cleanup VBO and shader
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &uvbuffer);
	glDeleteBuffers(1, &normalbuffer);
	glDeleteProgram(programID);

	glDeleteVertexArrays(1, &VertexArrayID);*/
	
	return (void*)1;
}

bool ProteinFrame::Draw(void)
{	
	/*LoadFrame(wnd);
	glewExperimental=TRUE;
	GLenum err=glewInit();
//	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return false ;
	}*/

	//std::thread thread(MyThread, this);
	//thread.join();
//	return true;

	if(flag_threadCreated==false){
		std::thread thread(MyThread, this);
		std::thread::id id; id=t.get_id();
		//pthread_create(&thread,NULL, MyThread, (void *)this);
		flag_threadCreated=true;
		kill_thread=false;
	}
	else{
		kill_thread=true;
		//je peux pas faire de thread.join()ici car il est dÈclarÈ dans un if :(
		while(kill_thread){};
		std::thread thread(MyThread, this);
		kill_thread=false;
	}
	return true;
	/*LoadFrame(wnd);
	//define protein frame as curent context 
	::wglMakeCurrent(pDC->m_hDC,m_hRC);
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
			::glEnable(GL_DEPTH_TEST);
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
	
	if(m_init==false){
		PIXELFORMATDESCRIPTOR pfd={sizeof(PIXELFORMATDESCRIPTOR),1,PFD_SUPPORT_OPENGL|PFD_DRAW_TO_WINDOW|PFD_DOUBLEBUFFER,PFD_TYPE_RGBA,32,
		0,0,0,0,0,0,0,0,0,0,0,0,0,16,0,0,PFD_MAIN_PLANE,0,0,0,0};
		m_init=true;
	
	
		wnd=pDlg;
	
		pDC=pDlg->GetDC();

		int PixelFormat=::ChoosePixelFormat(pDC->m_hDC,&pfd);
		::SetPixelFormat(pDC->m_hDC,PixelFormat,&pfd);
		//save protein frame context 
		m_hRC=::wglCreateContext(pDC->m_hDC);
		::wglMakeCurrent(pDC->m_hDC,m_hRC); 
	}
	CRect rect;
	pDlg->GetClientRect(rect);

	::glViewport(0,0,rect.Width(),rect.Height()); //÷ÿ÷√µ±«∞ ”ø⁄
	::glEnable(GL_CULL_FACE);
	::glMatrixMode(GL_PROJECTION);//—°‘ÒÕ∂”∞æÿ’Û
	::glLoadIdentity(); //÷ÿ÷√Õ∂”∞æÿ’Û
	::gluPerspective(45.0,rect.Width()/rect.Height(),0.1,1000.0);
	
	::glMatrixMode(GL_MODELVIEW);   //—°‘Òƒ£–Õπ€≤Ïæÿ’Û
	::glLoadIdentity();
	
	::glShadeModel(GL_SMOOTH);  //∆Ù”√“ı”∞∆Ωª¨
	::glClearColor(1.0f,1.0f,1.0f,0.3f); //…Ë÷√∫⁄…´±≥æ∞
	
	::glClearDepth(1.0f);  //…Ë÷√…Ó∂»ª∫¥Ê
	::glEnable(GL_DEPTH_TEST); //∆Ù”√…Ó∂»≤‚ ‘
	::glDepthFunc(GL_LESS);// À˘◊˜…Ó∂»≤‚ ‘¿‡–Õ
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
	//::glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	//::glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
	//::glHint(GL_POLYGON_SMOOTH_HINT,GL_NICEST);
}

void ProteinFrame::KeyInput(int wParam, int nTimes){
	static double previousTime=::GetTickCount();
	const double z=0.01;const double y=0.9;
	currentTime=::GetTickCount();
	deltaTime = float(currentTime - previousTime)/1000;//for seconde /1000 
	
	switch(wParam){
		case (int)'W': case (int)'Z':  case VK_UP: 
			m_rot_x+=deltaTime*speed;  //more fluid but only object on the left moves, wtf
			//m_rot_x+=y+nTimes/10000000000;
			//m_z+=deltaTime*nTimes/1000000000;
			//m_z+=z+nTimes/10000000000;			
			break;
		case (int)'Q': case (int)'A': case VK_LEFT:
			m_rot_y-=deltaTime*speed;
			//m_rot_y-= y+nTimes/10000000000;
			previousTime = currentTime;
			break;
		case (int)'S': case VK_DOWN: 
			m_rot_x-=deltaTime*speed;
			//m_rot_x-=y+nTimes/10000000000;
			//m_z-= z+nTimes/10000000000;			
			break;
		case (int)'D':  case VK_RIGHT:
			m_rot_y+=deltaTime*speed;
			//m_rot_y+= y+nTimes/10000000000;
			break;
	}
	previousTime = currentTime;		
}

void ProteinFrame::Reset(void){
	m_x=0;
	m_y=-0.5;
	m_z=-10.0;
	m_move=false;
	m_read=false;

	m_rot_x=0;
	m_rot_y=0;
	m_rot_z=0;
	m_rotation=0;
	//◊‘∂Ø–˝◊™Œ™y÷·£¨ƒ¨»œ≤ª–˝◊™
	free(m_rof);
	flag=false;
	m_AutoRotation=false;
	flag_threadCreated=false;
	kill_thread=false;
	//m_init=false;
}

bool ProteinFrame::Kill(void)
{
	kill_thread=true;
	while(kill_thread==false){}
	return true;

}