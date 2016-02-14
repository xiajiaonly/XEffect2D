#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XFrameEx.h"
#include "XResourcePack.h"
#include "XSprite.h"
#include "XFile.h"
namespace XE{
#define FRAME_DEBUG (0)
#if FRAME_DEBUG
int frameTestSum = 0;
#endif
XBool XFrameEx::init(const char *filename,XResourcePosition resoursePosition)
{
	if(filename == NULL) return false;
	if(m_isInited) 
	{
		LogStr("The action have initted!");
		return XTrue;
	}
	if(resoursePosition == RESOURCE_SYSTEM_DEFINE) resoursePosition = getDefResPos();

	if(!loadData(filename,resoursePosition)) return false;
	m_position.set(0.0f,0.0f);	//序列帧的位置
	setAngle(0);		//序列帧的角度
	m_scale.set(1.0f,1.0f);	//序列帧的缩放比例

	m_isInited = XTrue;
	return XTrue;
}
XBool XFrameEx::releaseMem()
{
	if(!m_isInited) return XTrue;
	if(m_isThreadLoading)//这里需要等待线程结束
		waitThreadEnd(m_loadThread);

	XMem::XDELETE_ARRAY(m_pSprite);
	
#if FRAME_DEBUG
	-- frameTestSum;
	if(frameTestSum == 10)
	{
		printf("haha\n");
	}
	printf("Frame new sum:%d\n",frameTestSum);
#endif
#if WITH_OBJECT_MANAGER	//在物件管理的类中注销这些物件
	XObjManager.decreaseAObject(this);
#endif

	m_isInited = XFalse;
	return XTrue;
}
void XFrameEx::draw()
{//序列帧精灵
	if(!m_isVisible ||
		!m_isInited ||
		(m_isDisappearAtEnd && m_isEnd)) return;	//播放完成之后消失
	if(m_isThreadLoading) return;	//正在载入的时候不能描绘
	int keyFrameIndex = 0;
	if(m_isEnd)
	{
		if(m_isStopAtEnd) keyFrameIndex = m_keyFrameArray[m_allFramesSum - 1];
		else keyFrameIndex = 0;
	}else
	{
		keyFrameIndex = m_keyFrameArray[(int)(m_curFramesNumble)];
	}
	if(keyFrameIndex >= 0 && keyFrameIndex < m_allKeyFramesSum)
	{
	//	m_pSprite[keyFrameIndex].m_pShaderProc = m_pShaderProc;
		m_pSprite[keyFrameIndex].setShaderClass(m_pShader);// = m_pShader;
		m_pSprite[keyFrameIndex].draw();
	//	m_pSprite[keyFrameIndex].m_isFlash = 0;
	//	m_pSprite[keyFrameIndex].m_pShaderProc = NULL;
	}
}
void XFrameEx::move(float timeDelay)
{
	if(!m_isInited || m_isEnd) return;
	if(m_isThreadLoading || m_isPause) return;	//正在载入的时候不能更新
	if(m_isEndImmediately && m_isSetEnd) m_isEnd = XTrue;

	float tmp = m_curFramesNumble;
	m_curFramesNumble += m_actionSpeed * timeDelay;			//推动序列帧动画
	if(m_pauseAtIndex >= 0 && 
		tmp < m_pauseAtIndex && m_curFramesNumble >= m_pauseAtIndex)
	{//这里控制播放到指定帧之后暂停
		m_curFramesNumble = m_pauseAtIndex;
		m_pauseAtIndex = -1;
		pause();
	}
	if(m_curFramesNumble >= m_allFramesSum) 
	{//如果播放到最后一帧，这里回帧
		m_curFramesNumble = 0.0f;

		if(!m_isEndImmediately && m_isSetEnd) m_isEnd = XTrue;//如果不是可以立刻结束的动画，这里播放完成一个循环之后，即可结束
		if(!m_isLoop) m_isEnd = XTrue;//如果是不循环的动画这里也可以比较结束
	}else	
	if(m_curFramesNumble < 0) 
	{//如果是倒序播放的话，这里判断是否播放完成(以及进行完成处理)
		m_curFramesNumble = m_allFramesSum - 0.05f;		//note this 0.05!!

		if(!m_isEndImmediately && m_isSetEnd) m_isEnd = XTrue;
		if(!m_isLoop) m_isEnd = XTrue;
	}
}
void XFrameEx::setAttribute(const XVector2& position,XBool loop,XBool endImmediately,int startFrame,float actionSpeed,XBool disappearAtEnd,XBool isOverturn)
{
	if(!m_isInited) return;
	m_position = position;	//序列帧的位置

	m_isLoop = loop;
	m_isEndImmediately = endImmediately;

	m_startFrame = startFrame;
	m_curFramesNumble = m_startFrame;

	m_actionSpeed = actionSpeed;
	m_isDisappearAtEnd = disappearAtEnd;

	if((m_isOverturn && !isOverturn) || (!m_isOverturn && isOverturn))
		m_isOverturn = isOverturn;
	if(m_isThreadLoading)
	{
		m_neadUpdateAttribute = true;
	}else
	{
		//将设置的属性设置到精灵内部使之生效
		for(int i = 0;i < m_allKeyFramesSum;++ i)
		{
			m_pSprite[i].setPosition(m_position);
			m_pSprite[i].setOverturn(m_isOverturn);
		}
	}
}
XFrameEx::XFrameEx()
	:m_resoursePosition(RESOURCE_AUTO)
	,m_pSprite(NULL)
	,m_angle(0.0f)
	,m_isVisible(XTrue)
	,m_position(0.0f,0.0f)
	,m_scale(1.0f,1.0f)
	,m_color(1.0f,1.0f,1.0f,1.0f)
//	,alpha(1.0f)
//	,colorRed(1.0f)
//	,colorGreen(1.0f)
//	,colorBlue(1.0f)
	,m_isOverturn(XFalse)	//默认情况下不翻转
	,m_allFramesSum(0)							//序列帧动画中总帧数
	,m_allKeyFramesSum(0)						//序列帧动画中关键帧的数量
	,m_curFramesNumble(0.0f)					//当前播放到第几帧
	,m_actionSpeed(0.0f)						//序列帧播放的速度
	,m_isLoop(XFalse)								//序列帧是否可以循环
	,m_startFrame(0)							//从哪一帧开始
	,m_isEnd(XFalse)								//序列帧是否播放完成
	,m_isEndImmediately(XFalse)					//动画是否为立刻结束类型
	,m_isSetEnd(XFalse)							//is end by user			//是否设置序列帧结束
	,m_isDisappearAtEnd(XFalse)					//动画帧是否在播放完成之后消失
	,m_isInited(XFalse)
	,m_frameName(NULL)
	,m_keyFrameArray(NULL)
	,m_isACopy(XFalse)
	,m_blendTypeScr(4)
	,m_blendTypeDst(5)
	//,m_pShaderProc(NULL)
	,m_pShader(NULL)
	,m_cp(NULL)
	,m_isThreadLoading(STATE_BEFORE_START)
	,m_neadUpdateAttribute(false)
	,m_isPause(false)
	,m_isStopAtEnd(false)
	,m_pauseAtIndex(-1)
{
	m_objType = OBJ_FRAMEEX;
}
XFrameEx::~XFrameEx()
{
	releaseMem();
	if(m_cp != NULL && -- m_cp->m_counter <= 0)
	{
		XMem::XDELETE_ARRAY(m_keyFrameArray);
		XMem::XDELETE_ARRAY(m_frameName);
		XMem::XDELETE(m_cp);
	}
}
XFrameEx& XFrameEx::operator = (const XFrameEx& temp)
{
	if(&temp == this) return *this;			//防止没必要的自我拷贝
	//if(m_isInited != 0) return *this;			//自身已经初始化过，就不能再复制别人
	if(temp.m_cp != NULL) ++temp.m_cp->m_counter;
	//释放自身的资源
	if(m_cp != NULL && -- m_cp->m_counter <= 0)
	{//自身没有引用需要释放
		XMem::XDELETE_ARRAY(m_keyFrameArray);
		XMem::XDELETE_ARRAY(m_frameName);
		XMem::XDELETE(m_cp);
	}
	m_cp = temp.m_cp;
	XMem::XDELETE_ARRAY(m_pSprite);

	m_blendTypeScr = temp.m_blendTypeScr;
	m_blendTypeDst = temp.m_blendTypeDst;
	//m_pShaderProc = temp.m_pShaderProc;
	m_pShader = temp.m_pShader;
	m_isVisible = temp.m_isVisible;					//精灵的角度
	m_angle = temp.m_angle;					//精灵的角度
	m_position = temp.m_position;		//精灵的位置
	m_scale = temp.m_scale;				//精灵的缩放尺寸
	m_color = temp.m_color;
//	alpha = temp.alpha;					//透明度
//	colorRed = temp.colorRed;			//红色
//	colorGreen = temp.colorGreen;		//绿色
//	colorBlue = temp.colorBlue;			//蓝色
	m_isOverturn = temp.m_isOverturn;	//是否x方向左右翻转
	m_isPause = temp.m_isPause;	
	m_isStopAtEnd = temp.m_isStopAtEnd;
	m_pauseAtIndex = temp.m_pauseAtIndex;

	m_allFramesSum = temp.m_allFramesSum;					//序列帧动画中总帧数
	m_allKeyFramesSum = temp.m_allKeyFramesSum;				//序列帧动画中关键帧的数量
	m_curFramesNumble = temp.m_curFramesNumble;				//当前播放到第几帧
	m_actionSpeed = temp.m_actionSpeed;						//序列帧播放的速度
	m_isLoop = temp.m_isLoop;								//序列帧是否可以循环
	m_startFrame = temp.m_startFrame;						//从哪一帧开始
	m_isEnd = temp.m_isEnd;									//序列帧是否播放完成
	m_isEndImmediately = temp.m_isEndImmediately;			//动画是否为立刻结束类型
	m_isSetEnd = temp.m_isSetEnd;							//是否设置序列帧结束
	m_isDisappearAtEnd = temp.m_isDisappearAtEnd;			//动画帧是否在播放完成之后消失

	m_frameName = temp.m_frameName;
	m_keyFrameArray = temp.m_keyFrameArray;
	if(temp.m_isInited)
	{
#if FRAME_DEBUG
		++ frameTestSum;
		printf("Frame new sum:%d\n",frameTestSum);
#endif
		m_pSprite = XMem::createArrayMem<XSprite>(m_allKeyFramesSum);
		if(m_pSprite == NULL)
		{
			m_isInited = XFalse;
			return *this;
		}
		for(int i = 0;i < m_allKeyFramesSum;++ i)
		{
			m_pSprite[i].setACopy(temp.m_pSprite[i]);
#if WITH_OBJECT_MANAGER	//在物件管理的类中注销这些物件
			XObjManager.decreaseAObject(&(m_pSprite[i]));
#endif
		}
	}
#if WITH_OBJECT_MANAGER	//注册序列帧这个物件
	if(!m_isACopy) XObjManager.addAObject(this);
#endif
	m_isInited = temp.m_isInited;
	m_isACopy = XTrue;
	return *this;
}
void XFrameEx::setACopy(const XFrameEx& temp)
{
	if(&temp == this) return;			//防止没必要的自我拷贝
	if(m_isThreadLoading || temp.m_isThreadLoading) return;	//正在多线程载入的元素不能复制
	//if(m_isInited != 0) return;			//自身已经初始化过，就不能再复制别人
	if(temp.m_cp != NULL) ++temp.m_cp->m_counter;
	//释放自身的资源
	if(m_cp != NULL && -- m_cp->m_counter <= 0)
	{//自身没有引用需要释放
		XMem::XDELETE_ARRAY(m_keyFrameArray);
		XMem::XDELETE_ARRAY(m_frameName);
		XMem::XDELETE(m_cp);
	}
	m_cp = temp.m_cp;
	XMem::XDELETE_ARRAY(m_pSprite);

	m_blendTypeScr = temp.m_blendTypeScr;
	m_blendTypeDst = temp.m_blendTypeDst;
	//m_pShaderProc = temp.m_pShaderProc;
	m_pShader = temp.m_pShader;
	m_isVisible = temp.m_isVisible;					//精灵的角度
	m_angle = temp.m_angle;					//精灵的角度
	m_position = temp.m_position;		//精灵的位置
	m_scale = temp.m_scale;				//精灵的缩放尺寸
	m_color = temp.m_color;
//	alpha = temp.alpha;					//透明度
//	colorRed = temp.colorRed;			//红色
//	colorGreen = temp.colorGreen;		//绿色
//	colorBlue = temp.colorBlue;			//蓝色
	m_isOverturn = temp.m_isOverturn;	//是否x方向左右翻转
	m_isPause = temp.m_isPause;
	m_isStopAtEnd = temp.m_isStopAtEnd;
	m_pauseAtIndex = temp.m_pauseAtIndex;

	m_allFramesSum = temp.m_allFramesSum;					//序列帧动画中总帧数
	m_allKeyFramesSum = temp.m_allKeyFramesSum;				//序列帧动画中关键帧的数量
	m_curFramesNumble = temp.m_curFramesNumble;				//当前播放到第几帧
	m_actionSpeed = temp.m_actionSpeed;						//序列帧播放的速度
	m_isLoop = temp.m_isLoop;								//序列帧是否可以循环
	m_startFrame = temp.m_startFrame;						//从哪一帧开始
	m_isEnd = temp.m_isEnd;									//序列帧是否播放完成
	m_isEndImmediately = temp.m_isEndImmediately;			//动画是否为立刻结束类型
	m_isSetEnd = temp.m_isSetEnd;							//是否设置序列帧结束
	m_isDisappearAtEnd = temp.m_isDisappearAtEnd;			//动画帧是否在播放完成之后消失

	m_frameName = temp.m_frameName;
	m_keyFrameArray = temp.m_keyFrameArray;
	if(temp.m_isInited)
	{
#if FRAME_DEBUG
		++ frameTestSum;
		printf("Frame new sum:%d\n",frameTestSum);
#endif
		m_pSprite = XMem::createArrayMem<XSprite>(m_allKeyFramesSum);
		if(m_pSprite == NULL)
		{
			m_isInited = XFalse;
			return;
		}
		for(int i = 0;i < m_allKeyFramesSum;++ i)
		{
			m_pSprite[i].setACopy(temp.m_pSprite[i]);
#if WITH_OBJECT_MANAGER	//在物件管理的类中注销这些物件
			XObjManager.decreaseAObject(&(m_pSprite[i]));
#endif
		}
	}
#if WITH_OBJECT_MANAGER	//注册序列帧这个物件,这个m_isACopy的判断是为了防止重复注册，不过注册本身就已经避免了重复注册
	if(!m_isACopy) XObjManager.addAObject(this);
#endif
	m_isInited = temp.m_isInited;
	m_isACopy = XTrue;
}
XFrameEx::XFrameEx(const XFrameEx &temp)
{
	if(&temp == this) return;	//防止资深拷贝
}
bool XFrameEx::loadFromFolder(const char *filename)	//从文件夹中载入资源
{
	FILE *fp = NULL;
	if((fp = fopen(filename,"rb")) == NULL)
	{
		LogStr("Action text file error!");
		return XFalse;
	}

	if(fscanf(fp,"%d:",&m_allKeyFramesSum) != 1) {fclose(fp);return XFalse;}
	if(m_allKeyFramesSum <= 0)
	{
		LogNull("Action text file data error:%s!\n",filename);
		fclose(fp);
		return XFalse;
	}
	if(fscanf(fp,"%d:",&m_allFramesSum) != 1) {fclose(fp);return XFalse;}
	if(m_allFramesSum <= 0)
	{
		LogNull("Action text file data error:%s!\n",filename);
		fclose(fp);
		return XFalse;
	}
	m_keyFrameArray = XMem::createArrayMem<int>(m_allFramesSum);
	if(m_keyFrameArray == NULL) 
	{
		return XFalse;
	}
	{//读取标记符	D：default, M：menutrue
		char tempFlag = ' ';
		if(fscanf(fp,"%c:",&tempFlag) != 1) {fclose(fp);return XFalse;}
		if(tempFlag == 'D' || tempFlag == 'd')
		{
			for(int i =0;i < m_allFramesSum;++ i)
			{
				m_keyFrameArray[i] = i;
			}
		}else
		{	
			for(int i =0;i < m_allFramesSum;++ i)
			{
				if(fscanf(fp,"%d,",&m_keyFrameArray[i]) != 1) {fclose(fp);return XFalse;}
				if(m_keyFrameArray[i] < 0 || m_keyFrameArray[i] >= m_allKeyFramesSum)
				{
					LogNull("Blank frame:%s -> %d!\n",filename,i);
				//	fclose(fp);
				//	XMem::XDELETE_ARRAY(m_keyFrameArray);
				//	return XFalse;
				}
			}
		}
	}
	fclose(fp);
	return true;
}
bool XFrameEx::loadFromPacker(const char *filename)	//从压缩包中载入资源
{
	unsigned char *p = XResPack.getFileData(filename);
	if(p == NULL) return XFalse;

	int offset = 0;
	if(sscanf((char *)(p + offset),"%d:",&m_allKeyFramesSum) != 1) {XMem::XDELETE_ARRAY(p);return XFalse;}
	offset += XString::getCharPosition((char *)(p + offset),':') + 1;
	if(m_allKeyFramesSum <= 0)
	{
		LogNull("Action text file data error:%s!\n",filename);
		XMem::XDELETE_ARRAY(p);
		XMem::XDELETE_ARRAY(m_frameName);
		return XFalse;
	}
	if(sscanf((char *)(p + offset),"%d:",&m_allFramesSum) != 1) {XMem::XDELETE_ARRAY(p);return XFalse;}
	offset += XString::getCharPosition((char *)(p + offset),':') + 1;
	if(m_allFramesSum <= 0)
	{
		LogNull("Action text file data error:%s!\n",filename);
		XMem::XDELETE_ARRAY(p);
		return XFalse;
	}
	m_keyFrameArray = XMem::createArrayMem<int>(m_allFramesSum);
	if(m_keyFrameArray == NULL) 
	{
		XMem::XDELETE_ARRAY(p);
		return XFalse;
	}
	{//读取标记符	D：default, M：menutrue
		char tempFlag = ' ';
		if(sscanf((char *)(p + offset),"%c:",&tempFlag) != 1) {XMem::XDELETE_ARRAY(p);return XFalse;}
		offset += XString::getCharPosition((char *)(p + offset),':') + 1;
		if(tempFlag == 'D' || tempFlag == 'd')
		{
			for(int i =0;i < m_allFramesSum;++ i)
			{
				m_keyFrameArray[i] = i;
			}
		}else
		{	
			for(int i =0;i < m_allFramesSum;++ i)
			{
				if(sscanf((char *)(p + offset),"%d,",&m_keyFrameArray[i]) != 1) {XMem::XDELETE_ARRAY(p);return XFalse;}
				offset += XString::getCharPosition((char *)(p + offset),',') + 1;
				if(m_keyFrameArray[i] < 0 || m_keyFrameArray[i] >= m_allKeyFramesSum)
				{
					LogNull("Blank frame:%s -> %d!\n",filename,i);
				//	XMem::XDELETE_ARRAY(p);
				//	XMem::XDELETE_ARRAY(m_keyFrameArray);
				//	return XFalse;
				}
			}
		}
	}
	XMem::XDELETE_ARRAY(p);
	return true;
}
bool XFrameEx::loadFromWeb(const char *filename)		//从网页中读取资源
{
	return true;
}
bool XFrameEx::loadData(const char *filename,			//图片的名称
		XResourcePosition resoursePosition)
{
	m_resoursePosition = resoursePosition;
	try
	{
		m_cp = new XSCounter;
		if(m_cp == NULL) return XFalse;
	}catch(...)
	{
		return XFalse;
	}
	m_frameName = XMem::createArrayMem<char>(MAX_FILE_NAME_LENGTH);
	if(m_frameName == NULL) 
	{
		XMem::XDELETE(m_cp);
		return XFalse;
	}
	m_frameName[0] = '\0';

	strcpy(m_frameName,filename);

	//依次载入所有的序列帧图片
	int nameLength = strlen(m_frameName);
	if(nameLength <= 0)
	{
		LogStr("Action file path error!");
		XMem::XDELETE_ARRAY(m_frameName);
		XMem::XDELETE(m_cp);
		return XFalse;
	}
	//下面读取序列帧动画的文本文档数据
	char tempFrameName[MAX_FILE_NAME_LENGTH];
	strcpy(tempFrameName,m_frameName);
	//for(int i = nameLength -1;i > 0;-- i)
	//{
	//	if(tempFrameName[i] == '/' || tempFrameName[i] == '\\') 
	//	{//find the key char
	//		tempFrameName[i + 1] = '\0';
	//		strcat(tempFrameName,"frame.txt");
	//		break;
	//	}
	//}	
	int index = XFile::getPathDeepByChar(tempFrameName);
	tempFrameName[index + 1] = '\0';
	strcat(tempFrameName,"frame.txt");

	//下面这个要考虑多线程资源互斥，唯一的一个多线程中已经做了互斥处理
	switch(m_resoursePosition)
	{
	case RESOURCE_LOCAL_PACK:
		if(!loadFromPacker(tempFrameName)) 
		{
			XMem::XDELETE_ARRAY(m_frameName);
			XMem::XDELETE(m_cp);
			return false;
		}
		break;
	case RESOURCE_LOCAL_FOLDER:
		if(!loadFromFolder(tempFrameName))
		{
			XMem::XDELETE_ARRAY(m_frameName);
			XMem::XDELETE(m_cp);
			return false;
		}
		break;
	case RESOURCE_WEB:
		if(!loadFromWeb(tempFrameName))
		{
			XMem::XDELETE_ARRAY(m_frameName);
			XMem::XDELETE(m_cp);
			return false;
		}
		break;
	case RESOURCE_AUTO:
		if(!loadFromPacker(tempFrameName) && !loadFromFolder(tempFrameName) &&
			!loadFromWeb(tempFrameName))
		{
			XMem::XDELETE_ARRAY(m_frameName);
			XMem::XDELETE(m_cp);
			return false;
		}
		break;
	}

#if FRAME_DEBUG
	++ frameTestSum;
	printf("Frame new sum:%d\n",frameTestSum);
#endif
	if(nameLength < 7) return XFalse;
	m_pSprite = XMem::createArrayMem<XSprite>(m_allKeyFramesSum);
	if(m_pSprite == NULL) 
	{
		XMem::XDELETE_ARRAY(m_frameName);
		XMem::XDELETE(m_cp);
		XMem::XDELETE_ARRAY(m_keyFrameArray);
		return XFalse;
	}
	strcpy(tempFrameName,m_frameName);
	for(int i = 0;i < m_allKeyFramesSum;++ i)
	{
		tempFrameName[nameLength - 5] = '0' + i%10;
		tempFrameName[nameLength - 6] = '0' + (i/10)%10;
		tempFrameName[nameLength - 7] = '0' + (i/100)%10;

		if(!m_pSprite[i].init(tempFrameName,m_resoursePosition,POINT_CENTER))
		{
			LogStr("The action pictrue load error!");
			for(int j = 0;j < i;++ j)
			{
				m_pSprite[j].release();
			}
			XMem::XDELETE_ARRAY(m_pSprite);
			XMem::XDELETE_ARRAY(m_frameName);
			XMem::XDELETE(m_cp);
			XMem::XDELETE_ARRAY(m_keyFrameArray);
			return XFalse;
		}
#if WITH_OBJECT_MANAGER	//在物件管理的类中注销这些物件
		XObjManager.decreaseAObject(&(m_pSprite[i]));
#endif
		m_pSprite[i].setPosition(0.0f,0.0f);
		m_pSprite[i].setAngle(0);
		m_pSprite[i].setScale(1.0f,1.0f);
	}
#if WITH_OBJECT_MANAGER	//注册序列帧这个物件
	XObjManager.addAObject(this);
#endif

	m_isEnd = XFalse;
	m_isSetEnd = XFalse;
	return XTrue;
}
DWORD WINAPI XFrameEx::loadThread(void *p)
{
	XFrameEx &pPar = *(XFrameEx *)p;
	pPar.m_loadThread = STATE_START;
	//下面是数据载入
	printf("开始载入!\n");
	if (gFrameworkData.pFrameWork == NULL)
	{
		printf("引擎尚未初始化!");
		pPar.m_loadThread = STATE_END;
		pPar.m_isThreadLoading = false;
		return 1;
	}
	wglMakeCurrent(gFrameworkData.pFrameWork->getHDC(), 
		gFrameworkData.pFrameWork->getCopyHGLRC());
	if(!pPar.loadData(pPar.m_threadFilename.c_str(),pPar.m_threadResPos))
	{
		pPar.m_isInited = false;
		LogStr("Frame load error!");
	}
	if(pPar.m_neadUpdateAttribute)
	{
		pPar.m_neadUpdateAttribute = false;
		for(int i = 0;i < pPar.m_allKeyFramesSum;++ i)
		{
			pPar.m_pSprite[i].setPosition(pPar.m_position);
			pPar.m_pSprite[i].setOverturn(pPar.m_isOverturn);
		}
	}
    wglMakeCurrent(NULL, NULL);//nullptr

	printf("载入完成!\n");
	pPar.m_loadThread = STATE_END;
	pPar.m_isThreadLoading = false;
	return 1;
}
bool XFrameEx::initThread(const char *filename,			//图片的名称
		XResourcePosition resoursePosition)
{//这里开启一个线程载入资源
	if(filename == NULL) return false;
	if(resoursePosition == RESOURCE_SYSTEM_DEFINE) resoursePosition = getDefResPos();
	//貌似可以不用下面这个判断(注意)
	if(resoursePosition == RESOURCE_LOCAL_PACK)
		return init(filename,resoursePosition);	//如果是从资源包中载入资源，则不能进行多线程载入，直接使用普通方式。

	if(m_isInited) 
	{
		LogStr("The action have initted!");
		return XTrue;
	}
	//下面开启一个载入线程进行载入操作
	m_threadFilename = filename;
	m_threadResPos = resoursePosition;
	m_position.set(0.0f,0.0f);	//序列帧的位置
	m_scale.set(1.0f,1.0f);	//序列帧的缩放比例

	m_isInited = true;
	m_loadThread = STATE_BEFORE_START;
	m_isThreadLoading = true;
	if(CreateThread(0,0,loadThread,this,0,0) == 0)
	{
		m_isInited = false; 
		return false;
	}
	if(m_isInited)
		setAngle(0);		//序列帧的角度
	return XTrue;
}
char * XFrameExParamData::getStrFromData()
{
	char *tempStr = XMem::createArrayMem<char>(256);
	if(tempStr == NULL) return NULL;
	int offset = 0;
	sprintf(tempStr + offset,"ResPos:%d,\n",resourcePosition);
	offset = strlen(tempStr);
	sprintf(tempStr + offset,"ObjPos:%f|%f,\n",pos.x,pos.y);
	offset = strlen(tempStr);
	if(loop) sprintf(tempStr + offset,"Loop:1,\n");
	else sprintf(tempStr + offset,"Loop:0,\n");
	offset = strlen(tempStr);
	if(endImmediately) sprintf(tempStr + offset,"EndIMD:1,\n");
	else sprintf(tempStr + offset,"EndIMD:0,\n");
	offset = strlen(tempStr);
	sprintf(tempStr + offset,"StartFRM:%d,\n",startFrame);
	offset = strlen(tempStr);
	sprintf(tempStr + offset,"Speed:%f,\n",actionSpeed);
	offset = strlen(tempStr);
	if(disappearAtEnd) sprintf(tempStr + offset,"disappear:1,\n");
	else sprintf(tempStr + offset,"disappear:0,\n");
	offset = strlen(tempStr);
	if(isOverturn) sprintf(tempStr + offset,"Overturn:1,\n");
	else sprintf(tempStr + offset,"Overturn:0,\n");
	offset = strlen(tempStr);
	return tempStr;
}
int XFrameExParamData::getDataFromStr(const char * str)
{
	if(str == NULL) return 0;
	int tempData = 0;
	int offset = 0;
	if(sscanf(str + offset,"ResPos:%d,\n",&resourcePosition) != 1) return offset;
	offset += XString::getCharPosition(str + offset,',') + 1;
	if(sscanf(str + offset,"ObjPos:%f|%f,\n",&pos.x,&pos.y) != 2) return offset;
	offset += XString::getCharPosition(str + offset,',') + 1;
	if(sscanf(str + offset,"Loop:%d,\n",&tempData) != 1) return offset;
	if(tempData == 0) loop = XFalse;
	else loop = XTrue;
	offset += XString::getCharPosition(str + offset,',') + 1;
	if(sscanf(str + offset,"EndIMD:%d,\n",&tempData) != 1) return offset;
	if(tempData == 0) endImmediately = XFalse;
	else endImmediately = XTrue;
	offset += XString::getCharPosition(str + offset,',') + 1;
	if(sscanf(str + offset,"StartFRM:%d,\n",&startFrame) != 1) return offset;
	offset += XString::getCharPosition(str + offset,',') + 1;
	if(sscanf(str + offset,"Speed:%f,\n",&actionSpeed) != 1) return offset;
	offset += XString::getCharPosition(str + offset,',') + 1;
	if(sscanf(str + offset,"disappear:%d,\n",&tempData) != 1) return offset;
	if(tempData == 0) disappearAtEnd = XFalse;
	else disappearAtEnd = XTrue;
	offset += XString::getCharPosition(str + offset,',') + 1;
	if(sscanf(str + offset,"Overturn:%d,\n",&tempData) != 1) return offset;
	if(tempData == 0) isOverturn = XFalse;
	else isOverturn = XTrue;
	offset += XString::getCharPosition(str + offset,',') + 1;
	return offset;
}
XBool XFrameExParamData::getDataFromFile(FILE *fp)
{
	if(fp == NULL) return XFalse;
	int tempData = 0;
	if(fscanf(fp,"ResPos:%d,\n",&resourcePosition) != 1) return XFalse;
	if(fscanf(fp,"ObjPos:%f|%f,\n",&pos.x,&pos.y) != 2) return XFalse;
	if(fscanf(fp,"Loop:%d,\n",&tempData) != 1) return XFalse;
	if(tempData == 0) loop = XFalse;
	else loop = XTrue;
	if(fscanf(fp,"EndIMD:%d,\n",&tempData) != 1) return XFalse;
	if(tempData == 0) endImmediately = XFalse;
	else endImmediately = XTrue;
	if(fscanf(fp,"StartFRM:%d,\n",&startFrame) != 1) return XFalse;
	if(fscanf(fp,"Speed:%f,\n",&actionSpeed) != 1) return XFalse;
	if(fscanf(fp,"disappear:%d,\n",&tempData) != 1) return XFalse;
	if(tempData == 0) disappearAtEnd = XFalse;
	else disappearAtEnd = XTrue;
	if(fscanf(fp,"Overturn:%d,\n",&tempData) != 1) return XFalse;
	if(tempData == 0) isOverturn = XFalse;
	else isOverturn = XTrue;
	return XTrue;
}
void XFrameEx::setPosition(float a,float b)
{
	if(!m_isInited) return;
	m_position.set(a,b);
	for(int i = 0;i < m_allKeyFramesSum;++ i)
	{
		m_pSprite[i].setPosition(m_position);
	}
	updateChildPos();
}
void XFrameEx::setScale(float a,float b)
{
	if(!m_isInited || a < 0 || b < 0) return;
	m_scale.set(a,b);
	for(int i = 0;i < m_allKeyFramesSum;++ i)
	{
		m_pSprite[i].setScale(m_scale);
	}
	updateChildScale();
}
void XFrameEx::setAngle(float temp)
{
	if(!m_isInited) return;
	m_angle = temp;
	for(int i = 0;i < m_allKeyFramesSum;++ i)
	{
		m_pSprite[i].setAngle(m_angle);
	}
	updateChildAngle();
}
void XFrameEx::setColor(float r,float g,float b,float a)
{
	if(!m_isInited) return;
	if(r >= 0) m_color.fR = r;
	if(g >= 0) m_color.fG = g;
	if(b >= 0) m_color.fB = b;
	if(a >= 0) m_color.fA = a;
	for(int i = 0;i < m_allKeyFramesSum;++ i)
	{
		m_pSprite[i].setColor(r,g,b,a);
	}
	updateChildColor();
}
void XFrameEx::setAlpha(float a)
{
	if(!m_isInited) return;
	if(a >= 0) m_color.fA = a;
	for(int i = 0;i < m_allKeyFramesSum;++ i)
	{
		m_pSprite[i].setAlpha(a);
	}
	updateChildAlpha();
}
XBool XFrameEx::isInRect(float a,float b)	//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
{
	int temp = m_curFramesNumble;
	if(m_curFramesNumble < 0 || m_curFramesNumble >= m_allFramesSum) temp = 0;
	if(m_keyFrameArray[temp] < 0 || m_keyFrameArray[temp] >= m_allKeyFramesSum) return false;
	return m_pSprite[m_keyFrameArray[temp]].isInRect(a,b);
}
//获取四个顶点的坐标，目前先不考虑旋转和缩放
XVector2 XFrameEx::getBox(int order)
{
	int temp = m_curFramesNumble;
	if(m_curFramesNumble < 0 || m_curFramesNumble >= m_allFramesSum) temp = 0;
	if(m_keyFrameArray[temp] < 0 || m_keyFrameArray[temp] >= m_allKeyFramesSum) return XVector2::zero;
	return m_pSprite[m_keyFrameArray[temp]].getBox(order);
}
#if !WITH_INLINE_FILE
#include "XFrameEx.inl"
#endif
}