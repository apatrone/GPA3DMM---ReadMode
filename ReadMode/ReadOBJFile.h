#pragma once
#include <GL/glew.h>
#include <GL/GLAUX.H>
#include <glm/glm.hpp>
#include <vector>
#include <tbb/compat/thread>
#include <cmath>
//#include "vector3.h"


typedef struct  Vertex        //保存点信息
{
	GLfloat x;
	GLfloat y;
	GLfloat z;
	int nVertexIndex;
	glm::vec3 normal;
	//std::vector<Vector3> normal;
} GLVertex ;
typedef  struct Normal         //保存法线向量信息
{
	GLfloat  x;
	GLfloat  y;
	GLfloat  z;
	int nNormalIndex;
}GLNormal;
typedef  struct Texture         //保存贴图信息
{
	GLfloat  x;
	GLfloat  y;
	GLfloat  z;
	int nTextureIndex;
}GLTexture;



typedef  struct Face            //保存面信息
{
	int Vertex;      //指定顶点索引
	int Texture;     //指定纹理贴图坐标索引
	int Normal;      //指定法线向量索引
	char Mtl[256];    //材质信息
}GLFace;
typedef  struct Mtl//保存材质信息
{
	char MtlName[256];    //材质名称
	GLfloat ka[3];     //环境光分量
	GLfloat kd[3];      //漫反射分量
	GLfloat ks[3];      //镜面光分量
	GLfloat ke[3];      //散射分量
	GLfloat ns;         //材质的光亮度
}GLMtl;
class ReadOBJFile
{
public:
	ReadOBJFile(void);
	~ReadOBJFile(void);
	int EstimateNormals(void);
	void Draw();
	bool ReadFile(char * FileNmae);
	bool ReadLine(FILE *fp,char *str);
	void GetInfo();
	void GetMtlInfo();
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
	int res;
	int size_m_v;
	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3>* normal_buffer;
};

