#ifndef _JIA_XPICRECT_
#define _JIA_XPICRECT_
#include "XPicRectCommon.h"
#include "XSprite.h"

namespace XE {
class XPicRect
{
private:
	bool m_isInited;

	XVec2 m_rectSize;		//矩形的尺寸
	XVec2 m_windowsPosition;	//矩形窗口的位置
	XVec2 m_texturePosition;	//矩形贴图的位置
	GLuint *m_texture;			//矩形的贴图
	XPicState m_state;				//矩阵的状态
	XSprite m_sprite;			//精灵
	float m_turnAngle;
	float m_turnSpeed;
	XSprite m_spriteShader;	//精灵的阴影
	GLuint *m_textureShader;	//精灵的阴影
	XVec2 m_shaderSize;		//精灵阴影的尺寸
	bool m_isIntTurn;		//是否在反转的过程中

	GLuint *m_textureSecond;	//换贴图时的第二个贴图
	bool m_isChangeTexture;

	//为了是得对鼠标的操作更加理想而定义的
	//XVec2 m_lastPoint;		//上一次鼠标的位置
public:
	unsigned int* getCurTex() { return m_texture; }
	void setCurTex(unsigned int* tex) { m_texture = tex; }
	void setChangeTex(unsigned int* tex)
	{
		m_isChangeTexture = true;
		m_textureSecond = tex;
	}
	void setCurState(XPicState s) { m_state = s; }
	XPicState getCurState()const { return m_state; }
	XVec2 getCenterPos()const { return m_windowsPosition + m_rectSize * 0.5f; }
	void setTurnSpeed(float s) { m_turnSpeed = s; }
	XPicRect()
		:m_texture(nullptr)
		, m_textureSecond(nullptr)
		, m_textureShader(nullptr)
		, m_isInited(false)
	{}
	bool init(const XVec2& rectSize,	//板子的大小
		const XVec2& windowsPosition,	//板子的位置
		const XVec2& texturePosition,	//贴图的位置
		GLuint *texture, const XVec2& textureSize,
		const XVec2& shaderSize, GLuint *textureShader);
	//withS:是否描绘阴影
	void draw(bool withS = true);
	void move(float timeDelay);
};
}
#endif