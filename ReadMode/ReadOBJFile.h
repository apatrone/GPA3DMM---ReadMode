#pragma once
#include <GL/glew.h>
#include <GL/GLAUX.H>
#include <glm/glm.hpp>
#include <vector>
#include <tbb/compat/thread>
#include <cmath>
//#include "vector3.h"

typedef enum Curvature{MEAN, GAUSS, SHAPEINDEX,SGF, NONE};

typedef struct  Vertex        //�������Ϣ
{
	GLfloat x;
	GLfloat y;
	GLfloat z;
	int nVertexIndex;
	glm::vec3 normal; //vertex normal
	float kG;  //discrete gaussian curvature
	float kM;  //discrete mean curvature
	float shape_index;
	float kmin;
	float kmax;
	float SGF;
	float area;

	//std::vector<GLEdge> edges;
	
	
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
typedef struct Edge{  //Relative to a vertex v
	glm::vec3 e;   //edge, equal to v2 - v
	glm::vec3 p;  //other vertex of the edge, v2
}GLEdge; 


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

typedef  struct Edges{
	glm::vec3 e1;
	glm::vec3 e2;

}OrderedEdges;
//-------------
typedef struct { double x, y,z;int group; int original_index;} point_t, *point;
//-------------

class ReadOBJFile
{
public:
	ReadOBJFile(bool useNE);
	~ReadOBJFile(void);
	int EstimateNormals(void);
	void Draw();
	bool ReadFile(char * FileNmae);
	bool ReadLine(FILE *fp,char *str);
	std::vector<glm::vec3> OrderEdges(std::vector<glm::vec3> edges);
	std::vector<GLEdge> OrderGLEdges(std::vector<GLEdge> edges); //----
	std::vector<int>  ReadOBJFile::OrderCluster(std::vector<int> cluster);
	float GetShapeIndex( int i);
	void GetInfo();
	void GetMtlInfo();
	void GetCluster(void);
	void EstimatekGkM(void);
	void EstimateSGF(void);
	bool VertexEqual(GLVertex v1, GLVertex v2);
	float GetAngle(glm::vec3 v1, glm::vec3 v2,  bool direction=true);
	float GetArea(glm::vec3 v1, glm::vec3 v2);
	float GetMixedArea(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);
	glm::vec3 GetCrossProduct(glm::vec3 v1,glm::vec3 v2);
	bool ReadOBJFile::Collinear(glm::vec3 v1,glm::vec3 v2);
	void SimilarityMeasurement(void);
	//------------------------------
	double randf(double m);
	point gen_xy();
	inline double dist2(point a, point b);
	inline int nearest(point pt, point cent, int n_cluster, double *d2);
	void kpp(point pts, int len, point cent, int n_cent);
	point lloyd(point pts, int len, int n_cluster);

	//------------------------------
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
	std::vector<int>* cluster_indices;
	int gauss_sup;
	int gauss_inf;
	Curvature use_curvature;
	bool use_ridgeorvalley;
	point clusters;
};

