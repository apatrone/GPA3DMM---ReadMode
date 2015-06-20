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
	while (1)
	{
		memset(str,0,256);
		if(!this->ReadLine(fp,str))
			break;
		if (str[0]=='v'&&str[1]==' ')
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
			int flag=1;
			int start;
			for (int j=0;str[j]!='\0';j++)
			{
				if(str[j]==' '&&str[j+1]!=' ')
				{
					start=j+1;
					break;
				}
			}
			int n1=start,n2;
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
				char *VertexData;
				int nCount=n2-n1;
				VertexData=new char[nCount+1];
				memset(VertexData,0,nCount+1);
				strncpy(VertexData,str+n1,nCount);
				float ff;
				sscanf(VertexData,"%f",&ff);
				switch(flag)
				{
				case 1:
					m_v[nVertex-1].x=ff;
					break;
				case 2:
					m_v[nVertex-1].y=ff;
					break;
				case 3:
					m_v[nVertex-1].z=ff;
					break;
				}
				flag++;
				n1=n2+1;
				delete VertexData;
			}
			size_m_v++;
			nVertex++;
		}
		else if(str[0]=='v'&&str[1]=='t'&&str[2]==' ')
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
			int flag=1;
			int start;
			for(int x=0;str[x]!='\0';x++)
			{
				if(str[x]==' '&&str[x+1]!=' ')
				{
					start=x+1;
					break;

				}
			}
			int n1=start,n2;
			while(flag!=nEdge+1)
			{

				for(int i=n1;;i++)
				{
					if(str[i]==' '||str[i]=='\0')
					{
						n2=i;
						break;
					}
				}
				char *VertexData;
				int nCount=n2-n1;
				VertexData=new char[nCount+1];
				memset(VertexData,0,nCount+1);
				strncpy(VertexData,str+n1,nCount);
				float ff;
				sscanf(VertexData,"%f",&ff);
				switch(flag)
				{
				case 1:
					m_vt[nTexture-1].x=ff;
					break;
				case 2:
					m_vt[nTexture-1].y=ff;
					break;
				case 3:
					m_vt[nTexture-1].z=ff;
					break;

				}
				flag++;
				n1=n2+1;
				delete VertexData;
			}
			nTexture++;
		}
		else if(str[0]=='v'&&str[1]=='n'&&str[2]==' ')
		{
			vn=true;
			if(nNormal==1)
				this->m_vn=(Normal*)malloc(sizeof(Normal));
			else
				this->m_vn=(Normal*)realloc(this->m_vn,(nNormal-1)*sizeof(Normal)+sizeof(Normal));
			m_vn[nNormal-1].nNormalIndex=nNormal;
			int flag=1;
			int start;
			for(int x=0;str[x]!='\0';x++)
			{
				if(str[x]==' '&&str[x+1]!=' ')
				{
					start=x+1;
					break;
				}
			}
			int n1=start,n2;
			while(flag!=4)
			{
				;
				for(int i=n1;;i++)
				{
					if(str[i]==' '||str[i]=='\0')
					{
						n2=i;
						break;
					}
				}
				char *VertexData;
				int nCount=n2-n1;
				VertexData=new char[nCount+1];
				memset(VertexData,0,nCount+1);
				strncpy(VertexData,str+n1,nCount);
				float ff;
				sscanf(VertexData,"%f",&ff);
				switch(flag)
				{
				case 1:
					m_vn[nNormal-1].x=ff;
					break;
				case 2:
					m_vn[nNormal-1].y=ff;
					break;
				case 3:
					m_vn[nNormal-1].z=ff;
					break;

				}
				flag++;
				n1=n2+1;
				delete VertexData;
			}
			nNormal++;
		}

	}
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
		if (this->ReadLine(fp,str)==false)
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
		if(this->ReadLine(fp,str))
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
}
bool ReadOBJFile::ReadLine(FILE *fp,char *str)
{
	char c1,c2;
	c1=fgetc(fp);
	c2=fgetc(fp);
	int i=0;
	while (c1!=13&&c2!=10)
	{
		if(feof(fp))
		{
			fclose(fp);
			return false;
		}
		str[i]=c1;
		c1=c2;
		i++;
		c2=fgetc(fp);
	}
	return true;
}
void ReadOBJFile::Draw()
{

	if(init==false)
	{
		FILE * fp;
		fp=fopen(this->m_FileName,"rb");
		if(fp==NULL) return; 
		char str[256];
		while(1)
		{
			memset(str,0,256);
			if (this->ReadLine(fp,str)==false)
				break;
			if (strncmp("usemtl",str,6)==0)
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
			if (str[0]=='f'&&str[1]==' ')
			{
				int nEdge=0;
				for (int j=0;str[j]!='\0';j++)
				{
					if (str[j]==' '&&str[j+1]!=' '&&str[j+1]!='\0')
						nEdge++;
				}
				if(nEdge==3)
					::glBegin(GL_TRIANGLES);
				if (nEdge==4)
					::glBegin(GL_QUADS);
				int start;
				for (int j=0;str[j]!='\0';j++)
				{
					if(str[j]==' '&&str[j+1]!=' ')
					{
						start=j+1;
						break;
					}
				}
				int n1=start,n2;
				int flag=1;
				while (flag!=nEdge+1)
				{
					for (int i=n1;;i++)
					{
						if(str[i]==' '||str[i]=='\0')
						{
							n2=i;
							break;
						}
					}
					char *VertexData;
					int nCount=n2-n1;
					VertexData=new char[nCount+1];
					memset(VertexData,0,nCount+1);
					strncpy(VertexData,str+n1,nCount);
					int n3=0,n4;
					for(int j=0;j<nCount;j++)
					{
						char *Data;
						int d1,d2,d3;
						if(VertexData[j]=='/'&&VertexData[j+1]=='/')
						{
							vn=true;
							n4=j;
							int nLength=n4-n3;
							Data=new char[nLength+1];
							memset(Data,0,nLength+1);
							strncpy(Data,VertexData,nLength);
							sscanf(Data,"%d",&d1);
							delete Data;
							n3=n4+2;
							n4=nCount;
							nLength=n4-n3;
							Data=new char[nLength+1];
							memset(Data,0,nLength+1);
							strncpy(Data,VertexData+n3,nLength);
							sscanf(Data,"%d",&d2);
							vertexIndices.push_back(d1-1);
							normalIndices.push_back(d2-1);
							::glNormal3f(this->m_vn[d2-1].x,this->m_vn[d2-1].y,this->m_vn[d2-1].z);
							::glVertex3f(this->m_v[d1-1].x,this->m_v[d1-1].y,this->m_v[d1-1].z);
							delete Data;
							break;
						}
						else if(VertexData[j]=='/')
							{
								vt=true; vn=true;
								n4=j;
								int nLength=n4-n3;
								Data=new char[nLength+1];
								memset(Data,0,nLength+1);
								strncpy(Data,VertexData,nLength);
								sscanf(Data,"%d",&d1);
								delete Data;
								n3=n4+1;
								for(int k=n3;k<nCount;k++)
								{
									if(VertexData[k]=='/')
									{
										n4=k;
										break;
									}
								}
								nLength=n4-n3;
								Data=new char[nLength+1];
								memset(Data,0,nLength+1);
								strncpy(Data,VertexData+n3,nLength);
								sscanf(Data,"%d",&d2);
								//::glTexCoord2f(this->m_vt[dd-1].x,this->m_vt[dd-1].y);
								delete Data;
								n3=n4+1;
								n4=nCount;
								nLength=n4-n3;
								Data=new char[nLength+1];
								memset(Data,0,nLength+1);
								strncpy(Data,VertexData+n3,nLength);
								sscanf(Data,"%d",&d3);
								vertexIndices.push_back(d1-1);
								uvIndices.push_back(d2-1);
								normalIndices.push_back(d3-1);
								::glNormal3f(this->m_vn[d3-1].x,this->m_vn[d3-1].y,this->m_vn[d3-1].z);
								::glTexCoord3f(this->m_vt[d2-1].x,this->m_vt[d2-1].y,this->m_vt[d2-1].z);
								::glVertex3f(this->m_v[d1-1].x,this->m_v[d1-1].y,this->m_v[d1-1].z);
								delete Data;
								break;

							}
						else
						{
							std::string str(VertexData);
							if(str.find('/')==-1 && (j==lastj+length || j==0 ))
							{ //no '/' found //
								length=sizeof(VertexData);
								Data=new char[length+1];
								memset(Data,0,length+1);
								strncpy(Data,VertexData,length);
								sscanf(Data,"%d",&d1);
								delete Data;
								vertexIndices.push_back(d1-1);
								::glVertex3f(this->m_v[d1-1].x,this->m_v[d1-1].y,this->m_v[d1-1].z);
							}
						}
					}
					flag++;
					n1=n2+1;
					delete VertexData;
				}
				::glEnd();
			}

		}
		init=true;
		EstimateNormals();
		EstimatekGkM();
		res=(vt==true)+2*(vn==true); 
	}
	else 
	{


		int m=0, n=0, o=0;
		float comp;
		
		::glBegin(GL_TRIANGLES);
		for(int i=0; i<vertexIndices.size(); i++)
		{	
			if(use_curvature==SHAPEINDEX){
				comp=m_vcalc[vertexIndices[i]].shape_index *100;
				if(comp> 50)
					::glColor3f(0.5f,1.0f,0.0f);
				else if(comp < -50)
					::glColor3f(0.5f,0.0f,0.5f);
				else
					::glColor3f(0.0f,1.0f,1.0f);
			}
			else if(use_curvature!=NONE){
				if(use_curvature==GAUSS)
					comp=m_vcalc[vertexIndices[i]].kG * 100 ;
				else if(use_curvature==MEAN)
					comp=m_vcalc[vertexIndices[i]].kM *100;
				
				
				if(comp>gauss_sup)  
				{::glColor3f(0.5f,1.0f,0.0f); m++;} //green
				else if(comp<gauss_inf) 
				{::glColor3f(0.5f,0.0f,0.5f); n++;}  //mauve
				else{
					::glColor3f(0.0f,1.0f,1.0f); //turquoise
					o++;
				}

			}
			else 
				::glColor3f(0.0f,1.0f,1.0f);

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
	

}	



int ReadOBJFile::EstimateNormals(void)
{

	
normal_buffer = new std::vector<glm::vec3>[vertexIndices.size()]();

for( int i = 0; i < vertexIndices.size(); i += 3 )
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

 m_vcalc=(GLVertex *)malloc(sizeof(GLVertex)*vertexIndices.size());
 // Now loop through each vertex vector, and average out all the normals stored.
 for( int i = 0; i < size_m_v; ++i )   //900 tjr pas de réécriture  ... mais réécriture sur m_vn avant 925, mais pas sur uvindices
 {
	m_vcalc[i].x=m_v[i].x; m_vcalc[i].y=m_v[i].y; m_vcalc[i].z=m_v[i].z;
	m_vcalc[i].normal=glm::vec3(0,0,0);
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

	//for(int i=0; i<vertexIndices.size();i+=3)
	//{
	//	// get the three vertices that make the faces
	//	glm::vec3 p1 = glm::vec3(m_v[vertexIndices[i+0]].x, m_v[vertexIndices[i+0]].y, m_v[vertexIndices[i+0]].z);
	//	glm::vec3 p2 = glm::vec3(m_v[vertexIndices[i+1]].x, m_v[vertexIndices[i+1]].y, m_v[vertexIndices[i+1]].z);
	//	glm::vec3 p3 =  glm::vec3(m_v[vertexIndices[i+2]].x, m_v[vertexIndices[i+2]].y, m_v[vertexIndices[i+2]].z);
	 //
	//	glm::vec3 v1 = p2 - p1;
	//	glm::vec3 v2 = p3 - p1; 
	//	//cross
	//	glm::vec3 normal = glm::vec3((v1.y * v2.z) - (v1.z * v2.y),
	//						(v1.z * v2.x) - (v1.x* v2.z),
	//	
	//						(v1.x* v2.y) - (v1.y * v2.x));
	//	for(int k=0; k<3; k++){ //for each vertex in face
	//		for(int j=0; i<vertexIndices.size(); j+=3){
	//			glm::vec3 pb1 = glm::vec3(m_v[vertexIndices[j+0]].x, m_v[vertexIndices[j+0]].y, m_v[vertexIndices[j+0]].z);
	//			glm::vec3 pb2 = glm::vec3(m_v[vertexIndices[j+1]].x, m_v[vertexIndices[j+1]].y, m_v[vertexIndices[j+1]].z);
	//			glm::vec3 pb3 =  glm::vec3(m_v[vertexIndices[j+2]].x, m_v[vertexIndices[j+2]].y, m_v[vertexIndices[j+2]].z);
	//	
	//			if(pb1!=p1 || pb2!=p2 || pb3!=p3){ //if not the same face
	//				glm::vec3 vb1 = pb2 - pb1;
	//				glm::vec3 vb2 = pb3 - pb1; 
	//				//cross
	//				glm::vec3 normalb = glm::vec3((vb1.y * vb2.z) - (vb1.z * vb2.y),
	//									(vb1.z * vb2.x) - (vb1.x* vb2.z),
	//	
	//									(vb1.x* vb2.y) - (vb1.y * vb2.x));
	//				normal+= normalb ;
	//			}

	//		}
	//	}
	//	 float length = normal.x * normal.x + normal.y * normal.y + normal.z * normal.z;
	//	 length = sqrt(length);
	//	 normal.x /= length; normal.y /= length; normal.z /= length;

	//	 m_vcalc[i].normal=normal;
	//}

 return true;
}

void ReadOBJFile::EstimatekGkM(void)
{ 
	
    //std::sort(s.begin(), s.end(), customLess);
	for(int i=0; i<size_m_v; i++)
	{
		int p=0; //for test
		int r=0;//For test
		float sum_angles=0; //for kG
		float sum_area=0;//for kG and kM
		float sum_dihedral_angles=0; //for kM
		//std::list<OrderedEdges> ordered_edges ;
		std::vector<glm::vec3> ordered_edges;
		for(int j=0; j<vertexIndices.size(); j+=3)
		{	
			//if current vertex belongs to the current triangle/face
			//if(vertexIndices[j]==i || etc. is the same?
			if(VertexEqual(m_vcalc[vertexIndices[j]],m_vcalc[i]) ||  VertexEqual(m_vcalc[vertexIndices[j+1]],m_vcalc[i]) || VertexEqual(m_vcalc[vertexIndices[j+2]],m_vcalc[i]))
			{
				r++;
				glm::vec3 p1 = glm::vec3(m_vcalc[i].x, m_vcalc[i].y, m_vcalc[i].z);
				glm::vec3 p2; 
				glm::vec3 p3;
				float angle;
				float area;

				if( VertexEqual(m_vcalc[vertexIndices[j]],m_vcalc[i]))
				{
					p2 = glm::vec3(m_vcalc[vertexIndices[j+1]].x, m_vcalc[vertexIndices[j+1]].y, m_vcalc[vertexIndices[j+1]].z);
					p3 =  glm::vec3(m_vcalc[vertexIndices[j+2]].x, m_vcalc[vertexIndices[j+2]].y, m_vcalc[vertexIndices[j+2]].z);
				}
				else if(VertexEqual(m_vcalc[vertexIndices[j+1]],m_vcalc[i]))
				{
					p2 = glm::vec3(m_vcalc[vertexIndices[j+0]].x, m_vcalc[vertexIndices[j+0]].y, m_vcalc[vertexIndices[j+0]].z);
					p3 =  glm::vec3(m_vcalc[vertexIndices[j+2]].x, m_vcalc[vertexIndices[j+2]].y, m_vcalc[vertexIndices[j+2]].z);
				}
				else if(VertexEqual(m_vcalc[vertexIndices[j+2]],m_vcalc[i]))
				{
					p2 = glm::vec3(m_vcalc[vertexIndices[j+0]].x, m_vcalc[vertexIndices[j+0]].y, m_vcalc[vertexIndices[j+0]].z);
					p3 = glm::vec3(m_vcalc[vertexIndices[j+1]].x, m_vcalc[vertexIndices[j+1]].y, m_vcalc[vertexIndices[j+1]].z);

				}
				
				glm::vec3 v1 = p2 - p1;
				glm::vec3 v2 = p3 - p1; 
	/*			//angle for kG
				angle=GetAngle(v1,v2);
				sum_angles+=angle;
				//area for kG and kM
				area=GetArea(v1, v2);
				sum_area+=area;*/
				//store both edges for kM
				ordered_edges.push_back(v1);
				ordered_edges.push_back(v2);
			}

		}

		//calc kM
		glm::vec3 previous_edge;
		glm::vec3 edge;
		glm::vec3 next_edge;
		glm::vec3 cross_edge;
		glm::vec3 cross_next_edge;
		ordered_edges=OrderEdges(ordered_edges);

		for(int k=1; k<ordered_edges.size()-1; k++){ 

			previous_edge=ordered_edges[k-1];
			edge=ordered_edges[k];
			next_edge=ordered_edges[k+1];
			//angle for kG
			sum_angles+=GetAngle(previous_edge,edge);
			//area for kG and kM
			sum_area+=GetArea(previous_edge,edge);
			//get normals and calculate dihedral angle (angle between normals) * length of edge
			cross_edge=glm::cross(previous_edge, edge);
			cross_next_edge=glm::cross(edge, next_edge);
			sum_dihedral_angles+=GetAngle(cross_edge/glm::length(cross_edge),cross_next_edge/glm::length(cross_next_edge)) * glm::length(edge);
		
		}
		//angle for kG
		sum_angles+=GetAngle(edge,next_edge) + GetAngle(next_edge,ordered_edges[0]) ;
		//area for kG and kM
		sum_area+=GetArea(edge,next_edge)+ GetArea(next_edge,ordered_edges[0]);

		cross_edge=glm::cross(edge, next_edge);
		cross_next_edge=glm::cross(next_edge, ordered_edges[0]);
		sum_dihedral_angles+=GetAngle(cross_edge/glm::length(cross_edge),cross_next_edge/glm::length(cross_next_edge)) * glm::length(next_edge);
		cross_edge=glm::cross(next_edge,  ordered_edges[0]);
		cross_next_edge=glm::cross(ordered_edges[0], ordered_edges[1]);
		sum_dihedral_angles+=GetAngle(cross_edge/glm::length(cross_edge),cross_next_edge/glm::length(cross_next_edge))* glm::length( ordered_edges[0]);

		
		//calc kG
		m_vcalc[i].kG= (2* M_PI - sum_angles)/(sum_area) ;  
		//calc kM
		m_vcalc[i].kM=   sum_dihedral_angles/(4 * sum_area ) ;
		m_vcalc[i].shape_index = GetShapeIndex(m_vcalc[i].kG,m_vcalc[i].kM);
	}

}

bool ReadOBJFile::VertexEqual(GLVertex v1, GLVertex v2)
{
	if(v1.x==v2.x && v1.y==v2.y &&v1.z==v2.z )
		return true;
	return false;
}
float ReadOBJFile::GetAngle(glm::vec3 v1, glm::vec3 v2){
	//glm::vec3 v1= glm::vec3(4,4,2); //for test 
	//glm::vec3 v2= glm::vec3(2,3,5); //for test
	float prod_sum = v1.x * v2.x + v1.y * v2.y +  v1.z * v2.z;   //dot product...
	double abs_v1= sqrt( v1.x * v1.x + v1.y * v1.y +  v1.z * v1.z);  //glm::length
	double abs_v2=sqrt( v2.x * v2.x + v2.y * v2.y +  v2.z * v2.z); //glm::length

	double cos=(abs_v1*abs_v2)>0? ::cos( prod_sum / (abs_v1*abs_v2)): 0; //if division by zero cos= 0
	 float c=::acos(cos);
	if(c>=0 && c <= ( M_PI))
		return c;
	else
		return GetAngle(v2, v1);


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


float ReadOBJFile::GetShapeIndex(float kG, float kM){
		float k1 = kM + sqrt( kM*kM - kG); 
		float k2 = kM - sqrt( kM*kM - kG);
		float shape_index= -2 / M_PI * ::atan( (k1+k2) / (k1 - k2));
		return shape_index;

}