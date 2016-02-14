#include "XStdHead.h"
#include "XChart.h"
#include "XObjectManager.h" 
#include "XControlManager.h"
//1、考虑线的唯一性，可以选择是否显示某一条线，但是必须至少要有一条线显示出来
//2、线的线段越界是如何差值保证线的完整且不会画出去
//3、坐标系起始坐标和如何显示节点数据的问题
//4、极端值的处理，比如说线上只有一个点
namespace XE{
void XChart::chartChkCtrlFun(void *pClass,int objectID,int eventID)
{
	if(eventID != XCheck::CHK_STATE_CHANGE) return;
	XChart &pPar = *(XChart *)pClass;
	for(int i = 0;i < m_chartMaxLineSum;++ i)
	{
		if(objectID == pPar.m_dataChecks[i].getControlID() && !pPar.m_dataChecks[i].getState())
		{
			if(!pPar.checkCanHideLine())
			{//修改为不能改变
				pPar.m_dataChecks[i].setState(XTrue);
				return;
			}
			break;
		}
	}
	pPar.m_neddUpdateCurLineData = true;
}
bool XChart::checkCanHideLine()
{
	int sum = 0;
	for(int i = 0;i < m_chartMaxLineSum;++ i)
	{
		if(m_dataChecks[i].getState() && m_datas[i].size() > 1) ++ sum;
		if(sum > 0) return true;
	}
	return false;
}
XFColor XChart::m_chartLineColor[m_chartMaxLineSum];
bool XChart::initWithoutSkin(const XRect &rect,const char *caption,const XFontUnicode &font)
{
	if(m_isInited) return false;

	m_chartLineColor[0] = XFColor::red;
	m_chartLineColor[1] = XFColor::green;
	m_chartLineColor[2] = XFColor::blue;
	m_chartLineColor[3] = XFColor::yellow;
	m_chartLineColor[4] = XFColor::brown;
	m_chartLineColor[5] = XFColor::burlyWood;
	m_chartLineColor[6] = XFColor::royalBlue;
	m_chartLineColor[7] = XFColor::greenYellow;
	m_chartLineColor[8] = XFColor::hotPink;
	m_chartLineColor[9] = XFColor::aqua;
	m_chartLineColor[10] = XFColor::cornflowerBlue;
	m_chartLineColor[11] = XFColor::aquamarine;

	m_dataRect = rect;
	m_position.set(0.0f,0.0f);
	m_scale.set(1.0f,1.0f);

	m_horizontalNetSum = 10;
	m_verticalNetSum = 10;

	m_showDataRange.set(0.0f,0.5f);	//这个是主动设置的数据，下面两个数据是通过具体的数据计算出来的
	m_baseValue.set(0.0f,0.0f);	//原点的基准数值
	m_sizeValue.set(0.5f,2.0);	//盘面的大小

	if(!m_caption.setACopy(font)) return XFalse;
	m_caption.setString(caption);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_caption);
#endif
	if(!m_textFont.setACopy(font)) return XFalse;
	m_textFont.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT);
	m_textFont.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);
	m_textFont.setScale(0.5f,0.5f);
	m_textFont.setColor(0.5f,0.5f,0.5f,1.0f);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_textFont);
#endif

	for(int i = 0;i < m_chartMaxLineSum;++ i)
	{
		m_dataChecks[i].initWithoutSkin(" ",m_caption,0.5f,XRect(0.0f,0.0f,16.0f,16.0f),XVector2(16.0f,8.0f));
	//	m_dataChecks[i].setScale(0.5f);
		m_dataChecks[i].setPosition(m_position.x + m_dataRect.right + 16.0f,m_position.y + m_dataRect.top + 17.0f * i);
		m_dataChecks[i].setState(XTrue);
		m_dataChecks[i].setEventProc(chartChkCtrlFun,this);
		m_dataChecks[i].setTextColor(XFColor(0.5f,0.5f,0.5f,1.0f));
		m_dataChecks[i].disVisible();
		m_dataChecks[i].setWithAction(XFalse);
		XCtrlManager.decreaseAObject(&m_dataChecks[i]);	//注销这个物件
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_dataChecks[i]);
#endif
		glGenBuffersARB(1,&m_v[i]);
	//	glBindBufferARB(GL_ARRAY_BUFFER_ARB,m_v[i]);
		glGenBuffersARB(1,&m_curV[i]);
	//	glBindBufferARB(GL_ARRAY_BUFFER_ARB,m_curV[i]);
		glGenBuffersARB(1,&m_curPoint[i]);
	//	glBindBufferARB(GL_ARRAY_BUFFER_ARB,m_curPoint[i]);
		m_linePointSum[i] = 0;
		m_linePointsSum[i] = 0;
	}
	//glBindBufferARB(GL_ARRAY_BUFFER_ARB,0);	//取消绑定
	m_downHeight = 20.0f;			//下面坐标显示的高度
	m_allLineRectHeight = 100.0f;	//下面显示所有线的区域的高度

	m_isVisible = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;
	m_isInited = XTrue;

	XCtrlManager.addACtrl(this);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(this);
#endif
	return true;
}
void XChart::draw()
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isVisible) return;	//如果不可见直接退出
	checkRange();	//更新数据保证数据的合法性
	for(int i = 0;i < m_chartMaxLineSum;++ i)
	{
		m_dataChecks[i].draw();
	}
	if(m_neddUpdateCurLineData)
	{
		updateRange();
		m_neddUpdateCurLineData = false;
	}
	if(m_neadUpdateAllLineData)
	{
		allLineUpdateVbo();
		m_neadUpdateAllLineData = false;
	}
	char tmpStr[64];
	m_caption.draw();
	//画个背景
	XRect tmpDrawRect(m_position.x + m_dataRect.left * m_scale.x,m_position.y + m_dataRect.top * m_scale.y,
		m_position.x + m_dataRect.right * m_scale.x,m_position.y + m_dataRect.bottom * m_scale.y);	//描绘数据的区域
	XRender::drawFillBoxExA(XVector2(tmpDrawRect.left,tmpDrawRect.top),
		XVector2(tmpDrawRect.getWidth(),tmpDrawRect.getHeight()),XCCS::lightBGColor * m_color);
	//描绘标尺
		//垂直
	char printMode[] = "%.4f";
	printMode[2] = abs(m_dataAccuracyY - 1) % 10 + '0';
	m_textFont.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT);
	m_textFont.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_MIDDLE);
	float tmpDataValue;
	float tmpX;
	int ex = 0;
	tmpDataValue = m_firstNetValue.y;
	float pixelPerValueY = tmpDrawRect.getHeight() / m_sizeValue.y;	//每个像素对应多少值
	float pixelPerValueX = tmpDrawRect.getWidth() / m_sizeValue.x;	//每个像素对应多少值
	for(int i = 0;i < m_verticalNetSum ;++ i)
	{
		tmpX = (tmpDataValue - m_baseValue.y) * pixelPerValueY;
		XRender::drawLine(tmpDrawRect.left,tmpDrawRect.bottom - tmpX,
			tmpDrawRect.left - 5.0f * m_scale.x,tmpDrawRect.bottom - tmpX,1.0f,XCCS::blackColor * m_color);
		if(XMath::getApproximateData(tmpDataValue,1,ex) == 0.0f || ex < m_dataAccuracyY)
		{//特殊的线,零线
			XRender::drawLine(tmpDrawRect.left,tmpDrawRect.bottom - tmpX,
				tmpDrawRect.right, tmpDrawRect.bottom - tmpX, 1.0f, XCCS::blackDownColor * m_color);
		}else
		{
			XRender::drawLine(tmpDrawRect.left,tmpDrawRect.bottom - tmpX,
				tmpDrawRect.right, tmpDrawRect.bottom - tmpX, 1.0f, XCCS::lightSpecialColor * m_color, XRender::LS_DOTS);
		}

		sprintf(tmpStr,printMode,tmpDataValue);
		m_textFont.setString(tmpStr);
		m_textFont.setPosition(tmpDrawRect.left - m_textFont.getMaxPixelWidth(),tmpDrawRect.bottom - tmpX);
		m_textFont.draw();
		tmpDataValue += m_stepWidth.y;
	}
		//水平
	printMode[2] = abs(m_dataAccuracyX - 1) % 10 + '0';
	m_textFont.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_MIDDLE);
	m_textFont.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);
	tmpDataValue = m_firstNetValue.x;
	for(int i = 0;i < m_horizontalNetSum;++ i)
	{
		tmpX = (tmpDataValue - m_baseValue.x) * pixelPerValueX;
		XRender::drawLine(tmpDrawRect.left + tmpX,tmpDrawRect.bottom,
			tmpDrawRect.left + tmpX,tmpDrawRect.bottom + 5.0f * m_scale.y,1.0f,XCCS::blackColor * m_color);

		if(XMath::getApproximateData(tmpDataValue,1,ex) == 0.0f || ex < m_dataAccuracyX)
		{
			XRender::drawLine(tmpDrawRect.left + tmpX,tmpDrawRect.top,
				tmpDrawRect.left + tmpX,tmpDrawRect.bottom,1.0f,XCCS::specialColor * m_color);
		}else
		{
			XRender::drawLine(tmpDrawRect.left + tmpX,tmpDrawRect.top,
				tmpDrawRect.left + tmpX,tmpDrawRect.bottom,1.0f,XCCS::specialColor * m_color,XRender::LS_DOTS);
		}

		m_textFont.setPosition(tmpDrawRect.left + tmpX,tmpDrawRect.bottom);
		sprintf(tmpStr,printMode,tmpDataValue);
		m_textFont.setString(tmpStr);
		m_textFont.draw();
		tmpDataValue += m_stepWidth.x;
	}
	//描绘鼠标坐标
	if(tmpDrawRect.isInRect(getMousePos()))
	{//如果鼠标在范围内
		m_textFont.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT);
		m_textFont.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);
		XRender::drawLine(tmpDrawRect.left,getMousePos().y,
			tmpDrawRect.right, getMousePos().y, 1.0f, XCCS::mouseColor * m_color, XRender::LS_DASHES);
		XRender::drawLine(getMousePos().x,tmpDrawRect.top,
			getMousePos().x, tmpDrawRect.bottom, 1.0f, XCCS::mouseColor * m_color, XRender::LS_DASHES);
		sprintf(tmpStr,"%f",(tmpDrawRect.bottom - getMousePos().y) / pixelPerValueY + m_baseValue.y);
		m_textFont.setString(tmpStr);
		if(getMousePos().y < tmpDrawRect.getCenter().y) m_textFont.setPosition(tmpDrawRect.left,getMousePos().y);
		else m_textFont.setPosition(tmpDrawRect.left,getMousePos().y - m_textFont.getMaxPixelHeight());
		m_textFont.draw();

		sprintf(tmpStr,"%f",(getMousePos().x - tmpDrawRect.left) / pixelPerValueX + m_baseValue.x);
		m_textFont.setString(tmpStr);
		if(getMousePos().x < tmpDrawRect.getCenter().x) m_textFont.setPosition(getMousePos().x,tmpDrawRect.bottom - 16.0f * m_scale.x);
		else m_textFont.setPosition(getMousePos().x - m_textFont.getMaxPixelWidth(),tmpDrawRect.bottom - 16.0f * m_scale.x);
		m_textFont.draw();
	}
	//描绘曲线
	for(int i = 0;i < m_chartMaxLineSum;++ i)
	{
		if(m_datas[i].size() <= 1) continue;
		if(m_dataChecks[i].getState())
		{
			if(m_linePointSum[i] > 0) XRender::drawLinesVbo(m_curV[i],m_linePointSum[i],1.0f,m_chartLineColor[i].fR * m_color.fR,m_chartLineColor[i].fG * m_color.fG,m_chartLineColor[i].fB * m_color.fB,m_color.fA);
			if(m_withPoint && m_linePointsSum[i] > 0) XRender::drawPointsVbo(m_curPoint[i],m_linePointsSum[i],2,m_chartLineColor[i].fR * m_color.fR,m_chartLineColor[i].fG * m_color.fG,m_chartLineColor[i].fB * m_color.fB,m_color.fA);
		}
		//m_dataChecks[i].draw();
		XRender::drawFillBox(tmpDrawRect.right + 1.0f * m_scale.x,tmpDrawRect.top + (1.0f + 17.0f * i) * m_scale.y,
			14.0f * m_scale.x,14.0f * m_scale.y,m_chartLineColor[i].fR * m_color.fR,m_chartLineColor[i].fG * m_color.fG,m_chartLineColor[i].fB * m_color.fB,m_color.fA);
	}
	//描绘坐标系
	XRender::drawLine(tmpDrawRect.left,tmpDrawRect.top,
		tmpDrawRect.left,tmpDrawRect.bottom,1.0f,0.0f,0.0f,0.0f,m_color.fA);
	XRender::drawLine(tmpDrawRect.left,tmpDrawRect.bottom,
		tmpDrawRect.right,tmpDrawRect.bottom,1.0f,0.0f,0.0f,0.0f,m_color.fA);
	//下面描绘总体的曲线
	XRender::drawFillBoxExA(XVector2(tmpDrawRect.left,tmpDrawRect.bottom + m_downHeight * m_scale.y),
		XVector2(tmpDrawRect.getWidth(),m_allLineRectHeight * m_scale.y),XCCS::lightBGColor * m_color);
	//描绘起点线和终点线
	if(m_dataRange.getWidth() > 0.0f)
	{
		float sX = (m_baseValue.x - m_dataRange.left) / m_dataRange.getWidth() * tmpDrawRect.getWidth() + tmpDrawRect.left;
		XRender::drawFillBoxExA(XVector2(tmpDrawRect.left,tmpDrawRect.bottom + m_downHeight * m_scale.y),
			XVector2(sX - tmpDrawRect.left, m_allLineRectHeight * m_scale.y), XCCS::blackOnColor * m_color);//这里需要凸显选中的范围
		XRender::drawLine(sX,tmpDrawRect.bottom + m_downHeight * m_scale.y,
			sX,tmpDrawRect.bottom + (m_downHeight + m_allLineRectHeight) * m_scale.y,1.0f,XCCS::mouseColor * m_color);

		float eX = (m_baseValue.x + m_sizeValue.x - m_dataRange.left) / m_dataRange.getWidth() * tmpDrawRect.getWidth() + tmpDrawRect.left;
		XRender::drawFillBoxExA(XVector2(eX,tmpDrawRect.bottom + m_downHeight * m_scale.y),
			XVector2(tmpDrawRect.getWidth() - (eX - tmpDrawRect.left), m_allLineRectHeight * m_scale.y), XCCS::blackOnColor * m_color);
		XRender::drawLine(eX,tmpDrawRect.bottom + m_downHeight * m_scale.y,
			eX,tmpDrawRect.bottom + (m_downHeight + m_allLineRectHeight) * m_scale.y,1.0f,XCCS::mouseColor * m_color);
	}
	//描绘所有的点	
	for(int i = 0;i < m_chartMaxLineSum;++ i)
	{
		if(m_datas[i].size() <= 1) continue;
		XRender::drawLinesVbo(m_v[i],m_datas[i].size(),1.0f,m_chartLineColor[i].fR * m_color.fR,m_chartLineColor[i].fG * m_color.fG,m_chartLineColor[i].fB * m_color.fB,m_color.fA);
	}
}
void XChart::allLineUpdateVbo()
{
	if(m_maxLineLen <= 1) return;	//没有需要描绘的曲线
	if(m_drawDataBuff == NULL || m_drawDataBuffSize != m_maxLineLen)
	{
		m_drawDataBuff = XMem::createArrayMem<float>(m_maxLineLen * 2);
		m_drawDataBuffSize = m_maxLineLen;
	}
	float pixelPerValueY = m_allLineRectHeight * m_scale.y / m_dataRange.getHeight();	//每个像素对应多少值
	float pixelPerValueX = m_dataRect.getWidth() * m_scale.x / m_dataRange.getWidth();	//每个像素对应多少值
	for(int i = 0;i < m_chartMaxLineSum;++ i)
	{
		if(m_datas[i].size() <= 1) continue;
		for(unsigned int j = 0;j < m_datas[i].size();++ j)
		{
			m_drawDataBuff[j << 1] = (m_datas[i][j].x - m_dataRange.left) * pixelPerValueX + m_position.x + m_dataRect.left;
			m_drawDataBuff[(j << 1) + 1] = - (m_datas[i][j].y - m_dataRange.top) * pixelPerValueY + 
				m_position.y + (m_dataRect.bottom + m_downHeight + m_allLineRectHeight) * m_scale.y;
		}
		glBindBufferARB(GL_ARRAY_BUFFER_ARB,m_v[i]);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB,m_datas[i].size() * 2 * sizeof(float),m_drawDataBuff,GL_STATIC_DRAW_ARB);
	}
	glBindBufferARB(GL_ARRAY_BUFFER_ARB,0);	//取消绑定
}
void XChart::curLineUpdateVbo()
{
	if(m_maxLineLen <= 1) return;	//没有需要描绘的曲线
	if(m_drawDataBuff == NULL || m_drawDataBuffSize != m_maxLineLen)
	{
		m_drawDataBuff = XMem::createArrayMem<float>(m_maxLineLen * 2);
		m_drawDataBuffSize = m_maxLineLen;
	}
	int index;
	float tmpValue;
	bool flagHead = false;
	bool flagEnd = false;
	float pixelPerValueY = m_dataRect.getHeight() * m_scale.y / m_sizeValue.y;	//每个像素对应多少值
	float pixelPerValueX = m_dataRect.getWidth() * m_scale.x / m_sizeValue.x;	//每个像素对应多少值
	for(int i = 0;i < m_chartMaxLineSum;++ i)
	{
		if(m_datas[i].size() <= 1) continue;
		index = 0;
		flagHead = false;
		flagEnd = false;
		for(unsigned int j = 0;j < m_datas[i].size();++ j)
		{//这里尚未考虑越界的线如何差值
			if(m_datas[i][j].x >= m_showDataRange.x && m_datas[i][j].x <= m_showDataRange.y)
			{//起点和终点都需要符合现实范围
				m_drawDataBuff[index << 1] = (m_datas[i][j].x - m_baseValue.x) * pixelPerValueX + m_position.x + m_dataRect.left * m_scale.x;
				m_drawDataBuff[(index << 1) + 1] = - (m_datas[i][j].y - m_baseValue.y) * pixelPerValueY + m_position.y + m_dataRect.bottom * m_scale.y;
				++ index;
			}else
			if(j < (int)(m_datas[i].size()) - 1 && m_datas[i][j].x < m_showDataRange.x && m_datas[i][j + 1].x >= m_showDataRange.x)
			{//跨越前边界
				tmpValue = XMath::lineSlerp(m_datas[i][j].y,m_datas[i][j + 1].y,(m_showDataRange.x - m_datas[i][j].x) / (m_datas[i][j + 1].x - m_datas[i][j].x));
				m_drawDataBuff[index << 1] = (m_showDataRange.x - m_baseValue.x) * pixelPerValueX + m_position.x + m_dataRect.left * m_scale.x;
				m_drawDataBuff[(index << 1) + 1] = - (tmpValue- m_baseValue.y) * pixelPerValueY + m_position.y + m_dataRect.bottom * m_scale.y;
				++ index;
				flagHead = true;
			}else
			if(j > 0 && m_datas[i][j].x > m_showDataRange.y && m_datas[i][j - 1].x <= m_showDataRange.y)
			{//跨越后边界
				tmpValue = XMath::lineSlerp(m_datas[i][j - 1].y,m_datas[i][j].y,(m_showDataRange.y - m_datas[i][j - 1].x) / (m_datas[i][j].x - m_datas[i][j - 1].x));
				m_drawDataBuff[index << 1] = (m_showDataRange.y - m_baseValue.x) * pixelPerValueX + m_position.x + m_dataRect.left * m_scale.x;
				m_drawDataBuff[(index << 1) + 1] = - (tmpValue - m_baseValue.y) * pixelPerValueY + m_position.y + m_dataRect.bottom * m_scale.y;
				++ index;
				flagEnd = true;
			}
		}
		glBindBufferARB(GL_ARRAY_BUFFER_ARB,m_curV[i]);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB,index * 2 * sizeof(float),m_drawDataBuff,GL_STATIC_DRAW_ARB);
		m_linePointSum[i] = index;
		if(m_withPoint)
		{
			if(flagHead)
			{
				if(flagEnd)
				{
					glBindBufferARB(GL_ARRAY_BUFFER_ARB,m_curPoint[i]);
					glBufferDataARB(GL_ARRAY_BUFFER_ARB,(index - 2) * 2 * sizeof(float),m_drawDataBuff + 2,GL_STATIC_DRAW_ARB);
					m_linePointsSum[i] = index - 2;
				}else
				{
					glBindBufferARB(GL_ARRAY_BUFFER_ARB,m_curPoint[i]);
					glBufferDataARB(GL_ARRAY_BUFFER_ARB,(index - 1) * 2 * sizeof(float),m_drawDataBuff + 2,GL_STATIC_DRAW_ARB);
					m_linePointsSum[i] = index - 1;
				}
			}else
			{
				if(flagEnd)
				{
					glBindBufferARB(GL_ARRAY_BUFFER_ARB,m_curPoint[i]);
					glBufferDataARB(GL_ARRAY_BUFFER_ARB,(index - 1) * 2 * sizeof(float),m_drawDataBuff,GL_STATIC_DRAW_ARB);
					m_linePointsSum[i] = index - 1;
				}else
				{
					glBindBufferARB(GL_ARRAY_BUFFER_ARB,m_curPoint[i]);
					glBufferDataARB(GL_ARRAY_BUFFER_ARB,index * 2 * sizeof(float),m_drawDataBuff,GL_STATIC_DRAW_ARB);
					m_linePointsSum[i] = index;
				}
			}
		}
	}
	glBindBufferARB(GL_ARRAY_BUFFER_ARB,0);	//取消绑定
}
void XChart::setPosition(float x,float y)
{
	if(!m_isInited) return;
	m_position.set(x,y);
	m_caption.setPosition(m_position.x + m_dataRect.left * m_scale.x + m_dataRect.getWidth() * 0.5f * m_scale.x,m_position.y - 16.0f * m_scale.y);
	for(int i = 0;i < m_chartMaxLineSum;++ i)
	{
		m_dataChecks[i].setPosition(m_position.x + (m_dataRect.right + 16.0f) * m_scale.x,m_position.y + (m_dataRect.top + 17.0f * i) * m_scale.y);
	}
	m_neadUpdateAllLineData = true;
	m_neddUpdateCurLineData = true;
}
void XChart::setScale(float x,float y)
{
	if(!m_isInited) return;
	m_scale.set(x,y);
	m_caption.setPosition(m_position.x + m_dataRect.left * m_scale.x + m_dataRect.getWidth() * 0.5f * m_scale.x,m_position.y - 16.0f * m_scale.y);
	m_caption.setScale(m_scale);
	for(int i = 0;i < m_chartMaxLineSum;++ i)
	{
		m_dataChecks[i].setPosition(m_position.x + (m_dataRect.right + 16.0f) * m_scale.x,m_position.y + (m_dataRect.top + 17.0f * i) * m_scale.y);
		m_dataChecks[i].setScale(m_scale);
	}
	m_textFont.setScale(m_scale * 0.5f);
	m_neadUpdateAllLineData = true;
	m_neddUpdateCurLineData = true;
}
XBool XChart::mouseProc(float x,float y,XMouseState mouseState)
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||	//如果不可见直接退出
		!m_isEnable) return XFalse;		//如果无效则直接退出
	for(int i = 0;i < m_chartMaxLineSum;++ i)
	{
		m_dataChecks[i].mouseProc(x,y,mouseState);
	}
	XRect tmpRect;
	tmpRect.set(m_position.x + m_dataRect.left * m_scale.x,m_position.y + (m_dataRect.bottom + m_downHeight) * m_scale.y,
		m_position.x + m_dataRect.right * m_scale.x,m_position.y + (m_dataRect.bottom + m_downHeight + m_allLineRectHeight) * m_scale.y);
	switch(mouseState)
	{
	case MOUSE_LEFT_BUTTON_DOWN:
	case MOUSE_LEFT_BUTTON_DCLICK:
		if(tmpRect.isInRect(x,y) && m_dataRange.getWidth() > 0.0f)
		{
			float sX = (m_baseValue.x - m_dataRange.left) / m_dataRange.getWidth() * m_dataRect.getWidth() * m_scale.x + m_position.x + m_dataRect.left * m_scale.x;
			float eX = (m_baseValue.x + m_sizeValue.x - m_dataRange.left)/m_dataRange.getWidth() * m_dataRect.getWidth() * m_scale.x + m_position.x + m_dataRect.left * m_scale.x;
			if(x >= sX - 5 && x <= sX + 5)
			{//选择头
				m_chooseArm = 1;
			}else
			if(x >= eX - 5 && x <= eX + 5)
			{//选择尾
				m_chooseArm = 2;
			}else
			{//没有选择
				m_chooseArm = 0;
			}
		}
		break;
	case MOUSE_LEFT_BUTTON_UP:
		m_chooseArm = 0;
		break;
	case MOUSE_MOVE://移动数据
		switch(m_chooseArm)
		{
		case 1:
			if(m_dataRange.getWidth() > 0.0f)
			{
				//float sX = (m_baseValue.x - m_dataRange.left) / m_dataRange.getWidth() * m_dataRect.getWidth() + m_position.x + m_dataRect.left;
				float eX = (m_baseValue.x + m_sizeValue.x - m_dataRange.left)/m_dataRange.getWidth() * m_dataRect.getWidth() * m_scale.x + m_position.x + m_dataRect.left * m_scale.x;
				if(x < eX - 10)
				{
					m_showDataRange.x = (x - (m_position.x + m_dataRect.left * m_scale.x)) / (m_dataRect.getWidth() * m_scale.x) * m_dataRange.getWidth() + m_dataRange.left;
					if(m_showDataRange.x < m_dataRange.left) m_showDataRange.x = m_dataRange.left;
				}
			}
			break;
		case 2:
			if(m_dataRange.getWidth() > 0.0f)
			{
				float sX = (m_baseValue.x - m_dataRange.left) / m_dataRange.getWidth() * m_dataRect.getWidth() * m_scale.x + m_position.x + m_dataRect.left * m_scale.x;
				//float eX = (m_baseValue.x + m_sizeValue.x - m_dataRange.left)/m_dataRange.getWidth() * m_dataRect.getWidth() + m_position.x + m_dataRect.left;
				if(x > sX + 10)	
				{
					m_showDataRange.y = (x - (m_position.x + m_dataRect.left * m_scale.x)) / (m_dataRect.getWidth() * m_scale.x) * m_dataRange.getWidth() + m_dataRange.left;
					if(m_showDataRange.y > m_dataRange.right) m_showDataRange.y = m_dataRange.right;
				}
			}
			break;
		}
		if(m_chooseArm != 0)
		{
			m_baseValue.x = m_showDataRange.x;
			m_sizeValue.x = m_showDataRange.y - m_showDataRange.x;
			m_neddUpdateCurLineData = true;
			//updateRange();
		}
		break;
	}
	return XTrue;
}
void XChart::checkRange()
{
	if(m_dataRange.getWidth() > 0.0f)
	{
		float temp = m_dataRange.getWidth() / m_dataRect.getWidth() * 9.6f;
		if(m_showDataRange.x < m_dataRange.left ||
			m_showDataRange.y < m_dataRange.left ||
			m_showDataRange.x > m_dataRange.right ||
			m_showDataRange.y > m_dataRange.right ||
			m_showDataRange.y - m_showDataRange.x < temp)
		{//最小距离
			m_showDataRange.x = m_dataRange.left;
			m_showDataRange.y = m_showDataRange.x + temp;
			m_neddUpdateCurLineData = true;
		}
	}
}
void XChart::updateRange()
{//根据x范围的变化更新y的范围，并更新坐标轴上的相关数据
	bool flag = true;
	float tmpValue;
	XRect tmpRange;
	for(int i = 0;i < m_chartMaxLineSum;++ i)
	{
		if(m_datas[i].size() <= 1)
		{
			m_dataChecks[i].disVisible();
			continue;
		}else
		{
			m_dataChecks[i].setVisible();
		}
		if(m_datas[i].size() <= 1 || !m_dataChecks[i].getState()) continue;
		for(unsigned int j = 0;j < m_datas[i].size();++ j)
		{
			if(m_datas[i][j].x >= m_showDataRange.x && m_datas[i][j].x <= m_showDataRange.y)
			{//数据在范围内
				if(flag)
				{
					tmpRange.set(m_datas[i][j].x,m_datas[i][j].y,m_datas[i][j].x,m_datas[i][j].y);
					flag = false;
				}else
				{
					if(m_datas[i][j].y < tmpRange.top) tmpRange.top = m_datas[i][j].y;
					if(m_datas[i][j].y > tmpRange.bottom) tmpRange.bottom = m_datas[i][j].y;
				}
			}
			if(j < (int)(m_datas[i].size()) - 1 && m_datas[i][j].x < m_showDataRange.x && m_datas[i][j + 1].x >= m_showDataRange.x)
			{//跨越前边界，这里需要插值，这里使用线性插值
				tmpValue = XMath::lineSlerp(m_datas[i][j].y,m_datas[i][j + 1].y,(m_showDataRange.x - m_datas[i][j].x) / (m_datas[i][j + 1].x - m_datas[i][j].x));
				if(flag)
				{
					tmpRange.top = tmpValue;
					tmpRange.bottom = tmpValue;
					flag = false;
				}else
				{
					if(tmpValue < tmpRange.top) tmpRange.top = tmpValue;
					if(tmpValue > tmpRange.bottom) tmpRange.bottom = tmpValue;
				}
			}
			if(j > 0 && m_datas[i][j].x > m_showDataRange.y && m_datas[i][j - 1].x < m_showDataRange.y)
			{//跨越后边界，这里需要插值，这里使用线性插值
				tmpValue = XMath::lineSlerp(m_datas[i][j - 1].y,m_datas[i][j].y,(m_showDataRange.y - m_datas[i][j - 1].x) / (m_datas[i][j].x - m_datas[i][j - 1].x));
				if(flag)
				{
					tmpRange.top = tmpValue;
					tmpRange.bottom = tmpValue;
					flag = false;
				}else
				{
					if(tmpValue < tmpRange.top) tmpRange.top = tmpValue;
					if(tmpValue > tmpRange.bottom) tmpRange.bottom = tmpValue;
				}
			}
		}
	}
	m_baseValue.set(m_showDataRange.x,tmpRange.top - tmpRange.getHeight() * 0.05f);	//上下都留一点余地，防止上下切边
	m_sizeValue.set(m_showDataRange.y - m_showDataRange.x,tmpRange.getHeight() * 1.1f);

	m_stepWidth.y = XMath::getApproximateData(m_sizeValue.y / m_verticalNetSum,2,m_dataAccuracyY);
	float tmpData = pow(10.0f,m_dataAccuracyY - 1);
	m_stepWidth.y = m_stepWidth.y * tmpData * 10.0f;
	//计算第一个点的位置
	if(m_baseValue.y <= 0 && m_baseValue.y + m_sizeValue.y >= 0)
	{//跨越零的界线
		m_firstNetValue.y = m_baseValue.y - fmod(m_baseValue.y,m_stepWidth.y);
	}else
	{
		m_firstNetValue.y = (int)((m_baseValue.y + m_stepWidth.y) / tmpData) * tmpData;
	}
	
	m_stepWidth.x = XMath::getApproximateData(m_sizeValue.x / m_horizontalNetSum,2,m_dataAccuracyX);
	tmpData = pow(10.0f,m_dataAccuracyX - 1);
	m_stepWidth.x = m_stepWidth.x * tmpData * 10.0f;
	//计算第一个点的位置
	if(m_baseValue.x <= 0 && m_baseValue.x + m_sizeValue.x >= 0)
	{//跨越零的界线
		m_firstNetValue.x = m_baseValue.x - fmod(m_baseValue.x,m_stepWidth.x);
	}else
	{
		m_firstNetValue.x = (int)((m_baseValue.x + m_stepWidth.x) / tmpData) * tmpData;
	}
	curLineUpdateVbo();

	m_curMouseRect.set(m_position.x + m_dataRect.left * m_scale.x,
		m_position.y + (m_dataRect.top - 32.0f) * m_scale.x,
		m_position.x + m_dataRect.right * m_scale.x,
		m_position.y + (m_dataRect.bottom + m_downHeight + m_allLineRectHeight) * m_scale.x);
	float right;
	for(int i = 0;i < m_chartMaxLineSum;++ i)
	{
		right = m_dataChecks[i].getBox(2).x;
		if(right > m_curMouseRect.right) m_curMouseRect.right = right;
	}
}
void XChart::insertData(const XVector2 &data,int lineIndex)
{
	if(lineIndex < 0 || lineIndex >= m_chartMaxLineSum) return;	//数据不合法
	if (m_maxDataSum <= 0)
	{//不限制点的数量
		//下面要排序，这里使用原始的排序算法，可以进行优化
		if(m_dataSum == 0)
		{
			m_dataRange.set(data.x,data.y,data.x,data.y);
		}else
		{
			if(data.x < m_dataRange.left) m_dataRange.left = data.x;
			if(data.x > m_dataRange.right) m_dataRange.right = data.x;
			if(data.y < m_dataRange.top) m_dataRange.top = data.y;
			if(data.y > m_dataRange.bottom) m_dataRange.bottom = data.y;
		}
		m_datas[lineIndex].push_back(data);
		for(unsigned int i = 0;i < m_datas[lineIndex].size();++ i)
		{//对数据进行排序，
			if(m_datas[lineIndex][i].x > data.x)
			{//将数据插入
				for(int j = (int)(m_datas[lineIndex].size()) - 1;j > i;-- j)
				{
					m_datas[lineIndex][j] = m_datas[lineIndex][j - 1];
				}
				m_datas[lineIndex][i] = data;
				break;//插入完成之后退出
			}
		}
		if(m_datas[lineIndex].size() > m_maxLineLen || m_maxLineLen < 0) m_maxLineLen = m_datas[lineIndex].size();
		++ m_dataSum;	//标记数据
	}
	else
	{
		//下面要排序，这里使用原始的排序算法，可以进行优化
		if (m_datas[lineIndex].size() <= m_maxDataSum && m_maxDataSum > 0)
		{
			if(m_dataSum == 0)
			{
				m_dataRange.set(data.x,data.y,data.x,data.y);
			}else
			{
				if(data.x < m_dataRange.left) m_dataRange.left = data.x;
				if(data.x > m_dataRange.right) m_dataRange.right = data.x;
				if(data.y < m_dataRange.top) m_dataRange.top = data.y;
				if(data.y > m_dataRange.bottom) m_dataRange.bottom = data.y;
			}
			m_datas[lineIndex].push_back(data);
			for(unsigned int i = 0;i < m_datas[lineIndex].size();++ i)
			{//对数据进行排序，
				if(m_datas[lineIndex][i].x > data.x)
				{//将数据插入
					for(int j = (int)(m_datas[lineIndex].size()) - 1;j > i;-- j)
					{
						m_datas[lineIndex][j] = m_datas[lineIndex][j - 1];
					}
					m_datas[lineIndex][i] = data;
					break;//插入完成之后退出
				}
			}
			if(m_datas[lineIndex].size() > m_maxLineLen || m_maxLineLen < 0) m_maxLineLen = m_datas[lineIndex].size();
			++ m_dataSum;	//标记数据
		}
		else
		{//点的数量已经超出限制，这里需要剔除之前的点
			m_datas[lineIndex].pop_front();	//抛出前面的点
			m_datas[lineIndex].push_back(data);
			for(unsigned int i = 0;i < m_datas[lineIndex].size();++ i)
			{//对数据进行排序，
				if(m_datas[lineIndex][i].x > data.x)
				{//将数据插入
					for(int j = (int)(m_datas[lineIndex].size()) - 1;j > i;-- j)
					{
						m_datas[lineIndex][j] = m_datas[lineIndex][j - 1];
					}
					m_datas[lineIndex][i] = data;
					break;//插入完成之后退出
				}
			}
			if(m_datas[lineIndex].size() > m_maxLineLen || m_maxLineLen < 0) m_maxLineLen = m_datas[lineIndex].size();
			//重新统计点的范围
			if(m_dataSum == 0)
			{
				m_dataRange.set(data.x,data.y,data.x,data.y);
			}else
			{
				m_dataRange.set(data.x, data.y, data.x, data.y);
				for (int i = 0; i < m_chartMaxLineSum; ++i)
				{
					for (unsigned int j = 0; j < m_datas[i].size(); ++j)
					{
						if (m_datas[i][j].x < m_dataRange.left) m_dataRange.left = m_datas[i][j].x;
						if (m_datas[i][j].x > m_dataRange.right) m_dataRange.right = m_datas[i][j].x;
						if (m_datas[i][j].y < m_dataRange.top) m_dataRange.top = m_datas[i][j].y;
						if (m_datas[i][j].y > m_dataRange.bottom) m_dataRange.bottom = m_datas[i][j].y;
					}
				}
			}
		}
	}
	m_neddUpdateCurLineData = true;
	//updateRange();
	m_neadUpdateAllLineData = true;	
	//allLineUpdateVbo();
}
void XChart::setMaxDataSum(int sum)
{
	if (m_maxDataSum == sum) return;	//数据没有发生变化
	if (m_maxDataSum < sum && m_maxDataSum > 0)
	{//扩大数量，可以不用改变
		m_maxDataSum = sum;
		return;
	}
	m_maxDataSum = sum;
	if (m_maxDataSum > 0)
	{//这里需要抛弃不需要的数据
		m_dataSum = 0;
		m_maxLineLen = 0;
		for (int i = 0; i < m_chartMaxLineSum; ++i)
		{
			while (m_datas[i].size() > m_maxDataSum || m_maxDataSum < 0)
			{
				m_datas[i].pop_front();
			}
			m_dataSum += m_datas[i].size();
			if (m_datas[i].size() > m_maxLineLen || m_maxLineLen < 0) m_maxLineLen = m_datas[i].size();
		}
		//下面更新范围
		bool flag = true;
		for (int i = 0; i < m_chartMaxLineSum; ++i)
		{
			for (unsigned int j = 0; j < m_datas[i].size(); ++j)
			{
				if (flag)
				{//第一次进入
					flag = false;
					m_dataRange.set(m_datas[i][j].x, m_datas[i][j].y, m_datas[i][j].x, m_datas[i][j].y);
				}
				else
				{
					if (m_datas[i][j].x < m_dataRange.left) m_dataRange.left = m_datas[i][j].x;
					if (m_datas[i][j].x > m_dataRange.right) m_dataRange.right = m_datas[i][j].x;
					if (m_datas[i][j].y < m_dataRange.top) m_dataRange.top = m_datas[i][j].y;
					if (m_datas[i][j].y > m_dataRange.bottom) m_dataRange.bottom = m_datas[i][j].y;
				}
			}
		}
		m_neddUpdateCurLineData = true;
		//updateRange();
		m_neadUpdateAllLineData = true;
		//allLineUpdateVbo();
	}
}
XBool XChart::readDataFromFile(const char * filename)	//从文件中读取曲线数据
{
	if(filename == NULL) return XFalse;
	FILE *fp = NULL;
	if((fp = fopen(filename,"r")) == NULL) return false;
	int sum = 0;
	char tmpString[1024];
	if(fscanf(fp,"name:%s\n",tmpString) != 1) {fclose(fp);return XFalse;}
	if(fscanf(fp,"lineSum:%d,\n",&sum) != 1) {fclose(fp);return XFalse;}
	m_caption.setString(tmpString);
	//这里清除所有的原始数据
	for(int i = 0;i < m_chartMaxLineSum;++ i)
	{
		m_datas[i].clear();
//		m_linePointSum[i] = 0;
	}
	memset(m_linePointSum,0,m_chartMaxLineSum * sizeof(int));
	m_dataSum = 0;

	//下面依次读取数据
	XVector2 tmpPoint;
	int tmpSum;
	for(int i = 0;i < sum;++ i)
	{
		if(fscanf(fp,"lineName:%s\n",tmpString) != 1) {fclose(fp);return XFalse;}
		m_dataChecks[i].setCaptionText(tmpString);
		if(fscanf(fp,"pointSum:%d,\n",&tmpSum) != 1) {fclose(fp);return XFalse;}
		for(int j = 0;j < tmpSum;++ j)
		{
			if(fscanf(fp,"%f,%f,\n",&tmpPoint.x,&tmpPoint.y) != 2) {fclose(fp);return XFalse;}
			insertData(tmpPoint,i);
		}
	}
	fclose(fp);	
	return XTrue;
}
XBool XChart::saveDataToFile(const char * filename)	//从将曲线数据保存到文件中
{
	if(filename == NULL) return XFalse;
	FILE *fp = NULL;
	if((fp = fopen(filename,"w")) == NULL) return false;
	//下面写入数据
	int sum = 0;
	for(int i = 0;i < m_chartMaxLineSum;++ i)
	{
		if(m_datas[i].size() > 1) ++ sum;
	}
	fprintf(fp,"name:%s\n",m_caption.getString());
	fprintf(fp,"lineSum:%d,\n",sum);
	if(sum <= 0)
	{
		fclose(fp);	
		return XTrue;
	}
	//下面依次写入每条线的数据
	for(int i = 0;i < m_chartMaxLineSum;++ i)
	{
		if(m_datas[i].size() <= 1) continue;
		fprintf(fp,"lineName:%s\n",m_dataChecks[i].getCaptionString());
		fprintf(fp,"pointSum:%d,\n",m_datas[i].size());
		for(unsigned int j = 0;j < m_datas[i].size();++ j)
		{
			fprintf(fp,"%f,%f,\n",m_datas[i][j].x,m_datas[i][j].y);
		}
	}
	fclose(fp);	
	return XTrue;
}
void XChart::release()
{
	if(!m_isInited) return;
	XMem::XDELETE_ARRAY(m_drawDataBuff);
	for(int i = 0;i < m_chartMaxLineSum;++ i)
	{
		glDeleteBuffersARB(1,&m_v[i]);
		glDeleteBuffersARB(1,&m_curV[i]);
		glDeleteBuffersARB(1,&m_curPoint[i]);
	}
	m_isInited = false;
}
void XChart::setColor(float r,float g,float b,float a) 
{
	m_color.setColor(r,g,b,a);
	m_caption.setColor(m_color);
	m_textFont.setColor(m_color);
	for(int i = 0;i < m_chartMaxLineSum;++ i)
	{
		m_dataChecks[i].setColor(m_color);
	}
}
void XChart::setAlpha(float a)
{
	m_color.setA(a);
	m_caption.setAlpha(a);
	m_textFont.setAlpha(a);
	for(int i = 0;i < m_chartMaxLineSum;++ i)
	{
		m_dataChecks[i].setAlpha(a);
	}
}
#if !WITH_INLINE_FILE
#include "XChart.inl"
#endif
}