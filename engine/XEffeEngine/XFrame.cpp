#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XFrame.h"
#include "XResourcePack.h"
#include "XShaderGLSL.h"	
#include "XFile.h"
namespace XE {
bool XFrame::loadFromFolder(const char *filename)	//从文件夹中载入资源
{
	FILE *fp = NULL;
	if ((fp = fopen(filename, "rb")) == NULL)
	{
		LogStr("Action text file error!");
		return XFalse;
	}
	if (fscanf(fp, "%d:", &m_allKeyFramesSum) != 1) { fclose(fp); return XFalse; }
	if (m_allKeyFramesSum <= 0 || m_allKeyFramesSum >= m_maxFrameSum)
	{
		LogNull("Action text file data error:%s!\n", filename);
		fclose(fp);
		return XFalse;
	}
	if (fscanf(fp, "%d:", &m_allFramesSum) != 1) { fclose(fp); return XFalse; }
	if (m_allFramesSum <= 0 || m_allFramesSum >= m_maxFrameSum)
	{
		LogNull("Action text file data error:%s!\n", filename);
		fclose(fp);
		return XFalse;
	}
	{//读取标记符	D：default, M：menutrue
		char tempFlag = ' ';
		if (fscanf(fp, "%c:", &tempFlag) != 1) { fclose(fp); return XFalse; }
		if (tempFlag == 'D' || tempFlag == 'd')
		{
			for (int i = 0; i < m_allFramesSum; ++i)
			{
				m_keyFrameArray[i] = i;
			}
		}
		else
		{
			for (int i = 0; i < m_allFramesSum; ++i)
			{
				if (fscanf(fp, "%d,", &m_keyFrameArray[i]) != 1) { fclose(fp); return XFalse; }
				//新的特征中非法的帧编号，为空白帧
				if (m_keyFrameArray[i] < 0 || m_keyFrameArray[i] >= m_allKeyFramesSum)
				{
					LogNull("Blank frame:%s -> %d!\n", filename, i);
					//	fclose(fp);
					//	return XFalse;
				}
			}
		}
	}
	{//读取标记符	D：default, M：menutrue
		char tempFlag = ' ';
		if (fscanf(fp, "%c:", &tempFlag) != 1) { fclose(fp); return XFalse; }
		if (tempFlag == 'D' || tempFlag == 'd')
		{
			for (int i = 0; i < m_allFramesSum; ++i)
			{
				m_keyFramePosition[i].set(0.0f);
			}
		}
		else
		{//读取偏移信息
			int temp_x;
			int temp_y;
			int temp_z;
			for (int i = 0; i < m_allFramesSum; ++i)
			{
				if (fscanf(fp, "(%d,%d,%d),", &temp_z, &temp_x, &temp_y) != 3) { fclose(fp); return XFalse; }
				m_keyFramePosition[i].set(temp_x, temp_y);
			}
		}
	}
	fclose(fp);
	return true;
}
bool XFrame::loadFromPacker(const char *filename)	//从压缩包中载入资源
{
	unsigned char *p = XResPack.getFileData(filename);
	if (p == NULL) return XFalse;

	int offset = 0;
	if (sscanf((char *)(p + offset), "%d:", &m_allKeyFramesSum) != 1) { XMem::XDELETE_ARRAY(p); return XFalse; }
	offset += XString::getCharPosition((char *)(p + offset), ':') + 1;
	if (m_allKeyFramesSum <= 0 || m_allKeyFramesSum >= m_maxFrameSum)
	{
		LogNull("Action text file data error:%s!\n", filename);
		XMem::XDELETE_ARRAY(p);
		return XFalse;
	}
	if (sscanf((char *)(p + offset), "%d:", &m_allFramesSum) != 1) { XMem::XDELETE_ARRAY(p); return XFalse; }
	offset += XString::getCharPosition((char *)(p + offset), ':') + 1;
	if (m_allFramesSum <= 0 || m_allFramesSum >= m_maxFrameSum)
	{
		LogNull("Action text file data error:%s!\n", filename);
		XMem::XDELETE_ARRAY(p);
		return XFalse;
	}
	{//读取标记符	D：default, M：menutrue
		char tempFlag = ' ';
		if (sscanf((char *)(p + offset), "%c:", &tempFlag) != 1) { XMem::XDELETE_ARRAY(p); return XFalse; }
		offset += XString::getCharPosition((char *)(p + offset), ':') + 1;
		if (tempFlag == 'D' || tempFlag == 'd')
		{
			for (int i = 0; i < m_allFramesSum; ++i)
			{
				m_keyFrameArray[i] = i;
			}
		}
		else
		{
			for (int i = 0; i < m_allFramesSum; ++i)
			{
				if (sscanf((char *)(p + offset), "%d,", &m_keyFrameArray[i]) != 1) { XMem::XDELETE_ARRAY(p); return XFalse; }
				offset += XString::getCharPosition((char *)(p + offset), ',') + 1;
				if (m_keyFrameArray[i] < 0 || m_keyFrameArray[i] >= m_allKeyFramesSum)
				{
					LogNull("Blank frame:%s -> %d!\n", filename, i);
					//	XMem::XDELETE_ARRAY(p);
					//	return XFalse;
				}
			}
		}
	}
	{//读取标记符	D：default, M：menutrue
		char tempFlag = ' ';
		if (sscanf((char *)(p + offset), "%c:", &tempFlag) != 1) { XMem::XDELETE_ARRAY(p); return XFalse; }
		offset += XString::getCharPosition((char *)(p + offset), ':') + 1;
		if (tempFlag == 'D' || tempFlag == 'd')
		{
			for (int i = 0; i < m_allFramesSum; ++i)
			{
				m_keyFramePosition[i].set(0.0f);
			}
		}
		else
		{//读取偏移信息
			int temp_x;
			int temp_y;
			int temp_z;
			for (int i = 0; i < m_allFramesSum; ++i)
			{
				if (sscanf((char *)(p + offset), "(%d,%d,%d),", &temp_z, &temp_x, &temp_y) != 3) { XMem::XDELETE_ARRAY(p); return XFalse; }
				offset += XString::getCharPosition((char *)(p + offset), ',') + 1;
				offset += XString::getCharPosition((char *)(p + offset), ',') + 1;
				offset += XString::getCharPosition((char *)(p + offset), ',') + 1;
				m_keyFramePosition[i].set(temp_x, temp_y);
			}
		}
	}
	XMem::XDELETE_ARRAY(p);
	return true;
}
bool XFrame::loadFromWeb(const char *filename)		//从网页中读取资源
{
	return false;
}

XBool XFrame::init(const char *filename, XResPos resPos)
{
	if (m_isInited)
	{
		LogStr("The action have initted!");
		return XTrue;
	}
	if (resPos == RES_SYS_DEF) resPos = getDefResPos();
	m_resoursePosition = resPos;
	try
	{
		m_cp = new XSCounter;
		if (m_cp == NULL) return XFalse;
	}
	catch (...)
	{
		return XFalse;
	}
	//建立所有内存
	m_texnum = XMem::createArrayMem<XTexture>(m_maxFrameSum);
	m_frameName = XMem::createArrayMem<char>(MAX_FILE_NAME_LENGTH);
	m_keyFrameArray = XMem::createArrayMem<int>(m_maxFrameSum);
	wre = XMem::createArrayMem<GLint>(m_maxFrameSum);
	hre = XMem::createArrayMem<GLint>(m_maxFrameSum);
	xpa = XMem::createArrayMem<GLint>(m_maxFrameSum);
	ypa = XMem::createArrayMem<GLint>(m_maxFrameSum);
	if (m_texnum == NULL || m_frameName == NULL || m_keyFrameArray == NULL
		|| wre == NULL || hre == NULL || xpa == NULL || ypa == NULL)
	{
		XMem::XDELETE_ARRAY(m_texnum);
		XMem::XDELETE_ARRAY(m_frameName);
		XMem::XDELETE_ARRAY(m_keyFrameArray);
		XMem::XDELETE_ARRAY(wre);
		XMem::XDELETE_ARRAY(hre);
		XMem::XDELETE_ARRAY(xpa);
		XMem::XDELETE_ARRAY(ypa);
		return XFalse;
	}
	//精灵的位置
	x = y = 0.0f;
	setAngle(0.0f);
	//精灵的缩放比例
	xsize = ysize = 1.0f;

	strcpy_s(m_frameName, MAX_FILE_NAME_LENGTH, filename);

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
	if (nameLength < 7)
	{
		LogStr("Action file path error!");
		//exit(1);
		return XFalse;
	}
	//read text file		//下面读取序列帧动画的文本文档数据
	//char numble[3];
	char tempFrameName[MAX_FILE_NAME_LENGTH];
	strcpy_s(tempFrameName, MAX_FILE_NAME_LENGTH, m_frameName);

	//for(int i = nameLength -1;i >0;-- i)
	//{
	//	if(tempFrameName[i] == '/' || tempFrameName[i] == '\\') 
	//	{//find the key char
	//		tempFrameName[i + 1] = 'f';
	//		tempFrameName[i + 2] = 'r';
	//		tempFrameName[i + 3] = 'a';
	//		tempFrameName[i + 4] = 'm';
	//		tempFrameName[i + 5] = 'e';
	//		tempFrameName[i + 6] = '.';
	//		tempFrameName[i + 7] = 't';
	//		tempFrameName[i + 8] = 'x';
	//		tempFrameName[i + 9] = 't';
	//		tempFrameName[i +10] = '\0';
	//		break;
	//	}
	//}	
	int index = XFile::getPathDeepByChar(tempFrameName);
	tempFrameName[index + 1] = '\0';
	strcat_s(tempFrameName, MAX_FILE_NAME_LENGTH, "frame.txt");
	//下面这个要考虑多线程资源互斥，唯一的一个多线程中已经做了互斥处理
	switch (m_resoursePosition)
	{
	case RES_LOCAL_PACK:
		if (!loadFromPacker(tempFrameName)) return false;
		break;
	case RES_LOCAL_FOLDER:
		if (!loadFromFolder(tempFrameName)) return false;
		break;
	case RES_WEB:
		if (!loadFromWeb(tempFrameName)) return false;
		break;
	case RES_AUTO:
		if (!loadFromPacker(tempFrameName) && !loadFromFolder(tempFrameName) &&
			!loadFromWeb(tempFrameName)) return false;
		break;
	}

	strcpy_s(tempFrameName, MAX_FILE_NAME_LENGTH, m_frameName);
	for (int i = 0; i < m_allKeyFramesSum; ++i)
	{
		tempFrameName[nameLength - 5] = '0' + i % 10;;
		tempFrameName[nameLength - 6] = '0' + (i / 10) % 10;;
		tempFrameName[nameLength - 7] = '0' + (i / 100) % 10;

		if (!m_texnum[i].load(tempFrameName, m_resoursePosition))
		{
			LogStr("The action pictrue load error!");
			return XFalse;
		}
		else
		{
			//载入完成之后进行相关的计算
			if (XMath::isNPOT(m_texnum[i].m_w, m_texnum[i].m_h))
			{
				wre[i] = XMath::getMinWellSize2n(m_texnum[i].m_w);
				hre[i] = XMath::getMinWellSize2n(m_texnum[i].m_h);
				xpa[i] = (wre[i] - m_texnum[i].m_w) / 2;
				ypa[i] = (hre[i] - m_texnum[i].m_h) / 2;
			}
			else
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
	for (int i = 0; i < m_allKeyFramesSum; ++i)
	{
		m_centerDW[i] = m_centerX - (m_texnum[i].m_w * 0.5f + m_keyFramePosition[i].x);
		m_centerDH[i] = m_centerY - (m_texnum[i].m_h * 0.5f + m_keyFramePosition[i].y);
	}

	m_isInited = XTrue;
	m_isEnd = XFalse;
	m_isSetEnd = XFalse;
	return XTrue;
}
XBool XFrame::releaseMem()
{
	//	if(m_isACopy == 0 && m_texnum != NULL)
	//	{
	//		for(int i = 0;i < m_maxFrameSum;++ i)
	//		{
	//			m_texnum[i].release();
	//		}
	//	}
		//if(m_haveSelfMemery != 0)
		//{
		//	XMem::XDELETE_ARRAY(m_texnum);
		//	//XMem::XDELETE_ARRAY(m_width);
		//	//XMem::XDELETE_ARRAY(m_height);
		//	XMem::XDELETE_ARRAY(m_frameName);
		//	XMem::XDELETE_ARRAY(m_keyFrameArray);
		//	XMem::XDELETE_ARRAY(wre);
		//	XMem::XDELETE_ARRAY(hre);
		//	XMem::XDELETE_ARRAY(xpa);
		//	XMem::XDELETE_ARRAY(ypa);
		//	m_haveSelfMemery = 0;
		//}
	XMem::XDELETE_ARRAY(m_centerDW);
	XMem::XDELETE_ARRAY(m_centerDH);
	XMem::XDELETE_ARRAY(m_keyFramePosition);

	m_isInited = XFalse;
	return XTrue;
}
void XFrame::draw(const GLuint *pTexnum) const
{//序列帧精灵
	if (!m_isVisible) return;
	if (m_isDisappearAtEnd && m_isEnd) return;	//播放完成之后消失

	int keyFrameIndex = m_keyFrameArray[(int)(m_curFramesNumble)];
	if (keyFrameIndex < 0 || keyFrameIndex >= m_allKeyFramesSum) return;

	//	if(m_pShaderProc != NULL) glPushAttrib(GL_ALL_ATTRIB_BITS);
	XGL::EnableTexture2D();
	if (!m_isFlash) XGL::setBlendAlpha();
	else XGL::setBlendAdd();

#if WITHXSPRITE_EX
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
#endif
	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, (wre[keyFrameIndex] << 1), 0, (hre[keyFrameIndex] << 1), -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	//+++++++++++++++++++
	//下面这段需要考虑优化（这里把第一帧用于计算图片中心，所以第一帧绝对不能发生中心偏移）
	//float dh = m_centerDW[keyFrameIndex] * xsize * sinAngle + m_centerDH[keyFrameIndex] * ysize * (cosAngle - 1) + m_centerDH[keyFrameIndex] * (ysize - 1.0f);
	//float dw = m_centerDW[keyFrameIndex] * xsize * (1 - cosAngle) + m_centerDH[keyFrameIndex] * ysize * sinAngle - m_centerDW[keyFrameIndex] * (xsize - 1.0f);
	float dh = m_centerDW[keyFrameIndex] * xsize * sinAngle + m_centerDH[keyFrameIndex] * ysize * cosAngle - m_centerDH[keyFrameIndex];
	float dw = m_centerDW[keyFrameIndex] - m_centerDW[keyFrameIndex] * xsize * cosAngle + m_centerDH[keyFrameIndex] * ysize * sinAngle;
	int halfW = m_texnum[keyFrameIndex].m_w >> 1;
	int halfH = m_texnum[keyFrameIndex].m_h >> 1;
	//-------------------
	glTranslatef(x + m_keyFramePosition[keyFrameIndex].x + dw + halfW,
		y + m_keyFramePosition[keyFrameIndex].y - dh + halfH, 0.0f);
	glRotatef(angle, 0.0f, 0.0f, 1.0f);
	glScalef(xsize, ysize, 0.0f);

	//	if(m_pShaderProc == NULL)
	if (m_pShader == NULL)
	{
		if (pTexnum != NULL) XGL::BindTexture2D(pTexnum[keyFrameIndex]);
		else XGL::BindTexture2D(m_texnum[keyFrameIndex].m_texture);
	}
	else
	{
		//glActiveTexture(GL_TEXTURE0);
		//XGL::EnableTexture2D();
		//if(pTexnum != NULL) XGL::BindTexture2D(pTexnum[keyFrameIndex]);
		//else XGL::BindTexture2D(m_texnum[keyFrameIndex].m_texture);

		//m_pShaderProc();

		//glActiveTexture(GL_TEXTURE0);
		if (pTexnum != NULL) m_pShader->useShaderEx(pTexnum[keyFrameIndex]);//XGL::BindTexture2D(pTexnum[keyFrameIndex]);
		else m_pShader->useShaderEx(m_texnum[keyFrameIndex].m_texture);//XGL::BindTexture2D(m_texnum[keyFrameIndex].m_texture);
		//m_pShader->useShaderEx();
	}

	glBegin(GL_QUADS);
	glColor4f(colorRed, colorGreen, colorBlue, alpha);

#ifdef GET_ALL_PIXEL
	XEE_AllPixel += m_texnum[keyFrameIndex].m_w * m_texnum[keyFrameIndex].m_h;
#endif
	if (!m_isOverturn)
	{
		glTexCoord2f(xpa[keyFrameIndex], ypa[keyFrameIndex]);
		//glTexCoord2f(0, 0);
		glVertex2f(-halfW, -halfH);

		glTexCoord2f(xpa[keyFrameIndex] + m_texnum[keyFrameIndex].m_w, ypa[keyFrameIndex]);
		//glTexCoord2f(1, 0);
		glVertex2f(halfW, -halfH);

		glTexCoord2f(xpa[keyFrameIndex] + m_texnum[keyFrameIndex].m_w, ypa[keyFrameIndex] + m_texnum[keyFrameIndex].m_h);
		//glTexCoord2f(1, 1);
		glVertex2f(halfW, halfH);

		glTexCoord2f(xpa[keyFrameIndex], ypa[keyFrameIndex] + m_texnum[keyFrameIndex].m_h);
		//glTexCoord2f(0, 1);
		glVertex2f(-halfW, halfH);
	}
	else
	{
		glTexCoord2f(xpa[keyFrameIndex], ypa[keyFrameIndex]);
		//glTexCoord2f(0, 0);
		glVertex2f(halfW, -halfH);

		glTexCoord2f(xpa[keyFrameIndex] + m_texnum[keyFrameIndex].m_w, ypa[keyFrameIndex]);
		//glTexCoord2f(1, 0);
		glVertex2f(-halfW, -halfH);

		glTexCoord2f(xpa[keyFrameIndex] + m_texnum[keyFrameIndex].m_w, ypa[keyFrameIndex] + m_texnum[keyFrameIndex].m_h);
		//glTexCoord2f(1, 1);
		glVertex2f(-halfW, halfH);

		glTexCoord2f(xpa[keyFrameIndex], ypa[keyFrameIndex] + m_texnum[keyFrameIndex].m_h);
		//glTexCoord2f(0, 1);
		glVertex2f(halfW, halfH);
	}
	glEnd();
	//if(m_pShaderProc != NULL) glUseProgram(0);				//停止使用shader
	if (m_pShader != NULL) m_pShader->disShader();				//停止使用shader

	glMatrixMode(GL_TEXTURE);
	glPopMatrix();
#if WITHXSPRITE_EX
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
#endif
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	XGL::DisableBlend();
	//if(m_pShaderProc != NULL) glPopAttrib();
}
void XFrame::move(float timeDelay)
{
	if (!m_isInited || m_isEnd) return;
	if (m_isEndImmediately && m_isSetEnd) m_isEnd = XTrue;
	m_curFramesNumble += m_actionSpeed * timeDelay;			//推动序列帧动画
	if (m_curFramesNumble >= m_allFramesSum)
	{//如果播放到最后一帧，这里回帧
		m_curFramesNumble = 0.0f;

		if (!m_isEndImmediately)
		{//如果不是可以立刻结束的动画，这里播放完成一个循环之后，即可结束
			if (m_isSetEnd) m_isEnd = XTrue;
		}
		if (!m_isLoop) m_isEnd = XTrue;	//如果是不循环的动画这里也可以比较结束
	}
	else
	if (m_curFramesNumble < 0.0f)
	{//如果是倒序播放的话，这里判断是否播放完成(以及进行完成处理)
		m_curFramesNumble = m_allFramesSum - 0.05f;		//note this 0.05!!

		if (!m_isEndImmediately && m_isSetEnd) m_isEnd = XTrue;
		if (!m_isLoop) m_isEnd = XTrue;
	}
}
void XFrame::setAttribute(const XVec2& position, XBool loop, XBool endImmediately, int startFrame, float actionSpeed, XBool disappearAtEnd, XBool isOverturn)
{
	if (!m_isInited) return; //必须要初始化之后才能进行这个操作
	x = position.x;			//精灵的位置
	y = position.y;

	m_isLoop = loop;
	m_isEndImmediately = endImmediately;

	m_startFrame = startFrame;
	m_curFramesNumble = m_startFrame;

	m_actionSpeed = actionSpeed;

	m_isDisappearAtEnd = disappearAtEnd;

	if ((m_isOverturn && !isOverturn) || (!m_isOverturn && isOverturn))
	{
		m_isOverturn = isOverturn;

		m_centerX = m_texnum[0].m_w * 0.5f + m_keyFramePosition[0].x;			//总的序列帧中心坐标
		m_centerY = m_texnum[0].m_h * 0.5f + m_keyFramePosition[0].y;
		for (int i = 0; i < m_allKeyFramesSum; ++i)
		{
			m_keyFramePosition[i].x = 2.0f * m_centerX - m_texnum[i].m_w - m_keyFramePosition[i].x;
			m_centerDW[i] = m_centerX - (m_texnum[i].m_w * 0.5f + m_keyFramePosition[i].x);
			m_centerDH[i] = m_centerY - (m_texnum[i].m_h * 0.5f + m_keyFramePosition[i].y);
		}
	}
}

XFrame::XFrame()
	:m_isInited(XFalse)
	, m_resoursePosition(RES_AUTO)
	, angle(0.0f)
	, angleRadian(0.0f)
	, sinAngle(0.0f)
	, cosAngle(1.0f)
	, m_isFlash(XFalse)
	//,m_pShaderProc(NULL)
	, m_pShader(NULL)
	, x(0.0f), y(0.0f)
	, xsize(1.0f), ysize(1.0f)
	, alpha(1.0f)
	, colorRed(1.0f)
	, colorGreen(1.0f)
	, colorBlue(1.0f)
	, m_isOverturn(XFalse)	//默认情况下不翻转
	, m_isVisible(XTrue)
	, m_allFramesSum(0)							//序列帧动画中总帧数
	, m_allKeyFramesSum(0)						//序列帧动画中关键帧的数量
	, m_curFramesNumble(0.0f)					//当前播放到第几帧
	, m_actionSpeed(0.0f)						//序列帧播放的速度
	, m_isLoop(XFalse)								//序列帧是否可以循环
	, m_startFrame(0)							//从哪一帧开始
	, m_isEnd(XTrue)								//序列帧是否播放完成
	, m_isEndImmediately(XFalse)					//动画是否为立刻结束类型
	, m_isSetEnd(XFalse)							//is end by user			//是否设置序列帧结束
	, m_isDisappearAtEnd(XFalse)					//动画帧是否在播放完成之后消失
	, m_centerX(0.0f)
	, m_centerY(0.0f)
	//,m_haveSelfMemery(1)	//标志在构造函数中分配了内存空间
	, m_isACopy(XFalse)
	, m_cp(NULL)
	, m_texnum(NULL)
	, m_frameName(NULL)
	, m_keyFrameArray(NULL)
	, wre(NULL)
	, hre(NULL)
	, xpa(NULL)
	, ypa(NULL)
{
	////公有不变的
	//m_texnum = XMem::createArrayMem<XTexture>(m_maxFrameSum);
	//m_frameName = XMem::createArrayMem<char>(MAX_FILE_NAME_LENGTH);
	//m_keyFrameArray = XMem::createArrayMem<int>(m_maxFrameSum);

	//wre = XMem::createArrayMem<GLint>(m_maxFrameSum);
	//hre = XMem::createArrayMem<GLint>(m_maxFrameSum);
	//xpa = XMem::createArrayMem<GLint>(m_maxFrameSum);
	//ypa = XMem::createArrayMem<GLint>(m_maxFrameSum);
	//私有可能变化的
	m_centerDW = XMem::createArrayMem<float>(m_maxFrameSum);
	m_centerDH = XMem::createArrayMem<float>(m_maxFrameSum);
	m_keyFramePosition = XMem::createArrayMem<XVec2>(m_maxFrameSum);
}

//XFrame::XFrame(char withNoMemry)
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
//,m_curFramesNumble(0.0f)					//当前播放到第几帧
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
//	m_centerDW = XMem::createArrayMem<float>(m_maxFrameSum);
//	m_centerDH = XMem::createArrayMem<float>(m_maxFrameSum);
//	m_keyFramePosition = XMem::createArrayMem<XVec2>(m_maxFrameSum);
//}
XFrame::~XFrame()
{
	if (m_cp != NULL && --m_cp->m_counter <= 0)
	{//自身没有引用需要释放
		XMem::XDELETE_ARRAY(m_texnum);
		XMem::XDELETE_ARRAY(m_frameName);
		XMem::XDELETE_ARRAY(m_keyFrameArray);
		XMem::XDELETE_ARRAY(wre);
		XMem::XDELETE_ARRAY(hre);
		XMem::XDELETE_ARRAY(xpa);
		XMem::XDELETE_ARRAY(ypa);
		XMem::XDELETE(m_cp);
	}
	releaseMem();
}
XFrame& XFrame::operator = (const XFrame& temp)
{
	if (&temp == this) return *this;	//防止没必要的自我拷贝
	if (temp.m_cp != NULL) ++temp.m_cp->m_counter;
	//释放自身的资源
	if (m_cp != NULL && --m_cp->m_counter <= 0)
	{//自身没有引用需要释放
		XMem::XDELETE_ARRAY(m_texnum);
		XMem::XDELETE_ARRAY(m_frameName);
		XMem::XDELETE_ARRAY(m_keyFrameArray);
		XMem::XDELETE_ARRAY(wre);
		XMem::XDELETE_ARRAY(hre);
		XMem::XDELETE_ARRAY(xpa);
		XMem::XDELETE_ARRAY(ypa);
		XMem::XDELETE(m_cp);
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
	m_curFramesNumble = temp.m_curFramesNumble;				//当前播放到第几帧
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

	memcpy(m_centerDW, temp.m_centerDW, sizeof(float) * m_maxFrameSum);
	memcpy(m_centerDH, temp.m_centerDH, sizeof(float) * m_maxFrameSum);
	memcpy(m_keyFramePosition, temp.m_keyFramePosition, sizeof(XVec2) * m_maxFrameSum);

	m_isInited = temp.m_isInited;
	m_isACopy = XTrue;
	return *this;
}
void XFrame::setACopy(const XFrame& temp)
{
	if (&temp == this) return;			//防止没必要的自我拷贝
	if (temp.m_cp != NULL) ++temp.m_cp->m_counter;
	//释放自身的资源
	if (m_cp != NULL && --m_cp->m_counter <= 0)
	{//自身没有引用需要释放
		XMem::XDELETE_ARRAY(m_texnum);
		XMem::XDELETE_ARRAY(m_frameName);
		XMem::XDELETE_ARRAY(m_keyFrameArray);
		XMem::XDELETE_ARRAY(wre);
		XMem::XDELETE_ARRAY(hre);
		XMem::XDELETE_ARRAY(xpa);
		XMem::XDELETE_ARRAY(ypa);
		XMem::XDELETE(m_cp);
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
	m_curFramesNumble = temp.m_curFramesNumble;				//当前播放到第几帧
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

	memcpy(m_centerDW, temp.m_centerDW, sizeof(float) * m_maxFrameSum);
	memcpy(m_centerDH, temp.m_centerDH, sizeof(float) * m_maxFrameSum);
	memcpy(m_keyFramePosition, temp.m_keyFramePosition, sizeof(XVec2) * m_maxFrameSum);

	m_isInited = temp.m_isInited;
	m_isACopy = XTrue;
}
XFrame::XFrame(const XFrame &temp)
{
	if (&temp == this) return;			//防止没必要的自我拷贝
}
#if !WITH_INLINE_FILE
#include "XFrame.inl"
#endif
}