#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XModelObj.h"
#include "X3DWorld.h"
#include "XFile.h"
#include "XString.h"
namespace XE{
void removeRedundantSpace(char *p)	//删除连续的或者行尾的空格符
{
	int len = strlen(p);
	for(int i = 0;i < len;++ i)
	{
		if(i < len - 2 && p[i] == '/' && p[i + 1] == ' ')
		{//紧邻'/'的空格也要丢弃
			strcpy(p + i + 1,p + i + 2);
			-- i;
		}else
		if(p[i] == ' ' && p[i + 1] == '/')
		{//紧邻'/'的空格也要丢弃
			strcpy(p + i,p + i + 1);
			-- i;
		}else
		if(i == 0 && p[i] == ' ' && p[i + 1] != ' ')
		{//头的空格也丢弃
			strcpy(p + i,p + i + 1);
			-- i;
		}else
		if(i < len - 1 && p[i] == ' ' && p[i + 1] == ' ')
		{//连续的空格
			strcpy(p + i,p + i + 1);
			-- i;
		}else 
		if(i < len - 1 && p[i] == '\\' && p[i + 1] == '\n')
		{//换行直接转换成空格
			p[i + 1] = ' ';
			strcpy(p + i,p + i + 1);
			-- i;
		}else 
		if(i < len - 1 && p[i] == ' ' && p[i + 1] == '\0')
		{//结束的空格丢弃
			p[i] = '\0';
			break;
		}else 
		if(i < len - 1 && p[i] == ' ' && p[i + 1] == '\n')
		{//结束的空格丢弃
			strcpy(p + i,p + i + 1);
			-- i;
		}
		if(p[i] == '\0') break;
	}
}
XBool XModelObj::save(const char *filename)
{
	if(!m_isInited ||
		filename == NULL) return XFalse;
	FILE *fp = NULL;
	if((fp = fopen(filename,"w")) == NULL) return XFalse;	//文件打开失败
	fprintf(fp,"# xiajia_1981@163.com\n");
	//下面写入具体的模型内容
	for(unsigned int i = 0;i < m_materialFilename.size();++ i)
	{
		fprintf(fp,"mtllib %s\n",m_materialFilename[i].c_str());
	}
	if(!saveMaterial()) 
	{
		fclose(fp);
		return XFalse;
	}
	for(unsigned int i = 0;i < m_point.size();++ i)
	{//下面写入顶点数据
		fprintf(fp,"v %f %f %f\n",m_point[i].x,m_point[i].y,m_point[i].z);
	}
	for(unsigned int i = 0;i < m_texturePoint.size();++ i)
	{//下面写入贴图数据
		fprintf(fp,"vt %f %f\n",m_texturePoint[i].x,1.0f - m_texturePoint[i].y);
	}
	for(unsigned int i = 0;i < m_normal.size();++ i)
	{//下面写入法线数据
		fprintf(fp,"vn %f %f %f\n",m_normal[i].x,m_normal[i].y,m_normal[i].z);
	}
	for(unsigned int i = 0;i < m_group.size();++ i)
	{//下面写入组数据
		if(m_group[i] == NULL) continue;
		//写入组名
		if(m_group[i]->groupName.length() > 0)
			fprintf(fp,"g %s\n",m_group[i]->groupName.c_str());
		//写入材质名称
		if(m_group[i]->materialName.length() > 0)
			fprintf(fp,"usemtl %s\n",m_group[i]->materialName.c_str());

		if(m_group[i]->withTexInfo != 1 && m_group[i]->withNormalInfo != 1)
		{
			for(unsigned int j = 0;j < m_group[i]->face.size();++ j)
			{
				fprintf(fp,"f %d %d %d\n",m_group[i]->face[j].vIndex.x,m_group[i]->face[j].vIndex.y,m_group[i]->face[j].vIndex.z);
			}
		}else
		if(m_group[i]->withTexInfo == 1 && m_group[i]->withNormalInfo != 1)
		{
			for(unsigned int j = 0;j < m_group[i]->face.size();++ j)
			{
				fprintf(fp,"f %d/%d %d/%d %d/%d\n",m_group[i]->face[j].vIndex.x,m_group[i]->face[j].tIndex.x,
					m_group[i]->face[j].vIndex.y,m_group[i]->face[j].tIndex.y,
					m_group[i]->face[j].vIndex.z,m_group[i]->face[j].tIndex.z);
			}
		}else
		if(m_group[i]->withTexInfo != 1 && m_group[i]->withNormalInfo == 1)
		{
			for(unsigned int j = 0;j < m_group[i]->face.size();++ j)
			{
				fprintf(fp,"f %d//%d %d//%d %d//%d\n",m_group[i]->face[j].vIndex.x,m_group[i]->face[j].nIndex.x,
					m_group[i]->face[j].vIndex.y,m_group[i]->face[j].nIndex.y,
					m_group[i]->face[j].vIndex.z,m_group[i]->face[j].nIndex.z);
			}
		}else
		if(m_group[i]->withTexInfo == 1 && m_group[i]->withNormalInfo == 1)
		{
			for(unsigned int j = 0;j < m_group[i]->face.size();++ j)
			{
				fprintf(fp,"f %d/%d/%d %d/%d/%d %d/%d/%d\n",m_group[i]->face[j].vIndex.x,m_group[i]->face[j].tIndex.x,m_group[i]->face[j].nIndex.x,
					m_group[i]->face[j].vIndex.y,m_group[i]->face[j].tIndex.y,m_group[i]->face[j].nIndex.y,
					m_group[i]->face[j].vIndex.z,m_group[i]->face[j].tIndex.z,m_group[i]->face[j].nIndex.z);
			}
		}
	}
	fclose(fp);
	return XTrue;
}
std::string getObjInfo0FromStr(const char * p)
{
	if(p == NULL) return XString::gNullStr;
	char tempLineData[1024];	//注意这里的1024的最大限制
	std::string tmp;
	int len = strlen(p);
	if(XString::isUTF8(p,len)) tmp = XString::UTF82ANSI(p);
	else tmp = p;
	int s,e,b;
	if(!XString::getFirtCanShowString(tmp.c_str(),s,e))
	{
		printf("错误的数据:%s\n",tmp.c_str());
	}else
	{
		b = XString::getCharPosition(tmp.c_str() + s,' ');
		memcpy(tempLineData,tmp.c_str() + s + b + 1,e - s - b);
		tempLineData[e - s - b] = '\0';
	}
	return tempLineData;
}
XBool XModelObj::load(const char *filename)	//从文件中载入数据
{
	if(m_isInited ||
		filename == NULL) return XFalse;
	FILE *fp = NULL;
	if((fp = fopen(filename,"r")) == NULL) return XFalse;	//文件打开失败
	char lineData[512];
	char tempLineData[512];
	//下面开始解析文件
	XVec3 tempData;
	XVec2 tempData2;
	XFaceInfo tempFaceInfo;
	XGroupInfo *tempGroupInfo = NULL;
	int len = 0;
	while(true)
	{
		if(fgets(lineData,512,fp) == NULL) break;
		while(true)
		{
			len = strlen(lineData);
			if(len > 2 && lineData[len - 2] == '\\' && lineData[len - 1] == '\n')
			{//判断如果行尾是换行符号，则继续读一行
				if(fgets(tempLineData,512,fp) == NULL) break;
				strcat_s(lineData,512,tempLineData);
			}else
				break;
		}
		if(lineData[0] == '#') continue;	//注释
		if(lineData[0] == 'v' && lineData[1] == ' ')
		{//顶点数据
			if(sscanf(lineData,"v %f %f %f",&tempData.x,&tempData.y,&tempData.z) == 3)
			{
				//tempData.y = -tempData.y;
				m_point.push_back(tempData);
			}
		}else
		if(lineData[0] == 'v' && lineData[1] == 't')
		{//贴图数据
			if(sscanf(lineData,"vt %f %f %f",&tempData.x,&tempData.y,&tempData.z) == 3)
			{
				if(tempData.x >= 0.0f) tempData2.x = tempData.x;
				else tempData2.x = 1.0f - tempData.x;
				if(tempData.y >= 0.0f) tempData2.y = 1.0f - tempData.y;
				else tempData2.y = -tempData.y;
				m_texturePoint.push_back(tempData2);
			}else
			if(sscanf(lineData,"vt %f %f",&tempData2.x,&tempData2.y) == 2)
			{
				if(tempData2.x >= 0.0f) tempData2.x = tempData2.x;
				else tempData2.x = 1.0f - tempData2.x;
				if(tempData2.y >= 0.0f) tempData2.y = 1.0f - tempData2.y;
				else tempData2.y = -tempData2.y;
				m_texturePoint.push_back(tempData2);
			}else
			{//格式不支持
				fclose(fp);
				return XFalse;
			}
		}else
		if(lineData[0] == 'v' && lineData[1] == 'n')
		{//法线数据
			if(sscanf(lineData,"vn %f %f %f",&tempData.x,&tempData.y,&tempData.z) == 3)
			{
			//	tempData.x = -tempData.x;
			//	tempData.y = -tempData.y;
			//	tempData.z = -tempData.z;
				m_normal.push_back(tempData);
			}
		}else
		if(lineData[0] == 'f' && lineData[1] == ' ')
		{//面数据
			if(tempGroupInfo == NULL)
			{
				tempGroupInfo = XMem::createMem<XGroupInfo>();
				if(tempGroupInfo == NULL)
				{
					fclose(fp);
					return XFalse;
				}
				m_group.push_back(tempGroupInfo);
			}
			if(!readFaceInfo(tempGroupInfo,lineData))
			{
				fclose(fp);
				return XFalse;
			}
		}else
		if(lineData[0] == 's' && lineData[1] == ' ')
		{
		}else
		if(lineData[0] == 'g' && lineData[1] == ' ')
		{//组
			//读取组名
			std::string tmp = getObjInfo0FromStr(lineData);
			tempGroupInfo = getGroup(tmp.c_str());
			if(tempGroupInfo == NULL)
			{
				tempGroupInfo = XMem::createMem<XGroupInfo>();
				if(tempGroupInfo == NULL)
				{
					fclose(fp);
					return XFalse;
				}
				m_group.push_back(tempGroupInfo);
				tempGroupInfo->groupName = tmp;
			}
		}else
		{
			len = XString::getCharPosition(lineData,' ');
			if(len > 0)
			{
				memcpy(tempLineData,lineData,len);
				tempLineData[len] = '\0';
				if(strcmp(tempLineData,"usemtl") == 0)
				{//usemtl
					if(tempGroupInfo == NULL)
					{
						tempGroupInfo = XMem::createMem<XGroupInfo>();
						if(tempGroupInfo == NULL)
						{
							fclose(fp);
							return XFalse;
						}
						m_group.push_back(tempGroupInfo);
					}
					//读取材质名
					tempGroupInfo->materialName = getObjInfo0FromStr(lineData);
				}else
				if(strcmp(tempLineData,"mtllib") == 0)
				{//mtllib材质文件
					std::string tmp = getObjInfo0FromStr(lineData);
					if(tmp == "")
					{
						printf("错误的数据:%s\n",lineData);
						continue;
					}
					m_materialFilename.push_back(tmp);
					tmp = XFile::mergeFilename(filename,tmp);
					if(loadMaterial(tmp.c_str()))
					{
						m_materialIndex.push_back(m_material.size());
					}else
					{
						printf("材质文件丢失:%s\n",tmp.c_str());
					}
				}
			}
		}
		if(feof(fp)) break;	//文件读取完成之后直接退出
	}

	fclose(fp);
	if(!checkData()) return XFalse;
	//下面计算物体的碰撞盒子(尚未完成)
	bool first = true;
	float xMin = 0.0f,xMax = 0.0f,yMin = 0.0f,yMax = 0.0f,zMin = 0.0f,zMax = 0.0f;
	XVec3 tmpV;
	for(unsigned int i = 0;i < m_group.size();++ i)
	{
		for(unsigned int j = 0;j < m_group[i]->face.size();++ j)
		{
			tmpV = m_point[m_group[i]->face[j].vIndex.x - 1];
			if(first)
			{
				xMin = tmpV.x;xMax = tmpV.x;
				yMin = tmpV.y;yMax = tmpV.y;
				zMin = tmpV.z;zMax = tmpV.z;
				first = false;
			}else
			{
				if(tmpV.x < xMin) xMin = tmpV.x;
				if(tmpV.x > xMax) xMax = tmpV.x;
				if(tmpV.y < yMin) yMin = tmpV.y;
				if(tmpV.y > yMax) yMax = tmpV.y;
				if(tmpV.z < zMin) zMin = tmpV.z;
				if(tmpV.z > zMax) zMax = tmpV.z;
			}
			tmpV = m_point[m_group[i]->face[j].vIndex.y - 1];
			if(tmpV.x < xMin) xMin = tmpV.x;
			if(tmpV.x > xMax) xMax = tmpV.x;
			if(tmpV.y < yMin) yMin = tmpV.y;
			if(tmpV.y > yMax) yMax = tmpV.y;
			if(tmpV.z < zMin) zMin = tmpV.z;
			if(tmpV.z > zMax) zMax = tmpV.z;
			tmpV = m_point[m_group[i]->face[j].vIndex.z - 1];
			if(tmpV.x < xMin) xMin = tmpV.x;
			if(tmpV.x > xMax) xMax = tmpV.x;
			if(tmpV.y < yMin) yMin = tmpV.y;
			if(tmpV.y > yMax) yMax = tmpV.y;
			if(tmpV.z < zMin) zMin = tmpV.z;
			if(tmpV.z > zMax) zMax = tmpV.z;
		}
	}
	m_bandBox[0].set(xMin,yMin,zMin);
	m_bandBox[1].set(xMax,yMin,zMin);
	m_bandBox[2].set(xMax,yMax,zMin);
	m_bandBox[3].set(xMin,yMax,zMin);
	m_bandBox[4].set(xMin,yMin,zMax);
	m_bandBox[5].set(xMax,yMin,zMax);
	m_bandBox[6].set(xMax,yMax,zMax);
	m_bandBox[7].set(xMin,yMax,zMax);
	m_isInited = XTrue;
	return XTrue;
}
XBool XModelObj::readFaceInfo(XGroupInfo *group,char *lineData)
{
	/*{//简单文件的测试
		group->withTexInfo = 0;
		group->withNormalInfo = 1;
		int v[3],n[3];
		sscanf(lineData,"f %d//%d %d//%d %d//%d",&v[0],&n[0],&v[1],&n[1],&v[2],&n[2]);
		XFaceInfo tempxxx;
		tempxxx.vIndex.x = v[0];
		tempxxx.vIndex.y = v[1];
		tempxxx.vIndex.z = v[2];
		tempxxx.nIndex.x = n[0];
		tempxxx.nIndex.y = n[1];
		tempxxx.nIndex.z = n[2];
		group->face.push_back(tempxxx);
		return XTrue;
	}*/
	removeRedundantSpace(lineData);
	int sumSlash = XString::getCharSum(lineData,'/');	//间隔的数量
	int sumSpace = XString::getCharSum(lineData,' ');	//数据的数量
	if(sumSpace < 3) return XFalse;
	XFaceInfo tempFaceInfo;
	//std::vector<XVectorIndex3> tempData;
	int offset = XString::getCharPosition(lineData,' ') + 1;
	//XVectorIndex3 tempIndex;
	if(sumSlash == 0)
	{//v
		if(group->withTexInfo == 1 ||
			group->withNormalInfo == 1) return XFalse;
		group->withTexInfo = 0;
		group->withNormalInfo = 0;

		group->originalFaces.push_back(XFaceInfoEx());
		XFaceInfoEx &ref = group->originalFaces[group->originalFaces.size() - 1];
		ref.vsIndex.resize(sumSpace);
		for(int i = 0;i < sumSpace;++ i)
		{
			if(sscanf(lineData + offset,"%d ",&ref.vsIndex[i]) != 1) return XFalse;
			offset += XString::getCharPosition(lineData + offset,' ') + 1;
		}
		for(int i = 0;i < sumSpace - 2;++ i)
		{
			tempFaceInfo.vIndex.x = ref.vsIndex[0];
			tempFaceInfo.vIndex.y = ref.vsIndex[i + 1];
			tempFaceInfo.vIndex.z = ref.vsIndex[i + 2];
			group->face.push_back(tempFaceInfo);
		}
	}else
	if(sumSlash == sumSpace)
	{//v/t
		if(group->withTexInfo == 0 ||
			group->withNormalInfo == 1) return XFalse;
		group->withTexInfo = 1;
		group->withNormalInfo = 0;

		group->originalFaces.push_back(XFaceInfoEx());
		XFaceInfoEx &ref = group->originalFaces[group->originalFaces.size() - 1];
		ref.vsIndex.resize(sumSpace);
		ref.tsIndex.resize(sumSpace);
		for(int i = 0;i < sumSpace;++ i)
		{
			if(sscanf(lineData + offset,"%d/%d ",&ref.vsIndex[i],&ref.tsIndex[i]) != 2) return XFalse;
			offset += XString::getCharPosition(lineData + offset,' ') + 1;
		}
		for(int i = 0;i < sumSpace - 2;++ i)
		{
			tempFaceInfo.vIndex.x = ref.vsIndex[0];
			tempFaceInfo.vIndex.y = ref.vsIndex[i + 1];
			tempFaceInfo.vIndex.z = ref.vsIndex[i + 2];

			tempFaceInfo.tIndex.x = ref.tsIndex[0];
			tempFaceInfo.tIndex.y = ref.tsIndex[i + 1];
			tempFaceInfo.tIndex.z = ref.tsIndex[i + 2];
			group->face.push_back(tempFaceInfo);
		}
	}else
	{// 
		if(lineData[XString::getCharPosition(lineData, '/') + 1] == '/')
		{//v//n
			if(group->withTexInfo == 1 ||
				group->withNormalInfo == 0) return XFalse;
			group->withTexInfo = 0;
			group->withNormalInfo = 1;

			group->originalFaces.push_back(XFaceInfoEx());
			XFaceInfoEx &ref = group->originalFaces[group->originalFaces.size() - 1];
			ref.vsIndex.resize(sumSpace);
			ref.nsIndex.resize(sumSpace);
			for(int i = 0;i < sumSpace;++ i)
			{
				if (sscanf(lineData + offset, "%d//%d ", &ref.vsIndex[i], &ref.nsIndex[i]) != 2) return XFalse;
				offset += XString::getCharPosition(lineData + offset,' ') + 1;
			}
			for(int i = 0;i < sumSpace - 2;++ i)
			{
				tempFaceInfo.vIndex.x = ref.vsIndex[0];
				tempFaceInfo.vIndex.y = ref.vsIndex[i + 1];
				tempFaceInfo.vIndex.z = ref.vsIndex[i + 2];
				tempFaceInfo.nIndex.x = ref.nsIndex[0];
				tempFaceInfo.nIndex.y = ref.nsIndex[i + 1];
				tempFaceInfo.nIndex.z = ref.nsIndex[i + 2];
				group->face.push_back(tempFaceInfo);
			}
		}else
		{//v/t/n 
			if(group->withTexInfo == 0 ||
				group->withNormalInfo == 0) return XFalse;
			group->withTexInfo = 1;
			group->withNormalInfo = 1;

			group->originalFaces.push_back(XFaceInfoEx());
			XFaceInfoEx &ref = group->originalFaces[group->originalFaces.size() - 1];
			ref.vsIndex.resize(sumSpace);
			ref.tsIndex.resize(sumSpace);
			ref.nsIndex.resize(sumSpace);
			for(int i = 0;i < sumSpace;++ i)
			{
				if(sscanf(lineData + offset,"%d/%d/%d ",&ref.vsIndex[i],&ref.tsIndex[i],&ref.nsIndex[i]) != 3) return XFalse;
				offset += XString::getCharPosition(lineData + offset,' ') + 1;
			}
			for(int i = 0;i < sumSpace - 2;++ i)
			{
				tempFaceInfo.vIndex.x = ref.vsIndex[0];
				tempFaceInfo.vIndex.y = ref.vsIndex[i + 1];
				tempFaceInfo.vIndex.z = ref.vsIndex[i + 2];
				tempFaceInfo.tIndex.x = ref.tsIndex[0];
				tempFaceInfo.tIndex.y = ref.tsIndex[i + 1];
				tempFaceInfo.tIndex.z = ref.tsIndex[i + 2];
				tempFaceInfo.nIndex.x = ref.nsIndex[0];
				tempFaceInfo.nIndex.y = ref.nsIndex[i + 1];
				tempFaceInfo.nIndex.z = ref.nsIndex[i + 2];
				group->face.push_back(tempFaceInfo);
			}
		}
	}
	return XTrue;
}
XBool XModelObj::loadMaterial(const char *filename)
{
	if(filename == NULL) return XFalse;
	//下面打开贴图文件并读取相关信息
	FILE *fp = NULL;
	if((fp = fopen(filename,"r")) == NULL) return XFalse;
	char lineData[512];
	char tempLineData[512];
	int startIndex = 0;
	int endIndex = 0;
	XMaterialInfo * tempMaterialInfo = NULL;
	while(true)
	{
		if(fgets(lineData,512,fp) == NULL) break;
		if(lineData[0] == '#') continue;	//注释
		if(!XString::getFirstWord(lineData,startIndex,endIndex)) continue;
		memcpy(tempLineData,lineData + startIndex,endIndex - startIndex + 1);
		tempLineData[endIndex - startIndex + 1] = '\0';
		if(strcmp(tempLineData,"newmtl") == 0)
		{
			tempMaterialInfo = XMem::createMem<XMaterialInfo>();
			if(tempMaterialInfo == NULL)
			{
				fclose(fp);
				return XFalse;
			}
			//sscanf(lineData,"newmtl %s",tempLineData);
			tempMaterialInfo->matetialName = getObjInfo0FromStr(lineData);
			m_material.push_back(tempMaterialInfo);
		}else
		if(strcmp(tempLineData,"map_Kd") == 0)
		{//普通贴图
			if(tempMaterialInfo == NULL) return XFalse;
			//下面读取资源
			std::string tmp = getObjInfo0FromStr(lineData);
			if(tmp == "")
			{
				printf("错误的数据:%s\n",lineData);
				tempMaterialInfo->withTexture = XFalse;
				continue;
			}
			tempMaterialInfo->mapKdName = tmp;
			//路径整合
			tmp = XFile::mergeFilename(filename,tmp);
			if(!tempMaterialInfo->textureData.load(tmp.c_str()))
			{
				printf("贴图文件读取失败:%s\n",tmp.c_str());
				tempMaterialInfo->withTexture = XFalse;
			}else
				tempMaterialInfo->withTexture = XTrue;
		}else
		if(strcmp(tempLineData,"map_bump") == 0)
		{//法线贴图
			if(tempMaterialInfo == NULL) return XFalse;
			//下面读取资源
			std::string tmp = getObjInfo0FromStr(lineData);
			if(tmp == "")
			{
				printf("错误的数据:%s\n",lineData);
				tempMaterialInfo->withDumpTex = XFalse;
				continue;
			}
			tempMaterialInfo->mapDumpName = tmp;
			//路径整合
			tmp = XFile::mergeFilename(filename,tmp);
			if(!tempMaterialInfo->dumpTexData.load(tmp.c_str()))
			{
				printf("贴图文件读取失败:%s\n",tmp.c_str());
				tempMaterialInfo->withDumpTex = XFalse;
			}else
				tempMaterialInfo->withDumpTex = XTrue;
			//连接shader
		//	if(tempMaterialInfo->normalShader == NULL)
		//	{
		//		tempMaterialInfo->normalShader = XMem::createMem<XShaderGLSL>();
		//		if(tempMaterialInfo->normalShader == NULL) return XFalse;
		//		* tempMaterialInfo->normalShader = X3DWld.m_dumpMapShader;
		//		tempMaterialInfo->normalShader->connectTexture("Texture_00",NULL);
		//		tempMaterialInfo->normalShader->connectTexture("normalTexture",&tempMaterialInfo->dumpTexData.texture.m_texture);
		//	}
		}else
		if(strcmp(tempLineData,"illum") == 0)
		{//illum(尚未实现)
			if(tempMaterialInfo == NULL) return XFalse;
			if(sscanf(lineData + endIndex + 1," %d",&tempMaterialInfo->illum) != 1) return XFalse;
		}else
		if(tempLineData[0] == 'K' && tempLineData[1] == 'a')
		{//ka
			if(tempMaterialInfo == NULL) return XFalse;
			if(sscanf(lineData + endIndex + 1," %f %f %f",&tempMaterialInfo->ka.x,
				&tempMaterialInfo->ka.y,&tempMaterialInfo->ka.z) != 3) return XFalse;
			tempMaterialInfo->ka.w = 1.0f;
		}else
		if(tempLineData[0] == 'K' && tempLineData[1] == 'd')
		{//kd
			if(tempMaterialInfo == NULL) return XFalse;
			if(sscanf(lineData + endIndex + 1," %f %f %f",&tempMaterialInfo->kd.x,
				&tempMaterialInfo->kd.y,&tempMaterialInfo->kd.z) != 3) return XFalse;
			tempMaterialInfo->kd.w = 1.0f;
		}else
		if(tempLineData[0] == 'K' && tempLineData[1] == 's')
		{//ks
			if(tempMaterialInfo == NULL) return XFalse;
			if(sscanf(lineData + endIndex + 1," %f %f %f",&tempMaterialInfo->ks.x,
				&tempMaterialInfo->ks.y,&tempMaterialInfo->ks.z) != 3) return XFalse;
			tempMaterialInfo->ks.w = 1.0f;
		}else
		if(tempLineData[0] == 'K' && tempLineData[1] == 'e')
		{//ke
			if(tempMaterialInfo == NULL) return XFalse;
			if(sscanf(lineData + endIndex + 1," %f %f %f",&tempMaterialInfo->ke.x,
				&tempMaterialInfo->ke.y,&tempMaterialInfo->ke.z) != 3) return XFalse;
			tempMaterialInfo->ke.w = 0.0f;
		}else
		if(tempLineData[0] == 'N' && tempLineData[1] == 'i')
		{//Ni(尚未方式)
			if(tempMaterialInfo == NULL) return XFalse;
			if(sscanf(lineData + endIndex + 1," %f",&tempMaterialInfo->Ni) != 1) return XFalse;
		}else
		if(tempLineData[0] == 'N' && tempLineData[1] == 's')
		{//Ns
			if(tempMaterialInfo == NULL) return XFalse;
			float tempNs = 0;
			if(sscanf(lineData + endIndex + 1," %f",&tempNs) != 1) return XFalse;
			tempMaterialInfo->shininess = tempNs * 0.001f * 128.0f;
		}else
		if(tempLineData[0] == 'T' && tempLineData[1] == 'r')
		{//Tr
			if(tempMaterialInfo == NULL) return XFalse;
			float tempAlpha = 0;
			if(sscanf(lineData + endIndex + 1," %f",&tempAlpha) != 1) return XFalse;
			tempMaterialInfo->alpha = 1.0f - tempAlpha;
		}else
		if(tempLineData[0] == 'T' && tempLineData[1] == 'f')
		{//Tf(尚未实现)
			if(sscanf(lineData + endIndex + 1," %f %f %f",&tempMaterialInfo->Tf.x,
				&tempMaterialInfo->Tf.y,&tempMaterialInfo->Tf.z) != 3) return XFalse;
		}else
		if(tempLineData[0] == 'd')
		{//d
			if(tempMaterialInfo == NULL) return XFalse;
			if(sscanf(lineData + endIndex + 1," %f",&tempMaterialInfo->alpha) != 1) return XFalse;
		}
		if(feof(fp)) break;	//文件读取完成之后直接退出
	}

	fclose(fp);
	return XTrue;
}
XBool XModelObj::saveMaterial()
{
	int sum = m_materialFilename.size();
	if(sum == 0) return XTrue;	//如果没有材质文件直接返回
	int mSum = m_material.size();
	FILE * fp = NULL;
	for(int i = 0;i < sum;++ i)
	{
		if((fp = fopen(m_materialFilename[i].c_str(),"w")) == NULL) return XFalse;	//文件打开失败
		if(i == sum - 1)
		{//最后一个
			for(int j = m_materialIndex[i];j < mSum;++ j)
			{//写入mtl内容
				fprintf(fp,"newmtl %s\n",m_material[j]->matetialName.c_str());	//写入材质名称
				fprintf(fp,"\tNs %f\n",m_material[j]->shininess * 1000.0f);
				fprintf(fp,"\tNi %f\n",m_material[j]->Ni);
				fprintf(fp,"\td %f\n",m_material[j]->alpha);
				fprintf(fp,"\tTr %f\n",1.0f - m_material[j]->alpha);
				fprintf(fp,"\tTf %f %f %f\n",m_material[j]->Tf.x,m_material[j]->Tf.y,m_material[j]->Tf.z);
				fprintf(fp,"\tillum %d\n",m_material[j]->illum);
				fprintf(fp,"\tKa %f %f %f\n",m_material[j]->ka.x,m_material[j]->ka.y,m_material[j]->ka.z);
				fprintf(fp,"\tKd %f %f %f\n",m_material[j]->kd.x,m_material[j]->kd.y,m_material[j]->kd.z);
				fprintf(fp,"\tKs %f %f %f\n",m_material[j]->ks.x,m_material[j]->ks.y,m_material[j]->ks.z);
				fprintf(fp,"\tKe %f %f %f\n",m_material[j]->ke.x,m_material[j]->ke.y,m_material[j]->ke.z);
				fprintf(fp,"\tmap_Kd %s\n",m_material[j]->mapKdName.c_str());
				if(m_material[j]->withDumpTex) fprintf(fp,"\tmap_bump %s\n",m_material[j]->mapDumpName.c_str());
			}
		}else
		{
			for(int j = m_materialIndex[i];j < m_materialIndex[i + 1];++ j)
			{
				fprintf(fp,"newmtl %s\n",m_material[j]->matetialName.c_str());	//写入材质名称
				fprintf(fp,"\tNs %f\n",m_material[j]->shininess / 128.0f * 1000.0f);
				fprintf(fp,"\tNi %f\n",m_material[j]->Ni);
				fprintf(fp,"\td %f\n",m_material[j]->alpha);
				fprintf(fp,"\tTr %f\n",1.0f - m_material[j]->alpha);
				fprintf(fp,"\tTf %f %f %f\n",m_material[j]->Tf.x,m_material[j]->Tf.y,m_material[j]->Tf.z);
				fprintf(fp,"\tillum %d\n",m_material[j]->illum);
				fprintf(fp,"\tKa %f %f %f\n",m_material[j]->ka.x,m_material[j]->ka.y,m_material[j]->ka.z);
				fprintf(fp,"\tKd %f %f %f\n",m_material[j]->kd.x,m_material[j]->kd.y,m_material[j]->kd.z);
				fprintf(fp,"\tKs %f %f %f\n",m_material[j]->ks.x,m_material[j]->ks.y,m_material[j]->ks.z);
				fprintf(fp,"\tKe %f %f %f\n",m_material[j]->ke.x,m_material[j]->ke.y,m_material[j]->ke.z);
				fprintf(fp,"\tmap_Kd %s\n",m_material[j]->mapKdName.c_str());
				if(m_material[j]->withDumpTex) fprintf(fp,"\tmap_bump %s\n",m_material[j]->mapDumpName.c_str());
			}
		}
		fclose(fp);
	}
	return XTrue;
}
XBool XModelObj::checkData()
{
	int vSum = m_point.size();
	int tSum = m_texturePoint.size();
	int nSum = m_normal.size();
	XGroupInfo * tempGroup = NULL;
	float *vPointers = NULL;
	float *tPointers = NULL;
	float *nPointers = NULL;
	for(unsigned int i = 0;i < m_group.size();++ i)
	{
		tempGroup = m_group[i];
		//获取贴图的编号
		if(tempGroup->withTexInfo == 1)
		{
			tempGroup->materialIndex = getMaterialIndex(tempGroup->materialName);
			if(tempGroup->materialIndex == -1)
			{
				printf("缺失材质说明：%s\n",tempGroup->groupName.c_str());
				tempGroup->withTexInfo = 0;
			}
		}
		//建立绘图数据
		if(vPointers != NULL) XMem::XDELETE_ARRAY(vPointers);
		vPointers = XMem::createArrayMem<float>(tempGroup->face.size() * 9);
		if(vPointers == NULL) return XFalse;
		if(tempGroup->withTexInfo == 1)
		{
			if(tPointers != NULL) XMem::XDELETE_ARRAY(tPointers);
			tPointers = XMem::createArrayMem<float>(tempGroup->face.size() * 6);
			if(tPointers == NULL) return XFalse;
		}
		if(tempGroup->withNormalInfo == 1)
		{
			if(nPointers != NULL) XMem::XDELETE_ARRAY(nPointers);
			nPointers = XMem::createArrayMem<float>(tempGroup->face.size() * 9);
			if(nPointers == NULL) return XFalse;
		}
		//检查数据的正确性
		for(unsigned int j = 0;j < tempGroup->face.size();++ j)
		{
			if(tempGroup->face[j].vIndex.x < 0) tempGroup->face[j].vIndex.x = vSum + tempGroup->face[j].vIndex.x + 1;
			if(tempGroup->face[j].vIndex.y < 0) tempGroup->face[j].vIndex.y = vSum + tempGroup->face[j].vIndex.y + 1;
			if(tempGroup->face[j].vIndex.z < 0) tempGroup->face[j].vIndex.z = vSum + tempGroup->face[j].vIndex.z + 1;

			if(tempGroup->face[j].vIndex.x <= 0 || tempGroup->face[j].vIndex.x > vSum) return XFalse;
			if(tempGroup->face[j].vIndex.y <= 0 || tempGroup->face[j].vIndex.y > vSum) return XFalse;
			if(tempGroup->face[j].vIndex.z <= 0 || tempGroup->face[j].vIndex.z > vSum) return XFalse;

			vPointers[j * 9 + 0] = m_point[tempGroup->face[j].vIndex.x - 1].x;
			vPointers[j * 9 + 1] = m_point[tempGroup->face[j].vIndex.x - 1].y;
			vPointers[j * 9 + 2] = m_point[tempGroup->face[j].vIndex.x - 1].z;
			vPointers[j * 9 + 3] = m_point[tempGroup->face[j].vIndex.y - 1].x;
			vPointers[j * 9 + 4] = m_point[tempGroup->face[j].vIndex.y - 1].y;
			vPointers[j * 9 + 5] = m_point[tempGroup->face[j].vIndex.y - 1].z;
			vPointers[j * 9 + 6] = m_point[tempGroup->face[j].vIndex.z - 1].x;
			vPointers[j * 9 + 7] = m_point[tempGroup->face[j].vIndex.z - 1].y;
			vPointers[j * 9 + 8] = m_point[tempGroup->face[j].vIndex.z - 1].z;

			if(tempGroup->withTexInfo == 1)
			{
				if(tempGroup->face[j].tIndex.x < 0) tempGroup->face[j].tIndex.x = tSum - tempGroup->face[j].tIndex.x + 1;
				if(tempGroup->face[j].tIndex.y < 0) tempGroup->face[j].tIndex.y = tSum - tempGroup->face[j].tIndex.y + 1;
				if(tempGroup->face[j].tIndex.z < 0) tempGroup->face[j].tIndex.z = tSum - tempGroup->face[j].tIndex.z + 1;

				if(tempGroup->face[j].tIndex.x <= 0 || tempGroup->face[j].tIndex.x > tSum) return XFalse;
				if(tempGroup->face[j].tIndex.y <= 0 || tempGroup->face[j].tIndex.y > tSum) return XFalse;
				if(tempGroup->face[j].tIndex.z <= 0 || tempGroup->face[j].tIndex.z > tSum) return XFalse;

				tPointers[j * 6 + 0] = m_texturePoint[tempGroup->face[j].tIndex.x - 1].x;
				tPointers[j * 6 + 1] = m_texturePoint[tempGroup->face[j].tIndex.x - 1].y;
				tPointers[j * 6 + 2] = m_texturePoint[tempGroup->face[j].tIndex.y - 1].x;
				tPointers[j * 6 + 3] = m_texturePoint[tempGroup->face[j].tIndex.y - 1].y;
				tPointers[j * 6 + 4] = m_texturePoint[tempGroup->face[j].tIndex.z - 1].x;
				tPointers[j * 6 + 5] = m_texturePoint[tempGroup->face[j].tIndex.z - 1].y;
			}
			if(tempGroup->withNormalInfo == 1)
			{
				if(tempGroup->face[j].nIndex.x < 0) tempGroup->face[j].nIndex.x = nSum + tempGroup->face[j].nIndex.x + 1;
				if(tempGroup->face[j].nIndex.y < 0) tempGroup->face[j].nIndex.y = nSum + tempGroup->face[j].nIndex.y + 1;
				if(tempGroup->face[j].nIndex.z < 0) tempGroup->face[j].nIndex.z = nSum + tempGroup->face[j].nIndex.z + 1;

				if(tempGroup->face[j].nIndex.x <= 0 || tempGroup->face[j].nIndex.x > nSum) return XFalse;
				if(tempGroup->face[j].nIndex.y <= 0 || tempGroup->face[j].nIndex.y > nSum) return XFalse;
				if(tempGroup->face[j].nIndex.z <= 0 || tempGroup->face[j].nIndex.z > nSum) return XFalse;

				nPointers[j * 9 + 0] = m_normal[tempGroup->face[j].nIndex.x - 1].x;
				nPointers[j * 9 + 1] = m_normal[tempGroup->face[j].nIndex.x - 1].y;
				nPointers[j * 9 + 2] = m_normal[tempGroup->face[j].nIndex.x - 1].z;
				nPointers[j * 9 + 3] = m_normal[tempGroup->face[j].nIndex.y - 1].x;
				nPointers[j * 9 + 4] = m_normal[tempGroup->face[j].nIndex.y - 1].y;
				nPointers[j * 9 + 5] = m_normal[tempGroup->face[j].nIndex.y - 1].z;
				nPointers[j * 9 + 6] = m_normal[tempGroup->face[j].nIndex.z - 1].x;
				nPointers[j * 9 + 7] = m_normal[tempGroup->face[j].nIndex.z - 1].y;
				nPointers[j * 9 + 8] = m_normal[tempGroup->face[j].nIndex.z - 1].z;
			}
		}
		if(!tempGroup->vbo.init(tempGroup->face.size() * 3,vPointers,tempGroup->withTexInfo == 1?tPointers:NULL,
			tempGroup->withNormalInfo == 1?nPointers:NULL,NULL)) 
		{
			XMem::XDELETE_ARRAY(vPointers);
			XMem::XDELETE_ARRAY(tPointers);
			XMem::XDELETE_ARRAY(nPointers);
			return XFalse;
		}
		XMem::XDELETE_ARRAY(vPointers);
		XMem::XDELETE_ARRAY(tPointers);
		XMem::XDELETE_ARRAY(nPointers);
	}
	//下面使得材质数据生效
	for(unsigned int i = 0;i < m_material.size(); ++ i)
	{
		m_material[i]->setData();
	}
	return XTrue;
}
int XModelObj::getMaterialIndex(const std::string& str)
{
	for(unsigned int i = 0;i < m_material.size();++ i)
	{
		if(m_material[i]->matetialName == str) return i;
	}
	return -1;
}
void XModelObj::draw(XBool withTex,const XBasic3DObject *base,bool withBandbox)
{
	if(!m_isInited) return;
	if(gFrameworkData.p3DWorld == NULL) return;
	useMatrix(base);
	//glTranslatef(m_position.x,m_position.y,m_position.z);
	//glRotatef(m_angle.x,1,0,0);
	//glRotatef(m_angle.y,0,1,0);
	//glRotatef(m_angle.z,0,0,1);
	//glScalef(m_scale.x,m_scale.y,m_scale.z);
	//drawOrigin();
	glColor4fv(XFColor::white);

	XGroupInfo * tempGroup = NULL;
	for(unsigned int i = 0;i < m_group.size();++ i)
	{
		tempGroup = m_group[i];
		XBool needTexture = tempGroup->withTexInfo == 1 && withTex && m_material[tempGroup->materialIndex]->withTexture;
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		if(tempGroup->withTexInfo == 1 && withTex) 
		{//如果存在贴图
			m_material[tempGroup->materialIndex]->material.bindMaterial();
			if(m_material[tempGroup->materialIndex]->withTexture)
			{
				glActiveTexture(GL_TEXTURE0);
				XGL::EnableTexture2D();
				XGL::BindTexture2D(m_material[tempGroup->materialIndex]->textureData.texture.m_texture);
				//glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE); 
				if(m_material[tempGroup->materialIndex]->withDumpTex)
					gFrameworkData.p3DWorld->useShadow(needTexture,SHADER_DUMP_AND_SHADOW,m_material[tempGroup->materialIndex]->dumpTexData.texture.m_texture);	//使用dumpshader并传递dump贴图
				else
					gFrameworkData.p3DWorld->useShadow(needTexture,SHADER_SHADOW);
				//glColor4f(m_color.r,m_color.g,m_color.b,m_color.a * m_material[tempGroup->materialIndex]->alpha);
			}else
			{//有材质无贴图
				glColor4fv(m_color);
				gFrameworkData.p3DWorld->useShadow(needTexture,SHADER_SHADOW);
			}
		}else
		{//如果不存在贴图
			gFrameworkData.p3DWorld->m_worldMaterial.bindMaterial();	//如果没有材质信息则使用默认的材质
			glColor4fv(m_color);
			gFrameworkData.p3DWorld->useShadow(needTexture,SHADER_SHADOW);
		}

		tempGroup->vbo.drawByArray(GL_TRIANGLES,withTex);

		gFrameworkData.p3DWorld->removeShadow();
		if(tempGroup->withTexInfo == 1 && withTex) 
			m_material[tempGroup->materialIndex]->material.unbindMaterial();
		else
			gFrameworkData.p3DWorld->m_worldMaterial.unbindMaterial();
		
		glPopAttrib();
	}
	if(withBandbox)
	{
		XGL::setBlendAlpha();
		glColor4fv(XFColor::white);
		glLineWidth(1.0f);
		glBegin(GL_LINES);
		glVertex3fv(m_bandBox[0]); glVertex3fv(m_bandBox[1]);
		glVertex3fv(m_bandBox[1]); glVertex3fv(m_bandBox[2]);
		glVertex3fv(m_bandBox[2]); glVertex3fv(m_bandBox[3]);
		glVertex3fv(m_bandBox[3]); glVertex3fv(m_bandBox[0]);

		glVertex3fv(m_bandBox[4]); glVertex3fv(m_bandBox[5]);
		glVertex3fv(m_bandBox[5]); glVertex3fv(m_bandBox[6]);
		glVertex3fv(m_bandBox[6]); glVertex3fv(m_bandBox[7]);
		glVertex3fv(m_bandBox[7]); glVertex3fv(m_bandBox[4]);

		glVertex3fv(m_bandBox[0]); glVertex3fv(m_bandBox[4]);
		glVertex3fv(m_bandBox[1]); glVertex3fv(m_bandBox[5]);
		glVertex3fv(m_bandBox[2]); glVertex3fv(m_bandBox[6]);
		glVertex3fv(m_bandBox[3]); glVertex3fv(m_bandBox[7]);
		glEnd();
		XGL::DisableBlend();
	}

	//XGL::EnableBlend();
	//XGL::DisableTexture2D();

	endMatrix();
}

XMaterialInfo * XModelObj::getMaterialInfo(const std::string& MatName)
{
	for(unsigned int i = 0;i < m_material.size();++ i)
	{
		if(m_material[i]->matetialName == MatName) return m_material[i];
	}
	return NULL;
}
//XBool XModelObj::setNormalShader(const string &MatName,XShaderGLSL *shader)
//{
//	XMaterialInfo *tempMat = getMaterialInfo(MatName);
//	if(tempMat == NULL) return XFalse;
//	tempMat->normalShader = shader;
//	return XTrue;
//}
//void XModelObj::setWithDumpMap(XBool flag)
//{
//	if(flag)
//	{
//		for(unsigned int i = 0;i < m_material.size();++ i)
//		{
//			if(m_material[i]->normalShader != NULL) m_material[i]->withDumpTex = XTrue;
//		}
//	}else
//	{
//		for(unsigned int i = 0;i < m_material.size();++ i)
//		{
//			m_material[i]->withDumpTex = XFalse;
//		}
//	}
//}
XGroupInfo *XModelObj::getGroup(const char *name)
{
	if(name == NULL) return NULL;
	for(unsigned int i = 0;i < m_group.size();++ i)
	{
		if(strcmp(m_group[i]->groupName.c_str(),name) == 0) return m_group[i];
	}
	return NULL;
}
void XModelObj::release()
{
	if(!m_isInited) return;
	m_point.clear();
	m_texturePoint.clear();
	m_normal.clear();
	for(unsigned int i = 0;i < m_material.size();++ i)
	{
		XMem::XDELETE(m_material[i]);
	}
	m_material.clear();
	m_materialFilename.clear();
	m_materialIndex.clear();
	for(unsigned int i = 0;i < m_group.size();++ i)
	{
		XMem::XDELETE(m_group[i]);
	}
	m_group.clear();
	m_isInited = XFalse;
}
namespace XRender {
void drawModel(const std::vector<XVec3> &v, const std::vector<XVec2> &t,
	const std::vector<XFaceInfo> &f, const XVec3& offset, const XVec3& scale, 
	unsigned int tex)
{
	glActiveTexture(GL_TEXTURE0);
	XGL::EnableTexture2D();
	XGL::BindTexture2D(tex);
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < f.size(); ++i)
	{
		glTexCoord2fv(t[f[i].tIndex.x - 1]);
		glVertex3fv(v[f[i].vIndex.x - 1].multi(scale) + offset);

		glTexCoord2fv(t[f[i].tIndex.y - 1]);
		glVertex3fv(v[f[i].vIndex.y - 1].multi(scale) + offset);

		glTexCoord2fv(t[f[i].tIndex.z - 1]);
		glVertex3fv(v[f[i].vIndex.z - 1].multi(scale) + offset);
	}
	glEnd();
}
void drawModel(const std::vector<XVec3> &v, const std::vector<XVec2> &t,
	const std::vector<XFaceInfoEx> &f, const XVec3& offset, const XVec3& scale,
	unsigned int tex)
{
	glActiveTexture(GL_TEXTURE0);
	XGL::EnableTexture2D();
	XGL::BindTexture2D(tex);
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < f.size(); ++i)
	{
		for (int j = 0; j < (int)(f[i].vsIndex.size()) - 2; ++j)
		{
			glTexCoord2fv(t[f[i].tsIndex[0] - 1]);
			glVertex3fv(v[f[i].vsIndex[0] - 1].multi(scale) + offset);

			glTexCoord2fv(t[f[i].tsIndex[j + 1] - 1]);
			glVertex3fv(v[f[i].vsIndex[j + 1] - 1].multi(scale) + offset);

			glTexCoord2fv(t[f[i].tsIndex[j + 2] - 1]);
			glVertex3fv(v[f[i].vsIndex[j + 2] - 1].multi(scale) + offset);
		}
	}
	glEnd();
}
}
#if !WITH_INLINE_FILE
#include "XModelObj.inl" 
#endif
}