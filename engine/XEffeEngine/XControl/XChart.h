#ifndef _JIA_XCHART_
#define _JIA_XCHART_

//这是一个图标框的控件类，用于显示小规模和中等规模的数据
//功能基本完成，但是尚未经过优化和代码整理
#include <vector>
#include "XCheck.h"

#define CHART_MAX_LINE_SUM (12)	//允许的最大的线的数量

class _XChart:public _XControlBasic
{
private:
	bool m_isInited;	//是否初始化
	bool m_withPoint;

	_XRect m_dataRect;		//数据显示的区域

	int m_horizontalNetSum;	//水平细分格的数量
	int m_verticalNetSum;	//垂直细分格的数量

	std::vector<_XVector2> m_datas[CHART_MAX_LINE_SUM];	//这里是所有的数据
	//std::string m_dataName[CHART_MAX_LINE_SUM];	//线的说明文字
	_XCheck m_dataChecks[CHART_MAX_LINE_SUM];
	int m_maxLineLen;	//点成员最多的点的数量
	//下面是用于数值描绘的基准数据
	_XVector2 m_baseValue;	//原点的基准数值
	_XVector2 m_sizeValue;	//盘面的大小

	char m_chooseArm;	//选择的目标 0：没有选择，1选择头，2选择尾巴
	_XRect m_dataRange;	//数据的范围
	int m_dataSum;	//数据的数量
	_XVector2 m_showDataRange;

	//下面是文字显示的东西
	_XFontUnicode m_caption;
	_XFontUnicode m_textFont;	//显示的数字

	//下面是用于绘图的临时数据
	_XVector2 m_firstNetValue;	//描绘网格的第一个数据
	_XVector2 m_stepWidth;		//描绘网格的步进
	int m_dataAccuracyX;		//数字的精度
	int m_dataAccuracyY;		//数字的精度

	void checkRange();	//检查取值范围是否超过现有数据的范围，如果超过则修正这个范围
	void updateRange();	//更新范围
	//下面是vbo的相关定义
	unsigned int m_v[CHART_MAX_LINE_SUM];	//总体图的顶点信息
	void allLineUpdateVbo();
	unsigned int m_nowPoint[CHART_MAX_LINE_SUM];	//更新当前曲线上的点
	unsigned int m_nowV[CHART_MAX_LINE_SUM];	//更新当前曲线的点
	void nowLineUpdateVbo();
	int m_linePointSum[CHART_MAX_LINE_SUM];	//当前画图中需要描绘的点的数量
	int m_linePointsSum[CHART_MAX_LINE_SUM];	//当前画图中需要描绘的点的数量
	float *m_drawDataBuff;
	int m_drawDataBuffSize;
	bool m_neadUpdateAllLineData;	//是否需要更新所有的线数据
	bool m_neddUpdateNowLineData;	//是否需要更新当前的线数据

	friend void chartChkCtrlFun(void *pClass,int objectID);
	bool checkCanHideLine();	//检查是否可以影藏曲线，必须要有至少一条曲线处于显示状态
	//几个约定数据
	float m_downHeight;			//下面坐标显示的高度
	float m_allLineRectHeight;	//下面显示所有线的区域的高度
public:
	bool initWithoutTex(const _XRect &rect,const char *caption,const _XFontUnicode &font);
	bool initWithoutTex(const _XRect &rect,const char *caption) {return initWithoutTex(rect,caption,XEE::systemFont);}
	bool initWithoutTex(const _XVector2 &pixelSize,const char *caption) 
	{
		return initWithoutTex(_XRect(0.0f,0.0f,pixelSize.x,pixelSize.y),caption,XEE::systemFont);
	}
protected:
	void draw();
	void drawUp();
	void update(int stepTime);
	_XBool mouseProc(float x,float y,_XMouseState mouseState);		//对于鼠标动作的响应函数
	_XBool keyboardProc(int,_XKeyState) {return XFalse;}
	void insertChar(const char *,int){;}
	_XBool canGetFocus(float x,float y);//是否可以获得焦点
	_XBool canLostFocus(float,float) {return XTrue;}	//当前控件是否可以失去焦点，用于处理焦点抢夺问题
public:
	using _XObjectBasic::setPosition;	//避免覆盖的问题
	void setPosition(float x,float y);

	using _XObjectBasic::setSize;		//避免覆盖的问题
	void setSize(float x,float y);
	void insertData(const _XVector2 &data,int lineIndex);
	void setLineName(const std::string &name,int lineIndex);

	_XBool isInRect(float x,float y);
	_XVector2 getBox(int order);
	using _XObjectBasic::setColor;		//避免覆盖的问题
	void setColor(float r,float g,float b,float a);
	void setAlpha(float a);
	void release();
	void setDrawPointer(bool flag);	//设置是否描绘点
	bool getDrawPointer(){return m_withPoint;}
	void disable();
	void enable();
	_XChart()
		:m_isInited(false)
		,m_baseValue(0.0f,0.0f)
		,m_sizeValue(1.0f,1.0f)
		//,m_isMouseDown(false)
		,m_chooseArm(0)
		,m_dataSum(0)
		,m_maxLineLen(0)
		,m_drawDataBuff(NULL)
		,m_withPoint(false)
		,m_neadUpdateAllLineData(false)
		,m_neddUpdateNowLineData(false)
	{
		m_ctrlType = CTRL_OBJ_XCHART;
	}
	~_XChart(){release();}

	_XBool readDataFromFile(const char * filename);	//从文件中读取曲线数据
	_XBool saveDataToFile(const char * filename);	//从将曲线数据保存到文件中
};
#include "XChart.inl"
#endif