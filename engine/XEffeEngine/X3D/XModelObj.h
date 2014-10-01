#ifndef _JIA_XMODELOBJ_
#define _JIA_XMODELOBJ_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2013.9.20
//--------------------------------
//需要增加的内容
//+1、对数据完整性的检查(未完成)
//+2、对f数据更完整的支持 x/y/z 点/图/法
//+3、对多模型的支持
//+4、对贴图的支持,多张贴图的支持
//+5、对四角面的支持
//+6、资源释放

//这是一个从Obj格式的模型文件中读取模型数据，并进行处理的类
#include "XBasic3D.h"
#include "XTextureInformation.h" 
#include "XMaterial.h"
#include "XShaderGLSL.h"
#include <vector>
extern void removeRedundantSpace(char *p);	//删除连续的或者行尾的空格符（主要是为了读取OBJ文件时的格式整理）
//Obj文件中对面的描述
struct _XFaceInfo
{
    _XVectorIndex3 vIndex;	//顶点的索引
	_XVectorIndex3 tIndex;	//贴图的索引
	_XVectorIndex3 nIndex;	//法线的索引	
};
struct _XMaterialInfo
{//材质信息
	string matetialName;	//材质的名称
	string mapKdName;
	string mapDumpName;
	_XMaterial material;	//建立的材质
	_XTextureData textureData;	//贴图
	_XBool withTexture;			//是否拥有贴图
	_XBool withDumpTex;			//是否拥有法线贴图
	_XTextureData dumpTexData;	//法线贴图
	//_XShaderGLSL * normalShader;//法线贴图的shader
	//需要读取得参数
	_XVector4 ka;
	_XVector4 kd;
	_XVector4 ks;
	_XVector4 ke;
	float shininess;
	float alpha;
	//下面的参数有读取但是没有生效
	float Ni;
	_XVector3 Tf;
	int illum;
	void setData()	//使得数据生效
	{
		material.setAmblient(ka.x,ka.y,ka.z,ka.w);
		material.setDiffuse(kd.x,kd.y,kd.z,kd.w);
		material.setSpecular(ks.x,ks.y,ks.z,ks.w);
		material.setEmission(ke.x,ke.y,ke.z,ke.w);
		material.setShininess(shininess);
	}
	_XMaterialInfo()
		:withDumpTex(XFalse)
		,withTexture(XFalse)
		//,normalShader(NULL)
		,ka(0.2f,0.2f,0.2f,1.0f)
		,kd(0.8f,0.8f,0.8f,1.0f)
		,ks(0.0f,0.0f,0.0f,1.0f)
		,ke(0.0f,0.0f,0.0f,0.0f)
		,shininess(0.0f)
		,alpha(1.0f)
	{}
	~_XMaterialInfo()
	{
		//XDELETE(normalShader);
	}
};
struct _XGroupInfo
{
	string groupName;	//组名
	char withTexInfo;		//是否有贴图信息
	char withNormalInfo;	//是否有法线数据
	int materialIndex;
	string materialName;
	std::vector<_XFaceInfo> face;
	_XVBO vbo;

	_XGroupInfo()
		:withTexInfo(-1)
		,withNormalInfo(-1)
		,materialIndex(0)
	{}
	~_XGroupInfo(){release();}
	void release()
	{
		face.clear();
		vbo.release();
	}
};

class _XModelObj:public _XBasic3DObject
{
private:
	_XBool m_isInited;	//是否初始化
	_XVector3 m_bandBox[8];	//模型的包围盒子，实例中可能经过位移、旋转和缩放所以数据会变化。

	std::vector<_XVector3> m_point;				//顶点	//目前只解析这些数据
	std::vector<_XVector2> m_texturePoint;		//贴图数据
	std::vector<_XVector3> m_normal;			//法线
	std::vector<_XMaterialInfo*> m_material;	//材质信息
	std::vector<_XGroupInfo *> m_group;			//组
	std::vector<string> m_materialFilename;		//模型中涉及的材质的文件名
	std::vector<int> m_materialIndex;			//每个材质文件的起始索引

	_XBool readFaceInfo(_XGroupInfo *group,char *lineData);	//从一行数据中读取面的描述
	_XBool loadMaterial(const char *filename);	//读取材质文件
	_XBool saveMaterial();	//遍历材质队列，并保存材质数据
	int getMaterialIndex(const string &str);
	_XMaterialInfo * getMaterialInfo(const string &MatName);
	_XGroupInfo *getGroup(const char *name);	//通过名字获取组的指针
public:
	//_XBool setNormalShader(const string &MatName,_XShaderGLSL *shader);
	//void setWithDumpMap(_XBool flag);	//设置是否使用法线贴图

	_XBool save(const char *filename);	//将数据保存到文件，格式为OBJ
	_XBool load(const char *filename);	//从文件中载入数据
	void draw(_XBool withTex = XTrue,const _XBasic3DObject *base = NULL);			//withTex描绘的时候是否使用贴图信息
	void drawBandbox()
	{//暂时先不考虑位移、旋转和缩放(尚未完成)
		drawLine(m_bandBox[0],m_bandBox[1]);
		drawLine(m_bandBox[1],m_bandBox[2]);
		drawLine(m_bandBox[2],m_bandBox[3]);
		drawLine(m_bandBox[3],m_bandBox[0]);
		drawLine(m_bandBox[4],m_bandBox[5]);
		drawLine(m_bandBox[5],m_bandBox[6]);
		drawLine(m_bandBox[6],m_bandBox[7]);
		drawLine(m_bandBox[7],m_bandBox[4]);
		drawLine(m_bandBox[0],m_bandBox[4]);
		drawLine(m_bandBox[1],m_bandBox[5]);
		drawLine(m_bandBox[2],m_bandBox[6]);
		drawLine(m_bandBox[3],m_bandBox[7]);
	}
private:
	_XBool checkData();	//检查数据的合法性
public:
	int getVectorSum() const {return m_point.size();}
	_XVector3 getVector(int index) const 
	{
		if(index < 0 || index >= m_point.size()) return _XVector3::zero;
		return m_point[index];
	}
	_XVector3 getNormal(int index)
	{
		if(index < 0 || index >= m_normal.size()) return _XVector3::zero;
		return m_normal[index];
	}
	_XVectorIndex3 getFaceNormalIndex(int index) const
	{
		if(m_group.size() <= 0 || index < 0 || index >= m_group[0]->face.size()) return _XVectorIndex3(0.0f,0.0f,0.0f);
		return m_group[0]->face[index].nIndex;
	}
	int getFaceSum() const
	{
		if(m_group.size() <= 0) return 0;
		return m_group[0]->face.size();
	}
	_XVectorIndex3 getFaceVectorIndex(int index) const
	{
		if(m_group.size() <= 0 || index < 0 || index >= m_group[0]->face.size()) return _XVectorIndex3(0.0f,0.0f,0.0f);
		return m_group[0]->face[index].vIndex;
	}

	void release();
	_XModelObj()
		:m_isInited(XFalse)
	{}
	~_XModelObj(){release();}
};

//extern std::string getObjInfo0FromStr(const char * p);	//从OBJ文件读取的字符串中获取需要的字符串，如：组名，文件名等

#endif