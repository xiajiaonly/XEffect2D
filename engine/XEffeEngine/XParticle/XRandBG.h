#ifndef _JIA_GRANDBG_
#define _JIA_GRANDBG_
#include "XMemory.h"
#include "XShaderGLSL.h"
#include "XMath\XMath.h"
#include "XTextureInformation.h"
#include <deque>

#define WITH_DRAW_OP	//是否使用描绘优化
namespace XE {
struct XBGBaseObjInfo
{
	XBGBaseObjInfo()
		:m_recoveryRate(200.0f)
	{}
	XE::XVec2 m_maxSpeed;	//最大的速度
	XE::XVec2 m_curSpeed;	//当前的速度

	XE::XVec2 m_pos;		//当前的位置
	float m_scale;			//缩放比例
	float m_maxEnergy;		//总的能量数量
	float m_releaseEnergy;	//剩余的能量数量
	float m_curEnergy;		//当前的能量消耗
	float m_maxStrength;	//最大的能量数量
	float m_alpha;
	float m_life;			//以秒计算的生命
	float m_recoveryRate;	//生命恢复的速度
	//返回是否死亡，steptime：步进时间，单位为秒
	bool update(float steptime);
};

class XRandBG
{
private:
	bool m_isInited;
	XE::XBuffer m_buff;
	XE::XFBO m_fbo;
	int m_w;
	int m_h;
	std::vector<XBGBaseObjInfo> m_objs;
	std::deque<XBGBaseObjInfo*> m_freeObjs;
	std::deque<XBGBaseObjInfo*> m_usedObjs;
	XE::XTextureData m_tex;
	float m_avgBornTimer;
	float m_bornTime;

	void born();
public:
	//下面是一些特性变量的范围
	float m_minSpeed;
	float m_maxSpeed;
	float m_minScale;
	float m_maxScale;
	float m_minEnergy;
	float m_maxEnergy;
	float m_minStrength;
	float m_maxStrength;
	float m_minRecoveryRate;
	float m_maxRecoveryRate;
	void resetParameter();
public:
	XRandBG()
		:m_isInited(false)
	{}
	bool init(int w, int h, const char* filename, int objsSum = 12,
		bool m_withUI = false);
	void update(float steptime);
	void draw();
	unsigned int getPixel(int w, int h)const
	{
		if (w < 0 || w >= m_w || h < 0 || h >= m_h) return 0;
		return m_buff.getBuffer()[w + h * m_w];
	}
	unsigned int getPixel(const XE::XVec2& pos)const
	{
		return getPixel(pos.x * m_w, pos.y * m_h);
	}
	int getW()const { return m_w; }
	int getH()const { return m_h; }
};
}
#endif