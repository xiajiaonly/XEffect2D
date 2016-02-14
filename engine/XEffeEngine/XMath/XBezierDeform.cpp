#include "XStdHead.h"
#include "XBezierDeform.h"
#include "XInputEventCore.h"
#include "XMath\XBezierSpline.h"
#include "XXml.h"
namespace XE
{
bool XBezierDeform::init(int w,int h,int handX,int handY,int vSum)
{
	if(m_isInited) return false;
	if(w <= 0 || h <= 0 || handX <= 1 || handY <= 1) return false;
	m_handSumX = handX;
	m_handSumY = handY;
	m_vSum = vSum;
	m_handlePoint.resize(m_handSumX * m_handSumY);

	m_mesh.init(w,h,(m_handSumX - 1) * vSum + 1,(m_handSumY - 1) * vSum + 1,true);
	for(int i = 0;i < m_handSumX * m_handSumY;++ i)
	{
		m_handlePoint[i].set((i % m_handSumX) * w / (m_handSumX - 1.0f),((int)(i / m_handSumX)) * h / (m_handSumY - 1.0f));
	}
	m_tmpHandlePoint.resize((m_handSumX - 1) << 1);
	m_changeOldPoint.resize(m_handSumX * m_handSumY);
	m_isInited = true;
	return true;
}
bool XBezierDeform::initFromFile(int w,int h,const char *filename)
{
	if(m_isInited) return false;
	if(w <= 0 || h <= 0) return false;
	if(!load(filename)) return false;
	m_mesh.init(w,h,(m_handSumX - 1) * m_vSum + 1,(m_handSumY - 1) * m_vSum + 1,true);
	m_tmpHandlePoint.resize((m_handSumX - 1) << 1);
	m_changeOldPoint.resize(m_handSumX * m_handSumY);
	updateNet();
	m_isInited = true;
	return true;
}
void XBezierDeform::draw(unsigned int tex)
{
	if(!m_isInited) return;
	m_mesh.drawTexture(tex);
	if(m_needDrawHandle)
	{
		for(int i = 0;i < m_handSumX * m_handSumY;++ i)
		{
			if(m_curMouseIndex != i) XRender::drawBox(m_handlePoint[i].x,m_handlePoint[i].y,10,10,1,1,0,0,1);
			else XRender::drawBox(m_handlePoint[i].x,m_handlePoint[i].y,10,10,1,0,0,1,1);
		}
	}
}
void XBezierDeform::inputProc(const XInputEvent &event)
{
	if(event.isKeyBoardDown())
	{
		switch(event.keyValue)
		{
		case XKEY_S:save();break;
		//case XKEY_L:load();break;
		case XKEY_A:m_needDrawHandle = !m_needDrawHandle;break;
		case XKEY_M:
			if(m_isMouseDown)
			{
				m_mouseDownPos = getMousePos();
				for(int i = 0; i < m_handSumX * m_handSumY;++ i)
					m_changeOldPoint[i] = m_handlePoint[i];
			}
			m_changeType = BD_CHANGE_MOVE;
			break;
		case XKEY_R:
			if(m_isMouseDown)
			{
				m_mouseDownPos = getMousePos();
				for(int i = 0; i < m_handSumX * m_handSumY;++ i)
					m_changeOldPoint[i] = m_handlePoint[i];
			}
			m_changeType = BD_CHANGE_ROTATE;
			break;
		case XKEY_C:
			if(m_isMouseDown)
			{
				m_mouseDownPos = getMousePos();
				for(int i = 0; i < m_handSumX * m_handSumY;++ i)
					m_changeOldPoint[i] = m_handlePoint[i];
			}
			m_changeType = BD_CHANGE_SCALE;
			break;
		}
	}
	if(event.isKeyBoardUp())
	{
		switch(event.keyValue)
		{
		case XKEY_M:if(m_changeType == BD_CHANGE_MOVE) m_changeType = BD_CHANGE_NULL;break;
		case XKEY_R:if(m_changeType == BD_CHANGE_ROTATE) m_changeType = BD_CHANGE_NULL;break;
		case XKEY_C:if(m_changeType == BD_CHANGE_SCALE) m_changeType = BD_CHANGE_NULL;break;
		}
	}
	if(event.type == EVENT_MOUSE)
	{
		if(m_changeType == BD_CHANGE_NULL)
		{
			switch(event.mouseState)
			{
			case MOUSE_LEFT_BUTTON_DOWN:m_isMouseDown = true;break;
			case MOUSE_LEFT_BUTTON_UP:m_isMouseDown = false;break;
			case MOUSE_MOVE:
				if(!m_needDrawHandle) break;
				if(!m_isMouseDown) m_curMouseIndex = getCurMouseChoose(event.mouseX,event.mouseY);
				else
				{
					if(m_curMouseIndex >= 0) 
					{
						m_handlePoint[m_curMouseIndex].set(event.mouseX,event.mouseY);
						//handleFlag[nowMouseIndex] = true;
						updateNet();
						//handleFlag[nowMouseIndex] = false;
					}
				}
				break;
			}
		}else
		if(m_needDrawHandle)
		{//下面做相应的计算
			switch(event.mouseState)
			{
			case MOUSE_LEFT_BUTTON_DOWN:
				m_isMouseDown = true;
				m_mouseDownPos.set(event.mouseX,event.mouseY);
				for(int i = 0; i < m_handSumX * m_handSumY;++ i)
					m_changeOldPoint[i] = m_handlePoint[i];
				break;
			case MOUSE_LEFT_BUTTON_UP:m_isMouseDown = false;break;
			case MOUSE_MOVE:
				if(!m_isMouseDown) break;
				switch(m_changeType)
				{
				case BD_CHANGE_MOVE:
					for(int i = 0; i < m_handSumX * m_handSumY;++ i)
						m_handlePoint[i] = m_changeOldPoint[i] + XVector2(event.mouseX,event.mouseY) - m_mouseDownPos;
					updateNet();
					break;
				case BD_CHANGE_SCALE:
					{
						float r = 1.0f;
						if(event.mouseX <= m_mouseDownPos.x) r = (m_mouseDownPos.x - event.mouseX) / 300.0f + 1.0f;	//放大
						else r = 1.0f - (event.mouseX - m_mouseDownPos.x) / 600.0f;//缩小
						if(r < 0.0f) r = 0.0f;
						for(int i = 0; i < m_handSumX * m_handSumY;++ i)
							m_handlePoint[i] = (m_changeOldPoint[i] - m_mouseDownPos) * r + m_mouseDownPos;
						updateNet();
					}
					break;
				case BD_CHANGE_ROTATE:
					{
						float r = (event.mouseX - m_mouseDownPos.x) / 300.0f * PI;
						float sr = sin(r);
						float cr = cos(r);
						XVector2 tmp;
						for(int i = 0; i < m_handSumX * m_handSumY;++ i)
						{
							tmp = m_changeOldPoint[i] - m_mouseDownPos;
							m_handlePoint[i] = XVector2(tmp.x * cr - tmp.y * sr,tmp.x * sr + tmp.y * cr) + m_mouseDownPos;
						}
						updateNet();
					}
					break;
				}
				break;
			}
		}
	}
}
void XBezierDeform::updateNet()
{//更新网格数据
	//使用贝塞尔差值结算各个节点
	//更新水平的边
	XVector2 tmpVec2[3];
#ifdef OPTIMIZATION
	XSimpleBezierSpline spLine;	//用于差值的贝塞尔曲线
#else
	XBezierSpline spLine;	//用于差值的贝塞尔曲线
#endif
	for(int i = 0;i < m_handSumY;++ i)
	{
		//1、计算出一条线中的中间点
		int index = i * m_handSumX;
		for(int j = 0;j < m_handSumX - 1;++ j)
		{
			if(j == 0)
				m_tmpHandlePoint[(j << 1)] = XMath::lineSlerp(m_handlePoint[index + j],m_handlePoint[index + j + 1],0.3333f);
			else
			{
				tmpVec2[0] = XMath::lineSlerp(m_handlePoint[index + j - 1],m_handlePoint[index + j],0.6666f);	//与前一个点的中点
				tmpVec2[1] = XMath::lineSlerp(m_handlePoint[index + j],m_handlePoint[index + j + 1],0.3333f);	//与后一个点的中点
				tmpVec2[2] = (tmpVec2[0] + tmpVec2[1]) * 0.5f;
				m_tmpHandlePoint[(j << 1)] =  tmpVec2[1] + m_handlePoint[index + j] - tmpVec2[2];
			}
			if(j == m_handSumX - 2)
				m_tmpHandlePoint[(j << 1) + 1] = XMath::lineSlerp(m_handlePoint[index + j],m_handlePoint[index + j + 1],0.6666f);
			else
			{
				tmpVec2[0] = XMath::lineSlerp(m_handlePoint[index + j],m_handlePoint[index + j + 1],0.6666f);		//与后1个点的中点
				tmpVec2[1] = XMath::lineSlerp(m_handlePoint[index + j + 1],m_handlePoint[index + j + 2],0.3333f);	//后2个点的中点
				tmpVec2[2] = (tmpVec2[0] + tmpVec2[1]) * 0.5f;
				m_tmpHandlePoint[(j << 1) + 1] =  tmpVec2[0] + m_handlePoint[index + j + 1] - tmpVec2[2];
			}
		}
		//下面对一条线进行差值计算
		int curIndex = -1;
		index = i * m_vSum * m_mesh.getWidthSum();
		for(int j = 0;j < m_mesh.getWidthSum() - 1;++ j)
		{
			if(curIndex != j / m_vSum)
			{
				curIndex = j / m_vSum;
				spLine.init(m_handlePoint[curIndex + i * m_handSumX],m_tmpHandlePoint[curIndex << 1],m_tmpHandlePoint[(curIndex << 1) + 1],m_handlePoint[curIndex + i * m_handSumX + 1]);
			}
#ifdef OPTIMIZATION
			m_mesh.m_v[index + j] = spLine.getBezierSplineValue((j % m_vSum) / (float)(m_vSum));
#else
			m_mesh.m_v[index + j] = spLine.getBezierSplineValue(spLine.getT((j % m_vSum) / (float)(m_vSum)));
#endif
		}
		m_mesh.m_v[index + m_mesh.getWidthSum() - 1] = m_handlePoint[curIndex + i * m_handSumX + 1];	//最后一个点等于最后一个点
	}
	//更新垂直的边
	for(int i = 0;i < m_mesh.getWidthSum();++ i)
	{
		//计算中间节点
		//1、计算出一条线中的中间点
		for(int j = 0;j < m_handSumY - 1;++ j)
		{
			if(j == 0)
				m_tmpHandlePoint[(j << 1)] = XMath::lineSlerp(m_mesh.m_v[i + j * m_vSum * m_mesh.getWidthSum()],m_mesh.m_v[i + (j + 1) * m_vSum * m_mesh.getWidthSum()],0.3333f);
			else
			{
				tmpVec2[0] = XMath::lineSlerp(m_mesh.m_v[i + (j - 1) * m_vSum * m_mesh.getWidthSum()],m_mesh.m_v[i + j * m_vSum * m_mesh.getWidthSum()],0.6666f);	//与前一个点的中点
				tmpVec2[1] = XMath::lineSlerp(m_mesh.m_v[i + j * m_vSum * m_mesh.getWidthSum()],m_mesh.m_v[i + (j + 1) * m_vSum * m_mesh.getWidthSum()],0.3333f);	//与后一个点的中点
				tmpVec2[2] = (tmpVec2[0] + tmpVec2[1]) * 0.5f;
				m_tmpHandlePoint[(j << 1)] =  tmpVec2[1] + m_mesh.m_v[i + j * m_vSum * m_mesh.getWidthSum()] - tmpVec2[2];
			}
			if(j == m_handSumX - 2)
				m_tmpHandlePoint[(j << 1) + 1] = XMath::lineSlerp(m_mesh.m_v[i + j * m_vSum * m_mesh.getWidthSum()],m_mesh.m_v[i + (j + 1) * m_vSum * m_mesh.getWidthSum()],0.6666);
			else
			{
				tmpVec2[0] = XMath::lineSlerp(m_mesh.m_v[i + j * m_vSum * m_mesh.getWidthSum()],m_mesh.m_v[i + (j + 1) * m_vSum * m_mesh.getWidthSum()],0.6666f);		//与后1个点的中点
				tmpVec2[1] = XMath::lineSlerp(m_mesh.m_v[i + (j + 1) * m_vSum * m_mesh.getWidthSum()],m_mesh.m_v[i + (j + 2) * m_vSum * m_mesh.getWidthSum()],0.3333f);	//后2个点的中点
				tmpVec2[2] = (tmpVec2[0] + tmpVec2[1]) * 0.5f;
				m_tmpHandlePoint[(j << 1) + 1] =  tmpVec2[0] + m_mesh.m_v[i + (j + 1) * m_vSum * m_mesh.getWidthSum()] - tmpVec2[2];
			}
		}
		//下面对一条线进行差值计算
		int curIndex = -1;
		for(int j = 0;j < m_mesh.getHeightSum() - 1;++ j)
		{
			if(curIndex != j / m_vSum)
			{
				curIndex = j / m_vSum;
				spLine.init(m_mesh.m_v[curIndex * m_vSum * m_mesh.getWidthSum() + i],m_tmpHandlePoint[curIndex << 1],m_tmpHandlePoint[(curIndex << 1) + 1],m_mesh.m_v[(curIndex + 1) * m_vSum * m_mesh.getWidthSum() + i]);
			}
#ifdef OPTIMIZATION
			m_mesh.m_v[i + j * m_mesh.getWidthSum()] = spLine.getBezierSplineValue((j % m_vSum) / (float)(m_vSum));
#else
			m_mesh.m_v[i + j * m_mesh.getWidthSum()] = spLine.getBezierSplineValue(spLine.getT((j % m_vSum) / (float)(m_vSum)));
#endif
		}
	}
}
bool XBezierDeform::save(const char *filename)const 
{
	if(!m_isInited) return false;
	TiXmlElement elmRoot("root");
	if (!XXml::addLeafNode(elmRoot, "handX", XString::toString(m_handSumX))) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "handY", XString::toString(m_handSumY))) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "vSum", XString::toString(m_vSum))) return XFalse;
	char tmpStr[128];
	for(int i = 0;i < m_handSumX * m_handSumY;++ i)
	{
		sprintf(tmpStr,"handle%04d",i);
		if (!XXml::addLeafNode(elmRoot, tmpStr, m_handlePoint[i])) return XFalse;
	}

	if(filename == NULL)
	{
		TiXmlDocument doc(DEFAULT_BD_FILENAME);
		TiXmlDeclaration declaration("1.0","gb2312","yes");
		doc.InsertEndChild(declaration);
		if(doc.InsertEndChild(elmRoot) == NULL) return XFalse;
		if(!doc.SaveFile()) return XFalse;
	}else
	{
		TiXmlDocument doc(filename);
		TiXmlDeclaration declaration("1.0","gb2312","yes");
		doc.InsertEndChild(declaration);
		if(doc.InsertEndChild(elmRoot) == NULL) return XFalse;
		if(!doc.SaveFile()) return XFalse;
	}
	return true;
}//将数据保存到文件
bool XBezierDeform::load(const char *filename) //从文件中读取数据
{
	TiXmlDocument doc;
	if((filename == NULL && !doc.LoadFile(DEFAULT_BD_FILENAME)) || 
		(filename != NULL && !doc.LoadFile(filename))) return XFalse;
	TiXmlNode *rootNode = doc.FirstChild("root");
	if(rootNode == NULL) return XFalse;
	//下面开始读取数据
	XXml::getXmlAsInt(rootNode,"handX",m_handSumX);
	XXml::getXmlAsInt(rootNode,"handY",m_handSumY);
	XXml::getXmlAsInt(rootNode,"vSum",m_vSum);
	//下面依次读取所有点
	m_handlePoint.resize(m_handSumX * m_handSumY);
	char tmpStr[128];
	for(int i = 0;i < m_handSumX * m_handSumY;++ i)
	{
		sprintf(tmpStr,"handle%04d",i);
		XXml::getXmlAsVec2(rootNode,tmpStr,m_handlePoint[i]);
	}

	return true;
}
}