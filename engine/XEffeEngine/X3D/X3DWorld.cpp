#include "XEffeEngine.h"
#include "X3DWorld.h"
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
static _XMatrix4x4 biasMatrix(0.5f, 0.0f, 0.0f, 0.0f,
							  0.0f, 0.5f, 0.0f, 0.0f,
							  0.0f, 0.0f, 0.5f, 0.0f,
							  0.5f, 0.5f, 0.5f, 1.0f);	//bias from [-1, 1] to [0, 1]
_XBool _X3DWorld::init(void (* drawFun)(_XBool),_XResourcePosition resourcePosition)
{
	if(m_isInited) return XFalse;
	if(drawFun == NULL) return XFalse;
	m_drawFun = drawFun;
	//按键状态初始化
	m_keyUpState = KEY_STATE_UP;
	m_keyDownState = KEY_STATE_UP;
	m_keyAState = KEY_STATE_UP;
	m_keyDState = KEY_STATE_UP;
	m_keyWState = KEY_STATE_UP;
	m_keySState = KEY_STATE_UP;

	m_worldCam.setPosition(0.0f,0.0f,0.0f);
	m_worldCam.setLength(100.0f);
	m_worldCam.setAngle(0.0f,0.0f,0.0f);
	m_worldCam.useCam();
	//下面是灯光
	m_worldLight.setPosition(512.0f,1500.0f,512.0f,1.0f);
	m_worldLight.setLength(2000.0f);
	m_worldLight.setAngle(-90.0f,180.0f,0.0f);
	m_worldLight.setAmblient(0.5f,0.5f,0.5f,1.0f);//灯的颜色
	m_worldLight.setDiffuse(1.0f,1.0f,1.0f,1.0f);
	m_worldLight.setSpecular(-0.2f,-0.2f,-0.2f,1.0f);
	m_worldLight.setIndex(1);
	m_worldLight.useLight();
	//设置材质属性
	m_worldMaterial.setAmblient(1.0f,1.0f,1.0f,1.0f);
	m_worldMaterial.setDiffuse(1.0f,1.0f,1.0f,1.0f);
	m_worldMaterial.setSpecular(1.0f,1.0f,1.0f,1.0f);
	m_worldMaterial.setShininess(128.0f);
	m_worldMaterial.usetMaterial();
	//下面是雾
	m_worldLineFog.setColor(0.8f,0.8f,0.8f,1.0f);
	m_worldLineFog.setArea(1.0f,1000.0f);
	m_worldLineFog.setDensity(0.0005f);
	m_worldLineFog.useFog();
	//初始化法线贴图
	if(!m_dumpMapShader.init((XEE::windowData.commonResourcePos + "ResourcePack/Normal&HighLightMapShader.vrt").c_str(),
		(XEE::windowData.commonResourcePos + "ResourcePack/Normal&HighLightMapShader.frg").c_str(),resourcePosition)) return XFalse;
	m_dumpMapShader.connectTexture("Texture_00",NULL);//连接数据
	m_dumpMapShader.connectTexture("normalTexture",&m_dumpMapTexture);
	//初始化阴影shader
	if(!m_shadowMapShader.init((XEE::windowData.commonResourcePos + "ResourcePack/Shadowmap.vrt").c_str(),
		(XEE::windowData.commonResourcePos + "ResourcePack/Shadowmap.frg").c_str(),resourcePosition)) return XFalse;	
	m_shadowMapShader.connectTexture("Texture",NULL);//连接数据
	m_shadowMapShader.connectTexture("ShadowMap",&m_shadowMapTexture);
	m_shadowMapShader.connectData("Texturing",DATA_TYPE_INT,1,&m_drawWithTexture);
	m_shadowMapShader.connectData("ShadowMatrix",DATA_TYPE_4FLOAT_MATRIX,1,m_shadowMatrix);
	//++++++++++++++++++++++++++++++++++
	if(!m_DMapAndSMapGLSL.init((XEE::windowData.commonResourcePos + "ResourcePack/ShadowMap&DumpMap.vrt").c_str(),
		(XEE::windowData.commonResourcePos + "ResourcePack/ShadowMap&DumpMap.frg").c_str(),resourcePosition)) return XFalse;
	m_DMapAndSMapGLSL.connectTexture("Texture",NULL);//连接数据
	m_DMapAndSMapGLSL.connectTexture("normalTexture",&m_dumpMapTexture);
	m_DMapAndSMapGLSL.connectTexture("ShadowMap",&m_shadowMapTexture);
	m_DMapAndSMapGLSL.connectData("withTex",DATA_TYPE_INT,1,&m_drawWithTexture);
	m_DMapAndSMapGLSL.connectData("ShadowMatrix",DATA_TYPE_4FLOAT_MATRIX,1,m_shadowMatrix);
	
	//初始化天空盒子
	if(m_skyBox.init((XEE::windowData.commonResourcePos + "ResourcePack/SkyBox/top.png").c_str(),resourcePosition) == 0) return XFalse;
	m_skyBox.setBox(4096.0f,-4096.0f,-4096.0f,4096.0f,4096.0f,-4096.0f);

	if(!m_shadowMap.init()) return XFalse;

	m_shadowMapTexture = m_shadowMap.getShadowMapTexture();
	m_drawWithTexture = 1;
	m_shadowMatrix = biasMatrix * m_worldLight.getProjectMatrix() * 
			m_worldLight.getViewMatrix() * m_worldCam.getViewMatrix().inverse();

	m_mouseArcBall.setSize(XEE::windowWidth,XEE::windowHeight);
	m_mouseRotateMatrixO.loadIdentity();
	m_mouseRotateMatrixN.loadIdentity();

	m_isInited = XTrue;
	return XTrue;
}
void _X3DWorld::useShadow(_XBool withTexture,_XWorldShaderType type,unsigned int dumpTex)
{
	if(type == SHADER_SHADOW)
	{
		if(!m_withShadow) return;
		if(withTexture) m_drawWithTexture = 1;
		else m_drawWithTexture = 0;
		m_shadowMapShader.useShader();
		m_curShader = &m_shadowMapShader;
		//m_withShader = XTrue;
	}else 
	if(type == SHADER_DUMP) 
	{
		if(!m_withDumpMap) return;
		m_dumpMapTexture = dumpTex;
		m_dumpMapShader.useShader();
		m_curShader = &m_dumpMapShader;
		//m_withShader = XTrue;
	}else
	if(type == SHADER_DUMP_AND_SHADOW)
	{
		if(!m_withShadow && !m_withDumpMap) return;
		if(!m_withShadow)
		{//只使用dump
			m_dumpMapTexture = dumpTex;
			m_dumpMapShader.useShader();
			m_curShader = &m_dumpMapShader;
		}else
		if(!m_withDumpMap)
		{//只使用shadow
			if(withTexture) m_drawWithTexture = 1;
			else m_drawWithTexture = 0;
			m_shadowMapShader.useShader();
			m_curShader = &m_shadowMapShader;
		}else
		{//两者都使用
			m_dumpMapTexture = dumpTex;
			if(withTexture) m_drawWithTexture = 1;
			else m_drawWithTexture = 0;
			m_DMapAndSMapGLSL.useShader();
			m_curShader = &m_DMapAndSMapGLSL;
		}
		//m_withShader = XTrue;
	}
}
void _X3DWorld::removeShadow()
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
void _X3DWorld::draw()
{
	if(!m_isInited) return;
	if(m_drawFun == NULL) return;
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
		glViewport(0,0,XEE::windowData.w,XEE::windowData.h);
		if(m_withLight) m_worldLight.useLight();
		if(m_withSkyBox) m_skyBox.draw();
		//这个需要处理到各个描绘动作中
		m_withShadow = XTrue;
		m_drawFun(XTrue);

		////描绘光源的位置
		//drawLine(_XVector3(m_worldLight.getPosition().x - 10.0f,m_worldLight.getPosition().y,m_worldLight.getPosition().z),
		//	_XVector3(m_worldLight.getPosition().x + 10.0f,m_worldLight.getPosition().y,m_worldLight.getPosition().z),_XFColor::red);
		//drawLine(_XVector3(m_worldLight.getPosition().x,m_worldLight.getPosition().y,m_worldLight.getPosition().z - 10.0f),
		//	_XVector3(m_worldLight.getPosition().x,m_worldLight.getPosition().y,m_worldLight.getPosition().z + 10.0f),_XFColor::green);
		//drawLine(_XVector3(m_worldLight.getPosition().x,m_worldLight.getPosition().y - 10.0f,m_worldLight.getPosition().z),
		//	_XVector3(m_worldLight.getPosition().x,m_worldLight.getPosition().y + 10.0f,m_worldLight.getPosition().z),_XFColor::blue);
		////描绘原点的位置
		//drawOrigin();

		glActiveTexture(GL_TEXTURE1);	//取消深度贴图
		glDisable(GL_TEXTURE_2D);
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
		glViewport(0,0,XEE::windowData.w,XEE::windowData.h);

		//for test
		//glGetDoublev( GL_MODELVIEW_MATRIX, matModelView ); 
		//glGetDoublev( GL_PROJECTION_MATRIX, matProjection ); 
		//glGetIntegerv( GL_VIEWPORT, viewport ); 

		if(m_withLight) m_worldLight.useLight();
		m_worldMaterial.usetMaterial();
		if(m_withSkyBox) m_skyBox.draw();
		m_drawFun(XTrue);
		////描绘光源的位置
		//drawLine(_XVector3(m_worldLight.getPosition().x - 10.0f,m_worldLight.getPosition().y,m_worldLight.getPosition().z),
		//	_XVector3(m_worldLight.getPosition().x + 10.0f,m_worldLight.getPosition().y,m_worldLight.getPosition().z),_XFColor::red);
		//drawLine(_XVector3(m_worldLight.getPosition().x,m_worldLight.getPosition().y,m_worldLight.getPosition().z - 10.0f),
		//	_XVector3(m_worldLight.getPosition().x,m_worldLight.getPosition().y,m_worldLight.getPosition().z + 10.0f),_XFColor::green);
		//drawLine(_XVector3(m_worldLight.getPosition().x,m_worldLight.getPosition().y - 10.0f,m_worldLight.getPosition().z),
		//	_XVector3(m_worldLight.getPosition().x,m_worldLight.getPosition().y + 10.0f,m_worldLight.getPosition().z),_XFColor::blue);
		////描绘原点的位置
		//drawOrigin();

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
	}
}
_XVector3 _X3DWorld::worldToScreen(const _XVector3 &point,const _XRect &view)
{//这个映射函数有问题
	//_XMatrix4x4 temp = m_worldCam.getProjectMatrix() * m_worldCam.getViewMatrix();
	_XMatrix4x4 temp = m_worldCam.getProjXView();
	_XVector3 cameraXYZ = temp * point;
	_XVector3 screenXYZ;
	screenXYZ.x = (cameraXYZ.x + 1.0f) / 2.0f * view.getWidth() + view.left;
	screenXYZ.y = (1.0f - cameraXYZ.y) / 2.0f * view.getHeight() + view.top;
	screenXYZ.z = cameraXYZ.z;

	//double x,y,z;
	//int viewport[4];
	//viewport[0] = view.left;
	//viewport[1] = view.top;
	//viewport[2] = view.right;
	//viewport[3] = view.bottom;
	//double mvmatrix[16],projmatrix[16];
	//for(int i = 0;i < 16;++ i)
	//{
	//	mvmatrix[i] = m_worldCam.getViewMatrix().data[i];
	//	projmatrix[i] = m_worldCam.getProjectMatrix().data[i];
	//}

	//gluProject(point.x,point.y,point.z,mvmatrix,projmatrix,viewport,&x,&y,&z);
	//screenXYZ.x = x;
	//screenXYZ.y = view.bottom - y + view.top;
	//screenXYZ.z = z;
	return screenXYZ;
}
void _X3DWorld::keyEvent(const _XInputEvent& inputEvent)
{
	switch(inputEvent.type)
	{
	case EVENT_KEYBOARD:
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
			if(m_isMouseBtnDown)
			{
				if(m_targetObj != NULL)
				{//鼠标拖动
					_XVector4 mouseRotate;
					m_mouseArcBall.drag(XEE::mousePosition,mouseRotate);
					m_mouseRotateMatrixN = m_mouseRotateMatrixO * toMatrix3x3(mouseRotate).anti();
					m_targetObj->setMultRotate(_XMatrix4x4().getRotate(m_mouseRotateMatrixN));
				}
			}
			break;
		case MOUSE_LEFT_BUTTON_DOWN:
		case MOUSE_RIGHT_BUTTON_DOWN:
		case MOUSE_MIDDLE_BUTTON_DOWN:
			m_isMouseBtnDown = XTrue;
			m_mouseRotateMatrixO = m_mouseRotateMatrixN;
			m_mouseArcBall.click(XEE::mousePosition);
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
_XVector3 cameraPos(512.0f,800.0f,512.0f);
_XVector3 cameraAngle(-90.0f,180.0f,0.0f);
float cameraOpenAngle = 10.0f;
void updateCamera()
{
	_X3DWorld::GetInstance().m_worldCam.setPosition(cameraPos);
	_X3DWorld::GetInstance().m_worldCam.setAngle(cameraAngle.x,cameraAngle.y,cameraAngle.z);
	XEE::viewAngle3D = cameraOpenAngle;
}
void addCameraToCFG()
{
	_XConfigManager::GetInstance().addGroup("摄像机参数");
	_XConfigManager::GetInstance().addAItem(&cameraPos.x,CFG_DATA_TYPE_FLOAT,"摄像机X",1024.0f,0.0f,512.0f,updateCamera,"摄像机参数");
	_XConfigManager::GetInstance().addAItem(&cameraPos.y,CFG_DATA_TYPE_FLOAT,"摄像机Y",10000.0f,0.0f,800.0f,updateCamera,"摄像机参数");
	_XConfigManager::GetInstance().addAItem(&cameraPos.z,CFG_DATA_TYPE_FLOAT,"摄像机Z",1024.0f,0.0f,512.0f,updateCamera,"摄像机参数");
	_XConfigManager::GetInstance().addAItem(&cameraAngle.x,CFG_DATA_TYPE_FLOAT,"摄像机X",-70.0f,-110.0f,-90.0f,updateCamera,"摄像机参数");
	_XConfigManager::GetInstance().addAItem(&cameraAngle.y,CFG_DATA_TYPE_FLOAT,"摄像机Y",180.0f,-180.0f,180.0f,updateCamera,"摄像机参数");
	_XConfigManager::GetInstance().addAItem(&cameraAngle.z,CFG_DATA_TYPE_FLOAT,"摄像机Z",20.0f,-20.0f,0.0f,updateCamera,"摄像机参数");
	_XConfigManager::GetInstance().addAItem(&cameraOpenAngle,CFG_DATA_TYPE_FLOAT,"摄像机视角",180.0f,1.0f,10.0f,updateCamera,"摄像机参数");
}
void _X3DWorld::move(int stepTime)
{
	if(!m_isInited) return;
	if(m_keyUpState == KEY_STATE_DOWN) m_worldCam.forward(0.1f * stepTime);
	if(m_keyDownState == KEY_STATE_DOWN) m_worldCam.backward(0.1f * stepTime);
	if(m_keyAState == KEY_STATE_DOWN) m_worldCam.turnLeft(0.1f * stepTime);
	if(m_keyDState == KEY_STATE_DOWN) m_worldCam.turnRight(0.1f * stepTime);
	if(m_keyWState == KEY_STATE_DOWN) m_worldCam.turnUp(0.1f * stepTime);
	if(m_keySState == KEY_STATE_DOWN) m_worldCam.turnDown(0.1f * stepTime);
	if(m_worldCam.calculate())
	{//计算矩阵
		m_shadowMatrix = biasMatrix * m_worldLight.getProjectMatrix() * 
			m_worldLight.getViewMatrix() * m_worldCam.getViewMatrix().inverse();
	}
	cameraPos = m_worldCam.getPosition();
	cameraAngle = m_worldCam.getAngle();
	cameraOpenAngle = XEE::viewAngle3D;
}