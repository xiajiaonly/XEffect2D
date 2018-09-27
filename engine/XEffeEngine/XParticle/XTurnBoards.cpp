#include "XStdHead.h"
#include "XTurnBoards.h"
#include "XFX\XFXCommon.h"

namespace XE
{
bool XTurnBoards::initBoards(unsigned int *initTex, int dist)
{
#if TURN_METHOD == 0
	m_boardsSmall = XMem::createArrayMem<XPicRect>(m_sumX * m_sumY);
#endif
#if TURN_METHOD == 1
	m_boardsSmall = XMem::createArrayMem<XPicRectEx>(m_sumX * m_sumY);
#endif
	m_boardsSmallFlag.resize(m_sumX * m_sumY);
	for (int i = 0; i < m_boardsSmallFlag.size(); ++i)
	{
		m_boardsSmallFlag[i] = false;
	}

	//下面对变量进行初始化
	for (int h = 0, index = 0; h < m_sumY; h++)
	{
		for (int w = 0; w < m_sumX; w++, ++index)
		{
#if TURN_METHOD == 0
			//m_boardsSmall[index].init(XVec2(m_boardSize), XVec2(w,h) * m_boardSize, XVec2(w, h) * m_boardSize,
			//	initTex, XVec2(m_texWidth, m_texHeight), XVec2(128), &m_shaderTexs[0]);
			m_boardsSmall[index].init(m_boardSize - XVec2(dist * 2), XVec2(dist) + XVec2(w, h) * m_boardSize,
				XVec2(w, h) * m_boardSize, initTex, XVec2(m_texWidth, m_texHeight), XVec2(128), &m_shaderTexs[0]);
#endif
#if TURN_METHOD == 1
			m_boardsSmall[index].init(m_boardSize - XVec2(dist * 2), XVec2(dist) + XVec2(w, h) * m_boardSize,
				XVec2(w, h) * m_boardSize, initTex, XVec2(m_texWidth, m_texHeight), &m_shaderTexs[0]);
#endif
			m_boardsSmall[index].setTurnSpeed(0.2f);
		}
	}
	m_boards = m_boardsSmall;
	m_boardsFlag = &m_boardsSmallFlag;
	if (dist == 0)
	{//当间隙很小的时候可以内部切换变换方式
		//水平的
#if TURN_METHOD == 0
		m_boardsWidth = XMem::createArrayMem<XPicRect>(m_sumY);
#endif
#if TURN_METHOD == 1
		m_boardsWidth = XMem::createArrayMem<XPicRectEx>(m_sumY);
#endif
		m_boardsWidthFlag.resize(m_sumY);
		for (int i = 0; i < m_boardsWidthFlag.size(); ++i)
		{
			m_boardsWidthFlag[i] = false;
		}

		//下面对变量进行初始化
		for (int h = 0; h < m_sumY; h++)
		{
#if TURN_METHOD == 0
			m_boardsWidth[h].init(XVec2(m_texWidth, m_boardSize.y), XVec2(0.0f, h * m_boardSize.y),
				XVec2(0.0f, h * m_boardSize.y), initTex, XVec2(m_texWidth, m_texHeight), XVec2(128), &m_shaderTexs[0]);
#endif
#if TURN_METHOD == 1
			m_boardsWidth[h].init(XVec2(m_texWidth, m_boardSize.y), XVec2(0.0f, h * m_boardSize.y),
				XVec2(0.0f, h * m_boardSize.y), initTex, XVec2(m_texWidth, m_texHeight), &m_shaderTexs[0]);
#endif
			m_boardsWidth[h].setTurnSpeed(0.2f);
		}
		//垂直的
#if TURN_METHOD == 0
		m_boardsHeight = XMem::createArrayMem<XPicRect>(m_sumX);
#endif
#if TURN_METHOD == 1
		m_boardsHeight = XMem::createArrayMem<XPicRectEx>(m_sumX);
#endif
		m_boardsHeightFlag.resize(m_sumX);
		for (int i = 0; i < m_boardsHeightFlag.size(); ++i)
		{
			m_boardsHeightFlag[i] = false;
		}

		//下面对变量进行初始化
		for (int w = 0; w < m_sumX; w++)
		{
#if TURN_METHOD == 0
			m_boardsHeight[w].init(XVec2(m_boardSize.x, m_texHeight), XVec2(w * m_boardSize.x, 0.0f),
				XVec2(w * m_boardSize.x, 0.0f), initTex, XVec2(m_texWidth, m_texHeight), XVec2(128), &m_shaderTexs[0]);
#endif
#if TURN_METHOD == 1
			m_boardsHeight[w].init(XVec2(m_boardSize.x, m_texHeight), XVec2(w * m_boardSize.x, 0.0f),
				XVec2(w * m_boardSize.x, 0.0f), initTex, XVec2(m_texWidth, m_texHeight), &m_shaderTexs[0]);
#endif
			m_boardsHeight[w].setTurnSpeed(0.2f);
		}
	}
	return true;
}
bool XTurnBoards::init(int texW, int texH, const XVec2& boardSize,unsigned int *initTex, int dist)
{
	if (m_isInited || initTex == nullptr) return true;

	m_texWidth = texW;
	m_texHeight = texH;
	m_boardSize = boardSize;

	m_sumX = texW / boardSize.x;
	m_sumY = texH / boardSize.y;

	if (!m_shaderTex[0].load("ResourcePack/ShadowUp.png")) return false;
	if (!m_shaderTex[1].load("ResourcePack/ShadowDown.png")) return false;
	if (!m_shaderTex[2].load("ResourcePack/ShadowLeft.png")) return false;
	if (!m_shaderTex[3].load("ResourcePack/ShadowRight.png")) return false;
	m_shaderTexs.push_back(m_shaderTex[0].getTexGLID());
	m_shaderTexs.push_back(m_shaderTex[1].getTexGLID());
	m_shaderTexs.push_back(m_shaderTex[2].getTexGLID());
	m_shaderTexs.push_back(m_shaderTex[3].getTexGLID());

	if (!initBoards(initTex, dist)) return false;
	if (!m_fbo.init(m_texWidth, m_texHeight, COLOR_RGB)) return false;
#ifdef WITH_ALPHA
	m_fbo.addOneTexture(m_texWidth, m_texHeight, COLOR_RGB);
	m_fbo.addOneTexture(m_texWidth, m_texHeight, COLOR_RGB);
	m_fbo.addOneTexture(m_texWidth, m_texHeight, COLOR_RGBA);
	m_tex[0] = m_fbo.getTexture(1);
	m_tex[1] = m_fbo.getTexture(2);
	std::string frg = STRINGIFY(uniform sampler2D Texture_00;	//最终的黑图
	uniform sampler2D Texture_01;	//最终白图
	uniform sampler2D Texture_02;	//alpha黑图
	varying vec2 TexCoord;			//当前ps的位置
	void main()
	{//多纹理混合的shader，尚未实现
		float color0 = texture2D(Texture_00, TexCoord).r;
		float color1 = texture2D(Texture_01, TexCoord).r;
		float a = 1.0 - (color1 - color0);
		vec4 color3 = texture2D(Texture_02, TexCoord).rgba;

		gl_FragColor = vec4(color3.r / a, color3.g / a, color3.b / a, a);
	});
	m_shader.initFromStr(XFX::gCommonVrt.c_str(),frg.c_str());
	m_shader.connectTexture("Texture_00", nullptr);
	m_shader.connectTexture("Texture_01", m_tex);
	m_shader.connectTexture("Texture_02", m_tex + 1);
#endif

	m_sprite.init(m_texWidth, m_texHeight, 0);
	m_isInited = true;
	return true;
}
void XTurnBoards::update(float steptime)
{
	if (!m_isInited) return;
	float x, y;
	if (m_isInChangeImg)
	{//下面是变化的设置
		switch (m_turnMode)
		{
		case MODE_UP:
			m_changeRate[0].move(steptime);
			for (int i = 0; i < (*m_boardsFlag).size(); ++i)
			{
				y = m_boards[i].getCenterPos().y;
				if (!(*m_boardsFlag)[i] && y < m_changeRate[0].getCurData())
				{
					if(m_setState == PIC_STA_NULL) m_boards[i].setCurState(PIC_STA_UP_TURN_ENTER);
					else m_boards[i].setCurState(m_setState);
					//Sound.play(SoundHandle);
					(*m_boardsFlag)[i] = true;
				}
			}
			break;
		case MODE_DOWN:
			m_changeRate[0].move(steptime);
			for (int i = 0; i < (*m_boardsFlag).size(); ++i)
			{
				y = m_boards[i].getCenterPos().y;
				if (!(*m_boardsFlag)[i] && (XEG.getSceneH() - y) < m_changeRate[0].getCurData())
				{
					if (m_setState == PIC_STA_NULL) m_boards[i].setCurState(PIC_STA_DOWN_TURN_ENTER);
					else m_boards[i].setCurState(m_setState);
					//Sound.play(SoundHandle);
					(*m_boardsFlag)[i] = true;
				}
			}
			break;
		case MODE_LEFT:
			m_changeRate[0].move(steptime);
			for (int i = 0; i < (*m_boardsFlag).size(); ++i)
			{
				x = m_boards[i].getCenterPos().x;
				if (!(*m_boardsFlag)[i] && (XEG.getSceneW() - x) < m_changeRate[0].getCurData())
				{
					if (m_setState == PIC_STA_NULL) m_boards[i].setCurState(PIC_STA_LEFT_TURN_ENTER);
					else m_boards[i].setCurState(m_setState);
					//Sound.play(SoundHandle);
					(*m_boardsFlag)[i] = true;
				}
			}
			break;
		case MODE_RIGHT:
			m_changeRate[0].move(steptime);
			for (int i = 0; i < (*m_boardsFlag).size(); ++i)
			{
				x = m_boards[i].getCenterPos().x;
				if (!(*m_boardsFlag)[i] && x < m_changeRate[0].getCurData())
				{
					if (m_setState == PIC_STA_NULL) m_boards[i].setCurState(PIC_STA_RIGHT_TURN_ENTER);
					else m_boards[i].setCurState(m_setState);
					//Sound.play(SoundHandle);
					(*m_boardsFlag)[i] = true;
				}
			}
			break;
		case MODE_POINT:
			for (int k = 0; k < m_changeRate.size(); ++k)
			{
				m_changeRate[k].move(steptime);
				for (int i = 0; i < (*m_boardsFlag).size(); ++i)
				{
					x = m_boards[i].getCenterPos().x - m_changeCenter[k].x;
					y = m_boards[i].getCenterPos().y - m_changeCenter[k].y;

					if (!(*m_boardsFlag)[i] && x * x + y * y <
						m_changeRate[k].getCurData() * m_changeRate[k].getCurData())
					{
						if (m_setState == PIC_STA_NULL)
						{
							if (x*x > y*y)
							{
								if (x > 0)
									m_boards[i].setCurState(PIC_STA_LEFT_TURN_ENTER);
								else
									m_boards[i].setCurState(PIC_STA_RIGHT_TURN_ENTER);
							}
							else
							{
								if (y > 0)
									m_boards[i].setCurState(PIC_STA_UP_TURN_ENTER);
								else
									m_boards[i].setCurState(PIC_STA_DOWN_TURN_ENTER);
							}
						}
						else
						{
							m_boards[i].setCurState(m_setState);
						}
						//Sound.play(SoundHandle);
						(*m_boardsFlag)[i] = true;
					}
				}
			}
			break;
		case MODE_MANUAL:	//如果翻的比例超过80%则自动全翻
			x = 0.0f;
			for (int i = 0; i < (*m_boardsFlag).size(); ++i)
			{
				if ((*m_boardsFlag)[i]) x += 1.0f;
			}
			if (x / (*m_boardsFlag).size() > 0.8)
			{
				for (int i = 0; i < (*m_boardsFlag).size(); ++i)
				{
					if ((*m_boardsFlag)[i]) continue;
					(*m_boardsFlag)[i] = true;
					switch (XRand::random(DIR_MAX))
					{
					case DIR_UP:
						m_boards[i].setCurState(PIC_STA_UP_TURN_ENTER);
						break;
					case DIR_DOWN:
						m_boards[i].setCurState(PIC_STA_DOWN_TURN_ENTER);
						break;
					case DIR_LEFT:
						m_boards[i].setCurState(PIC_STA_LEFT_TURN_ENTER);
						break;
					case DIR_RIGHT:
						m_boards[i].setCurState(PIC_STA_RIGHT_TURN_ENTER);
						break;
					}
				}
			}
			break;
		}
		//判断动作是否完成
		bool isOver = true;
		for (int i = 0; i < (*m_boardsFlag).size(); ++i)
		{
			if (!(*m_boardsFlag)[i] || m_boards[i].getCurState() != PIC_STA_NORMAL)
			{
				isOver = false;
				break;
			}
		}
		if (isOver)
		{//变化结束而定义的
			LogStr("反转完成!");
			m_isInChangeImg = false;
			m_boards = m_boardsSmall;
			m_boardsFlag = &m_boardsSmallFlag;
			//下面设置恢复状态
			switch (m_setState)
			{
			case XE::PIC_STA_UP_TURN_ENTER:
			case XE::PIC_STA_DOWN_TURN_ENTER:
				for (int i = 0; i < (*m_boardsFlag).size(); ++i)
				{
					m_boards[i].setCurTex(m_boardsHeight[0].getCurTex());
					m_boards[i].setChangeTex(m_boardsHeight[0].getCurTex());
				}
				m_setState = PIC_STA_NULL;
				break;
			case XE::PIC_STA_LEFT_TURN_ENTER:
			case XE::PIC_STA_RIGHT_TURN_ENTER:
				for (int i = 0; i < (*m_boardsFlag).size(); ++i)
				{
					m_boards[i].setCurTex(m_boardsWidth[0].getCurTex());
					m_boards[i].setChangeTex(m_boardsWidth[0].getCurTex());
				}
				m_setState = PIC_STA_NULL;
				break;
			}
		}
	}
	for (int i = 0; i < (*m_boardsFlag).size(); ++i)
	{
		m_boards[i].move(steptime);
	}
}
void XTurnBoards::draw()
{
	if (!m_isInited) return;
#ifdef WITH_ALPHA
	m_fbo.useFBO();
	m_fbo.attachTex(0);
	XEG.clearScreen(XFColor::black);
	for (int i = 0; i < (*m_boardsFlag).size(); ++i)
	{
		m_boards[i].draw(false);
	}
	m_fbo.attachTex(1);
	XEG.clearScreen(XFColor::white);
	for (int i = 0; i < (*m_boardsFlag).size(); ++i)
	{
		m_boards[i].draw(false);
	}
	m_fbo.attachTex(2);
	XEG.clearScreen(XFColor::black);
	for (int i = 0; i < (*m_boardsFlag).size(); ++i)
	{
		m_boards[i].draw(true);
	}
	m_fbo.attachTex(3);
	XEG.clearScreen(XFColor::black);
	XRender::drawBlankPlane(m_texWidth, m_texHeight, m_fbo.getTexture(0), &m_shader);
	m_fbo.removeFBO();
	XGL::setBlendAlpha();
	//XRender::drawBlankPlane(m_texWidth, m_texHeight, m_fbo.getTexture(3));
	m_sprite.draw(m_fbo.getTexture(3));
#else
	m_fbo.useFBO();
	for (int i = 0; i < (*m_boardsFlag).size(); ++i)
	{
		m_boards[i].draw();
	}
	m_fbo.removeFBO();
	m_sprite.draw(m_fbo.getTexture());
#endif
}
void XTurnBoards::turnBoard(const XVec2& pos, XTurnBoardDir dir)
{
	if (!m_isInited) return;
	if (m_isInChangeImg)
	{
		if (m_turnMode != MODE_MANUAL) return;
		//计算当前板子的编号
		int x = pos.x / m_boardSize.x;
		int y = pos.y / m_boardSize.y;
		int index = x + y * m_sumX;
		if (index < 0 || index >= (*m_boardsFlag).size()) return;
		if (m_boards[index].getCurState() != PIC_STA_NORMAL) return;
		if ((*m_boardsFlag)[index]) return;	//如果已经翻过了，则不能再翻了

		switch (dir)
		{
		case DIR_UP:
			m_boards[index].setCurState(PIC_STA_UP_TURN_ENTER);
			break;
		case DIR_DOWN:
			m_boards[index].setCurState(PIC_STA_DOWN_TURN_ENTER);
			break;
		case DIR_LEFT:
			m_boards[index].setCurState(PIC_STA_LEFT_TURN_ENTER);
			break;
		case DIR_RIGHT:
			m_boards[index].setCurState(PIC_STA_RIGHT_TURN_ENTER);
			break;
		}
		(*m_boardsFlag)[index] = true;
	}
	else
	{
		//计算当前板子的编号
		int x = pos.x / m_boardSize.x;
		int y = pos.y / m_boardSize.y;
		int index = x + y * m_sumX;
		if (index < 0 || index >= (*m_boardsFlag).size()) return;
		if (m_boards[index].getCurState() != PIC_STA_NORMAL) return;

		switch (dir)
		{
		case DIR_UP:
			m_boards[index].setCurState(PIC_STA_UP_TURN_ENTER);
			break;
		case DIR_DOWN:
			m_boards[index].setCurState(PIC_STA_DOWN_TURN_ENTER);
			break;
		case DIR_LEFT:
			m_boards[index].setCurState(PIC_STA_LEFT_TURN_ENTER);
			break;
		case DIR_RIGHT:
			m_boards[index].setCurState(PIC_STA_RIGHT_TURN_ENTER);
			break;
		}
	}
}
bool XTurnBoards::getCanChangeImg()const
{
	if (!m_isInited || m_isInChangeImg) return false;
	//检查所有的板子是否都处于静止状态
	for (int i = 0; i < (*m_boardsFlag).size(); ++i)
	{
		if (m_boards[i].getCurState() != PIC_STA_NORMAL) return false;
	}
	return true;
}
bool XTurnBoards::changeImage(unsigned int *targetTex, XTurnBoardMode mode, const XVec2& center)
{
	if (!getCanChangeImg()) return false;
	//初始化一些属性
	switch (mode)
	{
	case MODE_DIRECT:
		for (int i = 0; i < (*m_boardsFlag).size(); ++i)
		{
			//(*m_boardsFlag)[i] = false;
			m_boards[i].setChangeTex(targetTex);
			int rate = XRand::random(100);
			if (rate < 25) m_boards[i].setCurState(PIC_STA_LEFT_TURN_ENTER); else
			if (rate < 50) m_boards[i].setCurState(PIC_STA_RIGHT_TURN_ENTER); else
			if (rate < 75) m_boards[i].setCurState(PIC_STA_UP_TURN_ENTER); else
				m_boards[i].setCurState(PIC_STA_DOWN_TURN_ENTER);
		}
		break;
	default:
		XMoveDataMode modes[] = { MD_MODE_LINE,MD_MODE_SIN_MULT ,MD_MODE_COS_MULT,MD_MODE_SQRT2_MULT, };
		XMoveData tmpMD;
		switch (mode)
		{
		case MODE_UP:
		case MODE_DOWN:
			if (m_boardsWidth != nullptr && XRand::random(100) < 50)
			{
				if(XRand::random(100) < 50) m_setState = PIC_STA_LEFT_TURN_ENTER;
				else m_setState = PIC_STA_RIGHT_TURN_ENTER;

				m_boards = m_boardsWidth;
				m_boardsFlag = &m_boardsWidthFlag;
				for (int i = 0; i < (*m_boardsFlag).size(); ++i)
				{
					m_boards[i].setCurTex(m_boardsSmall[0].getCurTex());
				}
			}
			tmpMD.setEx(0.0f, m_texHeight * 1.05f, m_changeTime, modes[XRand::random(3)]);
			break;
		case MODE_LEFT:
		case MODE_RIGHT:
			if (m_boardsWidth != nullptr && XRand::random(100) < 50)
			{
				if (XRand::random(100) < 50) m_setState = PIC_STA_UP_TURN_ENTER;
				else m_setState = PIC_STA_DOWN_TURN_ENTER;

				m_boards = m_boardsHeight;
				m_boardsFlag = &m_boardsHeightFlag;
				for (int i = 0; i < (*m_boardsFlag).size(); ++i)
				{
					m_boards[i].setCurTex(m_boardsSmall[0].getCurTex());
				}
			}
			tmpMD.setEx(0.0f, m_texWidth * 1.05f, m_changeTime,modes[XRand::random(3)]);
			break;
		case MODE_POINT:
			if (m_boardsWidth != nullptr)
			{
				switch (XRand::random(3))
				{
				case 0:
					if (XRand::random(100) < 50) m_setState = PIC_STA_LEFT_TURN_ENTER;
					else m_setState = PIC_STA_RIGHT_TURN_ENTER;

					m_boards = m_boardsWidth;
					m_boardsFlag = &m_boardsWidthFlag;
					for (int i = 0; i < (*m_boardsFlag).size(); ++i)
					{
						m_boards[i].setCurTex(m_boardsSmall[0].getCurTex());
					}
					break;
				case 1:
					if (XRand::random(100) < 50) m_setState = PIC_STA_UP_TURN_ENTER;
					else m_setState = PIC_STA_DOWN_TURN_ENTER;

					m_boards = m_boardsHeight;
					m_boardsFlag = &m_boardsHeightFlag;
					for (int i = 0; i < (*m_boardsFlag).size(); ++i)
					{
						m_boards[i].setCurTex(m_boardsSmall[0].getCurTex());
					}
					break;
				default:
					break;
				}
			}
			tmpMD.setEx(0.0f, XVec2(m_texWidth,m_texHeight).getLength() * 1.05f, m_changeTime,MD_MODE_SIN_MULT);
			break;
		}
		for (int i = 0; i < (*m_boardsFlag).size(); ++i)
		{
			(*m_boardsFlag)[i] = false;
			m_boards[i].setChangeTex(targetTex);
		}
		m_changeRate.clear();
		m_changeRate.push_back(tmpMD);

		m_changeCenter.clear();
		m_changeCenter.push_back(center);
		m_turnMode = mode;
		m_isInChangeImg = true;
		break;
	}

	return true;
}
bool XTurnBoards::addACenter(const XVec2& c)
{
	if (m_turnMode != MODE_POINT || !m_isInChangeImg) return false;
	XMoveData tmpMD;
	tmpMD.setEx(0.0f, XVec2(m_texWidth, m_texHeight).getLength() * 1.05f, MD_MODE_SIN_MULT);
	m_changeRate.push_back(tmpMD);
	m_changeCenter.push_back(c);
	return true;
}
void XTurnBoards::release()
{
	if (!m_isInited) return;
	XMem::XDELETE_ARRAY(m_boardsSmall);
	XMem::XDELETE_ARRAY(m_boardsWidth);
	XMem::XDELETE_ARRAY(m_boardsHeight);
	m_isInited = false;
}
}