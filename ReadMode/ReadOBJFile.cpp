#include "StdAfx.h"
#include "ReadOBJFile.h"
#include <string>
#include <glm/glm.hpp>
#include <vector>
#include <cmath> //for sqrt
#include <algorithm> 
#define _USE_MATH_DEFINES
#include <math.h> //for M_PI
#include <list>

ReadOBJFile::ReadOBJFile(bool useNE)
{	
	memset(this->m_FileName,0,256);
	m_nCount=0;
	lastj=0;
	length=0;
	res=0;
	init=false;
	size_m_v=0;
	useNormalEstimation=useNE;
	vn=false;  vt=false;
	gauss_inf=10;
	gauss_sup=60;
	use_curvature=MEAN;
	use_ridgeorvalley=true;
	cluster_number=48;
	AllocRGB();
}
ReadOBJFile::~ReadOBJFile(void)
{
}
void ReadOBJFile::GetInfo()
{
	FILE * fp;
	fp=fopen(this->m_FileName,"rb");
	char str[256];
	int nVertex=1;
	int nTexture=1;
	int nNormal=1;
	char MtlName[256];
	memset(MtlName,0,256);
	size_m_v=0;
	int nMtl=0;

	while (1)
	{
		memset(str,0,256);
		int tmp = fscanf(fp, "%s", str);
		if (tmp == EOF)
			break;
		if (strcmp( str, "v" ) == 0 )
		{
			if (nVertex==1)
			{
				this->m_v=(GLVertex *)malloc(sizeof(GLVertex));
			} 
			else
			{
				this->m_v=(GLVertex *)realloc(this->m_v,(nVertex-1)*sizeof(GLVertex)+sizeof(GLVertex));
			}
			m_v[nVertex-1].nVertexIndex=nVertex;
			fscanf(fp, "%f %f %f\n", &m_v[nVertex-1].x, &m_v[nVertex-1].y, &m_v[nVertex-1].z );
			nVertex++;
		}
		else if(strcmp( str, "vt" ) == 0)
		{
			vt=true;
			int nEdge=0;
			for(int x=0;str[x]!='\0';x++)
			{
				if(str[x]==' '&&str[x+1]!=' '&&str[x+1]!='\0')
					nEdge++;
			}
			if(nTexture==1)
				this->m_vt=(Texture*)malloc(sizeof(Texture));
			else
				this->m_vt=(Texture*)realloc(this->m_vt,(nTexture-1)*sizeof(Texture)+sizeof(Texture));
			m_vt[nTexture-1].nTextureIndex=nTexture;
			m_vt[nTexture-1].z=0.0;
			fscanf(fp, "%f %f\n", &m_vt[nTexture-1].x, &m_vt[nTexture-1].y );
			nTexture++;
		}
		else if(strcmp( str, "vn" ) == 0)
		{
			vn=true;
			if(nNormal==1)
				this->m_vn=(Normal*)malloc(sizeof(Normal));
			else
				this->m_vn=(Normal*)realloc(this->m_vn,(nNormal-1)*sizeof(Normal)+sizeof(Normal));
			m_vn[nNormal-1].nNormalIndex=nNormal;
			fscanf(fp, "%f %f %f\n", &m_vn[nNormal-1].x, &m_vn[nNormal-1].y, &m_vn[nNormal-1].z );
			nNormal++;
		}
		else if (strncmp("usemtl",str,6)==0)
		{
				int i=0;
				while (i!=m_nCount)
				{
					if (strcmp(this->m_mtl[i].MtlName,str+7)==0)
					{ 
						const GLfloat matAmbient[]    = {this->m_mtl[i].ka[0],this->m_mtl[i].ka[1],this->m_mtl[i].ka[2], 0.5};  
						const GLfloat matDiffuse[]    = {this->m_mtl[i].kd[0],this->m_mtl[i].kd[1],this->m_mtl[i].kd[2], 0.5};  
						const GLfloat matSpecular[]   = {this->m_mtl[i].ks[0],this->m_mtl[i].ks[1],this->m_mtl[i].ks[2], 0.5};  
						const GLfloat matEmission[]   = {this->m_mtl[i].ke[0],this->m_mtl[i].ke[1],this->m_mtl[i].ke[2], 0.5};     
						const GLfloat lightShininess[]={this->m_mtl[i].ns};
						glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matAmbient);  
						glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDiffuse);  
						glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpecular);  
						glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, matEmission);  
						glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, lightShininess); 
						break;
					}
					else
						i++;
				}
		}
		else if (strcmp( str, "f" ) == 0)
			{
				//PAS DE GESTION DES QUADS!
				unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
				if(vt==true && vn==true){
					int matches = fscanf(fp, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
					if (matches != 9){
						printf("File can't be read by our simple parser :-( Try exporting with other options\n");
						return;
					}
					vertexIndices.push_back(vertexIndex[0]-1);
					vertexIndices.push_back(vertexIndex[1]-1);
					vertexIndices.push_back(vertexIndex[2]-1);
					uvIndices    .push_back(uvIndex[0]-1);
					uvIndices    .push_back(uvIndex[1]-1);
					uvIndices    .push_back(uvIndex[2]-1);
					normalIndices.push_back(normalIndex[0]-1);
					normalIndices.push_back(normalIndex[1]-1);
					normalIndices.push_back(normalIndex[2]-1);
				}
				else if(vt==true && vn==false){
					int matches = fscanf(fp, "%d/%d %d/%d %d/%d\n", &vertexIndex[0], &uvIndex[0], &vertexIndex[1], &uvIndex[1],  &vertexIndex[2], &uvIndex[2]);
					if (matches != 6){
						printf("File can't be read by our simple parser :-( Try exporting with other options\n");
						return ;
					}
					vertexIndices.push_back(vertexIndex[0]-1);
					vertexIndices.push_back(vertexIndex[1]-1);
					vertexIndices.push_back(vertexIndex[2]-1);
					uvIndices    .push_back(uvIndex[0]-1);
					uvIndices    .push_back(uvIndex[1]-1);
					uvIndices    .push_back(uvIndex[2]-1);
				}
				else if(vt==false && vn==true){
					int matches = fscanf(fp, "%d//%d %d//%d %d//%d\n", &vertexIndex[0], &normalIndex[0], &vertexIndex[1], &normalIndex[1], &vertexIndex[2], &normalIndex[2] );
					if (matches != 6){
						printf("File can't be read by our simple parser :-( Try exporting with other options\n");
						return ;
					}
					vertexIndices.push_back(vertexIndex[0]-1);
					vertexIndices.push_back(vertexIndex[1]-1);
					vertexIndices.push_back(vertexIndex[2]-1);
					normalIndices.push_back(normalIndex[0]-1);
					normalIndices.push_back(normalIndex[1]-1);
					normalIndices.push_back(normalIndex[2]-1);
				}
				else if(vt==false && vn==false){
					int matches = fscanf(fp, "%d %d %d\n", &vertexIndex[0], &vertexIndex[1],&vertexIndex[2] );
					if (matches != 3){
						printf("File can't be read by our simple parser :-( Try exporting with other options\n");
						return;
					}
					vertexIndices.push_back(vertexIndex[0]-1);
					vertexIndices.push_back(vertexIndex[1]-1);
					vertexIndices.push_back(vertexIndex[2]-1);

				}

		}
		else{
			// Probably a comment, eat up the rest of the line
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, fp);
		}
	}
	init=true;
	size_m_v=nVertex-1;
	res=(vt==true)+2*(vn==true); 
	
	
}
void ReadOBJFile::GetMtlInfo()
{
	FILE *fp;
	fp=fopen(this->m_FileName,"rb");
	char str[256];
	char MtlName[256];
	memset(MtlName,0,256);
	int nMtl=0;
	while (1)
	{
		memset(str,0,256);
		int tmp = fscanf(fp, "%s", str);
		if (tmp == EOF)
			break;
		if(strncmp("mtllib",str,6)==0)
		{
			int n1=6,n2;
			int start=0;
			for(int j=6;str[j]!='\0';j++)
			{
				if(str[j]==' '&&str[j+1]!=' ')
				{
					start=j+1;
					break;
				}
			}
			if(start!=0)
				n1=start;
			for(int i=n1;;i++)
			{
				if(str[i]==' '||str[i]=='\0')
				{
					n2=i;
					break;
				}  
			}
			if(str[n1]==9)
				n1+=1;
			strncpy(MtlName,str+n1,n2-n1);
			fclose(fp);
			break;
		}
	}
	int n=0;
	for(int j=0;this->m_FileName[j]!='\0';j++)
	{
		if(m_FileName[j]=='\\')
			n=j+1;
	}
	strcpy(MtlName+n,MtlName);
	strncpy(MtlName,m_FileName,n);
	if((fp=fopen(MtlName,"rb"))==NULL)
		return;
	n=1;
	char * Data;
	while (1)
	{
		memset(str,0,256);
		int tmp = fscanf(fp, "%s", str);
		if (tmp == EOF)
			break;
		int nCount=0;
		if(str[0]==' '||str[0]==9)
		{
			for (int j=0;str[j]!='\0';j++)
			{
				if((str[j]==' '&&str[j+1]!=' ')||(str[0]==9&&str[j+1]!=9))
				{
					nCount=j+1;
					break;
				}
			}
		}
		if(strncmp("newmtl",str+nCount,6)==0)
		{
			nCount++;
			if(n==1)
				this->m_mtl=(GLMtl*)malloc(sizeof(GLMtl));
			else
				this->m_mtl=(GLMtl*)realloc(this->m_mtl,(n-1)*sizeof(GLMtl)+sizeof(GLMtl));
			memset(this->m_mtl[n-1].MtlName,0,256);
			strcpy(this->m_mtl[n-1].MtlName,str+nCount+7);
			n++;
		}
		if (strncmp("Ns",str+nCount,2)==0)
		{
			int start;
			for(int i=nCount;str[i]!='\0';i++)
			{
				if (str[i]==' '&&str[i+1]!=' ')
				{
					start=i+1;
					break;
				}
			}
			sscanf(str+start,"%f",&this->m_mtl[2].ns);
		}
		if (strncmp("Ka",str+nCount,2)==0)
		{
			int start;
			for (int i=nCount;str[i]!='\0';i++)
			{
				if (str[i]==' '&&str[i+1]!=' ')
				{
					start=i+1;
					break;
				}
			}
			int n1=start,n2;
			int flag=1;
			while (flag!=4)
			{
				for (int i=n1;;i++)
				{
					if (str[i]==' '||str[i]=='\0')
					{
						n2=i;
						break;
					}
				}
				Data=new char[n2-n1+1];
				memset(Data,0,n2-n1+1);
				strncpy(Data,str+n1,n2-n1);
				sscanf(Data,"%f",&this->m_mtl[n-2].ka[flag-1]);
				delete Data;
				n1=n2+1;
				flag++;
			}
		}
		if (strncmp("Kd",str+nCount,2)==0)
		{
			int start;
			for (int i=nCount;str[i]!='\0';i++)
			{
				if (str[i]==' '&&str[i+1]!=' ')
				{
					start=i+1;
					break;
				}
			}
			int n1=start,n2;
			int flag=1;
			while (flag!=4)
			{
				for (int i=n1;;i++)
				{
					if (str[i]==' '||str[i]=='\0')
					{
						n2=i;
						break;
					}
				}
				Data=new char[n2-n1+1];
				memset(Data,0,n2-n1+1);
				strncpy(Data,str+n1,n2-n1);
				sscanf(Data,"%f",&this->m_mtl[n-2].kd[flag-1]);
				delete Data;
				n1=n2+1;
				flag++;
			}
		}
		if (strncmp("Ks",str+nCount,2)==0)
		{
			int start;
			for (int i=nCount;str[i]!='\0';i++)
			{
				if (str[i]==' '&&str[i+1]!=' ')
				{
					start=i+1;
					break;
				}
			}
			int n1=start,n2;
			int flag=1;
			while (flag!=4)
			{
				for (int i=n1;;i++)
				{
					if (str[i]==' '||str[i]=='\0')
					{
						n2=i;
						break;
					}
				}
				Data=new char[n2-n1+1];
				memset(Data,0,n2-n1+1);
				strncpy(Data,str+n1,n2-n1);
				sscanf(Data,"%f",&this->m_mtl[n-2].ks[flag-1]);
				delete Data;
				n1=n2+1;
				flag++;
			}
		}
		if (strncmp("Ke",str+nCount,2)==0)
		{
			int start;
			for (int i=nCount;str[i]!='\0';i++)
			{
				if (str[i]==' '&&str[i+1]!=' ')
				{
					start=i+1;
					break;
				}
			}
			int n1=start,n2;
			int flag=1;
			while (flag!=4)
			{
				for (int i=n1;;i++)
				{
					if (str[i]==' '||str[i]=='\0')
					{
						n2=i;
						break;
					}
				}
				Data=new char[n2-n1+1];
				memset(Data,0,n2-n1+1);
				strncpy(Data,str+n1,n2-n1);
				sscanf(Data,"%f",&this->m_mtl[n-2].ke[flag-1]);
				delete Data;
				n1=n2+1;
				flag++;
			}
		}
	

	}
	int j=6;
	j++;
}
bool ReadOBJFile::ReadFile(char *FileName)
{
	FILE * fp;
	if((fp=fopen(FileName,"rb"))==NULL)
		return false;
	strcpy(this->m_FileName,FileName);
	fclose(fp);
	this->GetInfo();
	this->GetMtlInfo();
	EstimateNormals();
	GetCluster();
	EstimatekGkM();
	EstimateSGF();
	/*GetCluster(true);
	SimilarityMeasurement();*/
}

void ReadOBJFile::Draw()
{
	//print colours
	//CString str; str="";
	//char* s= new char[50];
	//for(int i=0; i<cluster_number;i++){
	//	sprintf(s, "%f \t %f \t %f\n", rgb[i][0], rgb[i][1], rgb[i][2]);
	//	str+=s;
	//}

	float comp;
	::glBegin(GL_TRIANGLES);
	#pragma parallel for private(comp)
	for(int i=0; i<vertexIndices.size(); i++)
	{	
		if(use_curvature==SHAPEINDEX ){
			comp=m_vcalc[vertexIndices[i]].shape_index;
			/*else if(use_curvature==SGF)	//scaling for SGF not good, too many results white
				comp=2 * (m_vcalc[vertexIndices[i]].SGF - min_SGF)/( max_SGF - min_SGF) - 1;*/
			/*if(comp> 0){
				::glColor3f(comp,0.2f,0.2f);
			}
			else if(comp <0)
				::glColor3f(0.2f,::abs(comp),0.2f);
			else
				::glColor3f(0.0f,1.0f,1.0f);*/
			if(comp<-0.875)
				::glColor3f(0.0f,1.0f,0.0f);
			else if(comp<-0.625)
				::glColor3f(0.0f,1.0f,0.5f);
			else if(comp<-0.375)
				::glColor3f(0.0f,1.0f,1.0f);
			else if(comp<-0.125)
				::glColor3f(0.5f,1.0f,1.0f);
			else if(comp<0.125)
				::glColor3f(1.0f,1.0f,1.0f);
			else if(comp<0.375)
				::glColor3f(1.0f,1.0f,0.5f);
			else if(comp<0.625)
				::glColor3f(1.0f,1.0f,0.0f);
			else if(comp<0.875)
				::glColor3f(1.0f,0.5f,0.0f);
			else
				::glColor3f(1.0f,0.0f,0.0f);
		}
		else if(use_curvature==LLOYD){
			//use colours of lloyd clustering
			int colour_index=m_vcalc[vertexIndices[i]].group;
			::glColor3f(rgb[colour_index][0],rgb[colour_index][1],rgb[colour_index][2]);
		}

		else if(use_curvature!=NONE ){
			if(use_curvature==GAUSS)
				comp=m_vcalc[vertexIndices[i]].kG * 100;				
			else if(use_curvature==MEAN)
				comp=m_vcalc[vertexIndices[i]].kM *100;
			else if(use_curvature==SGF)	
				comp=m_vcalc[vertexIndices[i]].SGF *100;
			
			if(comp>gauss_sup)  
			{::glColor3f(0.5f,1.0f,0.0f); } //green
			else if(comp<gauss_inf) 
			{::glColor3f(0.5f,0.0f,0.5f);}  //mauve
			else
			{
				::glColor3f(0.0f,1.0f,1.0f); //turquoise					
			}
		}
		else{
			::glColor3f(0.0f,1.0f,1.0f);
		}
		if(res>=2 && useNormalEstimation==false)  //if not using estimated normals
			::glNormal3f(this->m_vn[normalIndices[i]].x,this->m_vn[normalIndices[i]].y,this->m_vn[normalIndices[i]].z);
		else
		{   //if using estimated normals
			::glNormal3f(m_vcalc[vertexIndices[i]].normal.x, m_vcalc[vertexIndices[i]].normal.y,m_vcalc[vertexIndices[i]].normal.z);
		}	
		if(res==1 || res==3 ) //if texture is detailed in the file
			::glTexCoord3f(this->m_vt[uvIndices[i]].x,this->m_vt[uvIndices[i]].y,this->m_vt[uvIndices[i]].z);
			
		::glVertex3f(m_v[vertexIndices[i]].x,this->m_v[vertexIndices[i]].y,this->m_v[vertexIndices[i]].z);			
	}

	::glEnd();
}	
int ReadOBJFile::EstimateNormals(void)
{
	//to make the program faster, it would be better to estimate normals in the same function that we estimate kMkG
	
	std::vector<glm::vec3>* normal_buffer = new std::vector<glm::vec3>[vertexIndices.size()]();
	//#pragma omp parallel for  //Fait planter sur certains obj.... (1HLB, etc)
	for( int i = 0; i < vertexIndices.size()-2; i += 3 )
	{
		// get the three vertices that make the faces
		glm::vec3 p1 = glm::vec3(m_v[vertexIndices[i+0]].x, m_v[vertexIndices[i+0]].y, m_v[vertexIndices[i+0]].z);
		glm::vec3 p2 = glm::vec3(m_v[vertexIndices[i+1]].x, m_v[vertexIndices[i+1]].y, m_v[vertexIndices[i+1]].z);
		glm::vec3 p3 =  glm::vec3(m_v[vertexIndices[i+2]].x, m_v[vertexIndices[i+2]].y, m_v[vertexIndices[i+2]].z);
 
		glm::vec3 v1 = p2 - p1;
		glm::vec3 v2 = p3 - p1; 
		//cross product
		glm::vec3 normal=GetCrossProduct(v1,v2);

		//normalize  ------->better to do it after?
	 /*  float length = normal.x * normal.x + normal.y * normal.y + normal.z * normal.z;
	   length = sqrt(length);
	   normal.x /= length; normal.y /= length; normal.z /= length;*/
 
	   // Store the face's normal for each of the vertices that make up the face.
	   normal_buffer[vertexIndices[i+0]].push_back( normal );
	   normal_buffer[vertexIndices[i+1]].push_back( normal );
	   normal_buffer[vertexIndices[i+2]].push_back( normal );
	 }

	 m_vcalc=(GLVertex *)malloc(sizeof(GLVertex)* size_m_v);
	 // Now loop through each vertex vector, and average out all the normals stored.
	 #pragma omp parallel for 
	 for( int i = 0; i < size_m_v; ++i )   //900 tjr pas de réécriture  ... mais réécriture sur m_vn avant 925, mais pas sur uvindices
	 {
		m_vcalc[i].x=m_v[i].x; m_vcalc[i].y=m_v[i].y; m_vcalc[i].z=m_v[i].z;
		m_vcalc[i].normal=glm::vec3(0,0,0);
		#pragma omp parallel for  // changes result
	   for( int j = 0; j < normal_buffer[i].size(); ++j )
	   {	  
		 m_vcalc[i].normal += normal_buffer[i][j];
	   }
		m_vcalc[i].normal /= normal_buffer[i].size(); //This line doesn't seem to change much
		//code that was above before
	   float length = m_vcalc[i].normal.x *m_vcalc[i].normal.x + m_vcalc[i].normal.y * m_vcalc[i].normal.y + m_vcalc[i].normal.z * m_vcalc[i].normal.z;
	   length = sqrt(length);
	   m_vcalc[i].normal.x /= length; m_vcalc[i].normal.y /= length; m_vcalc[i].normal.z /= length;
	 }

	 //could have used glm::normalize(glm::cross(c - a, b - a));....
	 return true;
}
void ReadOBJFile::GetCluster(bool lloyd){
	//an array of columns, where cluster_indices[i] contains the indices of the vertices that are adjacent to vi

	if(lloyd==false){ //use adjacent clustering
		cluster_indices=new std::vector<int>[size_m_v]();
		#pragma omp parallel for schedule(static)
		for(int i=0; i<size_m_v; i++) //for each vertex
		{
			#pragma omp parallel for schedule(guided)  //Crashes when alone
			for(int j=0; j<vertexIndices.size()-2; j+=3) //for each faceof the mesh
			{	
				//if current vertex belongs to the current triangle/face
				if(vertexIndices[j]==i|| vertexIndices[j+1]==i || vertexIndices[j+2]==i)
				{

					if( vertexIndices[j]==i)
					{
						cluster_indices[i].push_back(vertexIndices[j+1]);
						cluster_indices[i].push_back(vertexIndices[j+2]);
					}
					else if(vertexIndices[j+1]==i)
					{
						cluster_indices[i].push_back(vertexIndices[j]);
						cluster_indices[i].push_back(vertexIndices[j+2]);
					}
					else if(vertexIndices[j+2]==i)
					{
						cluster_indices[i].push_back(vertexIndices[j]);
						cluster_indices[i].push_back(vertexIndices[j+1]);
					}
				}
			}

			cluster_indices[i]=OrderCluster(cluster_indices[i]);
		}
	}
	else{ //use lloyd clustering
		//cluster_indices_lloyd=new std::vector<int>[size_m_v]();
		cluster_indices_lloyd=new std::vector<int>[cluster_number]();
		point v = gen_xy();
		clusters = this->lloyd(v, size_m_v,cluster_number);
		//create array of size_m_v data points containing for vi all the vertices in its cluster
		/*for(int i=0; i<size_m_v; i++){ //for each vertex
			for (int j = 0; j < size_m_v; j++){
				if(v[i].group == v[j].group && v[j].original_index != i){ //if same group and not same point
					cluster_indices_lloyd[i].push_back(v[j].original_index);
				}
			}
			cluster_indices_lloyd[i]=OrderCluster(cluster_indices_lloyd[i]);
		}*/
		//create array of 48 groups: each column contains vertices of same group
		//#pragma omp parallel for
		for (int j = 0; j < size_m_v; j++){
			if(v[j].group>47)
				int r=1;
			cluster_indices_lloyd[v[j].group].push_back(v[j].original_index);
			m_vcalc[v[j].original_index].group=v[j].group;			
		}
		
		//CString str; str="";
		//char* s= new char[50];
		////print clusters
		//for(int i=0; i<cluster_number; i++){
		//	for(int j=0; j<cluster_indices_lloyd[i].size(); j++){
		//		sprintf(s, "%d", cluster_indices_lloyd[i][j]);
		//		str+=s;
		//		if(j==cluster_indices_lloyd[i].size()-1)
		//			str+="\n";
		//		else
		//			str+="\t";
		//	}

		//}
	}
}
void ReadOBJFile::EstimatekGkM(void)
{ 
	  CString str; str="[";
	//std::sort(s.begin(), s.end(), customLess);
	//#pragma omp parallel for schedule(static)
	for(int i=0; i<size_m_v; i++) //for each vertex
	{
		if(cluster_indices[i].size()!=0)
		{
			float sum_angles=0; //for kG
			float sum_area=0;//for kG and kM
			float sum_dihedral_angles=0; //for kM
			std::vector<GLEdge> order_edges;
			float angle;
			float area;
			//------
			GLEdge e1;
			GLEdge e2;
			glm::vec3 p1 = glm::vec3(m_vcalc[i].x, m_vcalc[i].y, m_vcalc[i].z);	
			//#pragma omp parallel for  schedule(static) //not good idea because the list wont be in order after
			for(int j=0; j<cluster_indices[i].size(); j+=1)
			{

				glm::vec3 p2 =  glm::vec3(m_vcalc[cluster_indices[i][j]].x, m_vcalc[cluster_indices[i][j]].y, m_vcalc[cluster_indices[i][j]].z);
				glm::vec3 v2 = p2 - p1;
				e1.e=v2;
				e1.p=p2;
				order_edges.push_back(e1);
			}
		
			glm::vec3 cross_edge;
			glm::vec3 cross_next_edge;

			GLEdge prev_edge;
			GLEdge current_edge;
			GLEdge nxt_edge;
			int ridge_or_valley=1;
			float coeff;

			for(int k=1; k<order_edges.size()-1; k++){ 
				prev_edge=order_edges[k-1];
				current_edge=order_edges[k];
				nxt_edge=order_edges[k+1];
				//angle for kG
				sum_angles+=GetAngle(prev_edge.e,current_edge.e);
				//area for kG and kM

				sum_area+=GetArea(prev_edge.e,current_edge.e);
				//get normals and calculate dihedral angle (angle between normals) * length of edge
				cross_edge=glm::cross(prev_edge.e, current_edge.e);
				cross_next_edge=glm::cross(current_edge.e, nxt_edge.e);

				if(use_ridgeorvalley){
					coeff=glm::dot((nxt_edge.p - prev_edge.p ) , cross_edge/glm::length(cross_edge));
					if(coeff<=-0.02)
						ridge_or_valley=1; //convex
					else if(coeff>=0.02)
						ridge_or_valley=-1; //concave
					else 
						ridge_or_valley=0;//planar
				}

				sum_dihedral_angles+= ridge_or_valley * GetAngle(cross_edge/glm::length(cross_edge),cross_next_edge/glm::length(cross_next_edge), false) * glm::length(current_edge.e);
		
			}
			//angle for kG
			sum_angles+=GetAngle(current_edge.e, nxt_edge.e) + GetAngle( nxt_edge.e,order_edges[0].e) ;
			//area for kG and kM
			sum_area+=GetArea(current_edge.e, nxt_edge.e)+ GetArea( nxt_edge.e,order_edges[0].e);

			cross_edge=glm::cross(current_edge.e,  nxt_edge.e);
			cross_next_edge=glm::cross( nxt_edge.e, order_edges[0].e);


			if(use_ridgeorvalley){
				coeff=glm::dot((order_edges[0].p - current_edge.p ) , cross_edge/glm::length(cross_edge));
				if(coeff<=-0.02)
					ridge_or_valley=1; //convex
				else if(coeff>=0.02)
					ridge_or_valley=-1; //concave
				else 
					ridge_or_valley=0;//planar
			}

			sum_dihedral_angles+=ridge_or_valley*GetAngle(cross_edge/glm::length(cross_edge),cross_next_edge/glm::length(cross_next_edge), false) * glm::length(nxt_edge.e);
		
			cross_edge=glm::cross( nxt_edge.e,  order_edges[0].e);
			cross_next_edge=glm::cross(order_edges[0].e, order_edges[1].e);

			if(use_ridgeorvalley){

				coeff=glm::dot((order_edges[1].p - nxt_edge.p ) , cross_edge/glm::length(cross_edge));
				if(coeff<=-0.02)
					ridge_or_valley=1; //convex
				else if(coeff>=0.02)
					ridge_or_valley=-1; //concave
				else 
					ridge_or_valley=0;//planar

			}
			
			sum_dihedral_angles+=ridge_or_valley*GetAngle(cross_edge/glm::length(cross_edge),cross_next_edge/glm::length(cross_next_edge), false)* glm::length( order_edges[0].e);

			if(use_ridgeorvalley){
				//calc kG
				m_vcalc[i].kG= (2* M_PI - sum_angles)/(sum_area/3) ;  
				//calc kM
				m_vcalc[i].kM=  sum_dihedral_angles/(4*sum_area/3 ) ;
			}
			else{
				//calc kG
				m_vcalc[i].kG= (2* M_PI - sum_angles)/(sum_area) ;  
				//calc kM
				m_vcalc[i].kM= sum_dihedral_angles/(4*sum_area) ;
			}
			if(sum_angles > 2 * M_PI) //in theory the sum of angles can not be greater then 2*M_PI
				m_vcalc[i].kG= 0;
			m_vcalc[i].shape_index = GetShapeIndex(i);
			m_vcalc[i].area=sum_area;

		}
		else{
			m_vcalc[i].kG= 0;
			m_vcalc[i].kM= 0;
			m_vcalc[i].shape_index = 0;
			m_vcalc[i].area=0;
			
		}
			char *s=new char[10];
			sprintf(s, "%f\n ", m_vcalc[i].shape_index);
			str+=s;
	}

}
void ReadOBJFile::EstimateSGF(void){


	#pragma omp parallel for schedule(static)
	for(int i=0; i<size_m_v; i++) //for each vertex
	{
		if(cluster_indices[i].size()!=0){
			float variance_SI=0;
			float sum_shape_index=0;
			float mean_SI;
			float sum=0;
			GLVertex v ;
			//#pragma omp parallel for  private(v) schedule(static) //dangerous because all are modifying the samevariables?
			for(int j=0; j<cluster_indices[i].size(); j+=1){ 
				v =m_vcalc[cluster_indices[i][j]];
				//area for kG and kM
				sum+= v.area*pow(v.shape_index, 3); 
				//sum_area+=current_edge.v.area; 
				sum_shape_index+=v.shape_index;
			}
			mean_SI= sum_shape_index/cluster_indices[i].size();
			//#pragma omp parallel for private (v) schedule(static)
			for(int j=0; j<cluster_indices[i].size(); j+=1){  //Estimate variance of the shape index of the vertices in the cluster
				v =m_vcalc[cluster_indices[i][j]];
				variance_SI+= pow( v.shape_index-mean_SI,2);
			}

			variance_SI=variance_SI/cluster_indices[i].size();
			m_vcalc[i].SGF= 0.5 * sum+0.5*variance_SI;
			int r = m_vcalc[i].SGF;
			if(r < 0)
				int p=1;
	
		}
		else
			 m_vcalc[i].SGF=0;

	}
	
}
bool ReadOBJFile::VertexEqual(GLVertex v1, GLVertex v2)
{
	if(v1.x==v2.x && v1.y==v2.y &&v1.z==v2.z )
		return true;
	return false;
}
float ReadOBJFile::GetAngle(glm::vec3 v1, glm::vec3 v2, bool direction){ 
	//glm::vec3 v1= glm::vec3(4,4,2); //for test 
	//glm::vec3 v2= glm::vec3(2,3,5); //for test
	//double prod_sum = v1.x * v2.x + v1.y * v2.y +  v1.z * v2.z;   //dot product...
	double prod_sum=glm::dot(glm::normalize(v1), glm::normalize(v2));
	//double abs_v1= sqrt( v1.x * v1.x + v1.y * v1.y +  v1.z * v1.z);  //glm::length
	//double abs_v2=sqrt( v2.x * v2.x + v2.y * v2.y +  v2.z * v2.z); //glm::length
	if(prod_sum > 1) 
		prod_sum=1;
	else if(prod_sum < -1)
		prod_sum = -1;
	double c=acos( prod_sum );//(abs_v1*abs_v2)>0? ::acos( prod_sum / (abs_v1*abs_v2)): (M_PI/2); //if division by zero cos= 0
	//if(direction){   //don't want obtuse angles
	//	/*if(c>M_PI/2 && c <= M_PI)
	//		return M_PI - c;
	//	else return c;*/
	//	if(c>M_PI && c <= ( 2*M_PI))
	//		return 2*M_PI - c;
	//	else if(c<0)
	//		return 2* M_PI + c;
	//	else return c; //always this
	//	
	//}
	//if(prod_sum<0)
	//	return -c;
	return c;

}
float ReadOBJFile::GetArea(glm::vec3 v1, glm::vec3 v2) 
{
	//glm::vec3 v1= glm::vec3(-3,1,-7); //for test 
	//glm::vec3 v2= glm::vec3(0,-5,-5); //for test
	glm::vec3 v = glm::cross(v1,v2);
	float area= 0.5* sqrt( v.x * v.x + v.y * v.y + v.z * v.z );  //half the absolute value of the cross product
	//float k= glm::dot(v1,v2) ; for test...

	return area;
}
float ReadOBJFile::GetMixedArea(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3){
		float a1,a2,a3;
		glm::vec3 v1= p2-p1;   //p1 = 
		glm::vec3 v2= p3-p2;
		glm::vec3 v3= p1-p3;
		a1 = GetAngle(v1, v2);
		a2 = GetAngle(v2, v3);
		a3 = GetAngle(v3, v1);
		if(a1 > M_PI || a2> M_PI || a3 > M_PI){ //triangle obtuse, not safe for voronoi
			//if angle at v1 obtuse return area(triangle)/2
			
			//else  return area(triangle)/4
		}
		else{
			//return voronoi area
		}
	return 0;
} //not finished writing
glm::vec3 ReadOBJFile::GetCrossProduct(glm::vec3 v1,glm::vec3 v2) //glm::cross....
{

	glm::vec3 v=glm::vec3((v1.y * v2.z) - (v1.z * v2.y),
						(v1.z * v2.x) - (v1.x* v2.z),
						(v1.x* v2.y) - (v1.y * v2.x));
	return v;

}
std::vector<glm::vec3> ReadOBJFile::OrderEdges(std::vector<glm::vec3> edges){
	//std::vector<glm::vec3> edges;  //For test
	//glm::vec3 v1=glm::vec3(1);
	//glm::vec3 v2=glm::vec3(2);
	//glm::vec3 v3=glm::vec3(3);
	//glm::vec3 v4=glm::vec3(4);
	//edges.push_back(v1);edges.push_back(v2);edges.push_back(v3);edges.push_back(v4);edges.push_back(v2);edges.push_back(v4);	edges.push_back(v1);edges.push_back(v3);
	
	
	for(int i=0; i<edges.size()-2; i+=2){
		if( edges[i+1] != edges[i] ){
			if(edges[i+1]== edges[i+3]){ //if next couple of vectors has it's second element equal to the second vector of this couple
				glm::vec3 tmp=edges[i+2]; //swap the vectors of the next couple
				edges[i+2]=edges[i+3];
				edges[i+3]=tmp;
			}
			else{    //find a couple of vectors which contain v1
				for(int j=i+4; j<edges.size(); j+=2){ //find another couple which contains the second vector
					if( edges[i+1] == edges[j] ){  //if the couple of vectors has it's first element equal to the second vector of this couple swap
						glm::vec3 tmp=edges[i+2];
						edges[i+2]=edges[j];
						edges[j]=tmp;
						tmp=edges[i+3];
						edges[i+3]=edges[j+1];
						edges[j+1]=tmp;
						break;
					}
					else if( edges[i+1] == edges[j+1] ){//if the couple of vectors has it's second element equal to the second vector of this couple swap 
						glm::vec3 tmp=edges[i+2];
						edges[i+2]=edges[j+1];
						edges[j+1]=tmp;
						tmp=edges[i+3];
						edges[i+3]=edges[j];
						edges[j]=tmp;
						break;
					}
	
				}

			}

		}

	}
	 //delete duplicates (ex: V1 V2 V2 V4 V4 V3 V3 V1 --> V1 V2 V4 V3)
	for(int i=2; i<edges.size(); i++){ 
		edges.erase(edges.begin() + i);
	}
	//edges.erase(edges.end()-1);
	if(edges[edges.size()-1] == edges[0])
		edges.erase(edges.end()-1);
	return edges;
	
}
std::vector<int> ReadOBJFile::OrderCluster(std::vector<int> cluster){
	if(cluster.size()==0 ||cluster.size()==1 || cluster.size()==2)
		return cluster;
	for(int i=0; i<cluster.size()-2; i+=2){
		if( cluster[i+1] != cluster[i] ){
			if(cluster[i+1]== cluster[i+3]){ //if next couple of vectors has it's second element equal to the second vector of this couple
				int tmp=cluster[i+2]; //swap the vectors of the next couple
				cluster[i+2]=cluster[i+3];
				cluster[i+3]=tmp;
			}
			else{    //find a couple of vectors which contain v1
				for(int j=i+4; j<cluster.size(); j+=2){ //find another couple which contains the second vector
					if( cluster[i+1] == cluster[j] ){  //if the couple of vectors has it's first element equal to the second vector of this couple swap
						int tmp=cluster[i+2];
						cluster[i+2]=cluster[j];
						cluster[j]=tmp;
						tmp=cluster[i+3];
						cluster[i+3]=cluster[j+1];
						cluster[j+1]=tmp;
						break;
					}
					else if( cluster[i+1] == cluster[j+1] ){//if the couple of vectors has it's second element equal to the second vector of this couple swap 
						int tmp=cluster[i+2];
						cluster[i+2]=cluster[j+1];
						cluster[j+1]=tmp;
						tmp=cluster[i+3];
						cluster[i+3]=cluster[j];
						cluster[j]=tmp;
						break;
					}
	
				}

			}

		}

	}
	 //delete duplicates (ex: V1 V2 V2 V4 V4 V3 V3 V1 --> V1 V2 V4 V3)
	for(int i=2; i<cluster.size(); i++){ 
		cluster.erase(cluster.begin() + i);
	}
	//cluster.erase(cluster.end()-1);
	if(cluster[cluster.size()-1] == cluster[0])
		cluster.erase(cluster.end()-1);

	return cluster;
}
float ReadOBJFile::GetShapeIndex( int i){
		//float tmpkM=kM;
		float kM= this->m_vcalc[i].kM;
		float kG= this->m_vcalc[i].kG;
		if(kM*kM - kG <0){
			return 0;
		}
		float k1 = kM + sqrt( ::abs(kM*kM - kG)); 
		float k2 = kM - sqrt( ::abs(kM*kM - kG));
		this->m_vcalc[i].kmax=k1;
		this->m_vcalc[i].kmin=k2;
		if(k1==k2)
			return 0;
		float shape_index= 2.0 / M_PI * ::atan( (k1+k2) / (k1 - k2));
		return shape_index;
}
//--------------------------//
std::vector<GLEdge> ReadOBJFile::OrderGLEdges(std::vector<GLEdge> edges){
	//std::vector<glm::vec3> edges;  //For test
	//glm::vec3 v1=glm::vec3(1);
	//glm::vec3 v2=glm::vec3(2);
	//glm::vec3 v3=glm::vec3(3);
	//glm::vec3 v4=glm::vec3(4);
	//edges.push_back(v1);edges.push_back(v2);edges.push_back(v3);edges.push_back(v4);edges.push_back(v2);edges.push_back(v4);	edges.push_back(v1);edges.push_back(v3);
	
	
	for(int i=0; i<edges.size()-2; i+=2){
		if( edges[i+1].e != edges[i].e ){
			if(edges[i+1].e== edges[i+3].e){ //if next couple of vectors has it's second element equal to the second vector of this couple
				GLEdge tmp=edges[i+2]; //swap the vectors of the next couple
				edges[i+2]=edges[i+3];
				edges[i+3]=tmp;
			}
			else{    //find a couple of vectors which contain v1
				for(int j=i+4; j<edges.size(); j+=2){ //find another couple which contains the second vector
					if( edges[i+1].e == edges[j].e ){  //if the couple of vectors has it's first element equal to the second vector of this couple swap
						GLEdge tmp=edges[i+2];
						edges[i+2]=edges[j];
						edges[j]=tmp;
						tmp=edges[i+3];
						edges[i+3]=edges[j+1];
						edges[j+1]=tmp;
						break;
					}
					else if( edges[i+1].e == edges[j+1].e ){//if the couple of vectors has it's second element equal to the second vector of this couple swap 
						GLEdge tmp=edges[i+2];
						edges[i+2]=edges[j+1];
						edges[j+1]=tmp;
						tmp=edges[i+3];
						edges[i+3]=edges[j];
						edges[j]=tmp;
						break;
					}
	
				}

			}

		}

	}
	 //delete duplicates (ex: V1 V2 V2 V4 V4 V3 V3 V1 --> V1 V2 V4 V3)
	for(int i=2; i<edges.size(); i++){ 
		edges.erase(edges.begin() + i);
	}
	//edges.erase(edges.end()-1);
	if(edges[edges.size()-1].e == edges[0].e)
		edges.erase(edges.end()-1);
	return edges;
	
}
bool ReadOBJFile::Collinear(glm::vec3 v1,glm::vec3 v2)
{
	float a, b ,c; //if collinear, a, b and c are equal, so a/b = 1 and b/c = 1
	a=v1.x/v2.x;
	b=v1.y/v2.y;
	c=v1.z/v2.z;
	if(a/b >0.9 && a/b <1.1 && b/c >0.9 && b/c <1.1)
		return true;
	return false;

}

/////////////
void ReadOBJFile::SimilarityMeasurement(void)
{
	float flt;
	CString str;str= "["; 
	CString str2; str2="[";
	char* s= new char[10];
	//float matrix[48][2]; //matrix 
	//float matrix_t[2][48]; //transposed matrix
	//float matrix_mult[48][48]; //transposed matrix * matrix
	float **matrix=new float*[cluster_number],  **matrix_t=new float*[2],  **matrix_mult=new float*[cluster_number];
	for(int i=0; i<cluster_number; i++) {matrix[i]=new float[2]; matrix_mult[i]=new float[cluster_number];}
	for(int i=0; i<2; i++){ matrix_t[i]=new float[cluster_number];}
	
	float *mean_shape_index=new float[cluster_number];//to store the shape index of the 48 data points
	float *mean_sgf=new float[cluster_number]; //to store the SGF of the 48 data points
	
	//compute matrix and transposed matrix 
		str="[";
	for(int i=0; i<cluster_number; i++){
		float sum_si=0;
		float sum_sgf=0;
		for(int j=0; j<cluster_indices_lloyd[i].size();j++){
			sum_si+=m_vcalc[cluster_indices_lloyd[i][j]].shape_index;//j=41
			sum_sgf+=m_vcalc[cluster_indices_lloyd[i][j]].SGF;
			
				/*if(i==1){
				flt=m_vcalc[cluster_indices_lloyd[i][j]].shape_index;
				sprintf(s, "%i: %.4g, ",cluster_indices_lloyd[i][j], flt );  
				str+=s;
				}*/
				
		}
		
		if(cluster_indices_lloyd[i].size()!=0)
		{
			int k=cluster_indices_lloyd[i].size();
			mean_shape_index[i]=sum_si/cluster_indices_lloyd[i].size();
			mean_sgf[i]=sum_sgf/cluster_indices_lloyd[i].size();
		}
		else 
			mean_shape_index[i]=mean_sgf[i]=0;
		
		
		matrix_t[0][i]=mean_shape_index[i];
		matrix_t[1][i]=mean_sgf[i];
		matrix[i][0]=mean_shape_index[i];
		matrix[i][1]=mean_sgf[i];
	}
	//str+="]";
	////string for matrix
	//str="[";
	//for(int i=0; i<2; i++){ //rows
	//	if(i!=0)
	//		str+="; ";
	//	for(int j=0; j<cluster_number; j++){  //columns
	//		flt=matrix[j][i];
	//		sprintf(s, "%.4g", flt );  
	//		str+=s;
	//		if(j!=47)
	//			str+= ", ";
	//	}
	//}
	//str+="]";
	////get string for mean si and sgf
	//str= str2="["; 
	//for(int i=0; i<cluster_number; i++){ 
	//	flt=mean_shape_index[i];
	//	sprintf(s, "%.4g", flt );  
	//	str+=s;
	//	flt=mean_sgf[i];
	//	sprintf(s, "%.4g", flt );  
	//	str2+=s;
	//	if(i!=cluster_number-1){ str+=", ";	 str2+=", ";}
	//	
	//}
	//str+="]"; //mean shape index
	//str2+="]";//mean sgf

	//compute transposed matrix * matrix
	for(int i=0; i<cluster_number; i++){
		for(int j=0; j<cluster_number; j++){
		feature_matrix[i][j]=0;
			for(int k=0; k<2; k++){
				feature_matrix[i][j] +=  matrix_t[k][j] * matrix[i][k];
			}
		}
	}
	
	//get string of matrix_mult for matlab
	//str="[";
	//for(int i=0; i<cluster_number; i++){ //rows
	//	if(i!=0)
	//		str+="; ";
	//	for(int j=0; j<cluster_number; j++){ //columns
	//		flt=feature_matrix[j][i];
	//		sprintf(s, "%.4g", flt );  
	//		str+=s;
	//		if(j!=47)
	//			str+= ", ";
	//	}
	//}
	//str+="]";
	////string for matrix_t
	//str="[";
	//for(int i=0; i<cluster_number; i++){ //rows
	//	if(i!=0)
	//		str+="; ";
	//	for(int j=0; j<2; j++){  //columns
	//		flt=matrix_t[j][i];
	//		sprintf(s, "%.4g", flt );  
	//		str+=s;
	//		if(j!=1)
	//			str+= ", ";
	//	}
	//}
	//str+="]";
	
}

double ReadOBJFile::randf(double m)
{
	return m * rand() / (RAND_MAX - 1.);
}
 
point ReadOBJFile::gen_xy()
{
	
	point p, pt = new point_t[size_m_v];
	
	/* note: this is not a uniform 2-d distribution */
	int i=0;
	for   (p = pt + size_m_v; p-- > pt;) {
		p->x = m_vcalc[i].x;
		p->y =m_vcalc[i].y;
		p->z= m_vcalc[i].z;
		p->original_index=i;
		i++;
	}
 
	return pt;
}
 
inline double ReadOBJFile::dist2(point a, point b) //euclidian distance
{
	double x = a->x - b->x, y = a->y - b->y, z = a->z - b->z;
	return x*x + y*y + z*z;
}
 
inline int
ReadOBJFile::nearest(point pt, point cent, int n_cluster, double *d2)
{
	int i, min_i;
	point c;
	double d, min_d;
 
#	define for_n for (c = cent, i = 0; i < n_cluster; i++, c++)
	for_n {   //for each centroid
		min_d = HUGE_VAL;//minimal distance
		min_i = pt->group; //minimal group
		for_n {  //for each centroid
			if (min_d > (d = dist2(c, pt))) { //if the distance between another centroid and the point is less than current one 
				min_d = d; min_i = i;         //update distance and group for the point
			}
		}
	}
	if (d2) *d2 = min_d;
	return min_i;
}
 
void ReadOBJFile::kpp(point pts, int len, point cent, int n_cent)
{
#	define for_len for (j = 0, p = pts; j < len; j++, p++)
	int i, j;
	int n_cluster;
	double sum;   //contains the sum of distances between each pt and nearest centroid
	double *d = new double[len]; //contains distances between each pt and nearest centroid
 
	point p, c;
	cent[0] = pts[ rand() % len ]; //first centroid is a random data point
	for (n_cluster = 1; n_cluster < n_cent; n_cluster++) //for each cluster define original centroid
	{ 
		sum = 0;
		
		for_len {				//for each data point find nearest centroid
			nearest(p, cent, n_cluster, d + j);
			sum += d[j];        
		}
		sum = randf(sum);
		for_len {
			if ((sum -= d[j]) > 0) continue;
			cent[n_cluster] = pts[j];
			break;
		}
	}
	for_len p->group = nearest(p, cent, n_cluster, 0); //Returns nearest centroid for p
		//print cent
	CString str; str=" ";
	char* s= new char[50];
	for_n{
		sprintf(s, "%.4g , %.4g, %.4g, %d , %d \n ", c->x, c->y, c->z, c->group, c->original_index );  
		str+=s;
	}
	free(d);
}
 
point ReadOBJFile::lloyd(point pts, int len, int n_cluster)
{
	int i, j, min_i;
	int changed;
 	CString str; str=" ";
	char* s= new char[50];
	point cent =new point_t[n_cluster], p , c;
 
	/* assign init grouping randomly */
	//for_len p->group = j % n_cluster;  //does not work well ->>a lot of clusters are empty!

	/* or call k++ init */
	kpp(pts, len, cent, n_cluster);
 
	do {
		/* group element for centroids are used as counters */
		for_n { c->group = 0; c->x = c->y  = c->z= 0; } 
		for_len {										 //for each data point p 
			c = cent + p->group;							//get adress of the centroid corresponding to the group for this data point
			c->group++;									//modify size of the group
			c->x += p->x; c->y += p->y; c->z += p->z;  //centroid=sum of x, y and z of the pts p in this group
			c->original_index= p->original_index;	//rajout, ça n'a aucun sens mais bon (affectation de l'index du dernier data point du groupe
		}
		//
		str=" ";
		int i=0;
		for_len{
			sprintf(s, "%d \t", p->group );  
			str+=s;
			i++;
			if(i%10==0)
				str+="\n";
		}
		str=" ";
		for_n{
			if(c->group==0)
				int p=1;
			sprintf(s, "%.4g , %.4g, %.4g, %d , %d \n ", c->x, c->y, c->z, c->group, c->original_index );  
			str+=s;
		}
		//
		for_n { if(c->group!=0){ c->x /= (float)c->group; c->y /=(float) c->group; c->z /= (float)c->group; } }//get mean coordinates (=centroid) of each group 

		changed = 0;
		/* find closest centroid of each point */
		for_len {
			min_i = nearest(p, cent, n_cluster, 0);
			if (min_i != p->group) {
				changed++;
				p->group = min_i;
			}
		}
	} while (changed > (len >> 10)); /* stop when 99.9% of points are good */
 
	for_n { c->group = i; }
 	//print cent
	str=" ";
	for_n{
		sprintf(s, "%.4g , %.4g, %.4g, %d , %d \n ", c->x, c->y, c->z, c->group, c->original_index );  
		str+=s;
	}
	return cent;
}
 
void ReadOBJFile::AllocRGB(void){
	if(init)
	{
		/*delete [] rgb[0];/ //heap corruption error :(
		delete [] rgb;*/ 
		delete [] feature_matrix[0];
		delete [] feature_matrix;
	}
	rgb=new GLfloat *[cluster_number];
	rgb[0] = new GLfloat[3 * cluster_number];
	for (int i = 1; i < cluster_number; ++i)
		rgb[i] = rgb[0]+i*3;


	feature_matrix= new float *[cluster_number];
	for(int i=0; i <cluster_number; i++) feature_matrix[i]=new float[cluster_number];
}