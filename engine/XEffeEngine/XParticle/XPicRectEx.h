#ifndef _JIA_XPICRECTEX_
#define _JIA_XPICRECTEX_
#include "XPicRectCommon.h"
#include "XSprite.h"
#include "XMath\XVector3.h"

namespace XE {
class XPicRectEx
{
private:
	bool m_isInited;

	XVec2 m_texturePosition;	//矩形贴图的位置
	GLuint *m_texture;			//矩形的贴图
	XPicState m_state;				//矩阵的状态
	float m_turnAngle;
	float m_turnSpeed;
	GLuint *m_textureShader;	//精灵的阴影
	bool m_isIntTurn;			//是否在反转的过程中

	GLuint *m_textureSecond;	//换贴图时的第二个贴图
	//bool m_isChangeTexture;

	XVec2 m_position;
	XVec2 m_size;
	std::vector<XVec3> m_v;
	std::vector<XVec2> m_u;
	std::vector<XVec2> m_su;

	void setCurAngle(float angle);	//通过角度计算各个点的位置
public:
	bool init(const XVec2& rectSize,	//板子的大小
		const XVec2& windowsPosition,	//板子的位置
		const XVec2& texturePosition,	//贴图的位置
		GLuint *texture, const XVec2& textureSize,
		GLuint *textureShader);

	XPicRectEx()
		:m_isInited(false)
		, m_texture(nullptr)
		, m_textureSecond(nullptr)
		, m_textureShader(nullptr)
		, m_isIntTurn(false)
	{}

	unsigned int* getCurTex() { return m_texture; }
	void setCurTex(unsigned int* tex) { m_texture = tex; }
	void setChangeTex(unsigned int* tex) { m_textureSecond = tex;}
	void setCurState(XPicState s) { m_state = s; }
	XPicState getCurState()const { return m_state; }
	XVec2 getCenterPos()const { return m_position; }
	void setTurnSpeed(float s) { m_turnSpeed = s; }
	//withS:是否描绘阴影
	void draw(bool withS = true);
	void move(float timeDelay);
};
}
#endif