#ifndef _JIA_XPASSWARDPAD_
#define _JIA_XPASSWARDPAD_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.11.20
//--------------------------------

#include "XButton.h"
//这是一个用于输入数字或者用于修改密码的类，目前这个尚未实现，之后有时间在实现

#define MAX_PASSWARD_LENGTH (14)	//最大的密码输入长度
#define MAX_ENTER_TRY (3)			//输入密码的最大尝试次数
#define MIN_PASSWARD_LENGTH (4)		//最小的密码输入长度

class _XPasswardPadTexture
{
private:
	char m_isInited;
public:
	_XButtonTexture m_bottomTex;		//按键的贴图

	_XTextureData *backGround;				//密码输入的背景贴图

	_XPasswardPadTexture();
	~_XPasswardPadTexture();
	int init(const char *normal,const char *down,const char *on,const char *disable,const char *BG,int resoursePosition = 0);
	void release();
};

//密码输入板需要的一些数据
struct _XPasswardPadData
{
	_XVector2 bottonOffsetPosition;	//按钮区的偏移坐标
	_XVector2 bottonEffectArea;		//按钮响应的区域
	_XVector2 bottonDistance;		//按钮之间的距离
	_XVector2 bottonTextPosition;	//按钮上文字的位置
	_XVector2 captionPosition;		//中文字的位置
};
//目前这个控件尚不完善，不能从控件的基类中派生出来，需要完善
class _XPasswardPad
{
private:
	char m_isInited;	//是否初始化

	int m_mode;			//0:密码输入的模式 1:密码修改的模式
	_XVector2 m_position;
	_XPasswardPadData m_passwardPadData;	//密码输入属性的一些数据

	int m_stage;		//密码输入的阶段性标记量
	int m_couter;
	char m_oldPassward[MIN_PASSWARD_LENGTH + 1];	//老的密码
	char m_newPassward[MAX_PASSWARD_LENGTH + 1];	//新的密码
	char m_enterPassward[MAX_PASSWARD_LENGTH + 1];	//输入的密码
	char m_enterPasswardLength;
	char m_showString[MAX_PASSWARD_LENGTH + 1];		//用于显示的字符串

	_XFontUnicode m_caption;			//显示文字的字体
	_XSprite m_sprite;		//显示背景贴图
	_XButton m_button[12];	//12个小键盘的按键

	const _XPasswardPadTexture *m_texture;	//控件的贴图
	//_XButtonTexture m_buttonTexture;	//按钮控件的贴图

	bool m_isVisiable;	//是否显示

	friend void btnFunc00(void *pClass,int ID);
	friend void btnFunc01(void *pClass,int ID);
	friend void btnFunc02(void *pClass,int ID);
	friend void btnFunc03(void *pClass,int ID);
	friend void btnFunc04(void *pClass,int ID);
	friend void btnFunc05(void *pClass,int ID);
	friend void btnFunc06(void *pClass,int ID);
	friend void btnFunc07(void *pClass,int ID);
	friend void btnFunc08(void *pClass,int ID);
	friend void btnFunc09(void *pClass,int ID);
	friend void btnFunc10(void *pClass,int ID);
	friend void btnFunc11(void *pClass,int ID);

	void addChar(char temp);
	void btn11Proc();
	void btn10Proc();

public:
	int init(const _XVector2& position,_XPasswardPadTexture *tex,const _XFontUnicode& font,const _XPasswardPadData& data);
	void draw();
	int getIsEnd();		//返回输入的结果 密码输入模式：0：尚未结束 1：密码输入正确 2：密码输入错误 3：用户主动退出
										//密码修改模式：0：尚未结束 1：密码修改成功 2：两次密码输入错误 3：用户主动退出
//	void mouseProc(float x,float y,_XMouseState mouseState);
	void setPassward(const char *p);	//设置需要输入的正确密码
	void getPassward(char *p) const;	//获取新的密码
	void setStart(int mode);	//设置密码输入板开始工作
	void setDisShow();

	void setPosition(const _XVector2& position);

	_XPasswardPad();
private://为了防止意外调用造成的错误，这里重载赋值操作符和赋值构造函数
	_XPasswardPad(const _XPasswardPad &temp);
	_XPasswardPad& operator = (const _XPasswardPad& temp);
};

inline int _XPasswardPad::getIsEnd()
{
	return m_stage;
}

inline void _XPasswardPad::setDisShow()
{
	m_isVisiable = false;
}

inline void _XPasswardPad::getPassward(char *p) const	//获取新的密码
{
	if(p == NULL) return;
	if(strlen(m_newPassward) < MIN_PASSWARD_LENGTH) return;
	strcpy(p,m_newPassward);
}

inline void _XPasswardPad::setPassward(const char *p)	//设置需要输入的正确密码
{
	if(p == NULL) return;	//输入参数非法
	if(strlen(p) <= MIN_PASSWARD_LENGTH || strlen(p) > MAX_PASSWARD_LENGTH) return;	//密码长度不合法
	strcpy(m_oldPassward,p);
}
#endif