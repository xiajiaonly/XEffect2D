//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XStdHead.h"
#include "X3DWorld.h"
#include "..\XConfigManager.h"
#include "../XGameBasic.h"
namespace XE{
static const XMat4 biasMatrix(0.5f, 0.0f, 0.0f, 0.0f,
							  0.0f, 0.5f, 0.0f, 0.0f,
							  0.0f, 0.0f, 0.5f, 0.0f,
							  0.5f, 0.5f, 0.5f, 1.0f);	//bias from [-1, 1] to [0, 1]
XBool X3DWorld::init(void(*drawFun)(XBool), XResPos resourcePosition)
{
	if (m_isInited) return XFalse;
	m_drawFun = drawFun;
	//按键状态初始化
	m_keyUpState = KEY_STATE_UP;
	m_keyDownState = KEY_STATE_UP;
	m_keyAState = KEY_STATE_UP;
	m_keyDState = KEY_STATE_UP;
	m_keyWState = KEY_STATE_UP;
	m_keySState = KEY_STATE_UP;

	m_worldCam.setPosition(XVec3::zero);
	m_worldCam.setLength(100.0f);
	m_worldCam.setAngle(XVec3::zero);
	m_worldCam.useCam();
	//下面是灯光
	m_worldLight.setPosition(512.0f, 1500.0f, 512.0f, 1.0f);
	m_worldLight.setLength(2000.0f);
	m_worldLight.setAngle(-90.0f, 180.0f, 0.0f);
	m_worldLight.setAmblient(0.5f, 0.5f, 0.5f, 1.0f);//灯的颜色
	m_worldLight.setDiffuse(1.0f, 1.0f, 1.0f, 1.0f);
	m_worldLight.setSpecular(-0.2f, -0.2f, -0.2f, 1.0f);
	m_worldLight.setIndex(0);
	m_worldLight.useLight();
	//设置材质属性
	m_worldMaterial.setAmblient(1.0f, 1.0f, 1.0f, 1.0f);
	m_worldMaterial.setDiffuse(1.0f, 1.0f, 1.0f, 1.0f);
	m_worldMaterial.setSpecular(1.0f, 1.0f, 1.0f, 1.0f);
	m_worldMaterial.setShininess(128.0f);
	m_worldMaterial.bindMaterial();
	//下面是雾
	m_worldLineFog.setColor(0.8f, 0.8f, 0.8f, 1.0f);
	m_worldLineFog.setArea(1.0f, 1000.0f);
	m_worldLineFog.setDensity(0.0005f);
	m_worldLineFog.useFog();
	//初始化法线贴图
	if (!m_dumpMapShader.init((getCommonResPos() + "ResourcePack/Normal&HighLightMapShader.vrt").c_str(),
		(getCommonResPos() + "ResourcePack/Normal&HighLightMapShader.frg").c_str(), resourcePosition)) return XFalse;
	m_dumpMapShader.connectTexture("Texture_00", NULL);//连接数据
	m_dumpMapShader.connectTexture("normalTexture", &m_dumpMapTexture);
	//初始化阴影shader
	if (!m_shadowMapShader.init((getCommonResPos() + "ResourcePack/Shadowmap.vrt").c_str(),
		(getCommonResPos() + "ResourcePack/Shadowmap.frg").c_str(), resourcePosition)) return XFalse;
	m_shadowMapShader.connectTexture("Texture", NULL);//连接数据
	m_shadowMapShader.connectTexture("ShadowMap", &m_shadowMapTexture);
	m_shadowMapShader.connectData("Texturing", DATA_TYPE_INT, 1, &m_drawWithTexture);
	m_shadowMapShader.connectData("ShadowMatrix", DATA_TYPE_4FLOAT_MATRIX, 1, m_shadowMatrix);
	//++++++++++++++++++++++++++++++++++
	if (!m_DMapAndSMapGLSL.init((getCommonResPos() + "ResourcePack/ShadowMap&DumpMap.vrt").c_str(),
		(getCommonResPos() + "ResourcePack/ShadowMap&DumpMap.frg").c_str(), resourcePosition)) return XFalse;
	m_DMapAndSMapGLSL.connectTexture("Texture", NULL);//连接数据
	m_DMapAndSMapGLSL.connectTexture("normalTexture", &m_dumpMapTexture);
	m_DMapAndSMapGLSL.connectTexture("ShadowMap", &m_shadowMapTexture);
	m_DMapAndSMapGLSL.connectData("withTex", DATA_TYPE_INT, 1, &m_drawWithTexture);
	m_DMapAndSMapGLSL.connectData("ShadowMatrix", DATA_TYPE_4FLOAT_MATRIX, 1, m_shadowMatrix);

	//初始化天空盒子
	if (m_skyBox.init((getCommonResPos() + "ResourcePack/SkyBox/top.png").c_str(), resourcePosition) == 0) return XFalse;
	m_skyBox.setBox(4096.0f, -4096.0f, -4096.0f, 4096.0f, 4096.0f, -4096.0f);

	if (!m_shadowMap.init()) return XFalse;

	m_shadowMapTexture = m_shadowMap.getShadowMapTexture();
	m_drawWithTexture = 1;
	m_shadowMatrix = biasMatrix * m_worldLight.getProjectMatrix() *
		m_worldLight.getViewMatrix() * m_worldCam.getViewMatrix().inverse();

	m_mouseArcBall.setSize(getWindowWidth(), getWindowHeight());
	m_mouseRotateMatrixO.loadIdentity();
	m_mouseRotateMatrixN.loadIdentity();

	m_isInited = XTrue;
	return XTrue;
}
void X3DWorld::useShadow(XBool withTexture, XWorldShaderType type, unsigned int dumpTex)
{
	switch (type)
	{
	case SHADER_SHADOW:
		if (!m_withShadow) return;
		if (withTexture) m_drawWithTexture = 1;
		else m_drawWithTexture = 0;
		m_shadowMapShader.useShader();
		m_curShader = &m_shadowMapShader;
		//m_withShader = XTrue;
		break;
	case SHADER_DUMP:
		if (!m_withDumpMap) return;
		m_dumpMapTexture = dumpTex;
		m_dumpMapShader.useShader();
		m_curShader = &m_dumpMapShader;
		//m_withShader = XTrue;
		break;
	case SHADER_DUMP_AND_SHADOW:
		if (!m_withShadow && !m_withDumpMap) return;
		if (!m_withShadow)
		{//只使用dump
			m_dumpMapTexture = dumpTex;
			m_dumpMapShader.useShader();
			m_curShader = &m_dumpMapShader;
		}
		else
			if (!m_withDumpMap)
			{//只使用shadow
				if (withTexture) m_drawWithTexture = 1;
				else m_drawWithTexture = 0;
				m_shadowMapShader.useShader();
				m_curShader = &m_shadowMapShader;
			}
			else
			{//两者都使用
				m_dumpMapTexture = dumpTex;
				if (withTexture) m_drawWithTexture = 1;
				else m_drawWithTexture = 0;
				m_DMapAndSMapGLSL.useShader();
				m_curShader = &m_DMapAndSMapGLSL;
			}
		//m_withShader = XTrue;
		break;
	}
}
void X3DWorld::removeShadow()
{
	if(m_curShader != NULL)
	{
		m_curShader->disShader();
		m_curShader = NULL;
	}
	//if(m_withShader) 
	//{
	//	glUseProgram(0);
	//	m_withShader = XFalse;
	//}
}
//double matModelView[16];
//double matProjection[16]; 
//int viewport[4]; 
void X3DWorld::draw()
{
	if(!m_isInited) return;
	if(m_withShadow)
	{
	//	glPushAttrib(GL_ALL_ATTRIB_BITS);
		m_withShadow = XFalse;
		m_shadowMap.updateShadowMap();	//更新shadowmap

		glClear(GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(m_worldCam.getProjectMatrix());
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(m_worldCam.getViewMatrix());
		glViewport(0,0,getWindowWidth(),getWindowHeight());
		if(m_withLight) m_worldLight.useLight();
		if(m_withSkyBox) m_skyBox.draw();
		//这个需要处理到各个描绘动作中
		m_withShadow = XTrue;
		if(m_drawFun != NULL) m_drawFun(XTrue);
		else if(XEG.m_pGame != NULL) ((XGameBasic3D *)XEG.m_pGame)->draw3D(XTrue);

		////描绘光源的位置
		//drawLine(XVec3(m_worldLight.getPosition().x - 10.0f,m_worldLight.getPosition().y,m_worldLight.getPosition().z),
		//	XVec3(m_worldLight.getPosition().x + 10.0f,m_worldLight.getPosition().y,m_worldLight.getPosition().z),XFColor::red);
		//drawLine(XVec3(m_worldLight.getPosition().x,m_worldLight.getPosition().y,m_worldLight.getPosition().z - 10.0f),
		//	XVec3(m_worldLight.getPosition().x,m_worldLight.getPosition().y,m_worldLight.getPosition().z + 10.0f),XFColor::green);
		//drawLine(XVec3(m_worldLight.getPosition().x,m_worldLight.getPosition().y - 10.0f,m_worldLight.getPosition().z),
		//	XVec3(m_worldLight.getPosition().x,m_worldLight.getPosition().y + 10.0f,m_worldLight.getPosition().z),XFColor::blue);
		////描绘原点的位置
		//drawOrigin();

//		glActiveTexture(GL_TEXTURE1);	//取消深度贴图
//		XGL::DisableTexture2D();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
	//	glPopAttrib();

		//m_shadowMap.draw();
	}else
	{
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadMatrixf(m_worldCam.getProjectMatrix());
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadMatrixf(m_worldCam.getViewMatrix());
		glViewport(0,0,getWindowWidth(),getWindowHeight());
		//glOrtho(0,XEG.m_windowData.sceneW,0,XEG.m_windowData.sceneH,-1000,1000);

		//for test
		//glGetDoublev( GL_MODELVIEW_MATRIX, matModelView ); 
		//glGetDoublev( GL_PROJECTION_MATRIX, matProjection ); 
		//glGetIntegerv( GL_VIEWPORT, viewport ); 
		if(m_withLight) m_worldLight.useLight();
		m_worldMaterial.bindMaterial();
		if(m_withSkyBox) m_skyBox.draw();
		if(m_drawFun != NULL) m_drawFun(XTrue);
		else if(XEG.m_pGame != NULL) ((XGameBasic3D *)XEG.m_pGame)->draw3D(XTrue);
		////描绘光源的位置
		//drawLine(XVec3(m_worldLight.getPosition().x - 10.0f,m_worldLight.getPosition().y,m_worldLight.getPosition().z),
		//	XVec3(m_worldLight.getPosition().x + 10.0f,m_worldLight.getPosition().y,m_worldLight.getPosition().z),XFColor::red);
		//drawLine(XVec3(m_worldLight.getPosition().x,m_worldLight.getPosition().y,m_worldLight.getPosition().z - 10.0f),
		//	XVec3(m_worldLight.getPosition().x,m_worldLight.getPosition().y,m_worldLight.getPosition().z + 10.0f),XFColor::green);
		//drawLine(XVec3(m_worldLight.getPosition().x,m_worldLight.getPosition().y - 10.0f,m_worldLight.getPosition().z),
		//	XVec3(m_worldLight.getPosition().x,m_worldLight.getPosition().y + 10.0f,m_worldLight.getPosition().z),XFColor::blue);
		////描绘原点的位置
		//drawOrigin();
		m_worldMaterial.unbindMaterial();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
	}
}
void X3DWorld::keyEvent(const XInputEvent& inputEvent)
{
	switch(inputEvent.type)
	{
	case EVENT_KEYBOARD:
		if (!m_withKeyCtrlCam) break;
		switch(inputEvent.keyState)
		{
		case KEY_STATE_UP:
			switch(inputEvent.keyValue)
			{
			case XKEY_UP:	m_keyUpState = KEY_STATE_UP;	break;
			case XKEY_DOWN:	m_keyDownState = KEY_STATE_UP;	break;
			case XKEY_A:	m_keyAState = KEY_STATE_UP;		break;
			case XKEY_D:	m_keyDState = KEY_STATE_UP;		break;
			case XKEY_W:	m_keyWState = KEY_STATE_UP;		break;
			case XKEY_S:	m_keySState = KEY_STATE_UP;		break;
			}
			break;
		case KEY_STATE_DOWN:
			switch(inputEvent.keyValue)
			{
			case XKEY_UP:	m_keyUpState = KEY_STATE_DOWN;	break;
			case XKEY_DOWN:	m_keyDownState = KEY_STATE_DOWN;break;
			case XKEY_A:	m_keyAState = KEY_STATE_DOWN;	break;
			case XKEY_D:	m_keyDState = KEY_STATE_DOWN;	break;
			case XKEY_W:	m_keyWState = KEY_STATE_DOWN;	break;
			case XKEY_S:	m_keySState = KEY_STATE_DOWN;	break;
			}
			break;
		}
		break;
	case EVENT_MOUSE:
		switch(inputEvent.mouseState)
		{
		case MOUSE_MOVE:
			if(m_isMouseBtnDown && m_targetObj != NULL && m_isArcBallAuto)
			{//鼠标拖动
				XVec4 mouseRotate;
				m_mouseArcBall.drag(getMousePos(),mouseRotate);
				m_mouseRotateMatrixN = m_mouseRotateMatrixO * XMath::toMatrix3x3(mouseRotate).anti();
				m_targetObj->setMultRotate(XMat4().getRotate(m_mouseRotateMatrixN));
			}
			break;
		case MOUSE_LEFT_BUTTON_DOWN:
		case MOUSE_RIGHT_BUTTON_DOWN:
		case MOUSE_MIDDLE_BUTTON_DOWN:
			m_isMouseBtnDown = XTrue;
			if(m_isArcBallAuto)
			{
				m_mouseRotateMatrixO = m_mouseRotateMatrixN;
				m_mouseArcBall.click(getMousePos());
			}
			break;
		case MOUSE_LEFT_BUTTON_UP:
		case MOUSE_RIGHT_BUTTON_UP:
		case MOUSE_MIDDLE_BUTTON_UP:
				m_isMouseBtnDown = XFalse;
			break;
		}
		break;
	}
}
namespace X3D
{
	XVec3 cameraPos(512.0f, 800.0f, 512.0f);
	XVec3 cameraAngle(-90.0f, 180.0f, 0.0f);
	float cameraOpenAngle = 10.0f;
	void updateCamera(void *, void *)
	{
		if (gFrameworkData.p3DWorld != NULL)
		{
			gFrameworkData.p3DWorld->m_worldCam.setPosition(cameraPos);
			gFrameworkData.p3DWorld->m_worldCam.setAngle(cameraAngle.x, cameraAngle.y, cameraAngle.z);
		}
		XEG.setViewAngle(cameraOpenAngle);
	}
	void addCameraToCFG()
	{
		if (gFrameworkData.pCfgManager == NULL) return;
		gFrameworkData.pCfgManager->addGroup("摄像机参数");
		gFrameworkData.pCfgManager->addAItem(&cameraPos.x, CFG_DATA_FLOAT, "摄像机X", 1024.0f, 0.0f, 512.0f, updateCamera, "摄像机参数");
		gFrameworkData.pCfgManager->addAItem(&cameraPos.y, CFG_DATA_FLOAT, "摄像机Y", 10000.0f, 0.0f, 800.0f, updateCamera, "摄像机参数");
		gFrameworkData.pCfgManager->addAItem(&cameraPos.z, CFG_DATA_FLOAT, "摄像机Z", 1024.0f, 0.0f, 512.0f, updateCamera, "摄像机参数");
		gFrameworkData.pCfgManager->addAItem(&cameraAngle.x, CFG_DATA_FLOAT, "摄像机X", 360.0f, 0.0f, 270.0f, updateCamera, "摄像机参数");
		gFrameworkData.pCfgManager->addAItem(&cameraAngle.y, CFG_DATA_FLOAT, "摄像机Y", 360.0f, 0.0f, 180.0f, updateCamera, "摄像机参数");
		gFrameworkData.pCfgManager->addAItem(&cameraAngle.z, CFG_DATA_FLOAT, "摄像机Z", 360.0f, 0.0f, 0.0f, updateCamera, "摄像机参数");
		gFrameworkData.pCfgManager->addAItem(&cameraOpenAngle, CFG_DATA_FLOAT, "摄像机视角", 180.0f, 1.0f, 10.0f, updateCamera, "摄像机参数");
	}
}
void X3DWorld::move(float stepTime)
{
	if (!m_isInited) return;
	if (m_keyUpState == KEY_STATE_DOWN) m_worldCam.forward(0.1f * stepTime);
	if (m_keyDownState == KEY_STATE_DOWN) m_worldCam.backward(0.1f * stepTime);
	if (m_keyAState == KEY_STATE_DOWN) m_worldCam.turnLeft(0.1f * stepTime);
	if (m_keyDState == KEY_STATE_DOWN) m_worldCam.turnRight(0.1f * stepTime);
	if (m_keyWState == KEY_STATE_DOWN) m_worldCam.turnUp(0.1f * stepTime);
	if (m_keySState == KEY_STATE_DOWN) m_worldCam.turnDown(0.1f * stepTime);
	if (m_worldCam.calculate())
	{//计算矩阵
		m_shadowMatrix = biasMatrix * m_worldLight.getProjectMatrix() *
			m_worldLight.getViewMatrix() * m_worldCam.getViewMatrix().inverse();
	}
	X3D::cameraPos = m_worldCam.getPosition();
	//X3D::cameraAngle = m_worldCam.getAngle() * RADIAN2DEGREE;
	X3D::cameraOpenAngle = XEG.getViewAngle();
}
}