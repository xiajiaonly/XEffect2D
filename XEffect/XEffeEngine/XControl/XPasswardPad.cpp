//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include "XPasswardPad.h"
void btnFunc00(void *pClass,int ID)
{
	printf("0被按下\n");
	_XPasswardPad *pPar = (_XPasswardPad *)pClass;
	pPar->addChar('0');
}

void btnFunc01(void *pClass,int ID)
{
	printf("1被按下\n");
	_XPasswardPad *pPar = (_XPasswardPad *)pClass;
	pPar->addChar('1');
}

void btnFunc02(void *pClass,int ID)
{
	printf("2被按下\n");
	_XPasswardPad *pPar = (_XPasswardPad *)pClass;
	pPar->addChar('2');
}

void btnFunc03(void *pClass,int ID)
{
	printf("3被按下\n");
	_XPasswardPad *pPar = (_XPasswardPad *)pClass;
	pPar->addChar('3');
}

void btnFunc04(void *pClass,int ID)
{
	printf("4被按下\n");
	_XPasswardPad *pPar = (_XPasswardPad *)pClass;
	pPar->addChar('4');
}

void btnFunc05(void *pClass,int ID)
{
	printf("5被按下\n");
	_XPasswardPad *pPar = (_XPasswardPad *)pClass;
	pPar->addChar('5');
}

void btnFunc06(void *pClass,int ID)
{
	printf("6被按下\n");
	_XPasswardPad *pPar = (_XPasswardPad *)pClass;
	pPar->addChar('6');
}

void btnFunc07(void *pClass,int ID)
{
	printf("7被按下\n");
	_XPasswardPad *pPar = (_XPasswardPad *)pClass;
	pPar->addChar('7');
}

void btnFunc08(void *pClass,int ID)
{
	printf("8被按下\n");
	_XPasswardPad *pPar = (_XPasswardPad *)pClass;
	pPar->addChar('8');
}

void btnFunc09(void *pClass,int ID)
{
	printf("9被按下\n");
	_XPasswardPad *pPar = (_XPasswardPad *)pClass;
	pPar->addChar('9');
}

void btnFunc10(void *pClass,int ID)
{
	printf("确认 被按下\n");
	_XPasswardPad *pPar = (_XPasswardPad *)pClass;
	pPar->btn10Proc();
}

void btnFunc11(void *pClass,int ID)
{
	printf("取消 被按下\n");
	_XPasswardPad *pPar = (_XPasswardPad *)pClass;
	pPar->btn11Proc();
}

_XPasswardPadTexture::_XPasswardPadTexture()
:m_isInited(0)
,backGround(NULL)
{
}

_XPasswardPadTexture::~_XPasswardPadTexture()
{
	release();
}

int _XPasswardPadTexture::init(const char *normal,const char *down,const char *on,const char *disable,const char *BG,int resoursePosition)
{
	if(m_isInited != 0) return 0;
	if(m_bottomTex.init(normal,on,down,disable,resoursePosition) == 0) return 0;

	if(BG != NULL && (backGround = createATextureData(BG,resoursePosition)) == NULL) return 0;

	m_isInited = 1;
	return 1;
}

void _XPasswardPadTexture::release()
{
	if(m_isInited == 0) return;
	m_bottomTex.release();
	if(backGround != NULL)
	{
		backGround->release();			//无效状态下的按钮贴图
		XDELETE(backGround);
	}
	m_isInited = 0;
}

_XPasswardPad::_XPasswardPad()
:m_isInited(0)
,m_mode(0)
,m_position(0,0)
,m_stage(1)
,m_couter(0)
,m_isVisiable(false)
{
	m_oldPassward[0] = '\0';
	m_newPassward[0] = '\0';
	m_enterPassward[0] = '\0';
	m_showString[0] = '\0';
}
//键盘的排列是4*3
int _XPasswardPad::init(const _XVector2& position,_XPasswardPadTexture *tex,const _XFontUnicode& font,const _XPasswardPadData& data)
{
	if(m_isInited != 0) return 1;
	if(tex == NULL) return 0;

	m_texture = tex;
	m_passwardPadData = data;
	if(m_caption.setACopy(font) == 0) return 0;
	m_caption.setPosition(m_passwardPadData.captionPosition.x,m_passwardPadData.captionPosition.y);
	//if(m_buttonTexture.set(m_texture->buttonNormal,m_texture->buttonDown,m_texture->buttonOn,m_texture->buttonDisable) == 0) return 0;


	//初始化按钮
	m_button[0].init(m_passwardPadData.bottonOffsetPosition,_XRect(0,0,m_passwardPadData.bottonEffectArea.x,m_passwardPadData.bottonEffectArea.y),
		m_texture->m_bottomTex,"0",m_caption,1.0f,m_passwardPadData.bottonTextPosition);
	m_button[0].setPosition(m_passwardPadData.bottonOffsetPosition);
	for(int i = 1;i < 12;++ i)
	{
		m_button[i].setACopy(m_button[0]);
		m_button[i].setPosition(m_passwardPadData.bottonOffsetPosition.x + (i % 3) * m_passwardPadData.bottonDistance.x,
			m_passwardPadData.bottonOffsetPosition.y + (int)(i / 3) * m_passwardPadData.bottonDistance.y);
	}
	m_button[0].setCaptionText("0");
	m_button[0].setCallbackFun(NULL,NULL,NULL,NULL,btnFunc00,this);
	m_button[1].setCaptionText("1");
	m_button[1].setCallbackFun(NULL,NULL,NULL,NULL,btnFunc01,this);
	m_button[2].setCaptionText("2");
	m_button[2].setCallbackFun(NULL,NULL,NULL,NULL,btnFunc02,this);
	m_button[3].setCaptionText("3");
	m_button[3].setCallbackFun(NULL,NULL,NULL,NULL,btnFunc03,this);
	m_button[4].setCaptionText("4");
	m_button[4].setCallbackFun(NULL,NULL,NULL,NULL,btnFunc04,this);
	m_button[5].setCaptionText("5");
	m_button[5].setCallbackFun(NULL,NULL,NULL,NULL,btnFunc05,this);
	m_button[6].setCaptionText("6");
	m_button[6].setCallbackFun(NULL,NULL,NULL,NULL,btnFunc06,this);
	m_button[7].setCaptionText("7");
	m_button[7].setCallbackFun(NULL,NULL,NULL,NULL,btnFunc07,this);
	m_button[8].setCaptionText("8");
	m_button[8].setCallbackFun(NULL,NULL,NULL,NULL,btnFunc08,this);
	m_button[9].setCaptionText("9");
	m_button[9].setCallbackFun(NULL,NULL,NULL,NULL,btnFunc09,this);
	m_button[10].setCaptionText("确定");
	m_button[10].setCallbackFun(NULL,NULL,NULL,NULL,btnFunc10,this);
	m_button[10].setCaptionPosition(m_passwardPadData.bottonTextPosition);
	m_button[11].setCaptionText("取消");
	m_button[11].setCallbackFun(NULL,NULL,NULL,NULL,btnFunc11,this);
	m_button[11].setCaptionPosition(m_passwardPadData.bottonTextPosition);

	m_sprite.init(m_texture->backGround->texture.m_w,m_texture->backGround->texture.m_h,1);
	m_sprite.setPosition(0.0f,0.0f);

	m_position = position;
	setPosition(position);

	m_isInited = 1;
	return 1;
}

void _XPasswardPad::draw()
{
	if(m_isVisiable)
	{
		m_sprite.draw(m_texture->backGround);
	//	for(int i = 0;i < 12;++ i)
	//	{
	//		m_button[i].draw();
	//	}
		m_caption.draw();
	}
}

//void _XPasswardPad::mouseProc(float x,float y,_XMouseState mouseState)
//{
//	if(m_isVisiable)
//	{
//		for(int i = 0;i < 12;++ i)
//		{
//			m_button[i].mouseProc(x,y,mouseState);
//		}
//	}
//}

void _XPasswardPad::setPosition(const _XVector2& position)
{
	m_position = position;
	for(int i = 0;i < 12;++ i)
	{
		m_button[i].setPosition(m_position.x + m_passwardPadData.bottonOffsetPosition.x + (i % 3) * m_passwardPadData.bottonDistance.x,
			m_position.y + m_passwardPadData.bottonOffsetPosition.y + (int)(i / 3) * m_passwardPadData.bottonDistance.y);
	}
	m_caption.setPosition(m_position.x + m_passwardPadData.captionPosition.x,m_position.y + m_passwardPadData.captionPosition.y);
	m_sprite.setPosition(m_position);
}

void _XPasswardPad::setStart(int mode)	//设置密码输入板开始工作
{
	if(m_stage != 0)
	{
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
		m_isVisiable = true;
		m_enterPasswardLength = 0;
		m_enterPassward[m_enterPasswardLength] = '\0';
		m_showString[m_enterPasswardLength] = '\0';
		for(int i = 0;i < 12;++ i)
		{
			m_button[i].enable();
		}
		m_button[10].disable();
		m_button[11].setCaptionText("退出");
	}
}

void _XPasswardPad::btn10Proc()
{
	if(m_stage != 0) return;
	int len;
	if(m_mode == 0)
	{//密码输入模式
		int flag = 0;
		if(strlen(m_oldPassward) == strlen(m_enterPassward))
		{
			len = (int)(strlen(m_oldPassward));
			for(int i = 0;i < len;++ i)
			{
				if(m_oldPassward[i] != m_enterPassward[i])
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
			m_couter ++;
			if(m_couter >= MAX_ENTER_TRY)
			{
				m_stage = 2;
				m_caption.setString("密码输入错误");
			}else
			{
				m_enterPasswardLength = 0;
				m_enterPassward[m_enterPasswardLength] = '\0';
				m_showString[m_enterPasswardLength] = '\0';
				m_button[10].disable();
				m_button[11].setCaptionText("退出");
				m_caption.setString("错误请重新输入");
			}
		}
	}else
	{//修改密码模式
		if(m_couter == 0)
		{//要求重新输入密码
			m_couter ++;
			strcpy(m_newPassward,m_enterPassward);

			m_enterPasswardLength = 0;
			m_enterPassward[m_enterPasswardLength] = '\0';
			m_showString[m_enterPasswardLength] = '\0';
			m_button[10].disable();
			m_button[11].setCaptionText("退出");
			m_caption.setString("重新输入新密码");
		}else
		{//检查两次输入是否一致
			int flag = 0;
			if(strlen(m_newPassward) == strlen(m_enterPassward))
			{
				len = (int)(strlen(m_newPassward));
				for(int i = 0;i < len;++ i)
				{
					if(m_newPassward[i] != m_enterPassward[i])
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

void _XPasswardPad::btn11Proc()
{
	if(m_stage != 0) return;
	if(m_enterPasswardLength <= 0)
	{
		m_stage = 3;
		m_caption.setString("正在退出...");
	}else
	{
		m_enterPasswardLength --;
		m_showString[m_enterPasswardLength] = '\0';
		m_enterPassward[m_enterPasswardLength] = '\0';
		m_caption.setString(m_showString);
		if(m_enterPasswardLength < MIN_PASSWARD_LENGTH)
		{
			m_button[10].disable();
		}else
		{
			m_button[10].enable();
		}
		if(m_enterPasswardLength > 0)
		{
			m_button[11].setCaptionText("后退");
		}else
		{
			m_button[11].setCaptionText("退出");
		}
	}
}

void _XPasswardPad::addChar(char temp)
{
	if(m_stage != 0) return;
	if(m_enterPasswardLength < MAX_PASSWARD_LENGTH)
	{
		m_enterPassward[m_enterPasswardLength] = temp;
		m_showString[m_enterPasswardLength] = '*';
		m_enterPasswardLength ++;
		m_showString[m_enterPasswardLength] = '\0';
		m_enterPassward[m_enterPasswardLength] = '\0';
		m_caption.setString(m_showString);
		if(m_enterPasswardLength < MIN_PASSWARD_LENGTH)
		{
			m_button[10].disable();
		}else
		{
			m_button[10].enable();
		}
		if(m_enterPasswardLength > 0)
		{
			m_button[11].setCaptionText("后退");
		}
	}
}
