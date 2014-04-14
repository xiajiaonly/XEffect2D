#ifndef _JIA_GGAME_
#define _JIA_GGAME_
#include "XEffeEngine.h"
#include "XControl/XControlManager.h"
#include "XResourcePack.h"

class _GGame
{
	//+++++++++++++++++++++++++++++++++++++++++++
	//下面需要将其设计为Singleton模式
protected:
	_GGame(){}
	_GGame(const _GGame&);
	_GGame &operator= (const _GGame&);
	virtual ~_GGame(){}
public:
	static _GGame& GetInstance()
	{
		static _GGame m_instance;
		return m_instance;
	}
	//-------------------------------------------
public:
	bool init();
	void move(int stepTime);
	void draw();
	void input(const _XInputEvent &input);
	void release(){;}

	_XFontUnicode FontUnicode;
	_XButton Button;		//按钮
	_XButton ButtonCopy;
	_XButtonEx ButtonEx;	//特殊按钮
	_XButtonEx ButtonExCopy;
	_XCheckTexture CheckTexture;
	_XCheck Check;			//复选框
	_XCheck CheckCopy;
	_XMouseRightButtonMenu MouseRightButtonMenu; //输入框的右键菜单
	_XEdit Edit;			//输入框
	_XEdit EditCopy;
	_XSlider Slider;		//滑动条
	_XSlider SliderCopy;
	_XRadios Radios;		//单选框
	_XRadios RadiosCopy;	
	_XNumber Number;
	_XProgress Progress;	//进度条
	_XProgress ProgressCopy;
	_XMultiText MutiText;	//多行文本显示
	_XMultiText MutiTextCopy;
	_XMultiList MutiList;	//多列列表框
	_XCombo Combo;			//下拉菜单
	_XCombo ComboCopy;
	_XSliderEx tmpSldEx;
	_XPasswordPad passwordPad;
};

#endif