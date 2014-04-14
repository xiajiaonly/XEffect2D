//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2013.6.3
//--------------------------------
#ifndef _JIA_XTERRAIN_
#define _JIA_XTERRAIN_

#include "XBasicFun.h"
#include "XTextureInformation.h"
#include "XCam.h"
#include "XBasic3D.h"
#include "XShaderGLSL.h"

//修改建议
//1、必须用VBO
//2、必须用索引模式

class _XWaterFlow;
//这是一个地形图的类
class _XTerrain
{
	friend _XWaterFlow;
private:
	_XBool m_isInited;				//是否进行了初始化
	float *m_terrainData;	//地图数据
	const float *m_newH;			//更新地形数据时传入的新的高度数据
	_XTextureData m_texture;		//地形的贴图数据
	_XBool m_withTexture;				//是否有贴图
	int m_width;					//地图的尺寸
	int m_height;	

	_XVector3 m_position;	//地形的位置
	_XVector3 m_size;		//地形的尺寸
	_XVector3 m_angle;		//地形的家都
	//float m_distance;
	float getHeight(float x,float z) const
	{
		if(x < 0 || x >= m_width) return 0.0f;
		if(z < 0 || z >= m_height) return 0.0f;
		return m_terrainData[(int)(z * m_width + x)];
	}
	_XVector2 getTexturePosition(float x,float z) const
	{
		if(x < 0) x = 0;
		if(x >= m_width) x = m_width;
		if(z < 0) z = 0;
		if(z >= m_height) z = m_height;
		//return _XVector2(x,z) / m_texture.textureSize;
		return _XVector2(x / m_width,z / m_height);
	}
	int m_allPointSum;		//总的顶点数
	int m_allFaceSum;		//总的面数
	float m_coordOffsetX;	//贴图的步进
	float m_coordOffsetY;

	_XVBO m_vbo;
public:
	float *m_v;
	float *m_n;
	float *m_t;
	float *m_c;
private:
	_XVector3 *m_faceN;		//面的法线
	//_XVector3 *m_pointN;	//点的法线
	unsigned int *m_index;	//索引

	bool m_withTexGen;	//是否使用金属反射
	float m_reflectRate;	//天空的反射系数
	int m_faceSum;		//三角面的数量
	_XBool m_withColor;	//是否使用差别化颜色

	_XShaderGLSL *m_pShader;	//shader的指针
	void releaseMemory()
	{
		if(m_pSubject == NULL)
		{
			XDELETE_ARRAY(m_v);
			XDELETE_ARRAY(m_n);
			XDELETE_ARRAY(m_faceN);
			//XDELETE_ARRAY(m_pointN);
			XDELETE_ARRAY(m_t);
			XDELETE_ARRAY(m_c);
			XDELETE_ARRAY(m_index);
		}else
		{
			if(!m_subjectV) XDELETE_ARRAY(m_v);
			if(!m_subjectN) XDELETE_ARRAY(m_n);
			if(!m_subjectC) XDELETE_ARRAY(m_c);
			if(!m_subjectT) XDELETE_ARRAY(m_t);
			if(!m_subjectI) XDELETE_ARRAY(m_index);
			m_pSubject = NULL;
		}
	}
public:
	float getReflectRate(){return m_reflectRate;}
	void setShader(_XShaderGLSL *p){m_pShader = p;}
public:
	_XBool init(const char *mapFilename,const char * texFilename,int w,int h,
		_XBool withColor = false,	//是否使用颜色
		_XResourcePosition resourcePosition = RESOURCE_SYSTEM_DEFINE);	//从文件中读取地形数据
	_XBool init(const float *mapData,const char * texFilename,int w,int h,
		_XBool withColor = false,	//是否使用颜色
		_XResourcePosition resourcePosition = RESOURCE_SYSTEM_DEFINE);	//直接从数组中读取数据
	void draw(_XBool withTex = XTrue);
	void drawNormal(int step = 1);	//描绘法线
	void updateTexture(const unsigned char * data)
	{
		if(data == NULL) return;
		if(!m_isInited) return;
		if(!m_withTexture) return;
		m_texture.texture.updateTexture(data);
	}
	void setTexture(unsigned int tex)	//这个函数用于测试
	{
		if(!m_isInited) return;
		if(!m_withTexture) return;
		m_texture.texture.m_texture = tex;
	}
	void setTexture(const _XTextureData &tex)
	{
		if(!m_isInited) return;
		if(!m_withTexture) return;
		m_texture.setACopy(tex);
	}
private:
	void updateNormal(const float *heights);	//更新法线的数值
public:
	//flags 用于标记这些面是否需要描绘出来
	//pT 用于提供数据，当pT不为NULL时，显示数据不计算，直接从pT中读取
	_XBool updateHeight(const float *heights,const bool *flags = NULL);	//flags 用于标记面是否有效
	_XBool updateByHeightAndNormal(const float *heights,const float *normal);
	_XBool updateByFlag(const bool *flags,const float * w);
	_XBool updateVBO(bool v,bool n = false,bool c = false,bool t = false,bool i = false);
	_XBool updateByStep(int step);	//使用步进step更新深度图的显示，默认情况下step = 1;

	void setPosition(float x,float y,float z) {m_position.set(x,y,z);}
	void setPosition(const _XVector3 &pos) {m_position = pos;}
	void setSize(float x,float y,float z) {m_size.set(x,y,z);}
	void setSize(const _XVector3 &size) {m_size = size;}
	void setAngle(float x,float y,float z) {m_angle.set(x,y,z);}
	void setAngle(const _XVector3 &angle) {m_angle = angle;}

	void setWithTexGen(bool flag){m_withTexGen = flag;}
	void release()
	{
		if(!m_isInited) return;
		XDELETE_ARRAY(m_terrainData);
		XDELETE_ARRAY(m_tempFitTerrain);
		m_vbo.release();
		releaseMemory();
		m_isInited = XFalse;
	}
	void setReflectRate(float rate)
	{
		if(rate < 0.0f) rate = 0.0f;
		if(rate > 1.0f) rate = 1.0f;
		m_reflectRate = rate;
	}
	_XVector3 myNormalize(float x,float z) 
	{//经过特定优化的归一化函数
		float d = 1.0f / sqrt(x * x + 1.0f + z * z);
		return _XVector3(x * d,d,z * d);
	}
	bool getMousePoint(_XVector2 &outPoint);	//[点拾取]根据当前鼠标的位置获取目前鼠标在3D地形中拾取的位置，如果没有拾取到地形则返回false
	bool getMousePointEx(const _XVector3 &n,const _XVector3 &f,_XVector2 &outPoint);	//[面拾取]
	_XVector3 getMyNormal(int x,bool flag)	//计算法线
	{//经过特定优化的求法线函数
		//_XVector3 w,v;
		if(flag) return myNormalize(m_newH[x] - m_newH[x + 1],m_newH[x] - m_newH[x + m_width]);	//上半个面
		else return myNormalize(m_newH[x + m_width] - m_newH[x + 1 + m_width],m_newH[x + 1] - m_newH[x + 1 + m_width]);	//下半个面
	}
private:
	_XTerrain *m_pSubject;	//从属目标
	bool m_subjectV;		//从属的标志
	bool m_subjectN;
	bool m_subjectC;
	bool m_subjectT;
	bool m_subjectI;
public:
	//t:从属的目标
	//w,n,c,t:拥有的属性
	//cv,cn,cc,ct:从属的属性
	bool setSubject(_XTerrain *T,bool cv,bool cn,bool cc,bool ct,bool ci)	//设置为t的从属，自身没有属性，属性全部用t的
	{//这里没有进行严格的数据校验(这个之后需要严格检查逻辑的完整性)
		if(T == NULL) return false;
		if(m_pSubject != NULL) return false;
		m_vbo.setAsSubject(T->m_vbo,XTrue,XTrue,m_withColor,XTrue,XTrue,cv,cn,cc,ct,ci);

		m_subjectV = cv;
		m_subjectN = cn;
		m_subjectC = cc;
		m_subjectT = ct;
		m_subjectI = ci;
		if(m_subjectV)
		{
			XDELETE_ARRAY(m_v);
			m_v = T->m_v;
		}
		if(m_subjectN)
		{
			XDELETE_ARRAY(m_n);
			m_n = T->m_n;
		}
		if(m_subjectC)
		{
			XDELETE_ARRAY(m_c);
			m_c = T->m_c;
		}
		if(m_subjectT)
		{
			XDELETE_ARRAY(m_t);
			m_t = T->m_t;
		}
		if(m_subjectI)
		{
			XDELETE_ARRAY(m_index);
			m_index = T->m_index;
		}
		m_pSubject = T;
		return true;
	}

	_XTerrain()
		:m_isInited(XFalse)
		,m_terrainData(NULL)
		,m_position(0.0f,0.0f,0.0f)
		,m_size(1.0f,1.0f,1.0f)
		,m_angle(0.0f,0.0f,0.0f)
		,m_v(NULL)
		,m_n(NULL)
		,m_faceN(NULL)
		,m_c(NULL)
		,m_withColor(false)
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
	{
	}
	~_XTerrain()
	{
		release();
	}
private:
	float *m_tempFitTerrain;
	int m_oInX;
	int m_oInY;
public:
	void myFitTerrain(const float *inData,int inX,int inY,float *outData);	//经过本地优化的
};
//这个函数尚未经过测试
extern void fitTerrain(const float *inData,int inX,int inY,float *outData,int outX,int outY);//地形适应,将任意尺寸的地形按照3次贝塞尔差值成需要的地形
#endif
