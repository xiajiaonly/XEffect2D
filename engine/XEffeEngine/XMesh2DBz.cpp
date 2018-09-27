#include "XStdHead.h"
#include "XMesh2DBz.h"
#include "XMath\XBezierSpline.h"
#include "XXml.h"
#include "XFile.h"
#include "XMath\XColorSpace.h"
namespace XE{
bool XMesh2DBz::init(int w,int h,int handX,int handY,int vSum)
{
	if(m_isInited) return false;
	if(w <= 0 || h <= 0 || handX <= 1 || handY <= 1 || vSum < 2) return false;
	m_handSumX = handX;
	m_handSumY = handY;
	m_vSum = vSum;
	m_w = w;
	m_h = h;
	m_handlePoint.resize(m_handSumX * m_handSumY);

	m_mesh.init(w,h,(m_handSumX - 1) * vSum + 1,(m_handSumY - 1) * vSum + 1,true);
	for(int i = 0;i < m_handSumX * m_handSumY;++ i)
	{
		m_handlePoint[i].set((i % m_handSumX) * w / (m_handSumX - 1.0f),((int)(i / m_handSumX)) * h / (m_handSumY - 1.0f));
	}
	m_tmpHandlePoint.resize(((std::max)(m_handSumX, m_handSumY) - 1) << 1);
	m_isInited = true;
	return true;
}
void XMesh2DBz::reset()
{//回复初始状态
	if(!m_isInited) return;
	for(int i = 0;i < m_handSumX * m_handSumY;++ i)
	{
		m_handlePoint[i].set((i % m_handSumX) * m_w / (m_handSumX - 1.0f),((int)(i / m_handSumX)) * m_h / (m_handSumY - 1.0f));
	}
	updateMesh();
}
bool XMesh2DBz::initFromFile(const char *filename)
{
	if(m_isInited) return false;
	if(!load(filename)) return false;
	m_tmpHandlePoint.resize(((std::max)(m_handSumX, m_handSumY) - 1) << 1);
	updateMesh();
	m_isInited = true;
	return true;
}
bool XMesh2DBz::initFromFileB(const char *filename)
{
	if(m_isInited) return false;
	if(!loadB(filename)) return false;
	m_tmpHandlePoint.resize(((std::max)(m_handSumX, m_handSumY) - 1) << 1);
	updateMesh();
	m_isInited = true;
	return true;
}
void XMesh2DBz::updateMesh()
{//更新网格数据
	//使用贝塞尔插值结算各个节点
	//更新水平的边
	XVec2 tmpVec2[3];
#ifdef OPTIMIZATION
	XSimpleBezierSpline spLine;	//用于插值的贝塞尔曲线
#else
	XBezierSpline spLine;	//用于插值的贝塞尔曲线
#endif
	for(int i = 0;i < m_handSumY;++ i)
	{
		//1、计算出一条线中的中间点
		int index = i * m_handSumX;
		for(int j = 0,indexJ = 0;j < m_handSumX - 1;++ j,++index, indexJ += 2)
		{
			if(j == 0)
				m_tmpHandlePoint[indexJ] = XMath::lineSlerp(m_handlePoint[index],m_handlePoint[index + 1], ONE_THIRD);
			else
			{
				tmpVec2[0] = XMath::lineSlerp(m_handlePoint[index - 1],m_handlePoint[index], TWO_THIRD);	//与前一个点的中点
				tmpVec2[1] = XMath::lineSlerp(m_handlePoint[index],m_handlePoint[index + 1], ONE_THIRD);	//与后一个点的中点
			//	tmpVec2[2] = (tmpVec2[0] + tmpVec2[1]) * 0.5f;
			//	m_tmpHandlePoint[indexJ] =  tmpVec2[1] + m_handlePoint[index] - tmpVec2[2];
				m_tmpHandlePoint[indexJ] = (tmpVec2[1] - tmpVec2[0]) * 0.5f + m_handlePoint[index];
			}
			if(j == m_handSumX - 2)
				m_tmpHandlePoint[indexJ + 1] = XMath::lineSlerp(m_handlePoint[index],m_handlePoint[index + 1], TWO_THIRD);
			else
			{
				tmpVec2[0] = XMath::lineSlerp(m_handlePoint[index],m_handlePoint[index + 1], TWO_THIRD);		//与后1个点的中点
				tmpVec2[1] = XMath::lineSlerp(m_handlePoint[index + 1],m_handlePoint[index + 2], ONE_THIRD);	//后2个点的中点
			//	tmpVec2[2] = (tmpVec2[0] + tmpVec2[1]) * 0.5f;
			//	m_tmpHandlePoint[indexJ + 1] =  tmpVec2[0] + m_handlePoint[index + 1] - tmpVec2[2];
				m_tmpHandlePoint[indexJ + 1] = (tmpVec2[0] - tmpVec2[1]) * 0.5f + m_handlePoint[index + 1];
			}
		}
		//下面对一条线进行插值计算
		int curIndex = -1;
		index = i * m_vSum * m_mesh.getWidthSum();
		for(int j = 0;j < m_mesh.getWidthSum() - 1;++ j)
		{
			if(curIndex != j / m_vSum)
			{
				curIndex = j / m_vSum;
				spLine.init(m_handlePoint[curIndex + i * m_handSumX], m_tmpHandlePoint[curIndex << 1],
					m_tmpHandlePoint[(curIndex << 1) + 1], m_handlePoint[curIndex + i * m_handSumX + 1]);
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
		int wSum = m_vSum * m_mesh.getWidthSum();
		for(int j = 0,indexJ = 0,indexW = 0;j < m_handSumY - 1;++ j, indexJ += 2, indexW += wSum)
		{
			if(j == 0)
				m_tmpHandlePoint[indexJ] = XMath::lineSlerp(m_mesh.m_v[i + indexW],m_mesh.m_v[i + indexW + wSum], ONE_THIRD);
			else
			{
				tmpVec2[0] = XMath::lineSlerp(m_mesh.m_v[i + indexW - wSum], m_mesh.m_v[i + indexW], TWO_THIRD);	//与前一个点的中点
				tmpVec2[1] = XMath::lineSlerp(m_mesh.m_v[i + indexW], m_mesh.m_v[i + indexW + wSum], ONE_THIRD);	//与后一个点的中点
			//	tmpVec2[2] = (tmpVec2[0] + tmpVec2[1]) * 0.5f;
			//	m_tmpHandlePoint[indexJ] = tmpVec2[1] + m_mesh.m_v[i + indexW] - tmpVec2[2];
				m_tmpHandlePoint[indexJ] = (tmpVec2[1] - tmpVec2[0]) * 0.5f + m_mesh.m_v[i + indexW];
			}
			if(j == m_handSumY - 2)
				m_tmpHandlePoint[indexJ + 1] = XMath::lineSlerp(m_mesh.m_v[i + indexW],m_mesh.m_v[i + indexW + wSum], TWO_THIRD);
			else
			{
				tmpVec2[0] = XMath::lineSlerp(m_mesh.m_v[i + indexW], m_mesh.m_v[i + indexW + wSum], TWO_THIRD);		//与后1个点的中点
				tmpVec2[1] = XMath::lineSlerp(m_mesh.m_v[i + indexW + wSum], m_mesh.m_v[i + indexW + wSum + wSum], ONE_THIRD);	//后2个点的中点
			//	tmpVec2[2] = (tmpVec2[0] + tmpVec2[1]) * 0.5f;
			//	m_tmpHandlePoint[indexJ + 1] = tmpVec2[0] + m_mesh.m_v[i + indexW + wSum] - tmpVec2[2];
				m_tmpHandlePoint[indexJ + 1] = (tmpVec2[0] - tmpVec2[1]) * 0.5f + m_mesh.m_v[i + indexW + wSum];
			}
		}
		//下面对一条线进行插值计算
		int curIndex = -1;
		for(int j = 0;j < m_mesh.getHeightSum() - 1;++ j)
		{
			if(curIndex != j / m_vSum)
			{
				curIndex = j / m_vSum;
				spLine.init(m_mesh.m_v[curIndex * wSum + i],m_tmpHandlePoint[curIndex << 1],
					m_tmpHandlePoint[(curIndex << 1) + 1],m_mesh.m_v[(curIndex + 1) * wSum + i]);
			}
#ifdef OPTIMIZATION
			m_mesh.m_v[i + j * m_mesh.getWidthSum()] = spLine.getBezierSplineValue((j % m_vSum) / (float)(m_vSum));
#else
			m_mesh.m_v[i + j * m_mesh.getWidthSum()] = spLine.getBezierSplineValue(spLine.getT((j % m_vSum) / (float)(m_vSum)));
#endif
		}
	}
}
bool XMesh2DBz::save(const char *filename)const 
{
	if(!m_isInited) return false;
	TiXmlElement elmRoot("root");
	if (!XXml::addLeafNode(elmRoot, "handX", m_handSumX)) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "handY", m_handSumY)) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "vSum", m_vSum)) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "width", m_w)) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "height", m_h)) return XFalse;
	char tmpStr[128];
	for(int i = 0;i < m_handSumX * m_handSumY;++ i)
	{
		sprintf_s(tmpStr,128,"handle%04d",i);
		if (!XXml::addLeafNode(elmRoot, tmpStr, m_handlePoint[i])) return XFalse;
	}
	std::string name;
	if(filename == NULL) name = DEFAULT_BD_FILENAME;
	else name = filename;

	TiXmlDocument doc(name + ".xml");
	TiXmlDeclaration declaration("1.0","gb2312","yes");
	doc.InsertEndChild(declaration);
	if(doc.InsertEndChild(elmRoot) == NULL) return XFalse;
	if(!doc.SaveFile()) return XFalse;

	if(!m_mesh.saveToFile((name + ".dat").c_str())) return false;
	return true;
}//将数据保存到文件
bool XMesh2DBz::saveB(const char *filename)const 
{
	if(!m_isInited) return false;
	std::string name;
	if (filename == NULL) name = DEFAULT_BD_FILENAME;
	else name = filename;

	FILE * fp = NULL;
	if ((fp = fopen((name + ".bin").c_str(), "wb")) == NULL) return false;
	fwrite(&m_handSumX, sizeof(m_handSumX), 1, fp);
	fwrite(&m_handSumY, sizeof(m_handSumY), 1, fp);
	fwrite(&m_vSum, sizeof(m_vSum), 1, fp);
	fwrite(&m_w, sizeof(m_w), 1, fp);
	fwrite(&m_h, sizeof(m_h), 1, fp);
	//写入各个点的信息
	for (int i = 0; i < m_handSumX * m_handSumY; ++i)
	{
		fwrite(&m_handlePoint[i].x, sizeof(m_handlePoint[i].x), 1, fp);
		fwrite(&m_handlePoint[i].y, sizeof(m_handlePoint[i].y), 1, fp);
	}
	fclose(fp);

	if(!m_mesh.saveToFileB((name + ".dat").c_str())) return false;
	return true;
}//将数据保存到文件
bool XMesh2DBz::load(const char *filename) //从文件中读取数据
{//没有检测读取参数的合法性，可能会造成crash
	std::string name;
	if (filename == NULL) name = DEFAULT_BD_FILENAME;
	else name = filename;

	TiXmlDocument doc;
	if (!doc.LoadFile(name + ".xml")) return XFalse;
	TiXmlNode *rootNode = doc.FirstChild("root");
	if (rootNode == NULL) return XFalse;
	//下面开始读取数据
	XXml::getXmlAsInt(rootNode, "handX", m_handSumX);
	XXml::getXmlAsInt(rootNode, "handY", m_handSumY);
	XXml::getXmlAsInt(rootNode, "vSum", m_vSum);
	XXml::getXmlAsInt(rootNode, "width", m_w);
	XXml::getXmlAsInt(rootNode, "height", m_h);
	//下面依次读取所有点
	m_handlePoint.resize(m_handSumX * m_handSumY);
	char tmpStr[128];
	for (int i = 0; i < m_handlePoint.size(); ++i)
	{
		sprintf_s(tmpStr, 128, "handle%04d", i);
		XXml::getXmlAsVec2(rootNode, tmpStr, m_handlePoint[i]);
	}
	if (m_mesh.getIsInited())
	{
		if (!m_mesh.loadFromFile((name + ".dat").c_str())) return false;
	}
	else
	{
		if (!m_mesh.init((name + ".dat").c_str())) return false;
	}
	return true;
}
bool XMesh2DBz::loadB(const char *filename) //从文件中读取数据
{//没有检测读取参数的合法性，可能会造成crash
	std::string name;
	if(filename == NULL) name = DEFAULT_BD_FILENAME;
	else name = filename;

	FILE* fp = NULL;
	if ((fp = fopen((name + ".bin").c_str(), "rb")) == NULL) return false;
	fread(&m_handSumX, sizeof(m_handSumX), 1, fp);
	fread(&m_handSumY, sizeof(m_handSumY), 1, fp);
	fread(&m_vSum, sizeof(m_vSum), 1, fp);
	fread(&m_w, sizeof(m_w), 1, fp);
	fread(&m_h, sizeof(m_h), 1, fp);
	//写入各个点的信息
	m_handlePoint.resize(m_handSumX * m_handSumY);
	for (int i = 0; i < m_handlePoint.size(); ++i)
	{
		fread(&m_handlePoint[i].x, sizeof(m_handlePoint[i].x), 1, fp);
		fread(&m_handlePoint[i].y, sizeof(m_handlePoint[i].y), 1, fp);
	}
	fclose(fp);

	if(m_mesh.getIsInited())
	{
		if(!m_mesh.loadFromFileB((name + ".dat").c_str())) return false;
	}else
	{
		if(!m_mesh.initB((name + ".dat").c_str())) return false;
	}
	return true;
}
void XMesh2DBz::drawWireframeEx(bool special,bool onlyHandle)const
{
	if(special)
	{//需要描绘特殊信息
		if(!onlyHandle)
			m_mesh.drawWireframeEx(special,false);
		int index = 0;
		for(int h = 0;h < m_handSumY;++ h)
		{
			for(int w = 0;w < m_handSumX;++ w,++ index)
			{
				if(index == m_specialIndex)
				{//特殊点
					XRender::drawFillRing(m_handlePoint[index], 20, 30, 0, 360, 60, XFColor::green);
					XRender::drawCross(m_handlePoint[index], 10, 1, XFColor::green);
					continue;
				}
				if(h == m_specialLine || w == m_specialRow)
				{
					XRender::drawFillRing(m_handlePoint[index], 20, 30, 0, 360, 60, XFColor::green);
					XRender::drawCross(m_handlePoint[index], 10, 1, XFColor::green);
					continue;
				}
				XFColor c = XColorSpace::getWheelColor((float)(index) / (float)(m_vSum) * 360.0f);
				XRender::drawFillRing(m_handlePoint[index], 10, 20, 0, 360, 60, c);
				XRender::drawCross(m_handlePoint[index], 10, 1, c);
				c = XColorSpace::getWheelColor(XMath::angleReduce((float)(index) / (float)(m_vSum) * 360.0f + 180.0f));
				XEG.drawFont(m_handlePoint[index], XString::toString(index).c_str(), c);
			}
		}
	}else
	{//不需要描绘特殊信息
		m_mesh.drawWireframe();
	//	for(int i = 0;i < m_handSumX * m_handSumY;++ i)
	//	{
	//		XRender::drawBox(m_handlePoint[i].x,m_handlePoint[i].y,10,10,1,1,0,0);
	//	}
	}
}
void XMesh2DBz::drawWireframeEx(const XVec2& base, float scale, bool withWireframe, bool withHandle)const
{
	if (withWireframe)
	{
		m_mesh.drawWireframeEx(true, false);
		for (int h = 0,index = 0; h < m_handSumY; ++h)
		{
			for (int w = 0; w < m_handSumX; ++w, ++index)
			{
				if (index == m_specialIndex)
				{//特殊点
					XRender::drawCross(m_handlePoint[index], 10, 5, XFColor::green);
					continue;
				}
				if (h == m_specialLine || w == m_specialRow)
				{
					XRender::drawCross(m_handlePoint[index], 10, 5, XFColor::green);
					continue;
				}
				XRender::drawCross(m_handlePoint[index], 10, 5, XFColor::blue);
			}
		}
	}
	if (withHandle)
	{
		for (int h = 0, index = 0; h < m_handSumY; ++h)
		{
			for (int w = 0; w < m_handSumX; ++w, ++index)
			{
				XVec2 pos = (m_handlePoint[index] - base) * scale + base;
				if (index == m_specialIndex)
				{//特殊点
					XRender::drawFillRing(pos, 20, 30, 0, 360, 60, XFColor::green);
					XRender::drawCross(pos, 10, 1, XFColor::green);
					continue;
				}
				if (h == m_specialLine || w == m_specialRow)
				{
					XRender::drawFillRing(pos, 20, 30, 0, 360, 60, XFColor::green);
					XRender::drawCross(pos, 10, 1, XFColor::green);
					continue;
				}
				XFColor c = XColorSpace::getWheelColor((float)(index) / (float)(m_vSum) * 360.0f);
				XRender::drawFillRing(pos, 10, 20, 0, 360, 60, c);
				XRender::drawCross(pos, 10, 1, c);
				c = XColorSpace::getWheelColor(XMath::angleReduce((float)(index) / (float)(m_vSum) * 360.0f + 180.0f));
				XEG.drawFont(pos, XString::toString(index).c_str(), c);
			}
		}
	}
}
}