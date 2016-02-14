#ifndef _JIA_XTERRAIN_
#define _JIA_XTERRAIN_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2013.6.3
//--------------------------------
#include "XBasicFun.h"
#include "XTextureInformation.h"
#include "XCam.h"
#include "XBasic3D.h"
#include "XShaderGLSL.h"
namespace XE{
//修改建议
//1、必须用VBO
//2、必须用索引模式

class XWaterFlow;
//这是一个地形图的类
class XTerrain
{
	friend XWaterFlow;
private:
	XBool m_isInited;				//是否进行了初始化
	float *m_terrainData;	//地图数据
	const float *m_newH;			//更新地形数据时传入的新的高度数据
	XTextureData m_texture;		//地形的贴图数据
	XBool m_withTexture;				//是否有贴图
	int m_width;					//地图的尺寸
	int m_height;	

	XVector3 m_position;	//地形的位置
	XVector3 m_scale;		//地形的尺寸
	XVector3 m_angle;		//地形的家都
	//float m_distance;
	float getHeight(float x,float z) const;
	XVector2 getTexturePosition(float x,float z) const;
	int m_allPointSum;		//总的顶点数
	int m_allFaceSum;		//总的面数
	float m_coordOffsetX;	//贴图的步进
	float m_coordOffsetY;

	XVBO m_vbo;
	std::string m_textureName;
public:
	float *m_v;
	float *m_n;
	float *m_t;
	float *m_c;
private:
	XVector3 *m_faceN;		//面的法线
	//XVector3 *m_pointN;	//点的法线
	unsigned int *m_index;	//索引

	bool m_withTexGen;	//是否使用金属反射
	float m_reflectRate;	//天空的反射系数
	int m_faceSum;		//三角面的数量
	XBool m_withColor;	//是否使用差别化颜色

	XShaderGLSL *m_pShader;	//shader的指针
	void releaseMemory();
public:
	float getReflectRate(){return m_reflectRate;}
	void setShader(XShaderGLSL *p){m_pShader = p;}
public:
	XBool init(const char *mapFilename,const char * texFilename,int w,int h,
		XBool withColor = XFalse,	//是否使用颜色
		XResourcePosition resourcePosition = RESOURCE_SYSTEM_DEFINE);	//从文件中读取地形数据
	XBool init(const float *mapData,const char * texFilename,int w,int h,
		XBool withColor = XFalse,	//是否使用颜色
		XResourcePosition resourcePosition = RESOURCE_SYSTEM_DEFINE);	//直接从数组中读取数据
	XBool initEx(const char *picFilename,const char * texFilename,	//从黑白的图片中载入高度图
		XBool withColor = XFalse,	//是否使用颜色
		XResourcePosition resourcePosition = RESOURCE_SYSTEM_DEFINE);	//从文件中读取地形数据
	void draw(XBool withTex = XTrue);
	void drawNormal(int step = 1);	//描绘法线
	void updateTexture(const unsigned char * data);
	void setTexture(unsigned int tex);	//这个函数用于测试
	void setTexture(const XTextureData &tex);
	bool saveToObj(const char * filename);	//将数据保存为obj格式的模型文件(尚未实现)
private:
	bool saveMtl(const char * filename);
	void updateNormal(const float *heights);	//更新法线的数值
	bool loadFromFolder(const char *filename,unsigned char *data);	//从文件夹中载入资源
	bool loadFromPacker(const char *filename,unsigned char *data);	//从压缩包中载入资源
	bool loadFromWeb(const char *filename,unsigned char *data);		//从网页中读取资源
public:
	//flags 用于标记这些面是否需要描绘出来
	//pT 用于提供数据，当pT不为NULL时，显示数据不计算，直接从pT中读取
	XBool updateHeight(const float *heights,const bool *flags = NULL);	//flags 用于标记面是否有效
	XBool updateByHeightAndNormal(const float *heights,const float *normal);
	XBool updateByFlag(const bool *flags,const float * w);
	XBool updateVBO(bool v,bool n = false,bool c = false,bool t = false,bool i = false);
	XBool updateByStep(int step);	//使用步进step更新深度图的显示，默认情况下step = 1;

	void setPosition(float x,float y,float z) {m_position.set(x,y,z);}
	void setPosition(const XVector3 &pos) {m_position = pos;}
	void setScale(float x,float y,float z) {m_scale.set(x,y,z);}
	void setScale(const XVector3 &scale) {m_scale = scale;}
	void setAngle(float x,float y,float z) {m_angle.set(x,y,z);}
	void setAngle(const XVector3 &angle) {m_angle = angle;}

	void setWithTexGen(bool flag){m_withTexGen = flag;}
	void release();
	void setReflectRate(float rate);
	XVector3 myNormalize(float x,float z); //经过特定优化的归一化函数
	bool getMousePoint(XVector2 &outPoint);	//[点拾取]根据当前鼠标的位置获取目前鼠标在3D地形中拾取的位置，如果没有拾取到地形则返回false
	bool getMousePointEx(const XVector3 &n,const XVector3 &f,XVector2 &outPoint);	//[面拾取]
	XVector3 getMyNormal(int x,bool flag);	//经过特定优化的求法线函数
private:
	XTerrain *m_pSubject;	//从属目标
	bool m_subjectV;		//从属的标志
	bool m_subjectN;
	bool m_subjectC;
	bool m_subjectT;
	bool m_subjectI;
public:
	//t:从属的目标
	//w,n,c,t:拥有的属性
	//cv,cn,cc,ct:从属的属性
	bool setSubject(XTerrain *T,bool cv,bool cn,bool cc,bool ct,bool ci);	//设置为t的从属，自身没有属性，属性全部用t的

	XTerrain()
		:m_isInited(XFalse)
		,m_terrainData(NULL)
		,m_position(0.0f,0.0f,0.0f)
		,m_scale(1.0f,1.0f,1.0f)
		,m_angle(0.0f,0.0f,0.0f)
		,m_v(NULL)
		,m_n(NULL)
		,m_faceN(NULL)
		,m_c(NULL)
		,m_withColor(XFalse)
		//,m_pointN(NULL)
		,m_t(NULL)
		//,m_distance(1.0f)	//地形中片的尺寸
		,m_withTexGen(false)
		,m_reflectRate(0.5f)
		,m_faceSum(0)
		,m_pShader(NULL)
		,m_index(NULL)
		,m_pSubject(NULL)
		,m_tempFitTerrain(NULL)
		,m_subjectV(false)
		,m_subjectN(false)
		,m_subjectC(false)
		,m_subjectT(false)
	{}
	~XTerrain() {release();}
private:
	float *m_tempFitTerrain;
	int m_oInX;
	int m_oInY;
public:
	void myFitTerrain(const float *inData,int inX,int inY,float *outData);	//经过本地优化的
};
//这个函数尚未经过测试
namespace X3D
{
	extern void fitTerrain(const float *inData,int inX,int inY,float *outData,int outX,int outY);//地形适应,将任意尺寸的地形按照3次贝塞尔差值成需要的地形
}

//这是一个纯算法的类用于对太多面的地形就行优化，减少不必要的面
struct XTerrainFaceInfo
{
	bool isEnable;		//是否有效
	int index[3];		//三个顶点的索引
	XVector3 normal;	//法线方向
	XTerrainFaceInfo()
		:isEnable(false)
	{}
};
struct XTerrainVertexInfo
{//地形中顶点的相关数据
	int faceSum;	//面的数量
	int index;	//编号
	int lastIndex;	//最终的编号	//用于对顶点数据进行优化
	int normalIndex;	//法线的编号，用于对法线数据进行优化
	int x,y;	//位置
	float h;	//高度
	int faceIndex[6];	//临接6个面的索引
};
class XTerrainOptimization
{
private:
	XBool m_isInited;
	float * m_data;
	int m_w;
	int m_h;
	int m_dataSize;
	std::vector<XTerrainFaceInfo> m_faceInfos;		//顶点信息列表
	std::vector<XTerrainVertexInfo> m_vertexInfos;	//面信息列表
public:
	XBool init(const float *data,int w,int h);//直接从数据进行初始化
	XBool init(const char * filename);//直接从文件进行初始化
	
	void optimization();//优化函数，至关重要的一个函数
	XBool saveToObj(const char * filename,const char *texName = NULL);	//保存为指定文件名的obj文件
	XTerrainOptimization()
		:m_isInited(XFalse)
		,m_data(NULL)
	{}
	~XTerrainOptimization()
	{release();}
	void release()
	{
		if(!m_isInited) return;
		XMem::XDELETE_ARRAY(m_data);
		m_isInited = XFalse;
	}
};
#if WITH_INLINE_FILE
#include "XTerrain.inl" 
#endif
}
#endif
