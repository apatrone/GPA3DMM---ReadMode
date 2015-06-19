#include "gl/glut.h"
#include <math.h>

//#define _USE_MATH_DEFINES
#if !defined (TRACKBALL_H)
#define  TRACKBALL_H
//#define M_PI       3.14159265358979323846

#include "ProteinFrame.h"


class CTrackBall
{
public: 
	CTrackBall(int w, int h) {winWidth=w;winHeight=h;  init();}
	~CTrackBall() {};

	void SetWinWidth(int w) { winWidth=w; }
	void SetWinHeight(int h){ winHeight=h; }

	void init()
	{
		trackingMouse=FALSE;
		angle=0.0f;
		for(int i=0; i<3; i++)   
		{
			axis[i]=0.0f;
			lastPos[i]=0.0f;
		} 
		angle=0.0f;
		//for the use of mouse
		curX=0;
		curY=0;
		startX=0; 
		startY=0;
		for (int i=0;i<16;i++)
		{
			if(i%5==0)
			{
				rotaarry[i]=1.0f;
			}
			else
			{
				rotaarry[i]=0.0f;
			}
		}
		glPushMatrix();
		glLoadIdentity();
		glGetFloatv(GL_MODELVIEW_MATRIX,rotaarry);

		glPopMatrix();
	}

	void MakeRotate()
	{

		if (trackingMouse)
		{
			glPushMatrix();
			glLoadIdentity();
			glRotatef(angle, axis[0], axis[1], axis[2]);
			glMultMatrixf(rotaarry);

			glGetFloatv(GL_MODELVIEW_MATRIX,rotaarry);
			glPopMatrix();

		}	
		glMultMatrixf(rotaarry);
		// 		if (trackingMouse)	
		// 			glRotatef(angle, axis[0], axis[1], axis[2]);
	}
	//根据二维的坐标x，y产生一组三维的坐标，单位化后存放到v中
	void trackball_Prov(int x, int y, float v[3])
	{
		GLfloat    d, a;

		/*Project x,y to a hemi-sphere centered within width,height*/
		v[0]=(2.0f*x-winWidth) / winWidth;
		v[1]=(winHeight-2.0f*y) / winHeight;

		d=(float)sqrt(v[0]*v[0]+v[1]*v[1]);
		v[2]=(float)cos ( (3.14/2.0f)* ( (d<1.0) ? d : 1.0f ) );


		//make identity  单位化
		a=1.0F / (float)sqrt(d*d+v[2]*v[2]);
		v[0] *=a;
		v[1] *=a;
		v[2] *=a;
		

	}

	//GUI的接口函数
	void onMouseMotion(int x, int y)
	{
		float curPos[3], dx, dy, dz;

		trackball_Prov(x, y, curPos);

		dx=curPos[0]-lastPos[0];
		dy=curPos[1]-lastPos[1];
		dz=curPos[2]-lastPos[2];

		if (trackingMouse)
		{
			angle = 90.0F*sqrt(dx*dx+dy*dy+dz*dz);

			//vector Cross:lastPos×curPos;
			axis[0] = lastPos[1]*curPos[2] - lastPos[2]*curPos[1];
			axis[1] = lastPos[2]*curPos[0] - lastPos[0]*curPos[2];
			axis[2] = lastPos[0]*curPos[1] - lastPos[1]*curPos[0];

			//refresh the lastPos
			lastPos[0] = curPos[0];
			lastPos[1] = curPos[1];
			lastPos[2] = curPos[2];
		}


	}
	//交互方式可以修改的，


	void startMotion(int x, int y)
	{
		trackingMouse = true;
		//		reDrawContinue = false;
		startX = x;    startY = y;
		curX = x;    curY = y;

		trackball_Prov(x, y, lastPos);
		//    trackBallMove = true;
	}

	void stopMotion()
	{
		trackingMouse = false;
		//          if(startX != x || startY!= y)
		//              reDrawContinue=true;
		//          else
		//          {
		angle=0.0; 
		//        reDrawContinue=false;   // trackBallMove = false;*/
		//     }
	}

	//control switch variable:
	bool trackingMouse;
	//     bool trackBallMove;
	float angle;
	float axis[3];
private:
	int winWidth, winHeight;

	
	float lastPos[3];
	GLfloat  rotaarry[16];
	//for the use of mouse
	int curX, curY;
	int startX, startY;

};



#endif

