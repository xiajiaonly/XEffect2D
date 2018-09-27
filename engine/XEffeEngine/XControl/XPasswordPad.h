#ifndef _JIA_XPASSWARDPAD_
#define _JIA_XPASSWARDPAD_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.11.20
//--------------------------------
#include "XButton.h"
namespace XE{
//这是一个用于输入数字或者用于修改密码的类，目前这个尚未实现，之后有时间在实现

//密码输入板需要的一些数据
struct XPasswordPadData
{
	XVec2 bottonOffsetPosition;	//按钮区的偏移坐标
	XVec2 bottonEffectArea;		//按钮响应的区域
	XVec2 bottonDistance;		//按钮之间的距离
	XVec2 bottonTextPosition;	//按钮上文字的位置
	XVec2 captionPosition;		//中文字的位置
};
class XPasswordPadSkin
{
private:
	XBool m_isInited;
public:
	XButtonSkin m_bottomTex;		//按键的贴图
	XTextureData *backGround;				//密码输入的背景贴图
	XPasswordPadData m_data;			//数据
	XPasswordPadSkin()
		:m_isInited(XFalse)
		,backGround(NULL)
	{}
	~XPasswordPadSkin(){release();}
	XBool init(const char *normal,const char *down,const char *on,const char *disable,
		const char *BG,XResPos resPos = RES_SYS_DEF);
	XBool initEx(const char *filename,XResPos resPos = RES_SYS_DEF);
	void release();
private:
	bool loadFromFolder(const char *filename,XResPos resPos);	//从文件夹中载入资源
	bool loadFromPacker(const char *filename,XResPos resPos);	//从压缩包中载入资源
	bool loadFromWeb(const char *filename, XResPos resPos) { return false; }		//从网页中读取资源
};

//目前这个控件尚不完善，不能从控件的基类中派生出来，需要完善
class XPasswordPad:public XControlBasic
{
private:
	static const int m_maxEnterTry = 3;			//输入密码的最大尝试次数
	static const int m_maxPasswardLength = 14;	//最大的密码输入长度
	static const int m_minPasswardLength = 4;	//最小的密码输入长度

	int m_mode;			//0:密码输入的模式 1:密码修改的模式
//	XVec2 m_position;
	XPasswordPadData m_passwardPadData;	//密码输入属性的一些数据

	int m_stage;		//密码输入的阶段性标记量
	int m_couter;
	char m_oldPassword[m_maxPasswardLength + 1];	//老的密码
	char m_newPassword[m_maxPasswardLength + 1];	//新的密码
	char m_enterPassword[m_maxPasswardLength + 1];	//输入的密码
	char m_enterPasswordLength;
	char m_showString[m_maxPasswardLength + 1];		//用于显示的字符串
	XBool m_isInited;	//是否初始化

	XFontUnicode m_caption;			//显示文字的字体
	XSprite m_sprite;		//显示背景贴图
	XButton m_button[12];	//12个小键盘的按键

	const XPasswordPadSkin *m_texture;	//控件的贴图
	//XButtonTexture m_buttonTexture;	//按钮控件的贴图

	//XBool m_isVisible;	//是否显示

	static void ctrlProc(void*,int,int);

	void addChar(char temp);
	void btn11Proc();
	void btn10Proc();

	XResourceInfo *m_resInfo;
	XBool m_withoutTex;	//没有贴图的形式
public:
	XBool init(const XVec2& position,XPasswordPadSkin *tex,const XFontUnicode& font,const XPasswordPadData& data);
	XBool initEx(const XVec2& position,XPasswordPadSkin *tex,const XFontUnicode& font);
	XBool initPlus(const char * path,const XFontUnicode& font,
		XResPos resPos = RES_SYS_DEF);
	XBool initWithoutSkin(const XFontUnicode& font);	
	XBool initWithoutSkin() {return initWithoutSkin(getDefaultFont());}	
protected:
	void draw();
	void drawUp();
	void update(float stepTime);
	XBool mouseProc(const XVec2& p,XMouseState mouseState);
	XBool keyboardProc(int,XKeyState) {return XTrue;}
	virtual void insertChar(const char *,int) {;}
	XBool canGetFocus(const XVec2& p);				//用于判断当前物件是否可以获得焦点
	XBool canLostFocus(const XVec2&){return XTrue;}	//应该是可以随时失去焦点的
public:
	int getIsEnd();		//返回输入的结果 密码输入模式：0：尚未结束 1：密码输入正确 2：密码输入错误 3：用户主动退出
										//密码修改模式：0：尚未结束 1：密码修改成功 2：两次密码输入错误 3：用户主动退出
	void setPassword(const char *p);	//设置需要输入的正确密码
	void getPassword(char *p) const;	//获取新的密码
	void setStart(int mode);	//设置密码输入板开始工作
	void setDisShow();

	using XObjectBasic::setPosition;	//避免覆盖的问题
	void setPosition(const XVec2& p);

	using XObjectBasic::setScale;	//避免覆盖的问题
	void setScale(const XVec2& s);
	using XObjectBasic::setColor;	//避免覆盖的问题
	void setColor(const XFColor& c);
	void setAlpha(float a);
	//virtual void justForTest() {;}

	XBool isInRect(const XVec2& p);		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
	XVec2 getBox(int order);			//获取四个顶点的坐标，目前先不考虑旋转和缩放

	XPasswordPad();
	~XPasswordPad();
private://为了防止意外调用造成的错误，这里重载赋值操作符和赋值构造函数
	XPasswordPad(const XPasswordPad &temp);
	XPasswordPad& operator = (const XPasswordPad& temp);
};
#if WITH_INLINE_FILE
#include "XPasswordPad.inl"
#endif
}
#endif