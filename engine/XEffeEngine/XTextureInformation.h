#ifndef _JIA_XALLTEXTURE_
#define _JIA_XALLTEXTURE_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.11.20
//--------------------------------

#include "XOSDefine.h"
#ifdef XEE_OS_WINDOWS
#include "windows.h"
#endif

#include "SDL.h"
#include "SDL_image.h"
#include "gl.h"
#include "XBasicClass.h"

#include "math.h"
#include "XBasicFun.h"
#include "XBasicOpenGL.h"

#include "stdio.h"
#include <memory>
#include "string.h"
using namespace std;

#define TEX_INFO_DEBUG (0)

class _XTextureData	//贴图数据,只有这个接口对外
{
private:
	_XBool m_isInited;
	int m_ID;
public:
	void setNoInited()		//设置物件没有初始化
	{//这样子资源就不会被释放掉
		m_isInited = XFalse;
	}
	_XTexture texture;		//指向贴图的指针
	_XVector2 textureMove;		//资源文件在裁剪时的移动位置：左上角
	_XVector2 textureMove2;		//资源文件在裁剪时的移动位置：右下角
	char isEnableInsideClip;			//贴图模式：1：内部裁剪模式 0：没有经过内部裁剪
	_XRect clipInsideRect;		//内部剪切的数据
	_XVector2 textureSize;		//贴图的尺寸优化之前的贴图尺寸
	void updateTexture(const _XPBO &pbo){texture.updateTexture(pbo);}
	void updateTextureR2B(const _XPBO &pbo){texture.updateTextureR2B(pbo);}
	void updateTexture(const void * p){texture.updateTexture(p);}
	void updateTextureR2B(const void * p){texture.updateTextureR2B(p);}
	_XTextureData()
		:textureMove(0.0f,0.0f)
		//,pTexture(NULL)
		,textureMove2(0.0f,0.0f)
		,isEnableInsideClip(-1)
		,clipInsideRect(0,0,1,1)
		,m_isInited(XFalse)
		,textureSize(0.0f,0.0f)
	{
		static int id = 0;
		m_ID = id ++;
	}
	//下面会造成重复释放的问题
	~_XTextureData(){release();}
	int getID() const {return m_ID;}	//用于判断是否同一目标
	_XBool load(const char * filename,_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	_XBool loadEmpty();
	_XBool createWithTexture(int w,int h,_XColorMode colorMode,unsigned int tex);	//colorMode的参数参看texture创建的时候的参数说明
	//建立一张空白贴图0:RGBA,1:RGB,2:灰度图
	_XBool createTexture(int w,int h,_XColorMode colorMode);
	void release();
	_XBool setACopy(const _XTextureData& temp);
	_XTextureData& operator = (const _XTextureData& temp);
	void bind() {texture.bind();}
	_XBool reset() {return texture.reset();}//初始化贴图
private:
	_XTextureData(const _XTextureData& temp);	//防止因为调用拷贝构造函数而造成错误
};
inline _XTextureData * createATextureData(const char * filename,_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE)
{
	_XTextureData *ret = createMem<_XTextureData>();
	if(ret == NULL) return NULL;
	if(!ret->load(filename,resoursePosition))
	{
		XDELETE(ret);
		return NULL;
	}
	return ret;
}
#if TEX_INFO_DEBUG
extern int texInfoSum;
#endif
class _XTextureInfo	//期望这个接口与_XTextureData接口合并，这个接口只对内
{
private:
	_XSCounter *m_cp;
public:
	char *textureName;			//资源文件的名字
	//char *targetFileName;		//目标文件的名字
	//int targetOrder;			//目标的文件编号
	_XTexture texture;		//指向贴图的指针
	_XVector2 textureOffset;	//资源在目标文件中的偏移位置
	_XVector2 textureMove;		//资源文件在裁剪时的移动位置：左上角
	_XVector2 textureMove2;		//资源文件在裁剪时的移动位置：右下角
	_XVector2 textureSize;		//贴图的尺寸

	_XTextureInfo()
	:textureOffset(0.0f,0.0f)
	,textureMove(0.0f,0.0f)
	,textureMove2(0.0f,0.0f)
	,textureSize(0.0f,0.0f)
	//,targetOrder(0)
	{
		m_cp = new _XSCounter;
		textureName = createArrayMem<char>(MAX_FILE_NAME_LENGTH);
		textureName[0] = '\0';
	//	targetFileName = createArrayMem<char>(MAX_FILE_NAME_LENGTH);
	//	targetFileName[0] = '\0';
#if TEX_INFO_DEBUG
		++ texInfoSum;
		printf("%d-\n",texInfoSum);
#endif
	}
	~_XTextureInfo()
	{
		if(m_cp != NULL && --m_cp->m_counter <= 0)
		{
			XDELETE(m_cp);
			XDELETE_ARRAY(textureName);
		//	XDELETE_ARRAY(targetFileName);
#if TEX_INFO_DEBUG
			-- texInfoSum;
			printf("%d-\n",texInfoSum);
#endif
		}
	}
	//重定义拷贝构造函数
	_XTextureInfo(const _XTextureInfo & temp)
	{
		if(this == &temp) return;		//防止自生拷贝
		if(temp.m_cp != NULL) ++temp.m_cp->m_counter;
		m_cp = temp.m_cp;
		texture = temp.texture;
		textureOffset = temp.textureOffset;
		textureMove = temp.textureMove;
		textureMove2 = temp.textureMove2;
		textureSize = temp.textureSize;
		textureName = temp.textureName;
	//	targetFileName = temp.targetFileName;
	//	targetOrder = temp.targetOrder;
	}
	//重定义赋值操作符
	_XTextureInfo& operator = (const _XTextureInfo& temp)
	{
		if(& temp == this) return * this;	//防止自身赋值
		if(temp.m_cp != NULL) ++temp.m_cp->m_counter;
		if(m_cp != NULL && -- m_cp->m_counter <= 0)
		{
			XDELETE(m_cp);
			XDELETE_ARRAY(textureName);
		//	XDELETE_ARRAY(targetFileName);
#if TEX_INFO_DEBUG
			-- texInfoSum;
			printf("%d-\n",texInfoSum);
#endif
		}
		m_cp = temp.m_cp;
		texture = temp.texture;
		textureOffset = temp.textureOffset;
		textureMove = temp.textureMove;
		textureMove2 = temp.textureMove2;
		textureSize = temp.textureSize;
		textureName = temp.textureName;
	//	targetFileName = temp.targetFileName;
	//	targetOrder = temp.targetOrder;
		return * this;
	}
};
//下面这个类是一个单子系统，可以不用重载赋值操作符或者复制构造函数
class _XAllTexture
{
	//+++++++++++++++++++++++++++++++++++++++++++
	//下面需要将其设计为Singleton模式
protected:
	_XAllTexture();
	_XAllTexture(const _XAllTexture&);
	_XAllTexture &operator= (const _XAllTexture&);
	virtual ~_XAllTexture(); 
public:
	static _XAllTexture& GetInstance();
	//-------------------------------------------
private:
	_XResourcePosition m_resoursePosition;				//资源位置 0:外部 1:内部
private:
	_XBool m_isInited;	//是否进行了初始化
	_XBool m_isEnable;	//是否有效

	int m_textureSum;		//原始贴图的数量
	int m_targetTextureSum;	//经过处理之后的目标贴图数量

	_XTextureInfo *m_texInfo;	//所有原始贴图信息
	_XTexture *m_pTexture;		//所有目标贴图的信息，目标贴图是由原始贴图经过优化之后产生

public:
	_XBool init(_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);	//初始化数据
	_XBool getTexture(const char *temp,_XTextureInfo &texInfo);		//获取贴图的信息

	void release();
};

//还原贴图，p是优化之后的贴图文件的内容，length是p的数据长度，texInfo是优化之前的文件信息以及优化的内容
extern _XBool reductionTexture(const unsigned char *p,int length,const _XTextureInfo &texInfo);

#endif