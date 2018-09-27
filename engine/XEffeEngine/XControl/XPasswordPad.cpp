#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include "XPasswordPad.h"
#include "XObjectManager.h" 
#include "XControlManager.h"
#include "XResourcePack.h"
namespace XE{
void XPasswordPad::ctrlProc(void*pClass,int id,int eventID)
{
	if(eventID != XButton::BTN_MOUSE_UP) return;
	XPasswordPad &pPar = *(XPasswordPad *)pClass;
	for(int i = 0;i < 10;++ i)
	{
		if(id == pPar.m_button[i].getControlID())
		{
			printf("%d被按下\n",i);
			pPar.addChar('0' + i);
			return;
		}
	}
	if(id == pPar.m_button[10].getControlID())
	{
		printf("确认 被按下\n");
		pPar.btn10Proc();
		return;
	}
	if(id == pPar.m_button[11].getControlID())
	{
		printf("取消 被按下\n");
		pPar.btn11Proc();
		return;
	}
}
XBool XPasswordPadSkin::init(const char *normal,const char *down,const char *on,const char *disable,const char *BG,XResPos resPos)
{
	if(m_isInited) return XFalse;
	if(!m_bottomTex.init(normal,on,down,disable,resPos)) return XFalse;

	if(BG != NULL && (backGround = createATextureData(BG,resPos)) == NULL) return XFalse;

	m_isInited = XTrue;
	return XTrue;
}
#define PASSWORDPAD_CONFIG_FILENAME "PasswordPad.txt"
bool XPasswordPadSkin::loadFromFolder(const char *filename, XResPos resPos)	//从文件夹中载入资源
{
	char tempFilename[MAX_FILE_NAME_LENGTH];
	sprintf_s(tempFilename, MAX_FILE_NAME_LENGTH, "%s/%s", filename, PASSWORDPAD_CONFIG_FILENAME);
	FILE *fp = NULL;
	if ((fp = fopen(tempFilename, "r")) == NULL) return XFalse; //信息文件读取失败
	//下面开始依次读取数据
	int flag = 0;
	char resFilename[MAX_FILE_NAME_LENGTH] = "";
	if (fscanf(fp, "%d:", &flag) != 1) { fclose(fp); return XFalse; }
	if (flag != 0)
	{
		if (fscanf(fp, "%s", resFilename) != 1) { fclose(fp); return XFalse; }
		sprintf_s(tempFilename, MAX_FILE_NAME_LENGTH, "%s/%s", filename, resFilename);
		if ((backGround = createATextureData(tempFilename, resPos)) == NULL)
		{//资源读取失败
			fclose(fp);
			return XFalse;
		}
	}
	int x, y;
	if (fscanf(fp, "%d,%d,", &x, &y) != 2) { fclose(fp); return XFalse; }
	m_data.bottonDistance.set(x, y);
	if (fscanf(fp, "%d,%d,", &x, &y) != 2) { fclose(fp); return XFalse; }
	m_data.bottonEffectArea.set(x, y);
	if (fscanf(fp, "%d,%d,", &x, &y) != 2) { fclose(fp); return XFalse; }
	m_data.bottonOffsetPosition.set(x, y);
	if (fscanf(fp, "%d,%d,", &x, &y) != 2) { fclose(fp); return XFalse; }
	m_data.bottonTextPosition.set(x, y);
	if (fscanf(fp, "%d,%d,", &x, &y) != 2) { fclose(fp); return XFalse; }
	m_data.captionPosition.set(x, y);
	fclose(fp);
	return true;
}
bool XPasswordPadSkin::loadFromPacker(const char *filename,XResPos resPos)	//从压缩包中载入资源
{
	char tempFilename[MAX_FILE_NAME_LENGTH];
	sprintf_s(tempFilename,MAX_FILE_NAME_LENGTH,"%s/%s",filename,PASSWORDPAD_CONFIG_FILENAME);
	unsigned char *p = XResPack.getFileData(tempFilename);
	if(p == NULL) return XFalse;
	//下面开始依次读取数据
	int flag = 0;
	char resFilename[MAX_FILE_NAME_LENGTH] = "";
	int offset = 0;
	//normal
	if(sscanf((char *)(p + offset),"%d:",&flag) != 1) {XMem::XDELETE_ARRAY(p);return XFalse;}
	offset += XString::getCharPosition((char *)(p + offset),':') + 1;
	if(flag != 0)
	{
		if(sscanf((char *)(p + offset),"%s",resFilename) != 1) {XMem::XDELETE_ARRAY(p);return XFalse;}
		offset += XString::getCharPosition((char *)(p + offset),'\n') + 1;
		sprintf_s(tempFilename,MAX_FILE_NAME_LENGTH,"%s/%s",filename,resFilename);
		if((backGround = createATextureData(tempFilename,resPos)) == NULL)
		{//资源读取失败
			XMem::XDELETE_ARRAY(p);
			return XFalse;
		}
	}
	int x,y;
	if(sscanf((char *)(p + offset),"%d,%d,",&x,&y) != 2) {XMem::XDELETE_ARRAY(p);return XFalse;}
	offset += XString::getCharPosition((char *)(p + offset),'\n') + 1;
	m_data.bottonDistance.set(x,y);
	if(sscanf((char *)(p + offset),"%d,%d,",&x,&y) != 2) {XMem::XDELETE_ARRAY(p);return XFalse;}
	offset += XString::getCharPosition((char *)(p + offset),'\n') + 1;
	m_data.bottonEffectArea.set(x,y);
	if(sscanf((char *)(p + offset),"%d,%d,",&x,&y) != 2) {XMem::XDELETE_ARRAY(p);return XFalse;}
	offset += XString::getCharPosition((char *)(p + offset),'\n') + 1;
	m_data.bottonOffsetPosition.set(x,y);
	if(sscanf((char *)(p + offset),"%d,%d,",&x,&y) != 2) {XMem::XDELETE_ARRAY(p);return XFalse;}
	offset += XString::getCharPosition((char *)(p + offset),'\n') + 1;
	m_data.bottonTextPosition.set(x,y);
	if(sscanf((char *)(p + offset),"%d,%d,",&x,&y) != 2) {XMem::XDELETE_ARRAY(p);return XFalse;}
	offset += XString::getCharPosition((char *)(p + offset),'\n') + 1;
	m_data.captionPosition.set(x,y);
	XMem::XDELETE_ARRAY(p);
	return true;
}
XBool XPasswordPadSkin::initEx(const char *filename,XResPos resPos)
{
	if(m_isInited || filename == NULL) return XFalse;
	if(!m_bottomTex.initEx(filename,resPos)) return XFalse;

	//先打开配置文件
	if(resPos == RES_SYS_DEF) resPos = getDefResPos();
	switch(resPos)
	{
	case RES_LOCAL_PACK:
		if(!loadFromPacker(filename,resPos)) return false;
		break;
	case RES_LOCAL_FOLDER:
		if(!loadFromFolder(filename,resPos)) return false;
		break;
	case RES_WEB:
		if(!loadFromWeb(filename,resPos)) return false;
		break;
	case RES_AUTO:
		if(!loadFromPacker(filename,resPos) && !loadFromFolder(filename,resPos) &&
			!loadFromWeb(filename,resPos)) return false;
		break;
	}

	m_isInited = XTrue;
	return XTrue;
}
XPasswordPad::XPasswordPad()
	:m_isInited(XFalse)
	, m_mode(0)
	//,m_position(0,0)
	, m_stage(1)
	, m_couter(0)
	//,m_isVisible(XFalse)
{
	m_oldPassword[0] = '\0';
	m_newPassword[0] = '\0';
	m_enterPassword[0] = '\0';
	m_showString[0] = '\0';
	m_ctrlType = CTRL_OBJ_PASSWORDPAD;
}
XPasswordPad::~XPasswordPad()
{
	XCtrlManager.decreaseAObject(this);	//注销这个物件
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(this);
#endif
	if(m_resInfo != NULL)
	{
		XResManager.releaseResource(m_resInfo);
		m_resInfo = NULL;
	}
}
//键盘的排列是4*3
XBool XPasswordPad::init(const XVec2& position, XPasswordPadSkin *tex, const XFontUnicode& font, const XPasswordPadData& data)
{
	if (m_isInited || tex == NULL) return XFalse;

	m_texture = tex;
	m_passwardPadData = data;
	if (!m_caption.setACopy(font)) return XFalse;
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_caption);
#endif
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT);
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);
	m_caption.setPosition(m_passwardPadData.captionPosition);
	m_caption.setColor(XFColor::white);
	//if(m_buttonTexture.set(m_texture->buttonNormal,m_texture->buttonDown,m_texture->buttonOn,m_texture->buttonDisable) == 0) return XFalse;
	m_scale.set(1.0f);
	//初始化按钮
	m_button[0].init(m_passwardPadData.bottonOffsetPosition, XRect(XVec2::zero, m_passwardPadData.bottonEffectArea),
		m_texture->m_bottomTex, "0", m_caption, 1.0f, m_passwardPadData.bottonTextPosition);
	m_button[0].setPosition(m_passwardPadData.bottonOffsetPosition);
	XCtrlManager.decreaseAObject(&m_button[0]);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_button[0]);
#endif
	for (int i = 1; i < 12; ++i)
	{
		m_button[i].setACopy(m_button[0]);
		m_button[i].setPosition(m_passwardPadData.bottonOffsetPosition.x + (i % 3) * m_passwardPadData.bottonDistance.x,
			m_passwardPadData.bottonOffsetPosition.y + (int)(i / 3) * m_passwardPadData.bottonDistance.y);
		XCtrlManager.decreaseAObject(&m_button[i]);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&m_button[i]);
#endif
	}
	for (int i = 0; i < 12; ++i)
	{
		m_button[i].setEventProc(ctrlProc, this);
	}
	m_button[0].setCaptionText("0");
	m_button[1].setCaptionText("1");
	m_button[2].setCaptionText("2");
	m_button[3].setCaptionText("3");
	m_button[4].setCaptionText("4");
	m_button[5].setCaptionText("5");
	m_button[6].setCaptionText("6");
	m_button[7].setCaptionText("7");
	m_button[8].setCaptionText("8");
	m_button[9].setCaptionText("9");
	m_button[10].setCaptionText("确定");
	m_button[10].setCaptionPosition(m_passwardPadData.bottonTextPosition);
	m_button[11].setCaptionText("取消");
	m_button[11].setCaptionPosition(m_passwardPadData.bottonTextPosition);

	m_sprite.init(m_texture->backGround->texture.m_w, m_texture->backGround->texture.m_h, 1);
	m_sprite.setPosition(XVec2::zero);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_sprite);
#endif
	m_mouseRect.set(0.0f, 0.0f, m_texture->backGround->texture.m_w, m_texture->backGround->texture.m_h);

	m_withoutTex = XFalse;

	XCtrlManager.addACtrl(this);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(this);
#endif

	m_isVisible = m_isEnable = m_isActive = XTrue;

	m_isInited = XTrue;
	setPosition(position);
	return XTrue;
}
XBool XPasswordPad::initWithoutSkin(const XFontUnicode& font)
{
	if (m_isInited) return XFalse;

	//m_texture = tex;
	//m_passwardPadData = data;
	m_passwardPadData.bottonOffsetPosition.set(10.0f, 42.0f);	//按钮区的偏移坐标
	m_passwardPadData.bottonEffectArea.set(64.0f, 32.0f);		//按钮响应的区域
	m_passwardPadData.bottonDistance.set(69.0f, 37.0f);		//按钮之间的距离
	m_passwardPadData.bottonTextPosition.set(32.0f, 16.0f);	//按钮上文字的位置
	m_passwardPadData.captionPosition.set(6.0f, 5.0f);		//中文字的位置

	if (!m_caption.setACopy(font)) return XFalse;
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_caption);
#endif
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT);
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);
	m_caption.setPosition(m_passwardPadData.captionPosition);
	m_caption.setColor(XFColor::white);
	//if(m_buttonTexture.set(m_texture->buttonNormal,m_texture->buttonDown,m_texture->buttonOn,m_texture->buttonDisable) == 0) return XFalse;
	m_scale.set(1.0f);

	//初始化按钮
	m_button[0].initWithoutSkin("0", m_caption, XRect(0.0f, 0.0f, 64.0f, 32.0f));
	//m_button[0].init(m_passwardPadData.bottonOffsetPosition,XRect(XVec2::zero,m_passwardPadData.bottonEffectArea),
	//	m_texture->m_bottomTex,"0",m_caption,1.0f,m_passwardPadData.bottonTextPosition);
	m_button[0].setPosition(m_passwardPadData.bottonOffsetPosition);
	XCtrlManager.decreaseAObject(&m_button[0]);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_button[0]);
#endif
	for (int i = 1; i < 12; ++i)
	{
		m_button[i].setACopy(m_button[0]);
		m_button[i].setPosition(m_passwardPadData.bottonOffsetPosition.x + (i % 3) * m_passwardPadData.bottonDistance.x,
			m_passwardPadData.bottonOffsetPosition.y + (int)(i / 3) * m_passwardPadData.bottonDistance.y);

		XCtrlManager.decreaseAObject(&m_button[i]);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&m_button[i]);
#endif
	}
	for (int i = 0; i < 12; ++i)
	{
		m_button[i].setEventProc(ctrlProc, this);
	}
	m_button[0].setCaptionText("0");
	m_button[1].setCaptionText("1");
	m_button[2].setCaptionText("2");
	m_button[3].setCaptionText("3");
	m_button[4].setCaptionText("4");
	m_button[5].setCaptionText("5");
	m_button[6].setCaptionText("6");
	m_button[7].setCaptionText("7");
	m_button[8].setCaptionText("8");
	m_button[9].setCaptionText("9");
	m_button[10].setCaptionText("确定");
	m_button[10].setCaptionPosition(m_passwardPadData.bottonTextPosition);
	m_button[11].setCaptionText("取消");
	m_button[11].setCaptionPosition(m_passwardPadData.bottonTextPosition);

	m_mouseRect.set(0.0f, 0.0f, 222.0f, 190.0f);

	m_isVisible = m_isEnable = m_isActive = XTrue;

	XCtrlManager.addACtrl(this);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(this);
#endif

	m_withoutTex = XTrue;
	m_isInited = XTrue;
	setPosition(XVec2::zero);
	return XTrue;
}
void XPasswordPad::draw()
{
	if (!m_isInited ||	//如果没有初始化直接退出
		!m_isVisible) return;	//如果不可见直接退出
	if (!m_withoutTex)
	{
		m_sprite.draw(m_texture->backGround);
	}
	else
	{
		XRender::drawFillRectExA(m_position, m_mouseRect.getSize() * m_scale,
			XCCS::disableColor * m_color, true, true);
		XRender::drawFillRectExA(m_position + XVec2(5.0f, 2.0f) * m_scale, XVec2(212.0f, 38.0f) * m_scale,
			XCCS::blackColor * m_color, false, true);
	}
	for (int i = 0; i < 12; ++i)
	{
		m_button[i].draw();
	}
	m_caption.draw();
}
XBool XPasswordPad::mouseProc(const XVec2& p, XMouseState mouseState)
{
	if (!m_isInited ||	//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||	//如果不可见直接退出
		!m_isEnable) return XFalse;		//如果无效则直接退出
	if (m_isSilent) return XFalse;

	for (int i = 0; i < 12; ++i)
	{
		m_button[i].mouseProc(p, mouseState);
	}
	if (mouseState == MOUSE_LEFT_BUTTON_UP && isInRect(p))
	{
		m_isBeChoose = XTrue;
	}
	return XTrue;
}
void XPasswordPad::setPosition(const XVec2& p)
{
	if(!m_isInited) return;
	m_position = p;
	m_curMouseRect.set(m_position + m_mouseRect.getLT() * m_scale,
		m_position + m_mouseRect.getRB() * m_scale);
	//设置各个物件的位置
	m_caption.setPosition(m_position + m_passwardPadData.captionPosition * m_scale);
	for(int i = 0;i < 12;++ i)
	{
		m_button[i].setPosition(m_position + 
			(m_passwardPadData.bottonOffsetPosition + XVec2(i % 3,(int)(i / 3)) * m_passwardPadData.bottonDistance) * m_scale);
	}
	m_sprite.setPosition(m_position);

	updateChildPos();
}
void XPasswordPad::setStart(int mode)	//设置密码输入板开始工作
{
	if (m_stage == 0) return;
	m_stage = 0;
	m_couter = 0;
	if(mode != 0)
	{
		m_mode = 1;
		m_caption.setString("请输入新密码");
	}else
	{
		m_mode = 0;
		m_caption.setString("请输入密码");
	}
	m_isVisible = XTrue;
	m_enterPasswordLength = 0;
	m_enterPassword[m_enterPasswordLength] = '\0';
	m_showString[m_enterPasswordLength] = '\0';
	for(int i = 0;i < 12;++ i)
	{
		m_button[i].enable();
	}
	m_button[10].disable();
	m_button[11].setCaptionText("退出");
}
void XPasswordPad::btn10Proc()
{
	if(m_stage != 0) return;
	int len;
	if(m_mode == 0)
	{//密码输入模式
		int flag = 0;
		if(strlen(m_oldPassword) == strlen(m_enterPassword))
		{
			len = (int)(strlen(m_oldPassword));
			for(int i = 0;i < len;++ i)
			{
				if(m_oldPassword[i] != m_enterPassword[i])
				{
					flag = 1;
					break;
				}
			}
		}else
		{
			flag = 1;
		}
		if(flag == 0)
		{//密码输入正确
			m_stage = 1;
			m_caption.setString("密码输入正确");
		}else
		{//密码输入错误
			++ m_couter;
			if(m_couter >= m_maxEnterTry)
			{
				m_stage = 2;
				m_caption.setString("密码输入错误");
			}else
			{
				m_enterPasswordLength = 0;
				m_enterPassword[m_enterPasswordLength] = '\0';
				m_showString[m_enterPasswordLength] = '\0';
				m_button[10].disable();
				m_button[11].setCaptionText("退出");
				m_caption.setString("错误请重新输入");
			}
		}
	}else
	{//修改密码模式
		if(m_couter == 0)
		{//要求重新输入密码
			++ m_couter;
			strcpy_s(m_newPassword,m_maxPasswardLength + 1,m_enterPassword);

			m_enterPasswordLength = 0;
			m_enterPassword[m_enterPasswordLength] = '\0';
			m_showString[m_enterPasswordLength] = '\0';
			m_button[10].disable();
			m_button[11].setCaptionText("退出");
			m_caption.setString("重新输入新密码");
		}else
		{//检查两次输入是否一致
			int flag = 0;
			if(strlen(m_newPassword) == strlen(m_enterPassword))
			{
				len = (int)(strlen(m_newPassword));
				for(int i = 0;i < len;++ i)
				{
					if(m_newPassword[i] != m_enterPassword[i])
					{
						flag = 1;
						break;
					}
				}
			}else
			{
				flag = 1;
			}
			if(flag == 0)
			{
				m_stage = 1;
				m_caption.setString("密码修改完成");
			}else
			{
				m_stage = 2;
				m_caption.setString("两次输入不匹配");
			}
		}
	}
}
void XPasswordPad::btn11Proc()
{
	if(m_stage != 0) return;
	if(m_enterPasswordLength <= 0)
	{
		m_stage = 3;
		m_caption.setString("正在退出...");
	}else
	{
		-- m_enterPasswordLength;
		m_showString[m_enterPasswordLength] = '\0';
		m_enterPassword[m_enterPasswordLength] = '\0';
		m_caption.setString(m_showString);
		if(m_enterPasswordLength < m_minPasswardLength)
		{
			m_button[10].disable();
		}else
		{
			m_button[10].enable();
		}
		if(m_enterPasswordLength > 0)
		{
			m_button[11].setCaptionText("后退");
		}else
		{
			m_button[11].setCaptionText("退出");
		}
	}
}
void XPasswordPad::addChar(char temp)
{
	if(m_stage != 0) return;
	if(m_enterPasswordLength < m_maxPasswardLength)
	{
		m_enterPassword[m_enterPasswordLength] = temp;
		m_showString[m_enterPasswordLength] = '*';
		++ m_enterPasswordLength;
		m_showString[m_enterPasswordLength] = '\0';
		m_enterPassword[m_enterPasswordLength] = '\0';
		m_caption.setString(m_showString);
		if(m_enterPasswordLength < m_minPasswardLength)
		{
			m_button[10].disable();
		}else
		{
			m_button[10].enable();
		}
		if(m_enterPasswordLength > 0)
		{
			m_button[11].setCaptionText("后退");
		}
	}
}
#if !WITH_INLINE_FILE
#include "XPasswordPad.inl"
#endif
}