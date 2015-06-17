#pragma once
#include <GL/glew.h>
#include <GL/GLAUX.H>
#include <glm/glm.hpp>
#include <vector>
#include <tbb/compat/thread>
#include <cmath>
//#include "vector3.h"


typedef struct  Vertex        //�������Ϣ
{
	GLfloat x;
	GLfloat y;
	GLfloat z;
	int nVertexIndex;
	glm::vec3 normal; //vertex normal
	float kG;  //gaussian curvature
	float kM;  //discrete mean curvature
	
	//std::vector<Vector3> normal;
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
	ReadOBJFile(bool useNE);
	~ReadOBJFile(void);
	int EstimateNormals(void);
	void Draw();
	bool ReadFile(char * FileNmae);
	bool ReadLine(FILE *fp,char *str);
	void GetInfo();
	void GetMtlInfo();
	void EstimatekGkM(void);
	bool VertexEqual(GLVertex v1, GLVertex v2);
	float GetAngle(glm::vec3 v1, glm::vec3 v2);
	float GetArea(glm::vec3 v1, glm::vec3 v2);
	glm::vec3 GetCrossProduct(glm::vec3 v1,glm::vec3 v2);
	char  m_FileName[256];
	int m_nCount;
	GLVertex *m_v;
	GLVertex *m_vcalc;
	GLNormal * m_vn;
	GLNormal *m_calcNorm;
	GLTexture* m_vt;
	GLFace  **m_face;
	GLMtl * m_mtl;
	Vertex *vertex;
	int lastj;
	int length;
	bool init;
	bool vn; bool vt;//indicate if the file contains normal and texture information 
	int res;
	int size_m_v;
	bool useNormalEstimation;
	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3>* normal_buffer;
};

