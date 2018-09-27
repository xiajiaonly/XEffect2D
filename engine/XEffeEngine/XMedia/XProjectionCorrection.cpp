#include "XStdHead.h"
#include "XInputEventCore.h"
#include "XProjectionCorrection.h"
#include "XMath\XMatrix2x2.h"
#include "XFX\XFXCommon.h"
namespace XE{
char XPCShaderFrg[] = STRINGIFY(uniform sampler2D Texture_00;	//草地的图
varying vec2 TexCoord;			//当前ps的位置
void main()
{//多纹理混合的shader，尚未实现
	vec4 colorG = texture2D(Texture_00,TexCoord).rgba;
	gl_FragColor = vec4(0,0,0,colorG.r);
});
void XProjectionCorrection::init(const XVec2& size,const XVec2& pos,const XVec2& scale,bool withFBO,XProjectionCorMode mode)
{
	if(m_isInited) return;
	m_position = pos;
	m_scale = scale;
	m_size = size;
	m_withFBO = withFBO;
	initCtrl();
	loadData();
	m_mode = mode;
	if(m_mode == PC_MODE_FILL || m_mode == PC_MODE_TEXTURE_FILL)
	{
		m_fillShader.initFromStr(XFX::gCommonVrt.c_str(),XPCShaderFrg);
		m_fillShader.connectTexture("Texture_00",NULL);
	}
	m_isInited = true;
}
void XProjectionCorrection::init(bool withFBO,XProjectionCorMode mode)
{
	if(m_isInited) return;
	m_position.reset();
	m_scale.set(0.5f);
	m_size.set(XEG.getSceneW(),XEG.getSceneH());
	m_withFBO = withFBO;
	initCtrl();
	loadData();
	m_mode = mode;
	if(m_mode == PC_MODE_FILL || m_mode == PC_MODE_TEXTURE_FILL)
	{
		m_fillShader.initFromStr(XFX::gCommonVrt.c_str(),XPCShaderFrg);
		m_fillShader.connectTexture("Texture_00",NULL);
	}
	m_isInited = true;
}
void XProjectionCorrection::initCtrl()
{
	m_pointPcl[0].init(m_position + m_size * XVec2(0.5f * (1.0f - m_scale.x)));
	m_pointPcl[0].disVisible();
	m_pointPcl[0].setColor(XFColor::red);
	m_pointPcl[1].init(m_position + m_size * XVec2(1.0f - m_scale.x,1.0f + m_scale.x) * 0.5f);
	m_pointPcl[1].disVisible();
	m_pointPcl[1].setColor(XFColor::green);
	m_pointPcl[2].init(m_position + m_size * XVec2(0.5f * (1.0f + m_scale.x)));
	m_pointPcl[2].disVisible();
	m_pointPcl[2].setColor(XFColor::blue);
	m_pointPcl[3].init(m_position + m_size * XVec2(1.0f + m_scale.x,1.0f - m_scale.x) * 0.5f);
	m_pointPcl[3].disVisible();
	m_pointPcl[3].setColor(XFColor::yellow);
	for(int i = 0;i < 4;++ i)
	{
		m_pointsPos.push_back(m_pointPcl[i].getPosition());
	}
	if(m_withFBO)
		m_fbo.init(m_size.x, m_size.y);// , COLOR_RGBA, false, GL_LINEAR, 2);
}
void XProjectionCorrection::setIsShowCtrl(bool flag)
{
	m_isShowCtrl = flag;
	if(m_isShowCtrl)
	{
		for(int i = 0;i < 4;++ i)
		{
			m_pointPcl[i].setVisible();
		}
		if(!XEG.m_windowData.isShowCursor)
			XWindow.setCurcor(true);
	}else
	{
		m_curAction = PC_ACTION_NULL;
		for(int i = 0;i < 4;++ i)
		{
			m_pointPcl[i].disVisible();
		}
		if(!XEG.m_windowData.isShowCursor)
			XWindow.setCurcor(false);
	}
}
bool XProjectionCorrection::inputProc(const XInputEvent &event)	//对输入命令的响应
{
	if(!m_isInputEnable) return false;
	if(event.isKeyBoardDown())
	{
		switch (event.keyValue)
		{
		case XKEY_SPACE:
			setIsShowCtrl(!m_isShowCtrl);
			return true;
		case XKEY_B:
			if(m_isShowCtrl)
			{
				if(saveData()) XEG.setTopMsgStr("矫正参数保存成功");
				else XEG.setTopMsgStr("矫正参数保存失败");
				return true;
			}else
				return false;
		case XKEY_L:
			if(m_isShowCtrl)
			{
				if(loadData()) XEG.setTopMsgStr("矫正参数读取成功");
				else XEG.setTopMsgStr("矫正参数读取失败");
				return true;
			}else
				return false;
		case XKEY_S:
			if(m_isShowCtrl)
			{
				m_curAction = PC_ACTION_SCALE;
				return true;
			}else
				return false;//缩放
		case XKEY_M:
			if(m_isShowCtrl)
			{
				m_curAction = PC_ACTION_MOVE;
				return true;
			}else
				return false;//移动
		case XKEY_R:
			if(m_isShowCtrl)
			{
				m_curAction = PC_ACTION_ROTATION;
				return true;
			}else
				return false;//旋转
		}
	}
	if(event.isKeyBoardUp() && m_isShowCtrl)
	{
		switch (event.keyValue)
		{
		case XKEY_S:
			if(m_curAction == PC_ACTION_SCALE)
				m_curAction = PC_ACTION_NULL;
			return true;//缩放
		case XKEY_M:
			if(m_curAction == PC_ACTION_MOVE)
				m_curAction = PC_ACTION_NULL;
			return true;//移动
		case XKEY_R:
			if(m_curAction == PC_ACTION_ROTATION)
				m_curAction = PC_ACTION_NULL;
			return true;//旋转
		}
	}
	if(event.isMouseLeftDown() && m_isShowCtrl)
	{
		switch (m_curAction)
		{
		case XE::PC_ACTION_NULL:break;
		case XE::PC_ACTION_MOVE:
		case XE::PC_ACTION_SCALE:
		case XE::PC_ACTION_ROTATION:
			m_curMousePos = getMousePos();
			for(int i = 0;i < 4;++ i)
			{
				m_pointsPos[i] = m_pointPcl[i].getPosition();
			}
			break;
		}
		return true;
	}
	if(XEG.getIsLBDrag(event) && m_isShowCtrl)
	{
		XVec2 offset;
		float scale;
		switch (m_curAction)
		{
		case XE::PC_ACTION_NULL:break;
		case XE::PC_ACTION_MOVE:
			offset = getMousePos() - m_curMousePos;
			m_curMousePos = getMousePos();
			for(int i = 0;i < 4;++ i)
			{
				m_pointPcl[i].setPosition(m_pointPcl[i].getPosition() + offset);
			}
			break;
		case XE::PC_ACTION_SCALE:
			scale = getMousePos().x - m_curMousePos.x;
			if(scale > 0.0f)
			{//放大
				scale = scale / XEG.getSceneW() + 1.0f;
			}else
			{//缩小
				scale = 1.0f / (-scale / XEG.getSceneW() + 1.0f);
			}
			offset = (m_pointsPos[0] + m_pointsPos[1] + m_pointsPos[2] + m_pointsPos[3]) * 0.25f;
			for(int i = 0;i < 4;++ i)
			{
				m_pointPcl[i].setPosition((m_pointsPos[i] - offset) * scale + offset);
			}
			break;
		case XE::PC_ACTION_ROTATION:
			scale = (getMousePos().x - m_curMousePos.x) / XEG.getSceneW() * PI2;	//计算旋转的角度
			offset = (m_pointsPos[0] + m_pointsPos[1] + m_pointsPos[2] + m_pointsPos[3]) * 0.25f;
			//以中心点为基准进行旋转
			for(int i = 0;i < 4;++ i)
			{
				//m_pointPcl[i].setPosition((m_pointsPos[i] - offset) * XMath::getMatrix2D(scale) + offset);
				m_pointPcl[i].setPosition((m_pointsPos[i] - offset).rotation(scale) + offset);
			}
			break;
		}
		return true;
	}
	return false;
}
bool XProjectionCorrection::saveData(const char *filename)	//保存数据到文件
{
	std::string nameStr = m_configFileName;
	if(filename != NULL) nameStr = filename;
	TiXmlDocument doc(nameStr.c_str());
	TiXmlDeclaration declaration("1.0","gb2312","yes");
	doc.InsertEndChild(declaration);
	TiXmlElement elmRoot("root");

	XXml::addLeafNode(elmRoot, "pos0", m_pointPcl[0].getPosition());
	XXml::addLeafNode(elmRoot, "pos1", m_pointPcl[1].getPosition());
	XXml::addLeafNode(elmRoot, "pos2", m_pointPcl[2].getPosition());
	XXml::addLeafNode(elmRoot, "pos3", m_pointPcl[3].getPosition());

	if(doc.InsertEndChild(elmRoot) == NULL) return false;
	if(!doc.SaveFile()) return false;
	return true;
}
bool XProjectionCorrection::loadData(const char *filename)
{
	std::string nameStr = m_configFileName;
	if(filename != NULL) nameStr = filename;
	TiXmlDocument doc(nameStr.c_str());
	if(!doc.LoadFile()) return false;
	TiXmlNode *rootNode = doc.FirstChild("root");
	if(rootNode == NULL) return false;
	XVec2 pos;
	XXml::getXmlAsVec2(rootNode,"pos0",pos);
	m_pointPcl[0].setPosition(pos);
	XXml::getXmlAsVec2(rootNode,"pos1",pos);
	m_pointPcl[1].setPosition(pos);
	XXml::getXmlAsVec2(rootNode,"pos2",pos);
	m_pointPcl[2].setPosition(pos);
	XXml::getXmlAsVec2(rootNode,"pos3",pos);
	m_pointPcl[3].setPosition(pos);

	return true;
}
void XProjectionCorrection::draw(unsigned int tex,bool withClear)
{
	std::vector<XVec2> pointsPos(4);
	XVec2 scale(1.0f / m_scale.x,1.0f / m_scale.y);
	for(int i = 0;i < 4;++ i)
	{
		//pointsPos[i] = (m_pointPcl[i].getPosition() - XEG.getSceneCenter()) * scale + XEG.getSceneCenter();
		pointsPos[i] = (m_pointPcl[i].getPosition() - m_size * 0.5f) * scale + m_size * 0.5f;
	}
	if(m_mode == PC_MODE_TEXTURE_FILL)
	{
		m_fbo.useFBO();
		XEG.clearScreen(XFColor::white);
		XRender::drawBlankPlaneEx(pointsPos,tex,NULL,XFColor::white, m_meshSubSum);
		m_fbo.removeFBO();
		if(withClear)
			XEG.clearScreen(XFColor::black);
		XRender::drawBlankPlane(m_size,m_fbo.getTexture(0),&m_fillShader);
	}else
	{
		if(withClear)
			XEG.clearScreen(XFColor::black);
		XRender::drawBlankPlaneEx(pointsPos, tex, NULL, XFColor::white, m_meshSubSum);
	}
	if(m_isShowCtrl)
	{
		XRender::drawQuad(m_pointPcl[0].getPosition(), m_pointPcl[1].getPosition(),
			m_pointPcl[2].getPosition(), m_pointPcl[3].getPosition());
		//XRender::drawLine(m_pointPcl[0].getPosition(),m_pointPcl[1].getPosition());
		//XRender::drawLine(m_pointPcl[1].getPosition(),m_pointPcl[2].getPosition());
		//XRender::drawLine(m_pointPcl[2].getPosition(),m_pointPcl[3].getPosition());
		//XRender::drawLine(m_pointPcl[3].getPosition(),m_pointPcl[0].getPosition());
	}
}
void XProjectionCorrection::draw(bool withClear)
{
	if(!m_withFBO) return;
	switch (m_mode)
	{
	case PC_MODE_TEXTURE:
	case PC_MODE_TEXTURE_FILL:
		draw(m_fbo.getTexture(0),withClear);
		break;
	case PC_MODE_FILL:
		std::vector<XVec2> pointsPos(4);
		XVec2 scale(1.0f / m_scale.x,1.0f / m_scale.y);
		for(int i = 0;i < 4;++ i)
		{
			//pointsPos[i] = (m_pointPcl[i].getPosition() - XEG.getSceneCenter()) * scale + XEG.getSceneCenter();
			pointsPos[i] = (m_pointPcl[i].getPosition() - m_size * 0.5f) * scale + m_size * 0.5f;
		}
		m_fbo.useFBO();
		XEG.clearScreen(XFColor::white);
		XRender::drawFillQuad(pointsPos[0],pointsPos[1],pointsPos[2],pointsPos[3],0,0,0);
		m_fbo.removeFBO();
		if(withClear)
			XEG.clearScreen(XFColor::black);
		XRender::drawBlankPlane(m_size,m_fbo.getTexture(0),&m_fillShader);
		if(m_isShowCtrl)
		{
			XRender::drawLine(m_pointPcl[0].getPosition(),m_pointPcl[1].getPosition());
			XRender::drawLine(m_pointPcl[1].getPosition(),m_pointPcl[2].getPosition());
			XRender::drawLine(m_pointPcl[2].getPosition(),m_pointPcl[3].getPosition());
			XRender::drawLine(m_pointPcl[3].getPosition(),m_pointPcl[0].getPosition());
		}
		break;
	}
}
XVec2 XProjectionCorrection::mapPos(const XVec2& pos) const
{//由于新的版本中已经进行了细分，所以下面的方式也需要修改
	XVec2 dstPos[4];
	XVec2 scale(1.0f / m_scale.x,1.0f / m_scale.y);
	for(int i = 0;i < 4;++ i)
	{
		dstPos[i] = (m_pointPcl[i].getPosition() - m_size * 0.5f) * scale + m_size * 0.5f;
	}
	XMath::swapTwoSum(dstPos[1],dstPos[3]);
	XVec2 srcPos[4];
	srcPos[0].set(0.0f);
	srcPos[1].set(m_size.x,0.0f);
	srcPos[2] = m_size;
	srcPos[3].set(0.0f,m_size.y);
	if (m_meshSubSum == 1)
	{
		if (XMath::getIsInTriangle(pos, srcPos))
			return XMath::mapping2DTriangle(srcPos, dstPos, pos);
		else
		{
			XMath::swapTwoSum(dstPos[0], dstPos[1]);
			XMath::swapTwoSum(srcPos[0], srcPos[1]);
			return XMath::mapping2DTriangle(srcPos + 1, dstPos + 1, pos);
		}
	}
	else
	{
		XVec2 sul, sdl, sur, sdr;
		XVec2 dul, ddl, dur, ddr;
		XVec2 tmpS[4], tmpD[4];
		float step = 1.0f / m_meshSubSum;
		//注意：drawBlankPlaneEx的顺序与当前的顺序不符所以下面需要进行特殊的处理
		XVec2 osl = (srcPos[3] - srcPos[0]) * step;
		XVec2 osr = (srcPos[2] - srcPos[1]) * step;
		XVec2 odl = (dstPos[3] - dstPos[0]) * step;
		XVec2 odr = (dstPos[2] - dstPos[1]) * step;
		XVec2 osu, osd, odu, odd;

		sdl = srcPos[0];
		sdr = srcPos[1];
		ddl = dstPos[0];
		ddr = dstPos[1];
		for (int h = 0; h < m_meshSubSum; ++h)
		{
			sul = sdl;
			sdl += osl;
			sur = sdr;
			sdr += osr;

			dul = ddl;
			ddl += odl;
			dur = ddr;
			ddr += odr;

			osu = (sur - sul) * step;
			osd = (sdr - sdl) * step;
			odu = (dur - dul) * step;
			odd = (ddr - ddl) * step;

			tmpS[1] = sul;
			tmpS[2] = sdl;
			tmpD[1] = dul;
			tmpD[2] = ddl;

			for (int w = 0; w < m_meshSubSum; ++w)
			{
				tmpS[0] = tmpS[1];
				tmpS[3] = tmpS[2];
				tmpS[1] += osu;
				tmpS[2] += osd;

				tmpD[0] = tmpD[1];
				tmpD[3] = tmpD[2];
				tmpD[1] += odu;
				tmpD[2] += odd;
				if (XMath::getIsInPolygon(pos, tmpS, 4))
				{
					if (XMath::getIsInTriangle(pos, tmpS))
						return XMath::mapping2DTriangle(tmpS, tmpD, pos);
					else
					{
						XMath::swapTwoSum(tmpD[0], tmpD[1]);
						XMath::swapTwoSum(tmpS[0], tmpS[1]);
						return XMath::mapping2DTriangle(tmpS + 1, tmpD + 1, pos);
					}
				}
			}
		}
		return XVec2::zero;
	}
	//return XMath::mapping2D(srcPos,dstPos,pos);
}
}