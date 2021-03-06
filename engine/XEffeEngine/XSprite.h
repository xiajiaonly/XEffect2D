#ifndef _JIA_XSPRITE_
#define _JIA_XSPRITE_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------
//精灵需要支持下面几个功能
//1、移动
//2、缩放
//3、裁剪
//4、旋转
//5、设置缩放的基准点
//6、设置旋转的基准点
//7、上下左右翻转
//注意序列帧动画必须要符合2的n次方

//精灵的缩放和位置存在逻辑的布严密性

//建议：实现精灵的多边形裁剪，以实现一些特殊的表现效果

#include "XTextureInformation.h"
#include "XObjectBasic.h"
#include "XStringFun.h"
#include "XThread.h"

namespace XE {
class XShaderGLSL;
enum XTransformMode
{
	POINT_LEFT_TOP = 0,	//左上角为基准点
	POINT_CENTER = 1	//中心点为基准点
};
enum XTurnOverMode	//精灵的翻转模式
{
	TURN_OVER_MODE_NULL,	//没有进行翻转
	TURN_OVER_MODE_LEFT_TO_RIGHT,	//左右翻转
	TURN_OVER_MODE_UP_TO_DOWN,		//上下翻转
	TURN_OVER_MODE_L2R_AND_U2D,	//上下左右翻转
};
#if WITHXSPRITE_EX
class XSprite :public XObjectBasic
{
private:
	XResPos m_resoursePosition;	//资源位置 0:外部 1:内部

	XTurnOverMode m_turnOverMode;	//翻转模式
	XVec2 m_position;			//位置
	XVec2 m_scale;				//缩放尺寸
	XVec2 m_pixelSize;			//像素尺寸
	float m_angle;					//角度
	XVec2 m_changeCenter;		//旋转或者缩放的中心
	//下面是为了便于计算的零时数据
	float m_sinAngle;				//角度的零时变量
	float m_cosAngle;
	XTextureData m_textureData;	//贴图Texture的信息
	XFColor m_color;				//物件的颜色

	void updateData(const XTextureData *tex = NULL);
	void drawInside();
	//下面是关于alpha混合方式的处理
//	XGL::XBlendType m_blendType;	//混合方式：0：简单混合2因子混合、1：复杂的4因子混合
	XGL::XBlendModel m_blendModel;
//	char m_blendRGBScr;
//	char m_blendRGBDst;
//	char m_blendAScr;
//	char m_blendADst;

	XBool m_isInited;				//是否完成初始化
	XBool m_needAngleClip;			//是否需要进行角度裁剪
//	char m_blendTypeScr;
//	char m_blendTypeDst;
	XBool m_isVisible;				//物件是否可见
	XBool m_needUpdateData;		//是否需要更新内部数据
	XBool m_needClip;				//是否需要裁剪
	XRect m_clipRect;				//裁剪的数据
	//下面是shader的指针
	XShaderGLSL *m_pShader;
	//下面是对于角度裁剪的定义：角度裁剪的中心为中点，暂时不考虑任一点的角度裁剪
	XVec2 m_rectPoint[4];	//包围盒
	int m_pointSum;			//顶点的数量
	int m_upTexDataID;		//上一次描绘的贴图的ID，这里可以防止重复的刷新数据

	float m_vPointer[14];	//顶点数据
	float m_uPointer[14];	//贴图数据
	float m_clipAngle;		//裁剪的角度[0 - 360]
private:
	XSprite(const XSprite& temp);
public:
	void setAngle(float angle);		//[0-360]
	float getAngle() const;
	using XObjectBasic::setScale;
	void setScale(const XVec2& s);	//设置精灵的缩放比例
	const XVec2& getScale() const;
	void fitScale(float x, float y, bool deformation = true);	//自动适应制定的像素尺寸
	void fitScale(const XVec2& s, bool deformation = true);	//自动适应制定的像素尺寸
	using XObjectBasic::setPosition;
	void setPosition(const XVec2& p);			//设置精灵的坐标
	void setPositionX(float x);
	void setPositionY(float y);
	const XVec2& getPosition() const;
	void setTurnOverMode(XTurnOverMode mode);
	void setClipRect(const XRect& clipRect);	//使用像素数据
	void setClipRect(float left, float top, float right, float bottom);
	void disClip();
	void setChangeCenter(const XVec2& center);
	XBool init(const char * filename,										//文件名
		XResPos resPos = RES_SYS_DEF,	//资源的位置 0外部 1内部
		const XVec2& changePoint = XVec2(0.5f));					//变形的据准点
	XBool init(int tempW = 0, int tempH = 0, int needSizeCheck = 1, const XVec2& changePoint = XVec2(0.5f));
	XBool init(XTextureData& texData, const XVec2& changePoint = XVec2(0.5f));
	XBool init(int w, int h, XColorMode colorMode, unsigned int tex, const XVec2& changePoint = XVec2(0.5f));	//从一张贴图建立一个精灵
	void draw();
	void draw(GLuint tex);	//对于内部资源由于内部裁剪所以会存在问题
	void draw(const XTextureData& texData);	//这里尚未考虑内部裁剪，所以目前会存在问题
	void draw(const XVec2 *u, const XVec2 *v, int w, int h);	//这个是为了变形而定义的接口
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++															//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	bool m_isThreadLoading;		//是否正在多线程载入
	XThreadState m_loadThread;
	std::string m_threadFilename;
	XResPos m_threadResPos;
public:
	//使用多线程的方式从后台载入资源
	bool initThread(const char *filename,			//图片的名称
		XResPos resPos = RES_SYS_DEF,
		const XVec2& changePoint = XVec2(0.5f));
	static DWORD WINAPI loadThread(void *);
	//数据是否加载完成
	bool getIsLoaded()const { return !m_isThreadLoading; }
	bool updateTex(const char *filename);	//从文件中更新数据到贴图，要求尺寸和像素格式一致
	bool updateThread(const char *filename);	//从文件中更新数据到贴图，要求尺寸和像素格式一致
	static DWORD WINAPI updateThread(void *);
	//------------------------------------------------------------
	XSprite()
		:m_isInited(XFalse)
		, m_turnOverMode(TURN_OVER_MODE_NULL)
		, m_needClip(XFalse)
		, m_color(1.0f, 1.0f)
		, m_isVisible(XTrue)
		, m_pShader(NULL)
		//,m_pShaderProc(NULL)	//为了向下兼容而定义的
//		, m_blendType(XGL::BLEND_TWO_DATA)
//		, m_blendTypeScr(4)
//		, m_blendTypeDst(5)
		, m_blendModel(XGL::BLEND_ALPHA)
		, m_needAngleClip(XFalse)
		, m_clipAngle(0.0f)
		, m_upTexDataID(-1)
		, m_isThreadLoading(false)
	{
		m_objType = OBJ_SPRITE;
		//	for(int i = 0;i < 14;++ i)
		//	{
		//		m_vPointer[i] = 0.0f;	//顶点数据
		//		m_uPointer[i] = 0.0f;	//贴图数据
		//	}
		memset(m_vPointer, 0, 14 * sizeof(float));
		memset(m_uPointer, 0, 14 * sizeof(float));
	}
	~XSprite() { release(); }
	void release();
	bool getIsInited()const { return m_isInited; }

	//下面是一些尚未实现的接口
	XBool isInRect(const XVec2& p);	//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
	XVec2 getBox(int order);

	void setAlpha(float temp);
	float getAlpha() const;
	using XObjectBasic::setColor;
	void setColor(const XFColor&c);		//小于0不会改变这一元素
	const XFColor& getColor() const { return m_color; }

	void setVisible();			//设置物件可见
	void disVisible();			//设置物件不可见
	XBool getVisible() const;
	//alpha混合的接口
//	void setBlendType(int typeScr, int typeDst);
//	void setBlendType(int rgbScr, int rgbDst, int aScr, int aDst);
//	void setBlendMode(XGL::XBlendType mode);
	void setBlendModel(XGL::XBlendModel model);
	//shader的接口
	void setShaderClass(XShaderGLSL * shader);
	//下面是尚未实现的接口
	XTexture * getTexture();	//返回当前帧的贴图指针
	XTextureData * getTextureData();	//返回当前帧的贴图数据

	void setAngleClip(float angle);
	void disAngleClip();

	XSprite& operator = (const XSprite& temp);	//对目标的资源进行共用，不拷贝
	void setACopy(const XSprite& temp);
	//下面实现一些向下兼容的接口
	void setIsTransformCenter(XTransformMode temp)
	{
		if (temp == POINT_LEFT_TOP) setChangeCenter(XVec2::zero);
		else setChangeCenter(XVec2(0.5f));
	}
	void draw(GLuint *tex) { draw(*tex); }	//对于内部资源由于内部裁剪所以会存在问题
	void draw(const XTextureData* texData) { draw(*texData); }	//这里尚未考虑内部裁剪，所以目前会存在问题
	void drawWithoutBlend() { draw(); }
	XBool init(const char * filename,					//文件名
		XResPos resPos,					//资源的位置 0外部 1内部
		XTransformMode isTransformCenter)	//变形的据准点
	{
		if (isTransformCenter == POINT_CENTER) return init(filename, resPos, XVec2(0.5f));
		else return init(filename, resPos, XVec2::zero);
	}
	XBool init(int tempW, int tempH, int needSizeCheck, XTransformMode isTransformCenter)
	{
		if (isTransformCenter == POINT_CENTER) return init(tempW, tempH, needSizeCheck, XVec2(0.5f));
		else return init(tempW, tempH, needSizeCheck, XVec2::zero);
	}
	XBool init(XTextureData& texData, XTransformMode isTransformCenter)
	{
		if (isTransformCenter == POINT_CENTER) return init(texData, XVec2(0.5f));
		else return init(texData, XVec2::zero);
	}
	XBool init(int w, int h, XColorMode colorMode, unsigned int tex, XTransformMode isTransformCenter)
	{
		if (isTransformCenter == POINT_CENTER) return init(w, h, colorMode, tex, XVec2(0.5f));
		else return init(w, h, colorMode, tex, XVec2::zero);
	}
	//	void (*m_pShaderProc)(void);
	void setOverturn(char temp)	//设置左右翻转(为了保持向旧版本的兼容保留这个接口)
	{
		if (temp == 0) resetLeftRightUpDown();
		else setLeft2Right();
	}
	void setLeft2Right() { setTurnOverMode(TURN_OVER_MODE_LEFT_TO_RIGHT); }//设置左右翻转
	void setUp2Down() { setTurnOverMode(TURN_OVER_MODE_UP_TO_DOWN); }		//设置上下翻转
	void resetLeftRightUpDown() { setTurnOverMode(TURN_OVER_MODE_NULL); }//回复上下左右的正确位置
	XTurnOverMode getTurnOverMode() const { return m_turnOverMode; }
	XTransformMode getTransformMode() const
	{
		if (m_changeCenter.x == 0.0f && m_changeCenter.y == 0.0f) return POINT_LEFT_TOP;
		else return POINT_CENTER;
	}
	int XSprite::getTW() const { return m_textureData.texture.m_w; }	//获取贴图的宽度，如果经过优化，则为优化之后的宽度
	int XSprite::getTH() const { return m_textureData.texture.m_h; }
	int XSprite::getW() const { return m_textureData.getOpWidth(); }	//获取贴图中有效部分的宽度，如果没有经过优化则为整个贴图大小
	int XSprite::getH() const { return m_textureData.getOpHeight(); }

	//	void justForTest() {;}
};
#else
//新功能的实现建议：实现多边形裁剪
//#define MAX_SPRITE_FILENAME_LENGTH (256)
#define IS_USE_SOLIDIFY (0)

class XSprite :public XObjectBasic	//精灵的结构体
{
private:
	XResPos m_resoursePosition;				//资源位置 0:外部 1:内部

	XTextureData m_textureData;

	XVec2 m_setPosition;			//精灵被设置的位置，内部使用的位置，与设置的位置是有区别的
	XVec2 m_setTransformCenter;		//用户设置的形状变换的位置，实际使用的变形的位置与设置的位置是有区别的

	XRect m_clipOutsideRect;			//用户设置的外部裁剪尺寸
	XRect m_clipRect;					//最终的裁剪尺寸，这个尺寸由内部裁剪尺寸与外部裁剪尺寸组成
	char m_isEnableOutsideChip;			//是否允许剪切
	char m_isVisible;

	void updateClipAndRotatoData();		//由于贴图的变更，更新裁剪和旋转的数据
	char m_needUpdateInsideData;		//是否需要更新内部数据
	void updateInsideData();			//由于精灵物件的属性变化而需要更新精灵的一些内部数据
public:
	void disClip();						//取消2D精灵裁减，默认为不裁减
	void setClipRect(const XRect& temp);		//设置2D精灵裁减范围
	//注意这里存在问题，如果图片小坐标方向有空余，裁剪之后拼合，然后调用这个函数，需要保证left和top为0，否则会出现问题。
	void setClipRect(float left, float top, float right, float bottom);	//设置2D精灵裁减范围

	int isInRect(float x, float y);	//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
	XVec2 getPosition() const { return m_setPosition; }
	//获取四个顶点的坐标，目前先不考虑旋转和缩放
	XVec2 getBox(int order);
private:
	// 下面添加一些变量来保存变形的处理
	XTurnOverMode m_turnOverMode;	//是否启动特殊形状的标签
	float m_shapeData[16];	//具体的形状描述的数据
private:
	//GLfloat x, y;			//精灵的显示位置
	XTransformMode m_isTransformCenter;

	//注意默认的旋转和缩放的中心点是统一的为图片的中心，如果图片存在裁剪，则为裁剪区域的左上角
	XVec2 m_transformCenter;		//变形或旋转的真实位置
	char m_changeTransformCenterManual;	//用户是否手动设置过旋转中心的位置，中心旋转或者左上角旋转不属于用户设置
public:
	void setIsTransformCenter(XTransformMode temp);	//设置缩放的基准点
	XTransformMode getTransformMode() const { return m_isTransformCenter; }

	void setTransformCenter(float x, float y)	//这个是否经过测试
	{
		m_changeTransformCenterManual = 1;
		if (m_textureData.isEnableInsideClip != 0)
		{
			m_setTransformCenter.set(x, y);
			if (m_clipOutsideRect.left < m_textureData.textureMove.x) m_transformCenter.x = m_clipOutsideRect.left - m_textureData.textureMove.x + m_setTransformCenter.x;
			else m_transformCenter.x = m_setTransformCenter.x;
			if (m_clipOutsideRect.top < m_textureData.textureMove.y) m_transformCenter.y = m_clipOutsideRect.top - m_textureData.textureMove.y + m_setTransformCenter.y;
			else m_transformCenter.y = m_setTransformCenter.y;

			//m_transformCenter.set(x - m_textureData.textureMove.x,y - m_textureData.textureMove.y);
		}
		else
		{
			m_transformCenter.set(x, y);
		}
	}
public:
	void setAlpha(float temp);
	float getAlpha() const;
	float getAngle() const { return angle; }
	void setAngle(float temp);
	int getW() const;
	int getH() const;
	void setVisible() { m_isVisible = 1; }					//设置物件可见
	void disVisible() { m_isVisible = 0; }						//设置物件不可见
	char getVisible() const { return m_isVisible; }					//获取物件是否可见的状态 

private:
	GLfloat angle;			//精灵的角度
	float sinAngle;			//角度的正弦	//这个是为了优化计算速度而设定的变量
	float cosAngle;			//角度的余弦

	GLfloat m_positionX;	//精灵放置的位置
	GLfloat m_positionY;
	GLfloat xsize, ysize;	//精灵的缩放尺寸
	GLfloat alpha;			//透明度
	GLfloat colorRed;		//红色
	GLfloat colorGreen;		//绿色
	GLfloat colorBlue;		//蓝色
//	char m_isOverturn;		//是否x方向左右翻转
	//为了提升处理速度，下面定义一些中间变量
	int wr, hr, xp, yp;
	//	char *m_frameName;				//序列帧文件名
public:
	void setPosition(const XVec2& position);			//设置精灵的坐标
	void setPosition(float a, float b);			//设置精灵的坐标
	void setPositionX(float x);
	void setPositionY(float y);

	void setSize(const XVec2& size);		//设置精灵的缩放比例
	void setSize(float a, float b);	//设置精灵的缩放比例
	void setSize(float s);
	XVec2 getScale() const
	{
		return XVec2(xsize, ysize);
	}

	XTexture * getTexture();	//返回当前帧的贴图指针
	XTextureData * getTextureData();	//返回当前帧的贴图数据
#if IS_USE_SOLIDIFY
//++++++++++++++++++++++++++++++++++++++++++++
//下面为了提升速度而定义的变量
//“固化”及将显示信息固化到显示列表中，在显示过程中不再变化显示细节，如果遇到固定的没有细节变化的元件可以使用这个特性
	void setIsUseSolidify(int temp);
private:
	int m_glListOrder;		//显示列表编号
	int m_needSolidify;		//是否需要固化
	//--------------------------------------------
#endif
private:
	char m_isInited;
	char m_blendTypeScr;
	char m_blendTypeDst;
public:
	void setBlendType(int typeScr, int typeDst)
	{
		if (typeScr < 0) typeScr = 0;
		if (typeDst < 0) typeDst = 0;
		if (typeScr >= 9) typeScr = 8;
		if (typeDst >= 8) typeDst = 7;
		m_blendTypeScr = typeScr;
		m_blendTypeDst = typeDst;
	}
	//下面是对混合类型的一些强化
private:
	XBlendType m_blendType;	//混合方式：0：简单混合2因子混合、1：复杂的4因子混合
	char m_blendRGBScr;
	char m_blendRGBDst;
	char m_blendAScr;
	char m_blendADst;
public:
	void setBlendType(int rgbScr, int rgbDst, int aScr, int aDst)
	{
		if (rgbScr < 0) rgbScr = 0;
		if (rgbScr >= 9) rgbScr = 8;
		if (aScr < 0) aScr = 0;
		if (aScr >= 9) aScr = 8;

		if (rgbDst < 0) rgbDst = 0;
		if (rgbDst >= 8) rgbDst = 7;
		if (aDst < 0) aDst = 0;
		if (aDst >= 8) aDst = 7;
		m_blendRGBScr = rgbScr;
		m_blendRGBDst = rgbDst;
		m_blendAScr = aScr;
		m_blendADst = aDst;
	}
	void setBlendMode(XBlendType mode) { m_blendType = mode; }
	XShaderGLSL *m_pShader;
	void(*m_pShaderProc)(void);	//定义一个shader的函数指针
public:
	int init(const char *filename,						//图片的名称
		XResPos resPos = RES_SYS_DEF,					//资源的位置 0外部 1内部
		XTransformMode isTransformCenter = POINT_CENTER);	//是否使用中心旋转	

	int init(int tempW = 0, int tempH = 0, int needSizeCheck = 1, XTransformMode isTransformCenter = POINT_CENTER);	//是否为序列帧动画
	int init(XTextureData& texData, XTransformMode isTransformCenter = POINT_CENTER);
	int init(int w, int h, int colorMode, unsigned int tex, XTransformMode isTransformCenter = POINT_CENTER);

	int release();									//释放资源

	//注意：
	//外部调用draw系列函数的时候是不能传递参数的

	//最基础的绘图方式
	void drawBasic(const GLuint *pTexnum = NULL);					//最原始最完整的绘图方式，性能较低
	void drawBasic(const XTextureData *pTexData);// = NULL);					//最原始最完整的绘图方式，性能较低
	//下面这个函数经过群组化优化
	//注意这个函数当从外部传入图片时一定要确认不是拼合优化有的图片，否则会造成显示错误。
	void draw(const GLuint *pTexnum = NULL);			//从外部传入贴图来进行绘制，这个传入的贴图必须与原有的贴图具有相同的尺寸，否则会造成问题
	void draw(const XTextureData *pTexData);// = NULL);			//从外部传入贴图来进行绘制，这个传入的贴图必须与原有的贴图具有相同的尺寸，否则会造成问题
	//没有裁剪，没有反转，没有透明通道的时候才使用这个方法，否则会造成问题,这种方法速度较快，资源较省
	void drawEx(const GLuint *pTexnum = NULL);				//最高效的绘图方式，不需要裁剪等功能
	void drawEx(const XTextureData *pTexDatam);// = NULL);				//最高效的绘图方式，不需要裁剪等功能
	//不需要进行混合
	void drawWithoutBlend(const GLuint *pTexnum = NULL);
	void drawWithoutBlend(const XTextureData *pTexDatam);
private:
	void drawInside();	//内部调用的绘图函数
public:
	void setColor(float r, float g, float b, float a);		//小于0不会改变这一元素
	void setColor(const XFColor& color);		//小于0不会改变这一元素
	//复制精灵
private:
	int m_isACopy;	//判断是否为复制体，复制体没有自己的资源所以释放资源的时候不需要处理
	XSprite(const XSprite& temp);					//重定义赋值构造函数为私有，防止意外调用而造成错误

	//下面是关于角度裁剪的属性
	char m_needAngleClip;	//是否需要角度裁剪
	float m_clipAngle;		//角度裁剪的角度 0 - 360度
	XVec2 m_pointArray[14];	//在需要的时候刷新这些数据
	float m_uPoint[14];	//绘图数据
	float m_vPoint[14];
	int m_pointArraySize;
#ifdef GET_ALL_PIXEL
	float m_thisDrawArea;	//这次描绘的面积
#endif
	char m_needUpdatePointArray;	//是否需要更新内部阵列数据
	void updatePointArray();	//根据内部数据刷新
public:
	void setAngleClip(float angle)
	{
		m_needAngleClip = 1;
		m_clipAngle = angle;	//这个角度需要化简
		m_needUpdatePointArray = 1;
	}
	void disAngleClip()
	{
		m_needAngleClip = 0;
		m_needUpdatePointArray = 1;
	}
public:
	XSprite& operator = (const XSprite& temp);	//对目标的资源进行共用，不拷贝
	void setACopy(const XSprite& temp);			//对目标的资源进行共用，不拷贝

	void setOverturn(char temp);	//设置左右翻转(为了保持向旧版本的兼容保留这个接口)
	void setLeft2Right();	//设置左右翻转
	void setUp2Down();		//设置上下翻转
	void resetLeftRightUpDown();	//回复上下左右的正确位置
	XTurnOverMode getTurnOverMode() const
	{
		return m_turnOverMode;
	}

	XSprite();
	~XSprite();
};
#endif

struct XSpriteParamData
{//精灵物件的参数
	XResPos resourcePosition;	//读取资源的位置
	XVec2 changePoint;					//旋转动作的中心点位置
	char * getStrFromData()
	{
		char *tempStr = XMem::createArrayMem<char>(256);
		if (tempStr == NULL) return NULL;
		int offset = 0;
		sprintf_s(tempStr + offset, 256 - offset, "ResPos:%d,\n", resourcePosition);
		offset = strlen(tempStr);
		sprintf_s(tempStr + offset, 256 - offset, "ChaCnt:%f|%f,\n", changePoint.x, changePoint.y);
		offset = strlen(tempStr);
		return tempStr;
	}
	int getDataFromStr(const char * str)
	{
		if (str == NULL) return 0;
		int offset = 0;
		if (sscanf(str + offset, "ResPos:%d,\n", &resourcePosition) != 1) return offset;	//数据错误
		offset += XString::getCharPosition(str + offset, ',') + 1;
		if (sscanf(str + offset, "ChaCnt:%f|%f,\n", &changePoint.x, &changePoint.y) != 1) return offset;
		offset += XString::getCharPosition(str + offset, ',') + 1;
		return offset;
	}
	XBool getDataFromFile(FILE *fp)
	{
		if (fp == NULL) return XFalse;
		if (fscanf(fp, "ResPos:%d,\n", &resourcePosition) != 1) return XFalse;
		if (fscanf(fp, "ChaCnt:%f|%f,\n", &changePoint.x, &changePoint.y) != 2) return XFalse;
		return XTrue;
	}
	//默认数据
	XSpriteParamData()
		:resourcePosition(RES_SYS_DEF)
		, changePoint(0.5f, 0.5f)
	{}
};
#if WITH_INLINE_FILE
#include "XSprite.inl"
#endif
}
#endif