#include "XEffeEngine.h"
#include "XErrorReporter.h"

void errorReporterBtnProc(void *pClass,int id)
{
	_XErrorReporter *p = (_XErrorReporter *)pClass;
	p->m_isBtnChecked = true;
}
bool _XErrorReporter::reportFile(const std::string & errorDescription,const std::string &filename,int line,const std::string &funName)
{
	_XSystemTime t;
	getTimeMs(t);
	//输出到后台
	printf("---------------- Head Of File ----------------\n");
	printf("时间：%d-%d-%d %d:%d:%d %d\n",t.year,t.month,t.day,t.hour,t.minute,t.second,t.millisecond);
	printf("错误描述：%s\n",errorDescription.c_str());
	printf("执行文件：%s\n",getCurrentExeFileFullPath().c_str());
	printf("文件：%s @ %d 行\n",filename.c_str(),line);
	printf("函数：%s\n",funName.c_str());
	printf("编译时间：%s %s\n",__DATE__,__TIME__);
	printf("项目名称：%s\n",XEE::windowData.windowTitle.c_str());
	printf("版本号：%s\n",XEE::windowData.versionString.c_str());
	printf("---------------- End Of File ----------------");
	//输出到文件
	FILE *fp = NULL;
	if((fp = fopen(ERROR_REPORTER_TXT_FILENAME,"w")) == NULL) return false;
	fprintf(fp,"---------------- Head Of File ----------------\n");
	fprintf(fp,"时间：%d-%d-%d %d:%d:%d %d\n",t.year,t.month,t.day,t.hour,t.minute,t.second,t.millisecond);
	fprintf(fp,"错误描述：%s\n",errorDescription.c_str());
	fprintf(fp,"执行文件：%s\n",getCurrentExeFileFullPath().c_str());
	fprintf(fp,"文件：%s @ %d 行\n",filename.c_str(),line);
	fprintf(fp,"函数：%s\n",funName.c_str());
	fprintf(fp,"编译时间：%s %s\n",__DATE__,__TIME__);
	fprintf(fp,"项目名称：%s\n",XEE::windowData.windowTitle.c_str());
	fprintf(fp,"版本号：%s\n",XEE::windowData.versionString.c_str());
	if(m_isInited)
	{//输出用户备注
		if(m_edit.getString() != NULL)
			fprintf(fp,"备注：%s\n",m_edit.getString());
	}
	fprintf(fp,"---------------- End Of File ----------------");
	fclose(fp);

	return true;
}
int _XErrorReporter::inputEvent()
{//这个也不支持多线程
	int ret = 0;
	_XInputEvent tmpEvent;

	while(getInputEvent(tmpEvent))	//注意SDL的事件队列是不支持多线程的，只能在主线程中调用
	{
		switch(tmpEvent.type)
		{
		case EVENT_EXIT:ret = 1;break;
		case EVENT_KEYBOARD:
			if(tmpEvent.keyState == KEY_STATE_DOWN)
			{
				switch(tmpEvent.keyValue)
				{
				case XKEY_ESCAPE:ret = 1;break;
				}
			}
			break;
		case EVENT_MOUSE:
			if(tmpEvent.mouseState == MOUSE_LEFT_BUTTON_DOWN
				|| tmpEvent.mouseState == MOUSE_RIGHT_BUTTON_DOWN
				|| tmpEvent.mouseState == MOUSE_MIDDLE_BUTTON_DOWN)
			{//空白区域按下任意鼠标键退出
				if(!m_button.isInRect(XEE::mousePosition.x,XEE::mousePosition.y)
					&& !m_edit.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
					ret = 1;
			}
			break;
		}
		XEE::inputEvent(tmpEvent);
	}
	return ret;
}
void _XErrorReporter::reportError(const std::string & errorDescription,const std::string &filename,int line,const std::string &funName)
{
	if(!m_needShow)
	{
		m_errDescription = errorDescription;	//错误描述
		m_errFilename = filename;		//错误发生的文件名
		m_errFunname = funName;		//错误发生的函数名
		m_errLine = line;					//错误发生的行号
		m_needShow = true;	//是否需要显示错误报告界面
	}
	if(GetCurrentThreadId() == XEE::mainThreadHandle) 
	{//如果是主线程则直接进入退出处理
		errorProc();
		exit(1);	//直接退出程序
	}else
	{//如果在其他线程中则阻塞线程并等待主线程处理
		//方案1、子线程自动挂起，主线程中检测状态并作相应的处理
		//问题，如果主线程与子线程存在互锁关系，那么会造成死锁
		//while(1) Sleep(1);
		//方案2、由于子线程不能进行OpenGL渲染，所以子线程中不能做图形处理
		//所以错误处理抛弃图形化界面直接做最简单的输出
		//m_isInited = false;	
		//errorProc();
		//exit(1);	//直接退出程序
		//方案3、直接切换上下文，会造成截图错误
		//Sleep(100);	//等待主线程接收处理，如果不能接收处理则，这里独自处理
		//if(!wglMakeCurrent(XEE::wHDC,XEE::wCurrentHGLRC))
		//{
		//	printf("使用副本上下文\n");
		//	wglMakeCurrent(XEE::wHDC,XEE::wCopyHGLRC);
		//}
		//errorProc();
		//exit(1);	//直接退出程序
		//方案4、先等待主线程处理如果指定时间内主线程没有处理则自己简单处理
		Sleep(500);
		if(m_isMainThreadProc)
		{
			printf("主线程接收了错误处理!\n");
			while(1) Sleep(1);
		}
		printf("主线程未接收错误处理!\n");
		m_isInited = false;	
		errorProc();
		exit(1);	//直接退出程序
	}
}
void _XErrorReporter::errorProc()
{
	if(!m_needShow) return;
	if(m_isInited)
	{//如果已经初始化，则这里是图形化界面的处理
		_XWindow.setCurcor(true);//显示鼠标光标
		gameShot(ERROR_REPORTER_PIC_FILENAME,XEE::windowData.w,XEE::windowData.h,false);//屏幕截图
		m_screenShot.init(ERROR_REPORTER_PIC_FILENAME,RESOURCE_LOCAL_FOLDER);
		m_edit.setVisiable();
		m_button.setVisiable();
		string temp = "错误描述:";
		temp += m_errDescription;
		m_fontDescrip.setString(temp.c_str());
		while(!inputEvent()) 
		{//程序主循环
			if(m_isBtnChecked) break;
			XEE::engineIdle();
			//TODO:这里运行你的逻辑
			XEE::clearScreen();			//清除屏幕
			//TODO:这里显示和更新你需要显示的内容
			XEE::begin2DDraw();
			draw();

			XEE::updateScreen();			//更新屏幕的内容
			mySleep(1);
		}
	}
	reportFile(m_errDescription,m_errFilename,m_errLine,m_errFunname);
	_XLogbook::GetInstance().releaseLog();	//输出所有日志信息
	XEE::release();
}
bool _XErrorReporter::init(_XResourcePosition resoursePosition)	//初始化
{
	if(m_isInited) return false;
#if WITH_COMMON_RESOURCE_FOLDER
	if(!m_background.init("../../Common/ResourcePack/pic/ErrorReporter/Back.png",resoursePosition)) return false;
	if(!m_button.initPlus("../../Common/ResourcePack/pic/ErrorReporter/btn"," ",XEE::systemFont,1.0f,resoursePosition)) return false;
#else
	if(!m_background.init("ResourcePack/pic/ErrorReporter/Back.png",resoursePosition)) return false;
	if(!m_button.initPlus("ResourcePack/pic/ErrorReporter/btn"," ",XEE::systemFont,1.0f,resoursePosition)) return false;
#endif
	if(!m_edit.initWithoutTex(_XRect(0.0f,0.0f,448.0f,40.0f),"此处输入备注.",XEE::systemFont,1.0f)) return false;
	m_edit.setPosition(XEE::windowData.w * 0.5f - 220.0f,XEE::windowData.h * 0.5f + 20.0f);
	m_edit.disVisiable();
	m_background.setPosition(XEE::windowData.w * 0.5f - 256.0f,XEE::windowData.h * 0.5f - 128.0f);
	m_button.setPosition(XEE::windowData.w * 0.5f - 77.0f,XEE::windowData.h * 0.5f + 65.0f);
	m_button.setCallbackFun(NULL,NULL,NULL,errorReporterBtnProc,NULL,this);
	m_button.disVisiable();
	m_font.setACopy(XEE::systemFont);
	m_font.setPosition(XEE::windowData.w * 0.5f - 220.0f,XEE::windowData.h * 0.5f - 50.0f);
	m_font.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT);
	m_font.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);
	m_font.setMaxStrLen(512);
	m_font.setSize(0.5f);
	m_font.setColor(0.0f,0.0f,0.0f,1.0f);
	m_font.setString("非常抱歉,程序遇到了不可修复的错误,即将退出.您可以点[确定]按钮\n\
生成错误报告并退出,或者按下键盘[Esc]键退出.您也可以按下鼠标任\n\
意键退出.我们的技术人员会尽快修复这个错误!");
	m_fontDescrip.setACopy(XEE::systemFont);
	m_fontDescrip.setPosition(XEE::windowData.w * 0.5f - 220.0f,XEE::windowData.h * 0.5f + 1.0f);
	m_fontDescrip.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT);
	m_fontDescrip.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);
	m_fontDescrip.setMaxStrLen(512);
	m_fontDescrip.setSize(0.5f);
	m_fontDescrip.setColor(0.0f,0.0f,0.0f,1.0f);
	m_fontDescrip.setString("错误描述");
	m_isInited = true;
	return true;
}