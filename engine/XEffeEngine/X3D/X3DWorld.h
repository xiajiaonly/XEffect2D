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
namespace XE{
//for test
//extern double matModelView[16];
//extern double matProjection[16]; 
//extern int viewport[4]; 

//这是一个世界的类，世界中公用的部分全在这个类中定义
//这个类是单子系统
enum XWorldShaderType
{
	SHADER_DUMP,
	SHADER_SHADOW,
	SHADER_DUMP_AND_SHADOW,
};
class X3DWorld
{
	friend XShadowMap;
public:
	X3DWorld()
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
		, m_withShadow(XFalse)
		, m_drawWithTexture(0)
	{}
	virtual ~X3DWorld(){} 
protected:
	X3DWorld(const X3DWorld&);
	X3DWorld &operator= (const X3DWorld&);
private:
	XBool m_isInited;
	//控制世界摄像机的按键
	XKeyState m_keyUpState;
	XKeyState m_keyDownState;
	XKeyState m_keyAState;
	XKeyState m_keyDState;
	XKeyState m_keyWState;
	XKeyState m_keySState;
	XBool m_isMouseBtnDown;

	XBasic3DObject * m_targetObj;

	XShadowMap m_shadowMap;
	XMatrix4x4 m_shadowMatrix;		//产生阴影的矩阵
	//下面是关于shadow的shader的变量
	XBool m_withShadow;				//是否使用阴影
	unsigned int m_shadowMapTexture;
	XShaderGLSL m_shadowMapShader; //产生阴影的shader
	int m_drawWithTexture;			//描绘是否有贴图
	//下面是dumpmap shader的变量
	XShaderGLSL m_dumpMapShader;	//法线贴图的shader
	XBool m_withDumpMap;
	unsigned int m_dumpMapTexture;
	//XBool m_withShader;	//是否使用shader
	XShaderGLSL *m_curShader;	//最近使用的一个shader

	XShaderGLSL m_DMapAndSMapGLSL;

	void (*m_drawFun)(XBool withTexture);	//绘图函数

	XMatrix3x3 m_mouseRotateMatrixO;		//鼠标的旋转矩阵
	XMatrix3x3 m_mouseRotateMatrixN;
//	XVector3 m_mouseRotateAngle;			//鼠标控制旋转的角度
	XArcBall m_mouseArcBall;
//	XVector4 m_mouseRotate;

	XBool m_withSkyBox;	//是否描绘天空
	XBool m_withLight;		//是否启用灯光
	XBool m_withFog;		//是否使用雾

	X3DDrawMode m_3dDrawMode;
public:
	void setUseSkyBox(XBool flag){m_withSkyBox = flag;}
	void setUseLight(XBool flag)
	{
		m_withLight = flag;
		if(m_withLight) m_worldLight.useLight();
		else m_worldLight.disAllLight();
	}
	void setUseFog(XBool flag)
	{
		m_withFog = flag;
		if(m_withFog) m_worldLineFog.useFog();
		else m_worldLineFog.disFog();
	}
	void set3DDrawMode(X3DDrawMode mode) 
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
	XSkyBox m_skyBox;				//世界包围盒，天空盒子
	XCamRoam m_worldCam;			//通用的世界摄像机
	XLight m_worldLight;			//世界灯光
	XMaterial m_worldMaterial;		//世界的通用材质
	XLineFog m_worldLineFog;		//世界雾
	void useShadow(XBool withTexture,XWorldShaderType type = SHADER_SHADOW,unsigned int dumpTex = 0);
	void removeShadow();
	//下面用于做映射
	XVector3 worldToScreen(const XVector3 &point,const XRect &view);	//目前这个函数存在问题
	void setTargetObj(XBasic3DObject * obj){m_targetObj = obj;}

	XBool init(void (* drawFun)(XBool),XResourcePosition resourcePosition = RESOURCE_SYSTEM_DEFINE);
	void draw();
	void move(float stepTime);
	void setWithShadow(XBool flag){m_withShadow = flag;}
	void setWithDumpMap(XBool flag){m_withDumpMap = flag;}
	XBool getWithShadow()const{return m_withShadow;}
	void keyEvent(const XInputEvent& inputEvent);	//按键事件
};
namespace X3D
{
	extern void addCameraToCFG();	//添加摄像机数据到配置
}
}
#endif