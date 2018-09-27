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
namespace XE{
extern void removeRedundantSpace(char *p);	//删除连续的或者行尾的空格符（主要是为了读取OBJ文件时的格式整理）
//Obj文件中对面的描述
struct XFaceInfo
{//只支持三角面
    XVectorIndex3 vIndex;	//顶点的索引
	XVectorIndex3 tIndex;	//贴图的索引
	XVectorIndex3 nIndex;	//法线的索引	
};
struct XFaceInfoEx
{//可以支持多边形
	std::vector<int> vsIndex;
	std::vector<int> tsIndex;
	std::vector<int> nsIndex;
};
struct XMaterialInfo
{//材质信息
	std::string matetialName;	//材质的名称
	std::string mapKdName;
	std::string mapDumpName;
	XMaterial material;	//建立的材质
	XTextureData textureData;	//贴图
	XBool withTexture;			//是否拥有贴图
	XBool withDumpTex;			//是否拥有法线贴图
	XTextureData dumpTexData;	//法线贴图
	//XShaderGLSL * normalShader;//法线贴图的shader
	//需要读取得参数
	XVec4 ka;
	XVec4 kd;
	XVec4 ks;
	XVec4 ke;
	float shininess;
	float alpha;
	//下面的参数有读取但是没有生效
	float Ni;
	XVec3 Tf;
	int illum;
	void setData()	//使得数据生效
	{
		material.setAmblient(ka.x,ka.y,ka.z,ka.w);
		material.setDiffuse(kd.x,kd.y,kd.z,kd.w);
		material.setSpecular(ks.x,ks.y,ks.z,ks.w);
		material.setEmission(ke.x,ke.y,ke.z,ke.w);
		material.setShininess(shininess);
	}
	XMaterialInfo()
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
	~XMaterialInfo()
	{
		//XMem::XDELETE(normalShader);
	}
};
struct XGroupInfo
{
	char withTexInfo;		//是否有贴图信息
	char withNormalInfo;	//是否有法线数据
	std::string groupName;	//组名
	int materialIndex;
	std::string materialName;
	std::vector<XFaceInfo> face;	//经过三角面拆分的面信息
	std::vector<XFaceInfoEx> originalFaces;	//原始的面信息
	XVBO vbo;

	XGroupInfo()
		:withTexInfo(-1)
		,withNormalInfo(-1)
		,materialIndex(0)
	{}
	~XGroupInfo(){release();}
	void release()
	{
		originalFaces.clear();
		face.clear();
		vbo.release();
	}
};

class XModelObj:public XBasic3DObject
{
private:
	XBool m_isInited;	//是否初始化
	XVec3 m_bandBox[8];	//模型的包围盒子，实例中可能经过位移、旋转和缩放所以数据会变化。

	std::vector<XVec3> m_point;				//顶点	//目前只解析这些数据
	std::vector<XVec2> m_texturePoint;		//贴图数据
	std::vector<XVec3> m_normal;			//法线
public:
	std::vector<XMaterialInfo*> m_material;	//材质信息
private:
	std::vector<XGroupInfo *> m_group;			//组
	std::vector<std::string> m_materialFilename;		//模型中涉及的材质的文件名
	std::vector<int> m_materialIndex;			//每个材质文件的起始索引

	XBool readFaceInfo(XGroupInfo *group,char *lineData);	//从一行数据中读取面的描述
	XBool loadMaterial(const char *filename);	//读取材质文件
	XBool saveMaterial();	//遍历材质队列，并保存材质数据
	int getMaterialIndex(const std::string& str);
	XMaterialInfo * getMaterialInfo(const std::string& MatName);
	XGroupInfo *getGroup(const char *name);	//通过名字获取组的指针
public:
	//XBool setNormalShader(const string &MatName,XShaderGLSL *shader);
	//void setWithDumpMap(XBool flag);	//设置是否使用法线贴图

	XBool save(const char *filename);	//将数据保存到文件，格式为OBJ
	XBool load(const char *filename);	//从文件中载入数据
	void draw(XBool withTex = XTrue,const XBasic3DObject *base = NULL,bool withBandbox = false);			//withTex描绘的时候是否使用贴图信息
private:
	XBool checkData();	//检查数据的合法性
public:
	int getTextureSum()const { return m_texturePoint.size(); }
	XVec2 getTexture(unsigned int index)const;
	int getVectorSum() const {return m_point.size();}
	XVec3 getVector(unsigned int index) const;
	XVec3 getNormal(unsigned int index)const;
	int getFaceSum() const;
	XVectorIndex3 getFaceVectorIndex(unsigned int index) const;
	XVectorIndex3 getFaceTextureIndex(unsigned int index) const;
	XVectorIndex3 getFaceNormalIndex(unsigned int index) const;
	void useMatrix(const XBasic3DObject *base = NULL);	//使用矩阵
	void endMatrix() { glPopMatrix(); }	//结束矩阵
	//获取原始数据
	int getOriFaceSum()const;
	std::vector<int> getOriFaceVIndex(unsigned int index) const;
	std::vector<int> getOriFaceTIndex(unsigned int index) const;
	std::vector<int> getOriFaceNIndex(unsigned int index) const;

	void release();
	XModelObj()
		:m_isInited(XFalse)
	{}
	~XModelObj(){release();}
};
namespace XRender {
extern void drawModel(const std::vector<XVec3> &v, const std::vector<XVec2> &t,
	const std::vector<XFaceInfo> &f, const XVec3& offset, const XVec3& scale, unsigned int tex);
extern void drawModel(const std::vector<XVec3> &v, const std::vector<XVec2> &t,
	const std::vector<XFaceInfoEx> &f, const XVec3& offset, const XVec3& scale, unsigned int tex);
}
//extern std::string getObjInfo0FromStr(const char * p);	//从OBJ文件读取的字符串中获取需要的字符串，如：组名，文件名等
#if WITH_INLINE_FILE
#include "XModelObj.inl" 
#endif
}
#endif