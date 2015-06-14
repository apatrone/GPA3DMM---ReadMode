#pragma once
#include <GL/glew.h>
#include <GL/GLAUX.H>

typedef struct  Vertex        //�������Ϣ
{
	GLfloat x;
	GLfloat y;
	GLfloat z;
	int nVertexIndex;
} GLVertex ;
typedef  struct Normal         //���淨��������Ϣ
{
	GLfloat  x;
	GLfloat  y;
	GLfloat  z;
	int nNormalIndex;
}GLNormal;
typedef  struct Texture         //������ͼ��Ϣ
{
	GLfloat  x;
	GLfloat  y;
	GLfloat  z;
	int nTextureIndex;
}GLTexture;
typedef  struct Face            //��������Ϣ
{
	int Vertex;      //ָ����������
	int Texture;     //ָ��������ͼ��������
	int Normal;      //ָ��������������
	char Mtl[256];    //������Ϣ
}GLFace;
typedef  struct Mtl//���������Ϣ
{
	char MtlName[256];    //��������
	GLfloat ka[3];     //���������
	GLfloat kd[3];      //���������
	GLfloat ks[3];      //��������
	GLfloat ke[3];      //ɢ�����
	GLfloat ns;         //���ʵĹ�����
}GLMtl;
class ReadOBJFile
{
public:
	ReadOBJFile(void);
	~ReadOBJFile(void);
public:
	void Draw();
	bool ReadFile(char * FileNmae);
	bool ReadLine(FILE *fp,char *str);
	void GetVertexInfo();
	void GetMtlInfo();
	void GetTextureInfo();
	void GetNormalInfo();
	char  m_FileName[256];
	int m_nCount;
	GLVertex *m_v;
	GLNormal * m_vn;
	GLTexture* m_vt;
	GLFace  **m_face;
	GLMtl * m_mtl;
};

