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
	m_z=-5.0;
	m_move=false;
	m_read=false;

	m_rot_x=0;
	m_rot_y=0;
	m_rot_z=0;
	m_rotation=0;
	//自动旋转为y轴，默认不旋转
	m_rof=NULL;//读取obj文件的对象
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

	bool res = loadOBJ(path, vertices, uvs, normals);

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
			glColorMaterial(GL_FRONT_AND_BACK,GL_DIFFUSE);//是环境材质颜色跟踪当前颜色
			glEnable(GL_COLOR_MATERIAL);//启用材质颜色跟踪当前颜色
			p->m_rof->Draw();
			::glPopMatrix();
			//::glEnable(GL_DEPTH_TEST);
			::glFlush();
			::SwapBuffers(p->pDC->m_hDC);
		}
		
	}
	p->kill_thread=false;

	/*::wglMakeCurrent(p->pDC->m_hDC,p->m_hRC);

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
		glColorMaterial(GL_FRONT_AND_BACK,GL_DIFFUSE);//是环境材质颜色跟踪当前颜色
		glEnable(GL_COLOR_MATERIAL);//启用材质颜色跟踪当前颜色
		
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

	glDeleteVertexArrays(1, &VertexArrayID);
	*/
	return 1;
}

void ProteinFrame::Draw(void)
{	
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return ;
	}
	if(!flag_threadCreated){
		std::thread thread(MyThread, this);
		flag_threadCreated=true;
		kill_thread=0;
	}
	else{
		kill_thread=true;
		//je peux pas faire de thread.join()ici car il est d閏lar� dans un if :(
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
			glColorMaterial(GL_FRONT_AND_BACK,GL_DIFFUSE);//是环境材质颜色跟踪当前颜色
			glEnable(GL_COLOR_MATERIAL);//启用材质颜色跟踪当前颜色
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

	::glViewport(0,0,rect.Width(),rect.Height()); //重置当前视口
	
	::glMatrixMode(GL_PROJECTION);//选择投影矩阵
	::glLoadIdentity(); //重置投影矩阵
	::gluPerspective(45.0,rect.Width()/rect.Height(),0.1,1000.0);
	
	::glMatrixMode(GL_MODELVIEW);   //选择模型观察矩阵
	::glLoadIdentity();
	
	::glShadeModel(GL_SMOOTH);  //启用阴影平滑
	::glClearColor(0.0f,0.0f,0.0f,0.3f); //设置黑色背景
	
	::glClearDepth(1.0f);  //设置深度缓存
	::glEnable(GL_DEPTH_TEST); //启用深度测试
	::glDepthFunc(GL_LEQUAL);// 所作深度测试类型
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
	//设置光照
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