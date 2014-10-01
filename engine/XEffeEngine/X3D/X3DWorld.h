#ifndef _JIA_X3DWORLD_
#define _JIA_X3DWORLD_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2013.9.20
//--------------------------------
#include "XBasic3D.h"
#include "XLight.h"
#include "XLineFog.h"
#include "XMaterial.h"
#include "XSkyBox.h"
#include "XModelObj.h"
#include "XModelStl.h"
#include "XCam.h"
#include "XTerrain.h"
#include "XInputEventCommon.h"
#include "XShadowMap.h"
#include "XArcBall.h"
#include "XBasicModel.h"

//for test
//extern double matModelView[16];
//extern double matProjection[16]; 
//extern int viewport[4]; 

//这是一个世界的类，世界中公用的部分全在这个类中定义
//这个类是单子系统
enum _XWorldShaderType
{
	SHADER_DUMP,
	SHADER_SHADOW,
	SHADER_DUMP_AND_SHADOW,
};
class _X3DWorld
{
	friend _XShadowMap;
protected:
	_X3DWorld()
		:m_isInited(XFalse)
		,m_drawFun(NULL)
		//,m_withShadow(XFalse)
		,m_curShader(NULL)
		,m_withDumpMap(XFalse)
		,m_isMouseBtnDown(XFalse)
		,m_targetObj(NULL)
		,m_withSkyBox(XTrue)
		,m_withLight(XTrue)
		,m_withFog(XTrue)
		,m_3dDrawMode(DRAW_MODE_POINT)
	{
	}
	_X3DWorld(const _X3DWorld&);
	_X3DWorld &operator= (const _X3DWorld&);
	virtual ~_X3DWorld(){} 
public:
	static _X3DWorld& GetInstance()
	{
		static _X3DWorld m_instance;
		return m_instance;
	}
private:
	_XBool m_isInited;
	//控制世界摄像机的按键
	_XKeyState m_keyUpState;
	_XKeyState m_keyDownState;
	_XKeyState m_keyAState;
	_XKeyState m_keyDState;
	_XKeyState m_keyWState;
	_XKeyState m_keySState;
	_XBool m_isMouseBtnDown;

	_XBasic3DObject * m_targetObj;

	_XShadowMap m_shadowMap;
	_XMatrix4x4 m_shadowMatrix;		//产生阴影的矩阵
	//下面是关于shadow的shader的变量
	_XBool m_withShadow;				//是否使用阴影
	unsigned int m_shadowMapTexture;
	_XShaderGLSL m_shadowMapShader; //产生阴影的shader
	int m_drawWithTexture;			//描绘是否有贴图
	//下面是dumpmap shader的变量
	_XShaderGLSL m_dumpMapShader;	//法线贴图的shader
	_XBool m_withDumpMap;
	unsigned int m_dumpMapTexture;
	//_XBool m_withShader;	//是否使用shader
	_XShaderGLSL *m_curShader;	//最近使用的一个shader

	_XShaderGLSL m_DMapAndSMapGLSL;

	void (*m_drawFun)(_XBool withTexture);	//绘图函数

	_XMatrix3x3 m_mouseRotateMatrixO;		//鼠标的旋转矩阵
	_XMatrix3x3 m_mouseRotateMatrixN;
//	_XVector3 m_mouseRotateAngle;			//鼠标控制旋转的角度
	_XArcBall m_mouseArcBall;
//	_XVector4 m_mouseRotate;

	_XBool m_withSkyBox;	//是否描绘天空
	_XBool m_withLight;		//是否启用灯光
	_XBool m_withFog;		//是否使用雾

	_X3DDrawMode m_3dDrawMode;
public:
	void setUseSkyBox(_XBool flag){m_withSkyBox = flag;}
	void setUseLight(_XBool flag)
	{
		m_withLight = flag;
		if(m_withLight) m_worldLight.useLight();
		else m_worldLight.disAllLight();
	}
	void setUseFog(_XBool flag)
	{
		m_withFog = flag;
		if(m_withFog) m_worldLineFog.useFog();
		else m_worldLineFog.disFog();
	}
	void set3DDrawMode(_X3DDrawMode mode) 
	{//这个目前还有问题，尚未实际生效
		m_3dDrawMode = mode;
		switch(m_3dDrawMode)
		{
		case DRAW_MODE_FILL:
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glEnable(GL_DEPTH_TEST);
			glEnable(GL_CULL_FACE);
			break;
		case DRAW_MODE_LINE:
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glDisable(GL_DEPTH_TEST);
			glDisable(GL_CULL_FACE);
			break;
		case DRAW_MODE_POINT:
			glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
			glDisable(GL_DEPTH_TEST);
			glDisable(GL_CULL_FACE);
			break;
		}
	}
public: //3D世界中的一些属性
	_XSkyBox m_skyBox;				//世界包围盒，天空盒子
	_XCamRoam m_worldCam;			//通用的世界摄像机
	_XLight m_worldLight;			//世界灯光
	_XMaterial m_worldMaterial;		//世界的通用材质
	_XLineFog m_worldLineFog;		//世界雾
	void useShadow(_XBool withTexture,_XWorldShaderType type = SHADER_SHADOW,unsigned int dumpTex = 0);
	void removeShadow();
	//下面用于做映射
	_XVector3 worldToScreen(const _XVector3 &point,const _XRect &view);	//目前这个函数存在问题
	void setTargetObj(_XBasic3DObject * obj){m_targetObj = obj;}

	_XBool init(void (* drawFun)(_XBool),_XResourcePosition resourcePosition = RESOURCE_SYSTEM_DEFINE);
	void draw();
	void move(int stepTime);
	void setWithShadow(_XBool flag){m_withShadow = flag;}
	void setWithDumpMap(_XBool flag){m_withDumpMap = flag;}
	_XBool getWithShadow()const{return m_withShadow;}
	void keyEvent(const _XInputEvent& inputEvent);	//按键事件
};
extern void addCameraToCFG();	//添加摄像机数据到配置
#endif