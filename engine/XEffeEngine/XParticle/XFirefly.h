#ifndef _JIA_GFIREFLY_
#define _JIA_GFIREFLY_
#include "XMath\XMath.h"
#include "XMath\XVector2.h"
#include "XMath\XFColor.h"
#include "XTextureInformation.h"
#include <deque>

#define WITH_DRAW_OP	//是否使用描绘优化
namespace XE {
struct XObject
{
	XE::XVec2 m_pos;		//位置
	XE::XVec2 m_maxSpeed;	//最大的速度
	XE::XVec2 m_curSpeed;	//当前的速度

	float m_lightRecoveryRate;	//亮度的恢复速度
	float m_energy;	//能量
	float m_maxEnergy;
	float m_light;	//亮度
	float m_cycle;	//脉冲周期
	float m_cycleSpeed;	//脉冲速度
	XE::XFColor m_color;	//颜色
	float m_scale;	//缩放
	float m_maxLigth;	//自然生长的最大能量消耗

	bool m_isNear;	//是否是近的物体
	bool update(float steptime);
#ifdef WITH_DRAW_OP
	void draw();
#else
	void draw(unsigned int tex);
#endif
};
class XRandBG;
class XFirefly
{
private:
	bool m_isInited;

	std::vector<XObject> m_allObjs;
	std::deque<XObject*> m_freeObj;
	std::deque<XObject*> m_usedObj;
	XE::XTextureData m_objTxt;

	int m_w;	//粒子出现的范围
	int m_h;	//粒子出现的范围
	float m_bornRate;	//每秒钟生成粒子的数量
	float m_bornSum;
public:
	//下面是一些外置的公共参数
	float m_minSpeed;
	float m_maxSpeed;
	float m_minEnergy;
	float m_maxEnergy;
	float m_minCycleSpeed;
	float m_maxCycleSpeed;
	float m_minRecoveryRate;
	float m_maxRecoveryRate;
	float m_colorBase;
	float m_colorArea;
	float m_minLight;
	float m_maxLight;
	float m_nearRate;

	void resetParameter();
public:
	XFirefly()
		:m_isInited(false)
		, m_w(512)
		, m_h(512)
		, m_bornRate(2048.0f)
		, m_bornSum(0.0f)
	{}
	bool init(int maxObjSum, const char* texfilename, int w, int h, bool withUI = true);
	void update(float steptime, const XRandBG& bg);
	void draw();
	int getObjSum()const { return m_usedObj.size(); }
};
}
#endif