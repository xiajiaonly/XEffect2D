#ifndef _JIA_XSIMPLECHART_
#define _JIA_XSIMPLECHART_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2014.4.2
//--------------------------------
//#include "XMath\XVector2.h"
//#include "XFont\XFontUnicode.h"
#include <deque>
#include "XControlBasic.h"
#include "XResourceManager.h"
//这是一个简单的数据表的类
//优化：
//1、统计当前区域的最大值和最小值
//2、统计当前区域的标准差
//3、统计当前区域的平均值
namespace XE{
enum XSimpleChartType
{
	TYPE_0_UP,		//0线在上面的模式
	TYPE_0_MIDDLE,	//0线在中间的模式
	TYPE_0_BOTTOM,	//0线在下面的模式
};
enum XSimpleChartShowInfo	//文字显示的信息
{
	INFO_NULL,	//没有显示信息
	INFO_CUR_VALUE,	//当前的数值
	INFO_ALL_RANGLE,	//整个的范围
	INFO_LOCAL_RANGLE,	//当前的范围
	INFO_LOCAL_AVERAGE,	//当前的平均值
	INFO_LOCAL_SD,		//标准差,standard deviation
};
class XSimpleChart:public XControlBasic
{
private:
	bool m_isInited;
	XVec2 m_size;		//大小
	float m_rateY;			//数据目前的缩放系数
	float m_rateX;			//数据之间的距离，单位为像素
	XSimpleChartType m_simpleChartType;	//曲线的样式，这个数据尚未生效
	XSimpleChartShowInfo m_showInfo;

	std::deque<float> m_datas;
	int m_maxDataSum;	//最多可以显示的data数量
	//特性的描述
	bool m_isAutoRateY;		//Y方向上的缩放是否使用自适应,true时最大值变化是缩放系数会跟随变化，false时，是有放大时会跟随变化
	XFontUnicode m_caption;	//标题
	bool m_showCaption;
	//下面是范围统计的数据
	bool m_isFirstPush;	//是否是第一次放入数据
	float m_maxData;	//推入数据的最大值
	float m_minData;	//推入数据的最小值
	float m_curData;	//当前的数据
	float m_localMaxData;
	float m_localMinData;
	float m_localAverageData;	//当前的平均值
	float m_localSDData;	//当前的标准差

	XFontUnicode m_infoFont;
	XFColor m_textColor;		//文字的颜色
public:
	XSimpleChartType getSimpleChartType()const{return m_simpleChartType;}
	void setSimpleChartType(XSimpleChartType type){m_simpleChartType = type;}
	XSimpleChartShowInfo getShowInfo()const{return m_showInfo;}
	void setShowInfo(XSimpleChartShowInfo info){m_showInfo = info;}
	XSimpleChart()
		:m_isInited(false)
		,m_maxDataSum(0)
		,m_rateY(1.0f)
		,m_rateX(1.0f)
//		,m_scale(1.0f)
		,m_size(128.0f,512.0f)
		,m_isAutoRateY(true)
		,m_showCaption(false)
		,m_isFirstPush(false)
		,m_maxData(0.0f)
		,m_minData(0.0f)
		,m_curData(0.0f)
		,m_localMaxData(0.0f)
		,m_localMinData(0.0f)
		,m_localAverageData(0.0f)
		,m_localSDData(0.0f)
		,m_simpleChartType(TYPE_0_MIDDLE)
		,m_showInfo(INFO_NULL)
	{
		m_ctrlType = CTRL_OBJ_SIMPLECHART;
	}
	~XSimpleChart(){release();}
	void release();

	void setIsAutoRateY(bool flag){m_isAutoRateY = flag;}
	bool getIsAutoRateY()const{return m_isAutoRateY;}
	XBool init(const XFontUnicode& font,const XVec2& size,float xRate);
	XBool init(const XVec2& size,float xRate)
	{
		return init(getDefaultFont(),size,xRate);
	}
	void setCaption(const char *p);
	XBool isInRect(const XVec2& p);		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
	XVec2 getBox(int order);			//获取四个顶点的坐标，目前先不考虑旋转和缩放
protected:						
	void draw();
	void drawUp(){}
	XBool mouseProc(const XVec2& p,XMouseState mouseState);		//对于鼠标动作的响应函数
	XBool keyboardProc(int keyOrder,XKeyState keyState){return XFalse;}			//返回是否触发按键动作
	void insertChar(const char *,int){;}
	XBool canGetFocus(const XVec2& p);				//用于判断当前物件是否可以获得焦点
	XBool canLostFocus(const XVec2& ){return XTrue;}	//应该是可以随时失去焦点的
//	void setLostFocus();	//设置失去焦点
public:
	void update(float stepTime){}
	bool insertAData(float data);
	void clearAll();
	XBool setACopy(const XButton &temp){return XFalse;}
private://下面为了防止错误，重载赋值操作符，复制构造函数
	XSimpleChart(const XSimpleChart &temp);
	XSimpleChart& operator = (const XSimpleChart& temp);
private:
	void updateInfoStr();
public:
	using XObjectBasic::setPosition;	//避免覆盖的问题
	void setPosition(const XVec2& p);
	using XObjectBasic::setScale;	//避免覆盖的问题
	void setScale(const XVec2& s);
	using XObjectBasic::setColor;	//避免覆盖的问题
	void setColor(const XFColor& c);
	void setAlpha(float a);
};
#if WITH_INLINE_FILE
#include "XSimpleChart.inl"
#endif
}
#endif