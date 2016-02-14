#ifndef _JIA_XALLTEXTURE_
#define _JIA_XALLTEXTURE_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.11.20
//--------------------------------
#include "XOSDefine.h"
#include "XMath\XRect.h"
#include "XBasicOpenGL.h"
#include "XPixelCommon.h"
#include <vector>
namespace XE{
#define TEX_INFO_DEBUG (0)

class XTextureData	//贴图数据,只有这个接口对外
{
private:
	int m_ID;
	XBool m_isInited;
public:
	char isEnableInsideClip;	//贴图模式：1：内部裁剪模式 0：没有经过内部裁剪
	XRect clipInsideRect;		//内部剪切的数据

	void setNoInited() {m_isInited = XFalse;}//设置物件没有初始化,这样子资源就不会被释放掉
	XTexture texture;		//指向贴图的指针
	XVector2 textureMove;		//资源文件在裁剪时的移动位置：左上角
	XVector2 textureMove2;		//资源文件在裁剪时的移动位置：右下角
	XVector2 textureSize;		//贴图的尺寸优化之前的贴图尺寸
	//++++++++++++++++++++++++++++++++++++++
	//由于贴图本身存在内部裁剪的问题，所以下面存在一个转换用于消除内部裁剪造成的问题
	XVector2 getCoord(const XVector2 &p)//p为贴图有效区域内的点,返回在内部裁剪后的归一化贴图坐标
	{
		if(isEnableInsideClip != 0)
		{
			return XVector2((p.x + clipInsideRect.left) / texture.m_w,
				(p.y + clipInsideRect.top) / texture.m_h);
		}else
			return p / textureSize;
	}
	XVector2 getCoordNormalized(const XVector2 &p)//p为贴图有效区域内的归一化的点,返回在内部裁剪后的归一化贴图坐标
	{
		if(isEnableInsideClip != 0)
		{
			return XVector2((p.x * (textureSize.x - 1.0f) + clipInsideRect.left + 1.0f) / texture.m_w,
				(p.y * (textureSize.y - 1.0f) + clipInsideRect.top + 1.0f) / texture.m_h);
		}else
			return p;
	}
	//--------------------------------------
	void updateTexture(const XPBO &pbo){texture.updateTexture(pbo);}
	void updateTextureR2B(const XPBO &pbo){texture.updateTextureR2B(pbo);}
	void updateTexture(const void * p);
	void updateTextureR2B(const void * p);
	void updateTextureEx(const void * p);		//不建议使用这个函数，但是某些情况下这个性能反而较好
	void updateTextureR2BEx(const void * p);	//不建议使用这个函数，但是某些情况下这个性能反而较好
	XTextureData()
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
	~XTextureData(){release();}
	int getID() const {return m_ID;}	//用于判断是否同一目标
	XBool load(const char * filename,XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	XBool loadEmpty();
	XBool createWithTexture(int w,int h,XColorMode colorMode,unsigned int tex);	//colorMode的参数参看texture创建的时候的参数说明
	//建立一张空白贴图0:RGBA,1:RGB,2:灰度图
	XBool createTexture(int w,int h,XColorMode colorMode);
	XBool createFromPixels(const XPixelCore &p)
	{
		if(!p.getIsInited()) return false;
		if(!createTexture(p.getWidth(),p.getHeight(),p.getColorMode())) return false;
		updateTexture(p.getPixelC());
		return true;
	}
	void release();
	XBool setACopy(const XTextureData& temp);
	XTextureData& operator = (const XTextureData& temp);
	void bind() {texture.bind();}
	XBool reset() {return texture.reset();}//初始化贴图
private:
	bool loadFromFolder(const char *filename,XResourcePosition resPos);	//从文件夹中载入资源
	bool loadFromPacker(const char *filename);	//从压缩包中载入资源
	bool loadFromWeb(const char *filename);		//从网页中读取资源
//下面的设计貌似已经过时
//	XTextureData(const XTextureData& temp);	//防止因为调用拷贝构造函数而造成错误
};
inline XTextureData * createATextureData(const char * filename,XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE)
{
	XTextureData *ret = XMem::createMem<XTextureData>();
	if(ret == NULL) return NULL;
	if(!ret->load(filename,resoursePosition))
	{
		XMem::XDELETE(ret);
		return NULL;
	}
	return ret;
}
#if TEX_INFO_DEBUG
extern int texInfoSum;
#endif
class XTextureInfo	//期望这个接口与XTextureData接口合并，这个接口只对内
{
private:
	XSCounter *m_cp;
public:
	char *textureName;			//资源文件的名字
	//char *targetFileName;		//目标文件的名字
	//int targetOrder;			//目标的文件编号
	XTexture texture;		//指向贴图的指针
	XVector2 textureOffset;	//资源在目标文件中的偏移位置
	XVector2 textureMove;		//资源文件在裁剪时的移动位置：左上角
	XVector2 textureMove2;		//资源文件在裁剪时的移动位置：右下角
	XVector2 textureSize;		//贴图的尺寸

	XTextureInfo()
		:textureOffset(0.0f,0.0f)
		,textureMove(0.0f,0.0f)
		,textureMove2(0.0f,0.0f)
		,textureSize(0.0f,0.0f)
	//,targetOrder(0)
	{
		m_cp = new XSCounter;
		textureName = XMem::createArrayMem<char>(MAX_FILE_NAME_LENGTH);
		textureName[0] = '\0';
	//	targetFileName = XMem::createArrayMem<char>(MAX_FILE_NAME_LENGTH);
	//	targetFileName[0] = '\0';
#if TEX_INFO_DEBUG
		++ texInfoSum;
		printf("%d-\n",texInfoSum);
#endif
	}
	~XTextureInfo()
	{
		if(m_cp != NULL && --m_cp->m_counter <= 0)
		{
			XMem::XDELETE(m_cp);
			XMem::XDELETE_ARRAY(textureName);
		//	XMem::XDELETE_ARRAY(targetFileName);
#if TEX_INFO_DEBUG
			-- texInfoSum;
			printf("%d-\n",texInfoSum);
#endif
		}
	}
	//重定义拷贝构造函数
	XTextureInfo(const XTextureInfo & temp);
	//重定义赋值操作符
	XTextureInfo& operator = (const XTextureInfo& temp);
};
//资源包的信息
struct XTxtureResPackInfo
{
	std::string m_resName;	//资源的名称
	std::vector<XTextureInfo> m_srcTexInfo;		//原始图片的信息
	std::vector<XTexture> m_dstTexInfo;			//目标图片的信息
};
//下面这个类是一个单子系统，可以不用重载赋值操作符或者复制构造函数
class XTextureResourceManager
{
public:
	XTextureResourceManager();
	virtual ~XTextureResourceManager(){release();}
protected:
	XTextureResourceManager(const XTextureResourceManager&);
	XTextureResourceManager &operator= (const XTextureResourceManager&);
private:
	//XResourcePosition m_resoursePosition;				//资源位置 0:外部 1:内部
private:
	XBool m_isInited;	//是否进行了初始化
	XBool m_isEnable;	//是否有效
	//+++++++++++++++++++++++++++++++++++++++++++++++++
	//下面是对这个类的升级
	std::vector<XTxtureResPackInfo> m_texResInfos;	//所有的贴图资源
	//-------------------------------------------------
	bool loadFromFolder(const char *filename,XResourcePosition resPos);	//从文件夹中载入资源
	bool loadFromPacker(const char *filename,XResourcePosition resPos);	//从压缩包中载入资源
	bool loadFromWeb(const char *filename);		//从网页中读取资源

public:
	XBool init(XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);	//初始化数据
	XBool getTexture(const char *temp,XTextureInfo &texInfo);		//获取贴图的信息
	XBool addATexResPack(const char *filename,XResourcePosition resPos = RESOURCE_SYSTEM_DEFINE);	//向管理器中增加一个资源包
	XBool releaseATexResPack(const char *filename);	//释放一个资源包
	XBool isLoaded(const char *filename);	//判断资源是都已经加载

	void release();
};
//还原贴图，p是优化之后的贴图文件的内容，length是p的数据长度，texInfo是优化之前的文件信息以及优化的内容
extern XBool reductionTexture(const unsigned char *p,int length,const XTextureInfo &texInfo);
#if WITH_INLINE_FILE
#include "XTextureInformation.inl"
#endif
}
#endif