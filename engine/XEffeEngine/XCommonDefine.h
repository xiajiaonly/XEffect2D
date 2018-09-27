#ifndef _JIA_XCOMMONDEFINE_
#define _JIA_XCOMMONDEFINE_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2014.1.1
//--------------------------------
//所有公共的宏开关都定义在此处，以便于统一修改
//注意：3D中1.0f代表1毫米，既1mm
//注意点1：
//程序申请管理员权限
//项目属性--连接器--清单文件--UAC执行级别改为requireAdministrator
#include <string>

namespace XE{
#include "XPersonalizedDefine.h"
#define WITH_VS2015
#ifdef WITH_VS2015
//在vs2015下需要包含下面的库,当使用到老的库时，好像在win10下不用包含这个(奇怪)
//#pragma comment(lib, "legacy_stdio_definitions.lib")
#endif

#ifndef AUDIO_MATHOD 
#define AUDIO_MATHOD (0)				//0：SDL_Mixer,1：Fmod
#endif
#ifndef CREATE_WINDOW_METHOD
#define CREATE_WINDOW_METHOD (0)		//0：SDL，1：GLUT，2：GLFW
#endif

#define WITH_INLINE_FILE (1)			//是否使能inline文件的形式 0更快的编译速度，1更好的执行性能
#if WITH_INLINE_FILE
#define INLINE inline
#else
#define INLINE 
#endif

#ifndef FLT_MAX
#define FLT_MAX 3.402823466e+38F
#endif
#ifndef FLT_MIN
#define FLT_MIN 1.175494351e-38F        // min normalized positive value
#endif

#ifndef BACKGROUND_ALPHA_MODE
#define BACKGROUND_ALPHA_MODE (1)		//窗口透明的实现方式:0、鼠标不能穿越透明区域，1、速度较为慢
#endif
#ifndef WITH_OBJECT_MANAGER
#define WITH_OBJECT_MANAGER (0)			//是否允许使用物件管理
#endif
#ifndef WITH_FULL_ALL_CHINESE
#define WITH_FULL_ALL_CHINESE (1)		//是否支持全中文 0:常用中文，1：全中文，2：只支持英文
#endif
#ifndef WITH_COMMON_RESOURCE_FOLDER
#define WITH_COMMON_RESOURCE_FOLDER (1)	//是否使用公用文件夹
#endif
#ifndef WITH_TOUCH_SUPPLY
#define WITH_TOUCH_SUPPLY (1)
#endif

#define WITHXSPRITE_EX (1)				//是否使用加强版的sprite，这里关系到是否使用归一化的纹理坐标
#define BASE_RESOURCE_PATH "ResourcePack/"	//默认封包资源文件夹的路径，放在这个文件夹下的资源才会被封包
#define WITH_64BIT_SUPPLY (0)	//是否支持64位的函数

#ifndef WITHXEE_CURSOR
//#define WITHXEE_CURSOR		//是否使用自己的光标，如果取消这个宏则直接使用windows的光标
#endif
#ifndef WITH_LOG
#define WITH_LOG (1)
#endif
#ifndef WITH_ENGINE_STATISTICS
#define WITH_ENGINE_STATISTICS	(0)		//是否启用引擎内部统计
#endif

#ifndef LIMITED_VERSION
#define LIMITED_VERSION (1)
#endif

//#define DEBUG_MODE		//所有需要调试与正式版本不同的模块都需要添加这个头文件
//#define GET_ALL_PIXEL		//统计整体的贴图面积，用于标称游戏的贴图效率

#define XEE_SYSTEM_CONFIG "XEEConfig.xml"	//默认引擎配置文件

#define MAX_FILE_NAME_LENGTH (256)	//文件名的长度
#define STRINGIFY(A) #A
#if 1
//typedef BOOL XBool;
//#define XFalse (FALSE)
//#define XTrue (TRUE)
typedef int XBool;
#define XFalse (0)
#define XTrue (1)
#else
typedef bool XBool;
#define XFalse (false)
#define XTrue (true)
#endif

//这里是对资源位置的定义
//原来为：XResPos
enum XResPos
{
	RES_LOCAL_FOLDER = 0,	//本地对应的文件夹
	RES_LOCAL_PACK,		//本地压缩包
	RES_WEB,				//网络资源
	RES_SYS_DEF,		//遵从系统设置
	RES_AUTO,				//自动载入，优先从压缩包中取，取不到才从文件夹中读取
};
enum XWindowSizeMode		//窗口尺寸的适应模式
{
	WINDOW_SIZE_MODE_CLIP_LP,		//左上角为基准点裁剪	pos,配合pos的数值可以实现任一点为基准点裁剪
	WINDOW_SIZE_MODE_CLIP_MID,		//中点为基准点裁剪		pos
	WINDOW_SIZE_MODE_CLIP_RESIZE,	//短边拉伸长边裁剪		pos
	WINDOW_SIZE_MODE_RESIZE_CLIP,	//长边拉伸短边填空白	pos
	WINDOW_SIZE_MODE_RESIZE,		//缩放拉伸				pos
};
enum XWindowRotateMode		//窗口的旋转模式
{
	WINDOW_ROTATE_MODE_0,			//0度旋转	
	WINDOW_ROTATE_MODE_90,			//90度旋转		
	WINDOW_ROTATE_MODE_180,			//180度旋转		
	WINDOW_ROTATE_MODE_270,			//270度旋转	
};
enum XWindowTurnOverMode	//窗口的反转模式
{
	WINDOW_TURNOVER_MODE_NULL,		//不反转
	WINDOW_TURNOVER_MODE_LTR,		//左右反转
	WINDOW_TURNOVER_MODE_TTD,		//上下反转
	WINDOW_TURNOVER_MODE_LTR_TTD,	//上下左右反转
};
enum XWindowType
{
	WINDOW_TYPE_NORMAL,			//普通类型的窗口
	WINDOW_TYPE_EX,				//强化版的窗口类型
	WINDOW_TYPE_3D,				//3D窗口类型
};
enum XColorMode		//颜色模式
{
	COLOR_RGBA,
	COLOR_RGBA8,
	COLOR_RGBA16,
	COLOR_RGBA16F,
	COLOR_RGBA32F_ARB,

	COLOR_RGB,
	COLOR_RGB8,
	COLOR_RGB16,
	COLOR_RGB16F,
	COLOR_RGB32F_ARB,

	COLOR_GRAY,
	COLOR_GRAY8,
	COLOR_GRAY16,
	COLOR_GRAY32F_ARB,

	COLOR_GRAY_ALPHA,
	COLOR_GRAY8_ALPHA8,
	COLOR_GRAY16_ALPHA16,
	COLOR_GRAY_ALPHA32F_ARB,

	COLOR_DEPTH,	//深度贴图
	COLOR_DEPTH16,
	COLOR_DEPTH24,
	COLOR_DEPTH32,
	COLOR_DEPTH_STENCIL,

	COLOR_R8,
	COLOR_R16,
	COLOR_R16F,
	COLOR_R32F,

	COLOR_ALPHA,
	COLOR_ALPHA8,

	COLOR_RG8,
	COLOR_RG16,
	COLOR_RG16F,
	COLOR_RG32F,

	COLOR_STENCIL_INDEX,
	//下面是自定义的格式
	COLOR_RED,		//这种速度较快
	COLOR_RGBA_F,	//内部用RGBA32F
	COLOR_RGB_F,	//内部用RGB32F
	COLOR_GRAY_F,	//内部用R32F	//这个需要验证	GL_LUMINANCE精度只有8个bits
	COLOR_BGRA,
	COLOR_BGR,
};
struct XWindowInfo
{
	int w;				//窗口的宽度pixel
	int h;				//窗口的高度pixel
	std::string windowTitle;	//窗口的标题
	int sceneW;			//场景的宽度pixel
	int sceneH;			//场景的高度pixel
	bool isFullScreen;	//是否全屏
	bool isTouch;		//是否使用触摸屏 这里设置这个参数的原因在于全屏时隐藏光标的情况下使用触摸屏时SDL有问题。(目前不使用SDL的隐藏光标的接口，这个问题已经解决)
	bool isShowCursor;	//是否显示光标
	bool withFrame;		//是否拥有边框
	int withLog;		//日志输出模式
	int positionX;		//窗口原点相对于场景的坐标X
	int positionY;		//窗口原点相对于场景的坐标Y
	XWindowType windowType;	//窗口类型
	XWindowSizeMode mode;
	int CFGMode;		//配置类的工作模式
	int startDelayTimer;	//启动延迟的时间，单位ms
	bool isStartDelay;	//是否开启启动延迟
	bool isShowVersion;		//是否显示版本号
	bool isAutoShutDown;	//是否自动关机
	bool isShowConsole;		//是否显示控制台
	std::string versionString;	//版本号字符串
	int autoShutDownYear;	//自动关机的时间
	int autoShutDownMonth;
	int autoShutDownDay;
	int autoShutDownHour;
	int autoShutDownMinute;
	int autoShutDownSecond;
	int BGVolume;	//背景音量
	int FWVolume;	//前景音量
	XWindowRotateMode rotateMode;		//旋转的模式
	XWindowTurnOverMode turnOverMode;	//翻转模式
	XResPos defResourcePos;	//默认的资源位置
	std::string systemFontFile;	//系统字体文件路径
	std::string commonResourcePos;	//公用资源文件的位置
	//下面是设置窗口的位置的参数
	bool isInitWindowPos;	//是否设置窗口的位置
	bool isAlwaysTop;		//是否制置顶
	bool withCustomTitle;	//是否使用自己的标题栏，尚未在配置工具中添加
	bool withAlphaBG;		//是否使用背景透明
	int windowPosX;			//窗口的位置
	int windowPosY;
	int systemFontSize;
	int logicFps;	//逻辑帧率,<=0为不受限制
	int drawFps;	//显示帧率,<=0为不受限制
	bool withException;			//是否处理异常
	bool withLowConsumption;	//是否开启低耗模式
	bool withClearBG;			//是否清楚背景
	bool isRestartWhenCrash;	//程序crash之后是否重新启动程序
	bool isProjectAsDaemon;		//是否作为后台程序
	bool isSingletonMode;		//是否只有一个实例
	bool withProjectCorrection;	//是否使用投影矫正
	bool withMaskCorrection;	//是否使用mask矫正
	std::string maskFilename;	//mask文件的文件名
	bool withCheatModule;		//是否支持命令行输入
	bool withSoftBoard;			//是否使用虚拟键盘
	bool withCaregiver;			//是否使用看护的类
	XWindowInfo()
		:w(800)
		, h(600)
		, sceneW(800)
		, sceneH(600)
		, isFullScreen(false)
		, isTouch(false)
		, withLog(3)	//LB_LEVEL_ALL
		, isShowCursor(true)
		, positionX(0)
		, positionY(0)
		, startDelayTimer(0)
		, autoShutDownYear(-1)	//自动关机的时间
		, autoShutDownMonth(-1)
		, autoShutDownDay(-1)
		, autoShutDownHour(-1)
		, autoShutDownMinute(-1)
		, autoShutDownSecond(-1)
		, withFrame(true)
		, mode(WINDOW_SIZE_MODE_CLIP_LP)
		, rotateMode(WINDOW_ROTATE_MODE_0)
		, turnOverMode(WINDOW_TURNOVER_MODE_NULL)
		, windowType(WINDOW_TYPE_NORMAL)
		, CFGMode(0)
		, isStartDelay(false)
		, isShowVersion(false)
		, isAutoShutDown(false)
		, isShowConsole(true)
		, isProjectAsDaemon(false)
		, defResourcePos(RES_LOCAL_FOLDER)
		, BGVolume(100)	//背景音量
		, FWVolume(100)	//前景音量
		, isInitWindowPos(false)
		, windowPosX(0)
		, windowPosY(0)
		, isAlwaysTop(false)
		, withCustomTitle(false)
		, windowTitle("xiajia_1981@163.com")
		, versionString("1.0.0.0")
#if WITH_COMMON_RESOURCE_FOLDER
		, systemFontFile("../../Common/SIMLI.TTF")
		, commonResourcePos("../../Common/")
#else
		, systemFontFile("SIMLI.TTF")
		, commonResourcePos("./")
#endif
		, systemFontSize(30)
		, withAlphaBG(false)
		, logicFps(-1)
		, drawFps(30)
		, withException(true)
		, withLowConsumption(true)	//是否开启低耗模式
		, withClearBG(true)			//是否清楚背景
		, isRestartWhenCrash(true)
		, isSingletonMode(false)
		, withProjectCorrection(false)
		, withMaskCorrection(false)
		, maskFilename("ResourcePack/normalResource/maskX.png")
		, withCheatModule(false)
		, withSoftBoard(false)
		, withCaregiver(false)
	{}
	XWindowInfo(int width, int height, XWindowType type = WINDOW_TYPE_NORMAL)
		:w(width)
		, h(height)
		, sceneW(width)
		, sceneH(height)
		, isFullScreen(false)
		, isTouch(false)
		, withLog(3)	//LB_LEVEL_ALL
		, isShowCursor(true)
		, positionX(0)
		, positionY(0)
		, startDelayTimer(0)
		, autoShutDownYear(-1)	//自动关机的时间
		, autoShutDownMonth(-1)
		, autoShutDownDay(-1)
		, autoShutDownHour(-1)
		, autoShutDownMinute(-1)
		, autoShutDownSecond(-1)
		, withFrame(true)
		, mode(WINDOW_SIZE_MODE_CLIP_LP)
		, rotateMode(WINDOW_ROTATE_MODE_0)
		, turnOverMode(WINDOW_TURNOVER_MODE_NULL)
		, windowType(type)
		, CFGMode(0)
		, isStartDelay(false)
		, isShowVersion(false)
		, isAutoShutDown(false)
		, isShowConsole(true)
		, isProjectAsDaemon(false)
		, defResourcePos(RES_LOCAL_FOLDER)
		, BGVolume(100)	//背景音量
		, FWVolume(100)	//前景音量
		, isInitWindowPos(false)
		, windowPosX(0)
		, windowPosY(0)
		, isAlwaysTop(false)
		, withCustomTitle(false)
		, windowTitle("xiajia_1981@163.com")
		, versionString("1.0.0.0")
#if WITH_COMMON_RESOURCE_FOLDER
		, systemFontFile("../../Common/SIMLI.TTF")
		, commonResourcePos("../../Common/")
#else
		, systemFontFile("SIMLI.TTF")
		, commonResourcePos("./")
#endif
		, systemFontSize(30)
		, withAlphaBG(false)
		, logicFps(-1)
		, drawFps(30)
		, withException(true)
		, withLowConsumption(true)	//是否开启低耗模式
		, withClearBG(true)			//是否清楚背景
		, isRestartWhenCrash(true)
		, isSingletonMode(false)
		, withProjectCorrection(false)
		, withMaskCorrection(false)
		, maskFilename("ResourcePack/normalResource/maskX.png")
		, withCheatModule(false)
		, withSoftBoard(false)
		, withCaregiver(false)
	{}
};
}
#endif