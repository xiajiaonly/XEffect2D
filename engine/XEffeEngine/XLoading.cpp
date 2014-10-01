//++++++++++++++++++++++++++++++++
//Author:    贾胜华(JiaShengHua)
//Version:    1.0.0
//Date:       2011.12.10
//--------------------------------
#include "XLoading.h"
#include "XResourcePack.h"

_XBool isLoadingStart = XFalse;
_XLoading *xLoading = NULL;
_XLoadingScene *xLoadingScene = NULL;

_XBool startLoading(_XLoadingScene * scene,
		_XResourcePosition/*resoursePosition*/)
{
	if(isLoadingStart) return XFalse;
	if(scene == NULL) return XFalse;

	xLoading = createMem<_XLoading>();
	if(xLoading == NULL) return XFalse;
	xLoadingScene = scene;

	xLoading->init(xLoadingScene,RESOURCE_LOCAL_PACK);
	xLoading->setStart();

	isLoadingStart = XTrue;
	return XTrue;
}
_XBool endLoading()
{
	if(!isLoadingStart) return XFalse;

	xLoading->setLoadPresent(100.0f);
	xLoading->setEnd();
	XDELETE(xLoading);
	XDELETE(xLoadingScene);

	isLoadingStart = XFalse;
	return XTrue;
}
#ifdef XEE_OS_WINDOWS
void _XLoading::init(_XLoadingScene * scene,_XResourcePosition resoursePosition)
{
	m_resoursePosition = resoursePosition;

	m_isShow = XFalse;
	m_loadPresent = 0.0f;
	m_isEnd = STATE_END;
	//m_windowSize = windowSize;
	m_pScene = scene;
}
void _XLoading::setStart()
{//开启一个线程
	m_isShow = XTrue;
	m_isEnd = STATE_BEFORE_START;
	pthread_t moveThreadP;

	m_hDC = wglGetCurrentDC();

	if(CreateThread(0,0,loadingProc,this,0,&moveThreadP) == 0) 
	{
		LogStr("多线程开启失败！");
	}
	while(true)
	{
		if(m_isEnd == STATE_START) break;
		mySleep(10);
	}
}
void _XLoading::setEnd()
{
	setLoadPresent(100.0);
	mySleep(100);
	m_isShow = XFalse;
	while(true)
	{
		if(m_isEnd == STATE_END) break;
		mySleep(10);
	}
}
DWORD WINAPI _XLoading::loadingProc(void * pParam)
{
	_XLoading &pPar = *(_XLoading *) pParam;

	//pPar.m_hDC = wglGetCurrentDC();
	HGLRC hGlrc = wglCreateContext(pPar.m_hDC);
	wglMakeCurrent(pPar.m_hDC,hGlrc);

	glMatrixMode(GL_PROJECTION);					//设置当前矩阵模式（对投影矩阵应用之后的矩阵操作）
	glLoadIdentity();								//变换坐标系函数
	glOrtho(0,XEE::windowWidth,XEE::windowHeight, 0, -1, 1);;
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClearColor(XEE::defaultBGColor.fR,XEE::defaultBGColor.fG,XEE::defaultBGColor.fB,XEE::defaultBGColor.fA);			//清除颜色
	glDisable(GL_DEPTH);							//2D项目不需要深度测试

	if(pPar.m_pScene != NULL) pPar.m_pScene->init(pPar.m_resoursePosition);

	LogStr("Enter LoadingProc!");
	pPar.m_isEnd = STATE_START;
	while(true)
	{
		if(XEE::isSuspend) break;
		pPar.move();

		//wglMakeCurrent(pPar.m_hDC,hGlrc);
		//清除屏幕
	//	clearScreen();
		pPar.draw();
		XEE::updateScreen();
		//wglMakeCurrent(NULL,NULL);

		if(!pPar.m_isShow)
		{
	//		clearScreen();
			break;
		}
		if(pPar.m_loadPresent < 100) pPar.m_loadPresent += pPar.m_speed;
		if(pPar.m_pScene != NULL) pPar.m_pScene->setLoadPresent(pPar.m_loadPresent);
		mySleep(20000);
	}
	pPar.release();
	wglMakeCurrent(NULL,NULL);
	wglDeleteContext(hGlrc);
	LogStr("Loading End!");
	pPar.m_isEnd = STATE_END;
	return 1;
}
#endif

#ifdef XEE_OS_LINUX
void _XLoading::init(const _XVector2 &windowSize,int resoursePosition)
{
	if(resoursePosition != RESOURCE_OUTSIDE)
	{
		m_resoursePosition = RESOURCE_INSIDE;
	}else
	{
		m_resoursePosition = RESOURCE_OUTSIDE;
	}

	m_isShow = 0;
	m_loadPresent = 0;
	m_isEnd = 2;
	m_windowSize = windowSize;
	//在这里载入相关资源
	m_loadingBG.init("pic/Loading/LoadingBG.png",NULL,1,m_resoursePosition);		//设置界面的第一个界面
	m_loadingBG.setPosition(_XVector2::zero);
//	m_loadingBG.setSize(m_windowSize.x/64.0,m_windowSize.y/64.0);
//	m_loadingBG.setIsResizeCenter(POINT_LEFT_TOP);

	Number.init("pic/Number.png",_XVector2I(15,30),_XVector2I(8,2),m_resoursePosition);
	Number.setPosition(10,10);
	ProgressTexture.init("pic/Progress/PB_00.png","pic/Progress/PM_00.png","pic/Progress/PU_00.png",m_resoursePosition);
	float tempSize = m_windowSize.x/1280.0;
	m_process.init(_XRect(m_windowSize.x * 0.5 - 256 * tempSize,m_windowSize.y * 0.5 - 16 * tempSize,
		m_windowSize.x * 0.5 - 256 * tempSize + 512,m_windowSize.y * 0.5 - 16 * tempSize + 32),
		&ProgressTexture,&Number,1.0,_XVector2(250,0));
	m_process.setSize(tempSize,tempSize);
	m_loadPresent = 0;
	m_process.setValue(m_loadPresent);
}

void _XLoading::setStart()
{//开启一个线程
	m_isShow = 1;
	m_isEnd = 0;
	pthread_t moveThreadP;
	//record the old context
	m_mainDisplay = glXGetCurrentDisplay();
	m_mainPBuffer = glXGetCurrentDrawable();
	m_mainContext = glXGetCurrentContext();
	//create new context
	int att[] = {GLX_RENDER_TYPE, GLX_RGBA_BIT,GLX_DOUBLEBUFFER, True,GLX_DEPTH_SIZE,24,None};
	//int att[] = {GLX_RENDER_TYPE,GLX_RGBA_BIT,GLX_DRAWABLE_TYPE,GLX_PBUFFER_BIT,GLX_RED_SIZE,8,
	//	GLX_GREEN_SIZE,8,GLX_BLUE_SIZE,8,GLX_ALPHA_SIZE,8,GLX_DEPTH_SIZE,24,GLX_STENCIL_SIZE,8,
	//	GLX_FLOAT_COMPONENTS_NV,XFalse,GLX_SAMPLE_BUFFERS_ARB,XTrue,GLX_SAMPLES_ARB,2,0};
	int pattrib[] = {GLX_LARGEST_PBUFFER,XTrue,GLX_PRESERVED_CONTENTS,XTrue,
		GLX_PBUFFER_WIDTH,m_windowSize.x,GLX_PBUFFER_HEIGHT,m_windowSize.y,0};

	int screen = DefaultScreen(m_mainDisplay);
	const char *extensions = glXQueryExtensionsString(m_mainDisplay,screen);
	int nelements;
	GLXFBConfig *config = glXChooseFBConfig(m_mainDisplay,screen,att,&nelements);
	m_threadPBuffer = glXCreatePbuffer(m_mainDisplay,*config,pattrib);
	m_threadContext = glXCreateNewContext (m_mainDisplay, *config, GLX_RGBA_TYPE,m_mainContext,XTrue);
	if(config == NULL ||m_threadContext == NULL || m_threadPBuffer == NULL)
	{
		printf("Create error!\n");
		exit(1);
	}else
	{
		printf("Create OK!\n");
	}


    	if(pthread_create(&moveThreadP, NULL, &loadingProc, (void*) this) != 0) 
	{
		LogStr("多线程开启失败！");
	}
}

void *_XLoading::loadingProc(void * pParam)
{
	_XLoading &pPar = *(_XLoading *) pParam;
	//切换上下文
	glXMakeCurrent(pPar.m_mainDisplay,pPar.m_threadPBuffer,pPar.m_threadContext);
	int isSuccess = 0;					//记录加载是否成功的标记变量
	if(pPar.m_loadFunc != NULL)
	{
		isSuccess = pPar.m_loadFunc();	//记录加载是否成功
	}
	//还原原有的上下文
	glXMakeCurrent(pPar.m_mainDisplay,None,NULL);
	glXDestroyContext(pPar.m_mainDisplay,pPar.m_threadContext);
	glXDestroyPbuffer(pPar.m_mainDisplay,pPar.m_threadPBuffer);
	glXMakeCurrent(pPar.m_mainDisplay,pPar.m_mainPBuffer,pPar.m_mainContext);
	if(isSuccess == 1) pPar.m_isEnd = 1;	//加载成功
	else pPar.m_isEnd = 2;					//加载失败
}
#endif
