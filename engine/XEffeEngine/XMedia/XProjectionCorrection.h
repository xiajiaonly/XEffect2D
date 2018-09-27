#ifndef _JIA_XPROJECTIONCORRECTION_
#define _JIA_XPROJECTIONCORRECTION_
#include "XControl\XControls.h"
#include "XMath\XVector2.h"
#include "XShaderGLSL.h"
//这是一个用于单台投影的矫正的类
//需要实现整体缩放、旋转、移动
//快捷键说明
//空格：显示或者隐藏
//B：保存
//M：移动
//R：旋转
//S：缩放
namespace XE{
enum XPCAction
{
	PC_ACTION_NULL,		//没有动作
	PC_ACTION_MOVE,		//移动
	PC_ACTION_SCALE,	//缩放
	PC_ACTION_ROTATION	//旋转
};
enum XProjectionCorMode
{
	PC_MODE_TEXTURE,	//贴图模式
	PC_MODE_FILL,		//填充模式
	PC_MODE_TEXTURE_FILL,	//图片填充
};
class XProjectionCorrection
{
private:
	bool m_isInputEnable;	//是否接收输入控制
	bool m_isInited;
	bool m_isShowCtrl;	//是否显示控件
	bool m_withFBO;		//是否带FBO
	XVec2 m_position;	//偏移位置
	XVec2 m_scale;	//控制点相对于原始大小缩放
	XVec2 m_size;	//整体范围的大小，默认为场景大小
	XPointCtrl m_pointPcl[4];	//四个角点的控件
	std::vector<XVec2> m_pointsPos;	//四个点的位置
	XFBO m_fbo;
	XProjectionCorMode m_mode;
	XShaderGLSL m_fillShader;

	XPCAction m_curAction;
	XE::XVec2 m_curMousePos;
	void initCtrl();	//初始化控件
	std::string m_configFileName;

	//bool m_haveMsg;
	//std::string m_massageStr;
	int m_meshSubSum;	//网格细分的数量
public:
	//const std::string& getMsgStr()const{return m_massageStr;}
	////这个函数智能调用一次有效
	//bool getHaveMsg()
	//{
	//	if(m_haveMsg)
	//	{
	//		m_haveMsg = false;
	//		return true;
	//	}
	//	return false;
	//}
	void setConfigFilename(const std::string& filename){m_configFileName = filename;}
	const std::string& getConfigFilename()const{return m_configFileName;}
	bool getEnableInput()const{return m_isInputEnable;}
	void setEnableInput(bool flag){m_isInputEnable = flag;}
	XProjectionCorrection()
		:m_isInited(false)
		, m_isShowCtrl(false)
		, m_curAction(PC_ACTION_NULL)
		, m_isInputEnable(true)
		, m_configFileName("PCConfig.xml")
	//	, m_haveMsg(false)
		, m_meshSubSum(10)
	{}
	void begin()
	{
		if(m_withFBO)
			m_fbo.useFBO();
	}
	void end()
	{
		if(m_withFBO)
			m_fbo.removeFBO();
	}
	bool getIsShowCtrl()const{return m_isShowCtrl;}
	void setIsShowCtrl(bool flag);
	void draw(bool withClear = true);
	XVec2 getCornerPos(int index)
	{
		if(index < 0 || index >= 4) return XVec2::zero;
		return m_pointPcl[index].getPosition();
	}
	void draw(unsigned int tex,bool withClear = true);
	void init(const XVec2& size,const XVec2& pos,const XVec2& scale,bool withFBO = true,XProjectionCorMode mode = PC_MODE_TEXTURE);
	void init(bool withFBO = true,XProjectionCorMode mode = PC_MODE_TEXTURE);
	bool inputProc(const XInputEvent &event);	//对输入命令的响应
	bool saveData(const char *filename = NULL);	//保存数据到文件
	bool loadData(const char *filename = NULL);
	XVec2 mapPos(const XVec2& pos) const;	//pos坐标经过上面变换之后计算变换之后的位置
};
}
#endif