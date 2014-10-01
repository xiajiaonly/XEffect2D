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

//密码输入板需要的一些数据
struct _XPasswordPadData
{
	_XVector2 bottonOffsetPosition;	//按钮区的偏移坐标
	_XVector2 bottonEffectArea;		//按钮响应的区域
	_XVector2 bottonDistance;		//按钮之间的距离
	_XVector2 bottonTextPosition;	//按钮上文字的位置
	_XVector2 captionPosition;		//中文字的位置
};
class _XPasswordPadTexture
{
private:
	_XBool m_isInited;
public:
	_XButtonTexture m_bottomTex;		//按键的贴图

	_XTextureData *backGround;				//密码输入的背景贴图

	_XPasswordPadData m_data;			//数据

	_XPasswordPadTexture()
		:m_isInited(XFalse)
		,backGround(NULL)
	{}
	~_XPasswordPadTexture(){release();}
	_XBool init(const char *normal,const char *down,const char *on,const char *disable,
		const char *BG,_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	_XBool initEx(const char *filename,_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	void release();
};

//目前这个控件尚不完善，不能从控件的基类中派生出来，需要完善
class _XPasswordPad:public _XControlBasic
{
private:
	_XBool m_isInited;	//是否初始化

	int m_mode;			//0:密码输入的模式 1:密码修改的模式
//	_XVector2 m_position;
	_XPasswordPadData m_passwardPadData;	//密码输入属性的一些数据

	int m_stage;		//密码输入的阶段性标记量
	int m_couter;
	char m_oldPassword[MIN_PASSWARD_LENGTH + 1];	//老的密码
	char m_newPassword[MAX_PASSWARD_LENGTH + 1];	//新的密码
	char m_enterPassword[MAX_PASSWARD_LENGTH + 1];	//输入的密码
	char m_enterPasswordLength;
	char m_showString[MAX_PASSWARD_LENGTH + 1];		//用于显示的字符串

	_XFontUnicode m_caption;			//显示文字的字体
	_XSprite m_sprite;		//显示背景贴图
	_XButton m_button[12];	//12个小键盘的按键

	const _XPasswordPadTexture *m_texture;	//控件的贴图
	//_XButtonTexture m_buttonTexture;	//按钮控件的贴图

	//_XBool m_isVisible;	//是否显示

	friend void btnFunc(void *pClass,int ID);

	void addChar(char temp);
	void btn11Proc();
	void btn10Proc();

	_XResourceInfo *m_resInfo;
	_XBool m_withoutTex;	//没有贴图的形式
public:
	_XBool init(const _XVector2& position,_XPasswordPadTexture *tex,const _XFontUnicode& font,const _XPasswordPadData& data);
	_XBool initEx(const _XVector2& position,_XPasswordPadTexture *tex,const _XFontUnicode& font);
	_XBool initPlus(const char * path,const _XFontUnicode& font,
		_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	_XBool initWithoutTex(const _XFontUnicode& font);	
	_XBool initWithoutTex() {return initWithoutTex(XEE::systemFont);}	
protected:
	void draw();
	void drawUp();
	void update(int stepTime);
	_XBool mouseProc(float x,float y,_XMouseState mouseState);
	_XBool keyboardProc(int,_XKeyState) {return XTrue;}
	virtual void insertChar(const char *,int) {;}
	_XBool canGetFocus(float x,float y);				//用于判断当前物件是否可以获得焦点
	_XBool canLostFocus(float,float){return XTrue;}	//应该是可以随时失去焦点的
public:
	int getIsEnd();		//返回输入的结果 密码输入模式：0：尚未结束 1：密码输入正确 2：密码输入错误 3：用户主动退出
										//密码修改模式：0：尚未结束 1：密码修改成功 2：两次密码输入错误 3：用户主动退出
	void setPassword(const char *p);	//设置需要输入的正确密码
	void getPassword(char *p) const;	//获取新的密码
	void setStart(int mode);	//设置密码输入板开始工作
	void setDisShow();

	using _XObjectBasic::setPosition;	//避免覆盖的问题
	void setPosition(float x,float y);

	using _XObjectBasic::setSize;	//避免覆盖的问题
	void setSize(float x,float y);
	using _XObjectBasic::setColor;	//避免覆盖的问题
	void setColor(float r,float g,float b,float a);
	void setAlpha(float a);
	//virtual void justForTest() {;}

	_XBool isInRect(float x,float y);		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
	_XVector2 getBox(int order);			//获取四个顶点的坐标，目前先不考虑旋转和缩放

	_XPasswordPad();
	~_XPasswordPad();
private://为了防止意外调用造成的错误，这里重载赋值操作符和赋值构造函数
	_XPasswordPad(const _XPasswordPad &temp);
	_XPasswordPad& operator = (const _XPasswordPad& temp);
};
#include "XPasswordPad.inl"

#endif