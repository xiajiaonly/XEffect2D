#include "GGameTail.h"
using namespace XE;
#ifdef WITH_THREAD
void GGame::ThreadFun0(void *p)
{
	GGame &pPar = *(GGame *)p;
	static XRect tmp(0,-100,1280,720);
	static float timer = 0.020f;
	for(int i = 0;i < (MAX_SUM >> 1);++ i)
	{
		pPar.m_tailingObj[i].Update(timer,tmp);
		if(pPar.m_tailingObj[i].GetIsEnd()) 
			pPar.m_tailingObj[i].Reset(XVector2(XRand::randomf(-512,512) + 640,-10),XVector2(XRand::randomf(-50,50),XRand::randomf(50,150)),&pPar.m_tailing[i]);
	}
}
void GGame::ThreadFun1(void *p)
{
	GGame &pPar = *(GGame *)p;
	static XRect tmp(0,-100,1280,720);
	static float timer = 0.020f;
	for(int i = (MAX_SUM >> 1);i < MAX_SUM;++ i)
	{
		pPar.m_tailingObj[i].Update(timer,tmp);
		if(pPar.m_tailingObj[i].GetIsEnd()) 
			pPar.m_tailingObj[i].Reset(XVector2(XRand::randomf(-512,512) + 640,-10),XVector2(XRand::randomf(-50,50),XRand::randomf(50,150)),&pPar.m_tailing[i]);
	}
}
#endif
void GGame::ResetParticles(int index)
{
#ifdef WITH_FIELD
	float angle = XRand::randomf(0,PI2);
	XVector2 pos = XVector2(640 + m_r[index] * cos(angle),360 + m_r[index] * sin(angle));
	m_tailingObj[index].Reset(pos,XVector2::zero,&m_tailing[index]);
#else
	m_tailingObj[index].Reset(XVector2(XRand::randomf(-512,512) + 640,-10),XVector2(XRand::randomf(-50,50),XRand::randomf(50,150)),&m_tailing[index]);
#endif
}
#ifdef WITH_GAUSS
float dx = 1.0f / 1280.0f;
float dy = 1.0f / 720.0f;
float gaussShaderData[50] = {-2.0f * dx,-2.0f * dy, -1.0f * dx,-2.0f * dy, 0.0f * dx,-2.0f * dy, 1.0f * dx,-2.0f * dy, 2.0f * dx,-2.0f * dy,
							 -2.0f * dx,-1.0f * dy, -1.0f * dx,-1.0f * dy, 0.0f * dx,-1.0f * dy, 1.0f * dx,-1.0f * dy, 2.0f * dx,-1.0f * dy,
							 -2.0f * dx, 0.0f * dy, -1.0f * dx, 0.0f * dy, 0.0f * dx, 0.0f * dy, 1.0f * dx, 0.0f * dy, 2.0f * dx, 0.0f * dy,
							 -2.0f * dx, 1.0f * dy, -1.0f * dx, 1.0f * dy, 0.0f * dx, 1.0f * dy, 1.0f * dx, 1.0f * dy, 2.0f * dx, 1.0f * dy,
							 -2.0f * dx, 2.0f * dy, -1.0f * dx, 2.0f * dy, 0.0f * dx, 2.0f * dy, 1.0f * dx, 2.0f * dy, 2.0f * dx, 2.0f * dy};
#endif
bool GGame::init()
{
	XEG.setBGColor(XFColor::black);
//	XEG.setFPS(60);
	for(int i = 0;i < MAX_SUM;++ i)
	{
#ifdef WITH_FIELD
		m_r[i] = XRand::randomf(0,750);
#endif
		ResetParticles(i);
		m_tailing[i].SetColor(XFColor(XRand::randomf(1.0,0.5),XRand::randomf(1.0,0.5),1,1));
		m_tailing[i].SetSize(XRand::randomf(3.0f,0.5f));
		m_tailing[i].SetMaxLife(XRand::randomf(1.0f,0.25f));
	}
#ifdef WITH_THREAD
	m_logicThread0.setup(ThreadFun0,this);
	m_logicThread1.setup(ThreadFun1,this);
#endif
#ifdef WITH_VBO
	m_v = XMem::createArrayMem<float>(MAX_SUM * MAX_POINT_SUM * 2);
	m_c = XMem::createArrayMem<float>(MAX_SUM * MAX_POINT_SUM * 4);
	m_sum = XMem::createArrayMem<int>(MAX_SUM);
	m_vbo.init2D(MAX_SUM * MAX_POINT_SUM,m_v,NULL,NULL,m_c);
#endif
#ifdef WITH_FIELD
	m_field2D.Init(128,128);
	//m_field2D.SetField2DType(XField2D::FIELD2D_WHIRL);
	m_field2D.SetField2DType(XField2D::FIELD2D_RAND);
#endif
#ifdef WITH_FBO
	m_fbo.init(1280,720);
	m_fboSprite.init(1280,720,0);
#endif
#ifdef WITH_GAUSS
	if(!m_shader.init("ResourcePack/shader.vrt","ResourcePack/shader.frg")) return false;
	m_shader.connectTexture("Texture_00",NULL);
	m_shader.connectData("tcOffset",DATA_TYPE_2FLOAT_ARRAY,25,&gaussShaderData);
	m_fboSprite.setShaderClass(&m_shader);
#endif

	m_frameData.init(XVector2(512,64),2.0f);
	m_frameData.setShowInfo(INFO_CUR_VALUE);
	m_frameData.setSimpleChartType(TYPE_0_BOTTOM);
	return true;
}
void GGame::move(float stepTime)
{
	m_frameData.insertAData(XEG.getLastFrameTime());
#ifndef WITH_THREAD
	static XRect tmp(0,-280,1280,1000);
	float timer = stepTime * 0.004f;
	//m_tailing.Update(stepTime);

#ifdef WITH_VBO
	m_allSum = 0;
	for(int i = 0;i < MAX_SUM;++ i)
	{
		m_tailingObj[i].Update(timer,tmp);
		m_sum[i] = m_tailingObj[i].UpdateData(m_v + (m_allSum << 1),m_c + (m_allSum << 2));
		m_allSum += m_sum[i];
		if(m_tailingObj[i].GetIsEnd()) ResetParticles(i);
	}
	m_vbo.updateDate2D(m_allSum,m_v,NULL,NULL,m_c);
#else
	XVector2 pos;
	for(int i = 0;i < MAX_SUM;++ i)
	{
#ifdef WITH_FIELD
		pos = m_tailingObj[i].GetPosition() * XVector2(0.1f,0.1f);
		pos.y += 28.0f;
		m_tailingObj[i].SetSpeed(m_tailingObj[i].GetSpeed() + m_field2D.GetVector(pos.x,pos.y));
#endif
		m_tailingObj[i].Update(timer,tmp);
		if(m_tailingObj[i].GetIsEnd()) ResetParticles(i);
	}
#endif
#endif
}
void GGame::draw()
{
#ifdef WITH_FBO
	m_fbo.bind();
	XEG.clearScreen();
#endif
	m_tailingObj[0].DrawBegin();
#ifdef WITH_VBO
	int sum = 0;
	for(int i = 0;i < MAX_SUM;++ i)
	{
		m_vbo.drawByArray(GL_LINE_STRIP,sum,m_sum[i],false);
		sum += m_sum[i];
	}
#else
	for(int i = 0;i < MAX_SUM;++ i)
	{
		m_tailingObj[i].DrawBody();
	//	m_tailingObj[i].Draw();
	}
#endif
	m_tailingObj[0].DrawEnd();
#ifdef WITH_FBO
	m_fbo.unbind();
	m_fboSprite.draw(m_fbo.getTexture(0));
#endif
#ifdef WITH_FIELD
	//m_field2D.Draw(XVector2(1280,720));
#endif
}
void GGame::input(const XInputEvent &event)
{
	switch(event.type)
	{
	case EVENT_KEYBOARD:
		if(event.keyState == KEY_STATE_DOWN)
		{
			switch(event.keyValue)
			{
			case XKEY_R:break;
			}
		}
		break;
	case EVENT_MOUSE:
//		switch(event.mouseState)
//		{
//		case MOUSE_MOVE:
//			if(m_isMouseDown) m_tailing.AddPoint(XE::XVector2(event.mouseX,event.mouseY));
//			break;
//		case MOUSE_LEFT_BUTTON_DOWN:m_isMouseDown = true;break;
//		case MOUSE_LEFT_BUTTON_UP:m_isMouseDown = false;break;
//		}
		break;
	}
}