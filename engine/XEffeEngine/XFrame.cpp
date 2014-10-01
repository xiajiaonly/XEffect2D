//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "glew.h"	//由于需要使用shader所以需要这个文件，具体需要磋商
#include "XFrame.h"
#include "XBasicWindow.h"
#include "XBasicOpenGL.h"

#include "XResourcePack.h"
#include "XLogBook.h"
//_XResourcePack ResourceTemp;

_XBool _XFrame::init(const char *filename,_XResourcePosition resoursePosition)
{
	if(m_isInited) 
	{
		LogStr("The action have initted!");
		return XTrue;
	}
	if(resoursePosition == RESOURCE_SYSTEM_DEFINE) resoursePosition = XEE::defaultResourcePosition;
	m_resoursePosition = resoursePosition;
	try
	{
		m_cp = new _XSCounter;
		if(m_cp == NULL) return XFalse;
	}catch(...)
	{
		return XFalse;
	}
	//建立所有内存
	m_texnum = createArrayMem<_XTexture>(MAX_FRAME_SUM);
	m_frameName = createArrayMem<char>(MAX_FILE_NAME_LENGTH);
	m_keyFrameArray = createArrayMem<int>(MAX_FRAME_SUM);
	wre = createArrayMem<GLint>(MAX_FRAME_SUM);
	hre = createArrayMem<GLint>(MAX_FRAME_SUM);
	xpa = createArrayMem<GLint>(MAX_FRAME_SUM);
	ypa = createArrayMem<GLint>(MAX_FRAME_SUM);
	if(m_texnum == NULL || m_frameName == NULL || m_keyFrameArray == NULL
			|| wre == NULL || hre == NULL || xpa == NULL || ypa == NULL)
	{
		XDELETE_ARRAY(m_texnum);
		XDELETE_ARRAY(m_frameName);
		XDELETE_ARRAY(m_keyFrameArray);
		XDELETE_ARRAY(wre);
		XDELETE_ARRAY(hre);
		XDELETE_ARRAY(xpa);
		XDELETE_ARRAY(ypa);
		return XFalse;
	}

	x = 0;			//精灵的位置
	y = 0;
	setAngle(0);
	xsize = 1;		//精灵的缩放比例
	ysize = 1;

	strcpy(m_frameName,filename);

	//依次载入所有的序列帧图片
	//load action's pictrue
	//note:the path of the action format is /namefold/name+action+xxx(frame).xxx
	//get name length
/*	int nameLength =0;
	for(int i = 0;i < 256;++ i)
	{
		if(m_frameName[i] == '\0') 
		{
			nameLength = i;
			break;
		}
	}*/
	int nameLength = strlen(m_frameName);
	if(nameLength < 7)
	{
		LogStr("Action file path error!");
		//exit(1);
		return XFalse;
	}
	//read text file		//下面读取序列帧动画的文本文档数据
	char numble[3];
	char tempFrameName[256];
	strcpy(tempFrameName,m_frameName);

	for(int i = nameLength -1;i >0;-- i)
	{
		if(tempFrameName[i] == '/' || tempFrameName[i] == '\\') 
		{//find the key char
			tempFrameName[i + 1] = 'f';
			tempFrameName[i + 2] = 'r';
			tempFrameName[i + 3] = 'a';
			tempFrameName[i + 4] = 'm';
			tempFrameName[i + 5] = 'e';
			tempFrameName[i + 6] = '.';
			tempFrameName[i + 7] = 't';
			tempFrameName[i + 8] = 'x';
			tempFrameName[i + 9] = 't';
			tempFrameName[i +10] = '\0';
			break;
		}
	}	
	//下面这个要考虑多线程资源互斥，唯一的一个多线程中已经做了互斥处理
	if(m_resoursePosition == RESOURCE_LOCAL_PACK)
	{
	/*	static int checkedresource = 0;
		if(checkedresource == 0)
		{
			checkedresource = 1;
			if(ResourceTemp.checkCheckData() == 0)
			{//严重的错误，这里定掉游戏
				DebugShow("Resource Data Error!\n");
				while(true)
				{
					Sleep(100);
				}
			}
		}*/

		unsigned char *p = _XResourcePack::GetInstance().getFileData(tempFrameName);
		if(p == NULL) return XFalse;

		int offset = 0;
		if(sscanf((char *)(p + offset),"%d:",&m_allKeyFramesSum) != 1) {XDELETE_ARRAY(p);return XFalse;}
		offset += getCharPosition((char *)(p + offset),':') + 1;
		if(m_allKeyFramesSum <= 0 || m_allKeyFramesSum >= MAX_FRAME_SUM)
		{
			LogNull("Action text file data error:%s!\n",tempFrameName);
			XDELETE_ARRAY(p);
			return XFalse;
		}
		if(sscanf((char *)(p + offset),"%d:",&m_allFramesSum) != 1) {XDELETE_ARRAY(p);return XFalse;}
		offset += getCharPosition((char *)(p + offset),':') + 1;
		if(m_allFramesSum <= 0 || m_allFramesSum >= MAX_FRAME_SUM)
		{
			LogNull("Action text file data error:%s!\n",tempFrameName);
			XDELETE_ARRAY(p);
			return XFalse;
		}
		{//读取标记符	D：default, M：menutrue
			char tempFlag = ' ';
			if(sscanf((char *)(p + offset),"%c:",&tempFlag) != 1) {XDELETE_ARRAY(p);return XFalse;}
			offset += getCharPosition((char *)(p + offset),':') + 1;
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
					if(sscanf((char *)(p + offset),"%d,",&m_keyFrameArray[i]) != 1) {XDELETE_ARRAY(p);return XFalse;}
					offset += getCharPosition((char *)(p + offset),',') + 1;
					if(m_keyFrameArray[i] < 0 || m_keyFrameArray[i] >= m_allKeyFramesSum)
					{
						LogNull("Action text file data error:%s -> %d!\n",tempFrameName,i);
						XDELETE_ARRAY(p);
						return XFalse;
					}
				}
			}
		}
		{//读取标记符	D：default, M：menutrue
			char tempFlag = ' ';
			if(sscanf((char *)(p + offset),"%c:",&tempFlag) != 1) {XDELETE_ARRAY(p);return XFalse;}
			offset += getCharPosition((char *)(p + offset),':') + 1;
			if(tempFlag == 'D' || tempFlag == 'd')
			{
				for(int i =0;i < m_allFramesSum;++ i)
				{
					m_keyFramePosition[i].set(0,0);
				}
			}else
			{//读取偏移信息
				int temp_x;
				int temp_y;
				int temp_z;
				for(int i =0;i < m_allFramesSum;++ i)
				{
					if(sscanf((char *)(p + offset),"(%d,%d,%d),",&temp_z,&temp_x,&temp_y) != 3) {XDELETE_ARRAY(p);return XFalse;}
					offset += getCharPosition((char *)(p + offset),',') + 1;
					offset += getCharPosition((char *)(p + offset),',') + 1;
					offset += getCharPosition((char *)(p + offset),',') + 1;
					m_keyFramePosition[i].set(temp_x,temp_y);
				}
			}
		}
		XDELETE_ARRAY(p);
	}else
	if(m_resoursePosition == RESOURCE_LOCAL_FOLDER)
	{
		FILE *fp = NULL;
		if((fp = fopen(tempFrameName,"rb")) == NULL)
		{
			LogStr("Action text file error!");
			return XFalse;
		}
		if(fscanf(fp,"%d:",&m_allKeyFramesSum) != 1) {fclose(fp);return XFalse;}
		if(m_allKeyFramesSum <= 0 || m_allKeyFramesSum >= MAX_FRAME_SUM)
		{
			LogNull("Action text file data error:%s!\n",tempFrameName);
			fclose(fp);
			return XFalse;
		}
		if(fscanf(fp,"%d:",&m_allFramesSum) != 1) {fclose(fp);return XFalse;}
		if(m_allFramesSum <= 0 || m_allFramesSum >= MAX_FRAME_SUM)
		{
			LogNull("Action text file data error:%s!\n",tempFrameName);
			fclose(fp);
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
						LogNull("Action text file data error:%s -> %d!\n",tempFrameName,i);
						fclose(fp);
						return XFalse;
					}
				}
			}
		}
		{//读取标记符	D：default, M：menutrue
			char tempFlag = ' ';
			if(fscanf(fp,"%c:",&tempFlag) != 1) {fclose(fp);return XFalse;}
			if(tempFlag == 'D' || tempFlag == 'd')
			{
				for(int i =0;i < m_allFramesSum;++ i)
				{
					m_keyFramePosition[i].set(0,0);
				}
			}else
			{//读取偏移信息
				int temp_x;
				int temp_y;
				int temp_z;
				for(int i =0;i < m_allFramesSum;++ i)
				{
					if(fscanf(fp,"(%d,%d,%d),",&temp_z,&temp_x,&temp_y) != 3) {fclose(fp);return XFalse;}
					m_keyFramePosition[i].set(temp_x,temp_y);
				}
			}
		}
		fclose(fp);
	}

	strcpy(tempFrameName,m_frameName);
	for(int i = 0;i < m_allKeyFramesSum;++ i)
	{
		numble[0] = '0' + i%10;
		numble[1] = '0' + (i/10)%10;
		numble[2] = '0' + (i/100)%10;
		tempFrameName[nameLength - 5] = numble[0];
		tempFrameName[nameLength - 6] = numble[1];
		tempFrameName[nameLength - 7] = numble[2];

		if(!m_texnum[i].load(tempFrameName,m_resoursePosition))
		{
			LogStr("The action pictrue load error!");
			return XFalse;
		}else
		{
			//载入完成之后进行相关的计算
			if(isNPOT(m_texnum[i].m_w,m_texnum[i].m_h)) 
			{
				wre[i] = (int)powf(2.0, ceilf(logf((float)m_texnum[i].m_w)/logf(2.0f)));
				hre[i] = (int)powf(2.0, ceilf(logf((float)m_texnum[i].m_h)/logf(2.0f)));
				xpa[i] = (wre[i] - m_texnum[i].m_w)/2;
				ypa[i] = (hre[i] - m_texnum[i].m_h)/2;
			}else 
			{
				wre[i] = m_texnum[i].m_w;
				hre[i] = m_texnum[i].m_h;
				xpa[i] = 0;
				ypa[i] = 0;
			}
		}
	}
	//为了序列帧中心偏移而计算的量
	m_centerX = m_texnum[0].m_w * 0.5f + m_keyFramePosition[0].x;			//总的序列帧中心坐标
	m_centerY = m_texnum[0].m_h * 0.5f + m_keyFramePosition[0].y;
	for(int i = 0;i < m_allKeyFramesSum;++ i)
	{
		m_centerDW[i] = m_centerX - (m_texnum[i].m_w * 0.5f + m_keyFramePosition[i].x);
		m_centerDH[i] = m_centerY - (m_texnum[i].m_h * 0.5f + m_keyFramePosition[i].y);
	}

	m_isInited = XTrue;
	m_isEnd = XFalse;
	m_isSetEnd = XFalse;
	return XTrue;
}
_XBool _XFrame::releaseMem()
{
//	if(m_isACopy == 0 && m_texnum != NULL)
//	{
//		for(int i = 0;i < MAX_FRAME_SUM;++ i)
//		{
//			m_texnum[i].release();
//		}
//	}
	//if(m_haveSelfMemery != 0)
	//{
	//	XDELETE_ARRAY(m_texnum);
	//	//XDELETE_ARRAY(m_width);
	//	//XDELETE_ARRAY(m_height);
	//	XDELETE_ARRAY(m_frameName);
	//	XDELETE_ARRAY(m_keyFrameArray);
	//	XDELETE_ARRAY(wre);
	//	XDELETE_ARRAY(hre);
	//	XDELETE_ARRAY(xpa);
	//	XDELETE_ARRAY(ypa);
	//	m_haveSelfMemery = 0;
	//}
	XDELETE_ARRAY(m_centerDW);
	XDELETE_ARRAY(m_centerDH);
	XDELETE_ARRAY(m_keyFramePosition);

	m_isInited = XFalse;
	return XTrue;
}
void _XFrame::draw(const GLuint *pTexnum) const
{//序列帧精灵
	if(!m_isVisible) return;
	if(m_isDisappearAtEnd && m_isEnd) return;	//播放完成之后消失

	int temp_Frame = m_keyFrameArray[(int)(m_nowFramesNumble)];

//	if(m_pShaderProc != NULL) glPushAttrib(GL_ALL_ATTRIB_BITS);
	glEnable(GL_TEXTURE_2D);

	if(!m_isFlash) glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	else glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	glEnable(GL_BLEND);
#if WITH_XSPRITE_EX
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); 
#endif
	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, wre[temp_Frame] << 1, 0, hre[temp_Frame] << 1, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	//+++++++++++++++++++
	//下面这段需要考虑优化（这里把第一帧用于计算图片中心，所以第一帧绝对不能发生中心偏移）
	//float dh = m_centerDW[temp_Frame] * xsize * sinAngle + m_centerDH[temp_Frame] * ysize * (cosAngle - 1) + m_centerDH[temp_Frame] * (ysize - 1.0f);
	//float dw = m_centerDW[temp_Frame] * xsize * (1 - cosAngle) + m_centerDH[temp_Frame] * ysize * sinAngle - m_centerDW[temp_Frame] * (xsize - 1.0f);
	float dh = m_centerDW[temp_Frame] * xsize * sinAngle + m_centerDH[temp_Frame] * ysize * cosAngle - m_centerDH[temp_Frame];
	float dw = m_centerDW[temp_Frame] - m_centerDW[temp_Frame] * xsize * cosAngle + m_centerDH[temp_Frame] * ysize * sinAngle;
	int halfW = m_texnum[temp_Frame].m_w >> 1;
	int halfH = m_texnum[temp_Frame].m_h >> 1;
	//-------------------
	glTranslatef(x + m_keyFramePosition[temp_Frame].x + dw + halfW,
		y + m_keyFramePosition[temp_Frame].y - dh +  halfH, 0);
	glRotatef(angle, 0, 0, 1);
	glScalef(xsize, ysize, 0);

//	if(m_pShaderProc == NULL)
	if(m_pShader == NULL)
	{
		if(pTexnum != NULL) glBindTexture(GL_TEXTURE_2D, pTexnum[temp_Frame]);
		else glBindTexture(GL_TEXTURE_2D, m_texnum[temp_Frame].m_texture);
	}else
	{
		//glActiveTexture(GL_TEXTURE0);
		//glEnable(GL_TEXTURE_2D);
		//if(pTexnum != NULL) glBindTexture(GL_TEXTURE_2D, pTexnum[temp_Frame]);
		//else glBindTexture(GL_TEXTURE_2D, m_texnum[temp_Frame].m_texture);

		//m_pShaderProc();

		//glActiveTexture(GL_TEXTURE0);
		if(pTexnum != NULL) m_pShader->useShaderEx(pTexnum[temp_Frame]);//glBindTexture(GL_TEXTURE_2D, pTexnum[temp_Frame]);
		else m_pShader->useShaderEx(m_texnum[temp_Frame].m_texture);//glBindTexture(GL_TEXTURE_2D, m_texnum[temp_Frame].m_texture);
		//m_pShader->useShaderEx();
	}

	glBegin(GL_QUADS);
	glColor4f(colorRed, colorGreen, colorBlue,alpha);

#ifdef GET_ALL_PIXEL
	XEE_AllPixel += m_texnum[temp_Frame].m_w * m_texnum[temp_Frame].m_h;
#endif
	if(!m_isOverturn)
	{
		glTexCoord2f(xpa[temp_Frame], ypa[temp_Frame]);
		glVertex2f(-halfW, -halfH);

		glTexCoord2f(xpa[temp_Frame] + m_texnum[temp_Frame].m_w, ypa[temp_Frame]);
		glVertex2f(halfW, -halfH);

		glTexCoord2f(xpa[temp_Frame] + m_texnum[temp_Frame].m_w, ypa[temp_Frame] + m_texnum[temp_Frame].m_h);
		glVertex2f(halfW, halfH);

		glTexCoord2f(xpa[temp_Frame], ypa[temp_Frame] + m_texnum[temp_Frame].m_h);
		glVertex2f(-halfW, halfH);
	}else
	{
		glTexCoord2f(xpa[temp_Frame], ypa[temp_Frame]);
		glVertex2f(halfW, -halfH);

		glTexCoord2f(xpa[temp_Frame] + m_texnum[temp_Frame].m_w, ypa[temp_Frame]);
		glVertex2f(-halfW, -halfH);

		glTexCoord2f(xpa[temp_Frame] + m_texnum[temp_Frame].m_w, ypa[temp_Frame] + m_texnum[temp_Frame].m_h);
		glVertex2f(-halfW, halfH);

		glTexCoord2f(xpa[temp_Frame], ypa[temp_Frame] + m_texnum[temp_Frame].m_h);
		glVertex2f(halfW, halfH);
	}
	glEnd();
	//if(m_pShaderProc != NULL) glUseProgram(0);				//停止使用shader
	if(m_pShader != NULL) m_pShader->disShader();				//停止使用shader

	glMatrixMode(GL_TEXTURE);
	glPopMatrix();
#if WITH_XSPRITE_EX
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);   
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
#endif
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glDisable(GL_BLEND);

	//if(m_pShaderProc != NULL) glPopAttrib();
}
void _XFrame::move(int timeDelay)
{
	if(!m_isInited || m_isEnd) return;
	if(m_isEndImmediately && m_isSetEnd) m_isEnd = XTrue;
	m_nowFramesNumble += m_actionSpeed * timeDelay;			//推动序列帧动画
	if(m_nowFramesNumble >= m_allFramesSum) 
	{//如果播放到最后一帧，这里回帧
		m_nowFramesNumble =0;

		if(!m_isEndImmediately)
		{//如果不是可以立刻结束的动画，这里播放完成一个循环之后，即可结束
			if(m_isSetEnd) m_isEnd = XTrue;
		}
		if(!m_isLoop) m_isEnd = XTrue;	//如果是不循环的动画这里也可以比较结束
	}else	
	if(m_nowFramesNumble < 0) 
	{//如果是倒序播放的话，这里判断是否播放完成(以及进行完成处理)
		m_nowFramesNumble = m_allFramesSum - 0.05f;		//note this 0.05!!

		if(!m_isEndImmediately && m_isSetEnd) m_isEnd = XTrue;
		if(!m_isLoop) m_isEnd = XTrue;
	}
}
void _XFrame::setAttribute(const _XVector2& position,_XBool loop,_XBool endImmediately,int startFrame,float actionSpeed,_XBool disappearAtEnd,_XBool isOverturn)
{
	if(!m_isInited) return; //必须要初始化之后才能进行这个操作
	x = position.x;			//精灵的位置
	y = position.y;

	m_isLoop = loop;
	m_isEndImmediately = endImmediately;

	m_startFrame = startFrame;
	m_nowFramesNumble = m_startFrame;

	m_actionSpeed = actionSpeed;

	m_isDisappearAtEnd = disappearAtEnd;

	if((m_isOverturn && !isOverturn) || (!m_isOverturn && isOverturn))
	{
		m_isOverturn = isOverturn;

		m_centerX = m_texnum[0].m_w * 0.5f + m_keyFramePosition[0].x;			//总的序列帧中心坐标
		m_centerY = m_texnum[0].m_h * 0.5f + m_keyFramePosition[0].y;
		for(int i = 0;i < m_allKeyFramesSum;++ i)
		{
			m_keyFramePosition[i].x = 2.0f * m_centerX - m_texnum[i].m_w - m_keyFramePosition[i].x;
			m_centerDW[i] = m_centerX - (m_texnum[i].m_w * 0.5f + m_keyFramePosition[i].x);
			m_centerDH[i] = m_centerY - (m_texnum[i].m_h * 0.5f + m_keyFramePosition[i].y);
		}
	}
}

_XFrame::_XFrame()
:m_isInited(XFalse)
,m_resoursePosition(RESOURCE_LOCAL_FOLDER)
,angle(0.0f)
,angleRadian(0.0f)
,sinAngle(0.0f)
,cosAngle(1.0f)
,m_isFlash(XFalse)
//,m_pShaderProc(NULL)
,m_pShader(NULL)
,x(0.0f),y(0.0f)
,xsize(1.0f),ysize(1.0f)
,alpha(1.0f)
,colorRed(1.0f)
,colorGreen(1.0f)
,colorBlue(1.0f)
,m_isOverturn(XFalse)	//默认情况下不翻转
,m_isVisible(XTrue)
,m_allFramesSum(0)							//序列帧动画中总帧数
,m_allKeyFramesSum(0)						//序列帧动画中关键帧的数量
,m_nowFramesNumble(0.0f)					//当前播放到第几帧
,m_actionSpeed(0.0f)						//序列帧播放的速度
,m_isLoop(XFalse)								//序列帧是否可以循环
,m_startFrame(0)							//从哪一帧开始
,m_isEnd(XTrue)								//序列帧是否播放完成
,m_isEndImmediately(XFalse)					//动画是否为立刻结束类型
,m_isSetEnd(XFalse)							//is end by user			//是否设置序列帧结束
,m_isDisappearAtEnd(XFalse)					//动画帧是否在播放完成之后消失
,m_centerX(0.0f)
,m_centerY(0.0f)
//,m_haveSelfMemery(1)	//标志在构造函数中分配了内存空间
,m_isACopy(XFalse)
,m_cp(NULL)
,m_texnum(NULL)
,m_frameName(NULL)
,m_keyFrameArray(NULL)
,wre(NULL)
,hre(NULL)
,xpa(NULL)
,ypa(NULL)
{
	////公有不变的
	//m_texnum = createArrayMem<_XTexture>(MAX_FRAME_SUM);
	//m_frameName = createArrayMem<char>(MAX_FILE_NAME_LENGTH);
	//m_keyFrameArray = createArrayMem<int>(MAX_FRAME_SUM);

	//wre = createArrayMem<GLint>(MAX_FRAME_SUM);
	//hre = createArrayMem<GLint>(MAX_FRAME_SUM);
	//xpa = createArrayMem<GLint>(MAX_FRAME_SUM);
	//ypa = createArrayMem<GLint>(MAX_FRAME_SUM);
	//私有可能变化的
	m_centerDW = createArrayMem<float>(MAX_FRAME_SUM);
	m_centerDH = createArrayMem<float>(MAX_FRAME_SUM);
	m_keyFramePosition = createArrayMem<_XVector2>(MAX_FRAME_SUM);
}

//_XFrame::_XFrame(char withNoMemry)
//:m_isInited(0)
//,m_resoursePosition(RESOURCE_OUTSIDE)
//,angle(0.0f)
//,angleRadian(0.0f)
//,sinAngle(0.0f)
//,cosAngle(1.0f)
//,m_isFlash(0)
//,m_pShaderProc(NULL)
//,x(0.0f),y(0.0f)
//,xsize(1.0f),ysize(1.0f)
//,alpha(1.0f)
//,colorRed(1.0f)
//,colorGreen(1.0f)
//,colorBlue(1.0f)
//,m_isOverturn(0)	//默认情况下不翻转
//,m_isVisible(1)
//,m_allFramesSum(0)							//序列帧动画中总帧数
//,m_allKeyFramesSum(0)						//序列帧动画中关键帧的数量
//,m_nowFramesNumble(0.0f)					//当前播放到第几帧
//,m_actionSpeed(0.0f)						//序列帧播放的速度
//,m_isLoop(0)								//序列帧是否可以循环
//,m_startFrame(0)							//从哪一帧开始
//,m_isEnd(1)								//序列帧是否播放完成
//,m_isEndImmediately(0)					//动画是否为立刻结束类型
//,m_isSetEnd(0)							//is end by user			//是否设置序列帧结束
//,m_isDisappearAtEnd(0)					//动画帧是否在播放完成之后消失
//,m_centerX(0.0f)
//,m_centerY(0.0f)
//,m_haveSelfMemery(0)	//标志在构造函数中分配了内存空间
//,m_isACopy(0)
//,m_texnum(NULL)
//,m_frameName(NULL)
//,m_keyFrameArray(NULL)
//,wre(NULL)
//,hre(NULL)
//,xpa(NULL)
//,ypa(NULL)
//{
//	m_centerDW = createArrayMem<float>(MAX_FRAME_SUM);
//	m_centerDH = createArrayMem<float>(MAX_FRAME_SUM);
//	m_keyFramePosition = createArrayMem<_XVector2>(MAX_FRAME_SUM);
//}
_XFrame::~_XFrame()
{
	if(m_cp != NULL && -- m_cp->m_counter <= 0)
	{//自身没有引用需要释放
		XDELETE_ARRAY(m_texnum);
		XDELETE_ARRAY(m_frameName);
		XDELETE_ARRAY(m_keyFrameArray);
		XDELETE_ARRAY(wre);
		XDELETE_ARRAY(hre);
		XDELETE_ARRAY(xpa);
		XDELETE_ARRAY(ypa);
		XDELETE(m_cp);
	}
	releaseMem();
}
_XFrame& _XFrame::operator = (const _XFrame& temp)
{
	if(&temp == this) return * this;	//防止没必要的自我拷贝
	if(temp.m_cp != NULL) ++temp.m_cp->m_counter;
	//释放自身的资源
	if(m_cp != NULL && -- m_cp->m_counter <= 0)
	{//自身没有引用需要释放
		XDELETE_ARRAY(m_texnum);
		XDELETE_ARRAY(m_frameName);
		XDELETE_ARRAY(m_keyFrameArray);
		XDELETE_ARRAY(wre);
		XDELETE_ARRAY(hre);
		XDELETE_ARRAY(xpa);
		XDELETE_ARRAY(ypa);
		XDELETE(m_cp);
	}
	m_cp = temp.m_cp;

	m_isVisible = temp.m_isVisible;					//精灵的角度
	angle = temp.angle;					//精灵的角度
	angleRadian = temp.angleRadian;		//弧度标志的角度
	sinAngle = temp.sinAngle;
	cosAngle = temp.cosAngle;
	x = temp.x;
	y = temp.y;							//精灵的位置
	xsize = temp.xsize;
	ysize = temp.ysize;					//精灵的缩放尺寸
	alpha = temp.alpha;					//透明度
	colorRed = temp.colorRed;			//红色
	colorGreen = temp.colorGreen;		//绿色
	colorBlue = temp.colorBlue;			//蓝色
	m_isOverturn = temp.m_isOverturn;	//是否x方向左右翻转

	m_texnum = temp.m_texnum;

	m_isFlash = temp.m_isFlash;
	//m_pShaderProc = temp.m_pShaderProc;
	m_pShader = temp.m_pShader;

	m_allFramesSum = temp.m_allFramesSum;					//序列帧动画中总帧数
	m_allKeyFramesSum = temp.m_allKeyFramesSum;				//序列帧动画中关键帧的数量
	m_nowFramesNumble = temp.m_nowFramesNumble;				//当前播放到第几帧
	m_actionSpeed = temp.m_actionSpeed;						//序列帧播放的速度
	m_isLoop = temp.m_isLoop;								//序列帧是否可以循环
	m_startFrame = temp.m_startFrame;						//从哪一帧开始
	m_isEnd = temp.m_isEnd;									//序列帧是否播放完成
	m_isEndImmediately = temp.m_isEndImmediately;			//动画是否为立刻结束类型
	m_isSetEnd = temp.m_isSetEnd;							//是否设置序列帧结束
	m_isDisappearAtEnd = temp.m_isDisappearAtEnd;			//动画帧是否在播放完成之后消失

	wre = temp.wre;
	hre = temp.hre;
	xpa = temp.xpa;
	ypa = temp.ypa;

	m_frameName = temp.m_frameName;
	m_keyFrameArray = temp.m_keyFrameArray;

	m_centerX = temp.m_centerX;			//总的序列帧中心坐标
	m_centerY = temp.m_centerY;

	memcpy(m_centerDW,temp.m_centerDW,sizeof(float) * MAX_FRAME_SUM);
	memcpy(m_centerDH,temp.m_centerDH,sizeof(float) * MAX_FRAME_SUM);
	memcpy(m_keyFramePosition,temp.m_keyFramePosition,sizeof(_XVector2) * MAX_FRAME_SUM);

	m_isInited = temp.m_isInited;
	m_isACopy = XTrue;
	return *this;
}
void _XFrame::setACopy(const _XFrame& temp)
{
	if(&temp == this) return;			//防止没必要的自我拷贝
	if(temp.m_cp != NULL) ++temp.m_cp->m_counter;
	//释放自身的资源
	if(m_cp != NULL && -- m_cp->m_counter <= 0)
	{//自身没有引用需要释放
		XDELETE_ARRAY(m_texnum);
		XDELETE_ARRAY(m_frameName);
		XDELETE_ARRAY(m_keyFrameArray);
		XDELETE_ARRAY(wre);
		XDELETE_ARRAY(hre);
		XDELETE_ARRAY(xpa);
		XDELETE_ARRAY(ypa);
		XDELETE(m_cp);
	}
	m_cp = temp.m_cp;

	m_isVisible = temp.m_isVisible;					//精灵的角度
	angle = temp.angle;					//精灵的角度
	angleRadian = temp.angleRadian;		//弧度标志的角度
	sinAngle = temp.sinAngle;
	cosAngle = temp.cosAngle;
	x = temp.x;
	y = temp.y;							//精灵的位置
	xsize = temp.xsize;
	ysize = temp.ysize;					//精灵的缩放尺寸
	alpha = temp.alpha;					//透明度
	colorRed = temp.colorRed;			//红色
	colorGreen = temp.colorGreen;		//绿色
	colorBlue = temp.colorBlue;			//蓝色
	m_isOverturn = temp.m_isOverturn;	//是否x方向左右翻转
	
	m_texnum = temp.m_texnum;

	m_isFlash = temp.m_isFlash;
	//m_pShaderProc = temp.m_pShaderProc;
	m_pShader = temp.m_pShader;

	m_allFramesSum = temp.m_allFramesSum;					//序列帧动画中总帧数
	m_allKeyFramesSum = temp.m_allKeyFramesSum;				//序列帧动画中关键帧的数量
	m_nowFramesNumble = temp.m_nowFramesNumble;				//当前播放到第几帧
	m_actionSpeed = temp.m_actionSpeed;						//序列帧播放的速度
	m_isLoop = temp.m_isLoop;								//序列帧是否可以循环
	m_startFrame = temp.m_startFrame;						//从哪一帧开始
	m_isEnd = temp.m_isEnd;									//序列帧是否播放完成
	m_isEndImmediately = temp.m_isEndImmediately;			//动画是否为立刻结束类型
	m_isSetEnd = temp.m_isSetEnd;							//是否设置序列帧结束
	m_isDisappearAtEnd = temp.m_isDisappearAtEnd;			//动画帧是否在播放完成之后消失

	wre = temp.wre;
	hre = temp.hre;
	xpa = temp.xpa;
	ypa = temp.ypa;

	m_frameName = temp.m_frameName;
	m_keyFrameArray = temp.m_keyFrameArray;

	m_centerX = temp.m_centerX;			//总的序列帧中心坐标
	m_centerY = temp.m_centerY;

	memcpy(m_centerDW,temp.m_centerDW,sizeof(float) * MAX_FRAME_SUM);
	memcpy(m_centerDH,temp.m_centerDH,sizeof(float) * MAX_FRAME_SUM);
	memcpy(m_keyFramePosition,temp.m_keyFramePosition,sizeof(_XVector2) * MAX_FRAME_SUM);

	m_isInited = temp.m_isInited;
	m_isACopy = XTrue;
}
_XFrame::_XFrame(const _XFrame &temp)
{
	if(&temp == this) return;			//防止没必要的自我拷贝
}