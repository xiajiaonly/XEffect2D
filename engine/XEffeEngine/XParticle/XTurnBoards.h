#ifndef _JIA_XTURNBOARDS_
#define _JIA_XTURNBOARDS_
#define TURN_METHOD 1
#if TURN_METHOD == 0
#include "XPicRect.h"
#endif
#if TURN_METHOD == 1
#include "XPicRectEx.h"
#endif
#define WITH_ALPHA

#include "XShaderGLSL.h"
//优化说明：
//+1、当间隙为0是可以变换方块大小
//+2、速度可以通过MD来动态控制
//3、图案散开成粒子
//4、粒子自合成文字
//+5、增加更多的花样翻转方式：水平单方向两种，垂直单方向两种
//6、加入position和scale的属性以及rotation
namespace XE {
enum XTurnBoardDir
{//反转图片的方向
	DIR_UP,
	DIR_DOWN,
	DIR_LEFT,
	DIR_RIGHT,
	DIR_MAX,
};
enum XTurnBoardMode
{
	MODE_DIRECT,
	MODE_UP,
	MODE_DOWN,
	MODE_LEFT,
	MODE_RIGHT,
	MODE_POINT,
	MODE_MANUAL,	//手动模式
};
class XTurnBoards:public XObjectBasic
{
private:
	bool m_isInited;
	int m_texWidth;
	int m_texHeight;
	XVec2 m_boardSize;	//方块的大小

	int m_sumX;
	int m_sumY;

	XSprite m_sprite;

#if TURN_METHOD == 0
	XPicRect* m_boards;
	XPicRect* m_boardsSmall;		//小的
	XPicRect* m_boardsWidth;		//宽的
	XPicRect* m_boardsHeight;		//高的
#endif
#if TURN_METHOD == 1
	XPicRectEx* m_boards;			//指针
	XPicRectEx* m_boardsSmall;		//小的
	XPicRectEx* m_boardsWidth;		//宽的
	XPicRectEx* m_boardsHeight;		//高的
#endif
	std::vector<bool>* m_boardsFlag;
	std::vector<bool> m_boardsSmallFlag;
	std::vector<bool> m_boardsWidthFlag;
	std::vector<bool> m_boardsHeightFlag;
	XPicState m_setState;

	XTextureData m_shaderTex[4];	//阴影的图
	std::vector<unsigned int> m_shaderTexs;

	bool m_isInChangeImg;
	std::vector<XMoveData> m_changeRate;
	std::vector<XVec2> m_changeCenter;
	XTurnBoardMode m_turnMode;
	float m_changeTime;	//变化的时长
	XFBO m_fbo;
#ifdef WITH_ALPHA
	XShaderGLSL m_shader;
	unsigned int m_tex[2];
#endif
	bool initBoards(unsigned int *initTex,int distance);
public:
	float getChangeTime()const { return m_changeTime; }
	void setChangeTime(float s) { m_changeTime = s; }
	//dist:方块与方块之间的距离
	bool init(int texW, int texH, const XVec2& boardSize,unsigned int *initTex,int dist = 0);
	void update(float steptime);
	void draw();
	void setBlendModel(XGL::XBlendModel model) { m_sprite.setBlendModel(model); }
	XTurnBoards()
		:m_isInited(false)
		, m_boards(nullptr)
		, m_isInChangeImg(false)
		, m_changeTime(2.0f)
		, m_boardsSmall(nullptr)
		, m_boardsWidth(nullptr)
		, m_boardsHeight(nullptr)
		, m_boardsFlag(nullptr)
		, m_setState(PIC_STA_NULL)
	{
		m_objType = OBJ_TURNBOARD;
	}
	~XTurnBoards() { release(); }
	void release();
	void turnBoard(const XVec2& pos, XTurnBoardDir dir);

	bool getCanChangeImg()const;
	bool changeImage(unsigned int *targetTex, XTurnBoardMode mode,
		const XVec2& center);
	bool addACenter(const XVec2& c);
public:
	virtual XBool isInRect(const XVec2& p) { return m_sprite.isInRect(p); }
	virtual XVec2 getBox(int order) { return m_sprite.getBox(order); }	
	virtual void setPosition(const XVec2& pos) { m_sprite.setPosition(pos); }
	virtual const XVec2& getPosition() const { return m_sprite.getPosition(); }	//获取四个顶点的坐标，目前先不考虑旋转和缩放

	virtual void setScale(const XVec2& scale) { m_sprite.setScale(scale); }
	virtual const XVec2& getScale() const { return m_sprite.getScale(); }					//获取物件的尺寸
	virtual void setAngle(float angle) { m_sprite.setAngle(angle); }				//设置物件的角度	+Child处理
	virtual float getAngle() const { return m_sprite.getAngle(); }						//获取物件的角度

	virtual void setVisible() { m_sprite.setVisible(); }					//设置物件可见		+Child处理
	virtual void disVisible() { m_sprite.disVisible(); }					//设置物件不可见	+Child处理
	virtual XBool getVisible() const { return m_sprite.getVisible(); }					//获取物件是否可见的状态 
	
	virtual void setColor(const XFColor& color) { m_sprite.setColor(color); }
	virtual const XFColor& getColor() const { return m_sprite.getColor(); }
	virtual void setAlpha(float a) { m_sprite.setAlpha(a); }							//+Child处理

	void setIsTransformCenter(XTransformMode tmp) { m_sprite.setIsTransformCenter(tmp); }
};
}
#endif