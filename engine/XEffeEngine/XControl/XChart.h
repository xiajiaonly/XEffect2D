#ifndef _JIA_XCHART_
#define _JIA_XCHART_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2014.5.19
//--------------------------------
//这是一个图标框的控件类，用于显示小规模和中等规模的数据
//功能基本完成，但是尚未经过优化和代码整理
//#include <vector>
#include <deque>
#include "XCheck.h"
namespace XE{
class XChart:public XControlBasic
{
private:
	static const int m_chartMaxLineSum = 12;	//允许的最大的线的数量
	static XFColor m_chartLineColor[m_chartMaxLineSum];
	bool m_isInited;	//是否初始化
	bool m_withPoint;
	char m_chooseArm;	//选择的目标 0：没有选择，1选择头，2选择尾巴
	bool m_neadUpdateAllLineData;	//是否需要更新所有的线数据
	bool m_neddUpdateCurLineData;	//是否需要更新当前的线数据

	XRect m_dataRect;		//数据显示的区域

	int m_horizontalNetSum;	//水平细分格的数量
	int m_verticalNetSum;	//垂直细分格的数量

	//std::vector<XVec2> m_datas[m_chartMaxLineSum];	//这里是所有的数据
	std::deque<XVec2> m_datas[m_chartMaxLineSum];
	int m_maxDataSum;	//单条线上允许显示的最大数据的数量
	//std::string m_dataName[m_chartMaxLineSum];	//线的说明文字
	XCheck m_dataChecks[m_chartMaxLineSum];
	int m_maxLineLen;	//点成员最多的点的数量
	//下面是用于数值描绘的基准数据
	XVec2 m_baseValue;	//原点的基准数值
	XVec2 m_sizeValue;	//盘面的大小

	XRect m_dataRange;	//数据的范围
	int m_dataSum;	//数据的数量
	XVec2 m_showDataRange;

	//下面是文字显示的东西
	XFontUnicode m_caption;
	XFontUnicode m_textFont;	//显示的数字

	//下面是用于绘图的临时数据
	XVec2 m_firstNetValue;	//描绘网格的第一个数据
	XVec2 m_stepWidth;		//描绘网格的步进
	int m_dataAccuracyX;		//数字的精度
	int m_dataAccuracyY;		//数字的精度

	void checkRange();	//检查取值范围是否超过现有数据的范围，如果超过则修正这个范围
	void updateRange();	//更新范围
	//下面是vbo的相关定义
	unsigned int m_v[m_chartMaxLineSum];	//总体图的顶点信息
	void allLineUpdateVbo();
	unsigned int m_curPoint[m_chartMaxLineSum];	//更新当前曲线上的点
	unsigned int m_curV[m_chartMaxLineSum];	//更新当前曲线的点
	void curLineUpdateVbo();
	int m_linePointSum[m_chartMaxLineSum];	//当前画图中需要描绘的点的数量
	int m_linePointsSum[m_chartMaxLineSum];	//当前画图中需要描绘的点的数量
	float *m_drawDataBuff;
	int m_drawDataBuffSize;

	static void chartChkCtrlFun(void *,int,int);
	bool checkCanHideLine();	//检查是否可以影藏曲线，必须要有至少一条曲线处于显示状态
	//几个约定数据
	float m_downHeight;			//下面坐标显示的高度
	float m_allLineRectHeight;	//下面显示所有线的区域的高度
public:
	bool initWithoutSkin(const XRect& rect,const char *caption,const XFontUnicode& font);
	bool initWithoutSkin(const XRect& rect,const char *caption) {return initWithoutSkin(rect,caption,getDefaultFont());}
	bool initWithoutSkin(const XVec2& pixelSize,const char *caption) 
	{
		return initWithoutSkin(XRect(XVec2::zero,pixelSize),caption,getDefaultFont());
	}
protected:
	void draw();
	void drawUp();
	void update(float stepTime);
	XBool mouseProc(const XVec2& p,XMouseState mouseState);		//对于鼠标动作的响应函数
	XBool keyboardProc(int,XKeyState) {return XFalse;}
	void insertChar(const char *,int){;}
	XBool canGetFocus(const XVec2& p);//是否可以获得焦点
	XBool canLostFocus(const XVec2& ) {return XTrue;}	//当前控件是否可以失去焦点，用于处理焦点抢夺问题
public:
	int getMaxDataSum()const{ return m_maxDataSum; }
	void setMaxDataSum(int sum);	//设置单条先允许插入的最大点的数量,<=0 为不受限制
	using XObjectBasic::setPosition;	//避免覆盖的问题
	void setPosition(const XVec2& p);

	using XObjectBasic::setScale;		//避免覆盖的问题
	void setScale(const XVec2& s);
	void insertData(const XVec2& data,int lineIndex);
	void clearData(int lineIndex);
	void clearAllData();
	void setLineName(const std::string& name,int lineIndex);

	XBool isInRect(const XVec2& p);
	XVec2 getBox(int order);
	using XObjectBasic::setColor;		//避免覆盖的问题
	void setColor(const XFColor& c);
	void setAlpha(float a);
	void release();
	void setDrawPointer(bool flag);	//设置是否描绘点
	bool getDrawPointer(){return m_withPoint;}
	void disable();
	void enable();
	XChart()
		:m_isInited(false)
		, m_baseValue(0.0f)
		, m_sizeValue(1.0f)
		//,m_isMouseDown(false)
		, m_chooseArm(0)
		, m_dataSum(0)
		, m_maxLineLen(0)
		, m_drawDataBuff(NULL)
		, m_withPoint(false)
		, m_neadUpdateAllLineData(false)
		, m_neddUpdateCurLineData(false)
		, m_maxDataSum(-1)
	{
		m_ctrlType = CTRL_OBJ_CHART;
	}
	~XChart(){release();}

	XBool readDataFromFile(const char * filename);	//从文件中读取曲线数据
	XBool saveDataToFile(const char * filename);	//从将曲线数据保存到文件中
};
#if WITH_INLINE_FILE
#include "XChart.inl"
#endif
}
#endif