#include "StdAfx.h"
#include "ReadOBJFile.h"
#include <string>

ReadOBJFile::ReadOBJFile(void)
{
	memset(this->m_FileName,0,256);
	m_nCount=0;
	lastj=0;
	length=0;
}


ReadOBJFile::~ReadOBJFile(void)
{
}

void ReadOBJFile::GetVertexInfo()
{
	FILE * fp;
	fp=fopen(this->m_FileName,"rb");
	char str[256];
	int nVertex=1;
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
				char *VerteData;
				int nCount=n2-n1;
				VerteData=new char[nCount+1];
				memset(VerteData,0,nCount+1);
				strncpy(VerteData,str+n1,nCount);
				float ff;
				sscanf(VerteData,"%f",&ff);
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
				delete VerteData;
			}

			nVertex++;
		}
	}
}
void ReadOBJFile::GetTextureInfo()
{
	FILE *fp;
	fp=fopen(this->m_FileName,"rb");
	char str[256];
	int nTexture=1;
	while(1)
	{
		memset(str,0,256);
		if(!this->ReadLine(fp,str))
			break;
		if(str[0]=='v'&&str[1]=='t'&&str[2]==' ')
		{
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
	}
}
void ReadOBJFile::GetNormalInfo()
{
	FILE *fp;
	fp=fopen(this->m_FileName,"rb");
	char str[256];
	int nNormal=1;
	while(1)
	{
		memset(str,0,256);
		if(!this->ReadLine(fp,str))
			break;
		if(str[0]=='v'&&str[1]=='n'&&str[2]==' ')
		{
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
		if (!this->ReadLine(fp,str))
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
	this->GetVertexInfo();
	this->GetTextureInfo();
	this->GetNormalInfo();
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
	FILE * fp;
	fp=fopen(this->m_FileName,"rb");
	if(fp==NULL) return; 
	char str[256];
	while(1)
	{
		memset(str,0,256);
		if (!this->ReadLine(fp,str))
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
						::glNormal3f(this->m_vn[d2-1].x,this->m_vn[d2-1].y,this->m_vn[d2-1].z);
						::glVertex3f(this->m_v[d1-1].x,this->m_v[d1-1].y,this->m_v[d1-1].z);
						delete Data;
						break;
					}
					else if(VertexData[j]=='/')
						{
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
							::glNormal3f(this->m_vn[d3-1].x,this->m_vn[d3-1].y,this->m_vn[d3-1].z);
							::glTexCoord3f(this->m_vt[d2-1].x,this->m_vt[d2-1].y,this->m_vt[d2-1].z);
							::glVertex3f(this->m_v[d1-1].x,this->m_v[d1-1].y,this->m_v[d1-1].z);
							delete Data;
							break;

						}
					else
					{
						std::string str(VertexData);
						if(str.find('/')==-1 && (j==lastj+length || j==0 )){ //no '/' found //
							length=sizeof(VertexData);
							Data=new char[length+1];
							memset(Data,0,length+1);
							strncpy(Data,VertexData,length);
							sscanf(Data,"%d",&d1);
							delete Data;
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
}