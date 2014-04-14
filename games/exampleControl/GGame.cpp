#include "GGame.h"
#define RES_POS (RESOURCE_LOCAL_FOLDER)//(RESOURCE_LOCAL_PACK)//
void sliderChange(void *pClass,int ID)
{
	_GGame &g = *(_GGame *)pClass;
	g.Progress.setValue(g.Slider.getNowValue());
	g.ProgressCopy.setValue(100.0f - g.Slider.getNowValue());
}
void cbMouseOn(void *pClass,int ID)
{
	printf("MouseOn ID:%d\n",ID);
}
void cbMouseDown(void *pClass,int ID)
{
	printf("MouseDown ID:%d\n",ID);
}
void cbMouseUp(void *pClass,int ID)
{
	printf("MouseUp ID:%d\n",ID);
}
bool _GGame::init()
{
	//_XLogbook::GetInstance().initLog("Log.txt");
	//if(_XAllTexture::GetInstance().init(RES_POS) == 0) return 0;	//拿一个资源优化做测试用例
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().init(RES_POS);
#endif
	//初始化字体
	FontUnicode.initEx("ResourcePack/pic/Font",RES_POS);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&FontUnicode);
#endif
	//初始化按钮
	//Button.initPlus("ResourcePack/pic/Button","xiajia",FontUnicode,0.5f,RES_POS);
	Button.initWithoutTex("xiajia",FontUnicode,0.5f,_XRect(0.0f,0.0f,90.0f,30.0f),_XVector2(45.0f,15.0f));
	Button.setPosition(0.0f,30.0f);
	Button.setCallbackFun(NULL,NULL,cbMouseOn,cbMouseDown,cbMouseUp,NULL);
	Button.setSize(1.0f,1.0f);
	Button.setCaptionText("你好啊");
	Button.setHotKey(SDLK_0);

	ButtonCopy.initPlus("ResourcePack/pic/Button","xiajia",FontUnicode,0.5f,RES_POS);
	ButtonCopy.setCallbackFun(NULL,NULL,cbMouseOn,cbMouseDown,cbMouseUp,NULL);
	ButtonCopy.setSize(1.0f,1.0f);
	ButtonCopy.setPosition(0.0f,100.0f);
	ButtonCopy.setCaptionText("你好个屁");
	ButtonCopy.disable();

	ButtonEx.initPlus("ResourcePack/pic/ButtonEx","静",1.0f,FontUnicode,RES_POS);
	ButtonEx.setPosition(150.0f,30.0f);
	ButtonExCopy.setACopy(ButtonEx);
	ButtonExCopy.setPosition(280.0f,30.0f);
	ButtonExCopy.setCaptionText("忍");
	//初始化复选框
	CheckTexture.initEx("ResourcePack/pic/Check",RES_POS);

	Check.initPlus("ResourcePack/pic/Check","复选框",FontUnicode,1.0f,RES_POS);
	Check.setPosition(0.0f,160.0f);
	CheckCopy.initWithoutTex("复选框",FontUnicode,1.0f,_XRect(0.0f,0.0f,32.0f,32.0f),_XVector2(34.0f,16.0f));
	//CheckCopy.setACopy(Check);
	CheckCopy.setPosition(150.0f,160.0f);
	CheckCopy.setText("复选个蛋");
	CheckCopy.setSize(0.5f,0.5f);
	//初始化右键菜单
	//MouseRightButtonMenu.initPlus("ResourcePack/pic/RightButtonMenu",4,FontUnicode,1.0f,RES_POS);
	MouseRightButtonMenu.initWithoutTex(4,_XRect(0,0,200,50),FontUnicode,1.0f,_XVector2(100,25));
	MouseRightButtonMenu.setText("剪切(T)",0);
	MouseRightButtonMenu.setText("复制(C)",1);
	MouseRightButtonMenu.setText("粘贴(V)",2);
	MouseRightButtonMenu.setText("撤销(S)",3);
	MouseRightButtonMenu.setHotKey(SDLK_t,0);
	MouseRightButtonMenu.setHotKey(SDLK_c,1);
	MouseRightButtonMenu.setHotKey(SDLK_v,2);
	MouseRightButtonMenu.setHotKey(SDLK_s,3);
	//MouseRightButtonMenu.setCallbackFun(NULL,NULL);
	MouseRightButtonMenu.setSize(0.5f,0.5f);
	_XControlManager::GetInstance().decreaseAObject(&MouseRightButtonMenu);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&MouseRightButtonMenu);
#endif
	//初始化输入框
	FontUnicode.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT); //设置字体左对齐
	FontUnicode.setColor(0.0f,0.0f,0.0f,1.0f);
	Edit.initPlus("ResourcePack/pic/Edit","Test",FontUnicode,1.0f,&MouseRightButtonMenu,RES_POS);
	//Edit.disable();
	//Edit.setCallbackFun(NULL,eInputOver,&Edit);
	Edit.setPosition(400.0f,50.0f);
	Edit.setString("This is a test for setString().");
	//EditCopy.setACopy(Edit);
	EditCopy.initWithoutTex(_XRect(0,0,512,40),"Test",FontUnicode,1.0f,&MouseRightButtonMenu);
	EditCopy.setPosition(400.0f,100.0f);
	EditCopy.setString("This is a test for setString().");
	EditCopy.setSize(0.5f,0.5f);
	EditCopy.setTextColor(_XFColor(1.0f,1.0f,1.0f,1.0f));
	//初始化滑动条
	//_XSliderTexture SliderTexture;
	//垂直滑动条
	//SliderTexture.init("pic/Slider/BLH_00.png","pic/Slider/BLH_01.png","pic/Slider/BLH_02.png","pic/Slider/BLH_03.png",
	//"pic/Slider/BH_00.png","pic/Slider/BH_01.png","pic/Slider/BH_02.png","pic/Slider/BH_03.png");
	//Slider.init(_XVector2(50.0f,250.0f),_XRect(6.0f,11.0f,32.0f,506.0f),_XRect(5.0f,4.0f,27.0f,26.0f),SliderTexture,_XSLIDER_TYPE_VERTICAL,100.0f,0.0f);
	//水平滑动条
	//SliderTexture.initEx("ResourcePack/pic/Slider",RES_POS);
	Slider.initPlus("ResourcePack/pic/Slider",100.0f,0.0f,SLIDER_TYPE_HORIZONTAL,RES_POS);
	Slider.setPosition(0.0f,250.0f);
	Slider.setCallbackFun(NULL,NULL,NULL,NULL,NULL,sliderChange,sliderChange,this);
	Slider.setNowValue(55.0f);
	//SliderCopy.setACopy(Slider);
	//SliderCopy.initWithoutTex(_XRect(0,0,512,16),_XRect(0,0,16,16),SLIDER_TYPE_HORIZONTAL,100.0f,0.0f,_XVector2(0.0f,0.0f));
	SliderCopy.initWithoutTex(_XRect(0,0,512,16));
	SliderCopy.setPosition(0.0f,300.0f);
	SliderCopy.setSize(0.5f,0.5f);
	//下面是单选框
	Radios.initPlus(5,_XVector2(0.0f,35.0f),"ResourcePack/pic/Check",FontUnicode,1.0f,RES_POS);
	Radios.setPosition(0.0f,350.0f);
	Radios.setRadiosText("我真的很蛋疼;我蛋疼而且要爆了;我要爆了;我已经爆了;I have a boom!");
	//Radios.setRadioText("我真的很蛋疼",0);
	//Radios.setRadioText("我蛋疼而且要爆了",1);
	//Radios.setRadioText("我要爆了",2);
	//Radios.setRadioText("我已经爆了",3);
	//Radios.setRadioText("I have a boom!",4);
	Radios.setChoosed(3);
	//RadiosCopy.setACopy(Radios);
	RadiosCopy.initWithoutTex(5,_XVector2(0.0f,34.0f),_XRect(1,1,31,31),FontUnicode,1.0f,_XVector2(35,16));
	RadiosCopy.setPosition(300.0f,350.0f);
	RadiosCopy.setRadioSum(8);
	RadiosCopy.setSize(0.5f,0.5f);
	RadiosCopy.setRadioText("蛋完美了",0);
	RadiosCopy.setRadioText("蛋好了",1);
	RadiosCopy.setRadioText("蛋开始好了",2);
	RadiosCopy.setRadioText("蛋没变化",3);
	RadiosCopy.setRadioText("蛋变坏了",4);
	RadiosCopy.setRadioText("蛋变胀痛",5);
	RadiosCopy.setRadioText("蛋要爆了",6);
	RadiosCopy.setRadioText("蛋已经爆了",7);
	//下面是进度条
	Number.initEx("ResourcePack/pic/Number.png",RES_POS);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&Number);
#endif
	//_XProgressTexture ProgressTexture;
	//ProgressTexture.initEx("ResourcePack/pic/Progress",RES_POS);
	Progress.initPlus("ResourcePack/pic/Progress",&Number,1.0f,0,RES_POS);
	Progress.setPosition(0.0f,550.0f);
	Progress.setValue(25.0f);
	//ProgressCopy.setACopy(Progress);
	ProgressCopy.initWithoutTex(_XRect(0,0,512,32),&Number,1.0f,_XVector2(256,3));
	ProgressCopy.setPosition(0.0f,600.0f);
	ProgressCopy.setSize(0.5f,0.5f);
	ProgressCopy.setColor(_XFColor(1.0f,1.0f,1.0f,0.5f));
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//下面是多行文本显示
//	_XSliderTexture SliderTextureH;
//	SliderTextureH.initEx("ResourcePack/pic/MutiText/SliderH",RES_POS);
//	_XSlider SliderH;
//	SliderH.initEx(_XVector2(200.0f,200.0f),SliderTextureH,SLIDER_TYPE_HORIZONTAL,100.0f,0.0f);
//	_XControlManager::GetInstance().decreaseAObject(&SliderH);
//#if WITH_OBJECT_MANAGER
//	_XObjectManager::GetInstance().decreaseAObject(&SliderH);
//#endif
//	_XSliderTexture SliderTextureV;
//	SliderTextureV.initEx("ResourcePack/pic/MutiText/SliderV",RES_POS);
//	_XSlider SliderV;
//	SliderV.initEx(_XVector2(200.0f,200.0f),SliderTextureV,SLIDER_TYPE_VERTICAL,100.0f,0.0f);
//	_XControlManager::GetInstance().decreaseAObject(&SliderV);
//#if WITH_OBJECT_MANAGER
//	_XObjectManager::GetInstance().decreaseAObject(&SliderV);
//#endif
	char tempTest[] = "你hao this is a test for 中文显示\n0 :jiashenghua\n1 :123456789012345678901234567890123456789\n2 :PI = 3.141592653589793\n3 :\n\n5 :don`t use Chinese\n6 :It is not supply for Chinese at now!\n7 :\n8 :\n9 :\n10:\n11:\n12:\n13:\n14:\n15:\n16:";
//	_XMultiTextTexture MutiTextTexture;
//	MutiTextTexture.initEx("ResourcePack/pic/MutiText",RES_POS);
//	MutiText.initEx(_XVector2(520.0f,150.0f),MutiTextTexture,tempTest,FontUnicode,1.0f,SliderV,SliderH);
	MutiText.initPlus("ResourcePack/pic/MutiText",tempTest,FontUnicode,1.0f,RES_POS);
	MutiText.setPosition(520.0f,150.0f);
	MutiText.setSize(0.5f,0.5f);
	//MutiTextCopy.setACopy(MutiText);
//	MutiTextCopy.initWithoutTex(_XRect(4,4,508,508),tempTest,FontUnicode,1.0f);
	MutiTextCopy.initWithoutTex(_XRect(0,0,520,600),tempTest,FontUnicode,1.0f);
	MutiTextCopy.setSize(0.5f,0.5f);
	MutiTextCopy.setPosition(790.0f,150.0f);
	MutiTextCopy.setTextColor(_XFColor(1.0f,1.0f,1.0f,1.0f));
//	MutiTextCopy.disActive();
//	MutiTextCopy.disVisiable();
	//多列列表框
//	_XSliderTexture SliderTextureHX;
//	SliderTextureHX.initEx("ResourcePack/pic/MutiList/SliderH",RES_POS);
//	_XSlider SliderHX;
//	SliderHX.initEx(_XVector2(200.0f,200.0f),SliderTextureHX,SLIDER_TYPE_HORIZONTAL,100.0f,0.0f);
//	_XControlManager::GetInstance().decreaseAObject(&SliderHX);
//#if WITH_OBJECT_MANAGER
//	_XObjectManager::GetInstance().decreaseAObject(&SliderHX);
//#endif
//	_XSliderTexture SliderTextureVX;
//	SliderTextureVX.initEx("ResourcePack/pic/MutiList/SliderV",RES_POS);
//	_XSlider SliderVX;
//	SliderVX.initEx(_XVector2(200.0f,200.0f),SliderTextureVX,SLIDER_TYPE_VERTICAL,100.0f,0.0f);
//	_XControlManager::GetInstance().decreaseAObject(&SliderVX);
//#if WITH_OBJECT_MANAGER
//	_XObjectManager::GetInstance().decreaseAObject(&SliderVX);
//#endif
//	_XMultiListTexture MutiListTexture;
//	MutiListTexture.initEx("ResourcePack/pic/MutiList",RES_POS);
//	MutiList.initEx(_XVector2(610.0f,450.0f),MutiListTexture,FontUnicode,1.0f,3,24,SliderVX,SliderHX);
//	MutiList.initPlus("ResourcePack/pic/MutiList",FontUnicode,1.0f,3,24,RES_POS);
	//MutiList.initWithoutTex(_XRect(4,4,508,508),FontUnicode,1.0f,3,24);
	MutiList.initWithoutTex(_XRect(0,0,800,600),FontUnicode,1.0f,3,24);
	char tempChar[] = "Boxxx"; 
	for(int i = 0;i < 24;++ i)
	{
		for(int j = 0; j < 3;++ j)
		{
			tempChar[3] = (i * 4 + j) /10 + '0';
			tempChar[4] = (i * 4 + j) %10 + '0';
			MutiList.setBoxStr(tempChar,i,j);
		}
	}
	MutiList.setTileStr("贾Title00",0);
	MutiList.setRowWidth(100,0);
	MutiList.setTileStr("Title胜",1);
	MutiList.setRowWidth(120,1);
	MutiList.setTileStr("Title02华",2);
	MutiList.setRowWidth(150,2);
	MutiList.exportData();
	//MutiList.importData();
	MutiList.setPosition(610,450);
	MutiList.setSize(0.5,0.5);
	//初始化下拉菜单+
	//_XComboTexture ComboTexture;
	//ComboTexture.initEx("ResourcePack/pic/Combo",RES_POS);
	//Combo.initEx(ComboTexture,_XVector2(250.0f,190.0f),10,5,FontUnicode,1.0f);
	Combo.initPlus("ResourcePack/pic/Combo",10,5,FontUnicode,1.0f,RES_POS);
	Combo.setPosition(250.0f,190.0f);
	Combo.setMenuStr("This input is long",0);
	Combo.setMenuStr("蛋疼!",1);
	Combo.setMenuStr("蛋巨疼!",2);
	Combo.setMenuStr("蛋碎了!",3);
	Combo.setMenuStr("你This a t中文显示!",4);
	//Combo.setMenuStr("蛋爆了!",4);
	//ComboCopy.setACopy(Combo);
//	ComboCopy.initWithoutTex(_XRect(8,1,217,43),_XRect(6,1,48,43),_XRect(8,1,261,23),_XRect(8,1,261,39),_XRect(8,1,261,23),
//		10,5,FontUnicode,1.0f);
	ComboCopy.initWithoutTex(150,10,5,FontUnicode,1.0f);
	ComboCopy.setPosition(250.0f,160.0f);
	ComboCopy.setSize(_XVector2(0.5,0.5));

	//tmpSldEx.initWithoutTex(_XRect(0.0f,32.0f,256.0f,64.0f),1000.0f,0.0f,SLIDER_TYPE_HORIZONTAL,_XVector2(0.0f,16.0f));
	tmpSldEx.initWithoutTex(_XRect(0.0f,0.0f,32.0f,256.0f),1000.0f,0.0f,SLIDER_TYPE_VERTICAL,_XVector2(48.0f,0.0f));
	tmpSldEx.setFontEx("测试:%.2f",XEE::systemFont,1.0f);

	printf("Now ctrl sum:%d\n",_XControlManager::GetInstance().getObjSum());
#if WITH_OBJECT_MANAGER
	printf("Now obj sum:%d\n",_XObjectManager::GetInstance().getNowObjectSum());
#endif
	_XResourcePack::GetInstance().release();
	return true;
}
void _GGame::move(int stepTime)
{
}
void _GGame::draw()
{
	drawLine(638,360,642,360);
	drawLine(640,358,640,362);
}
void _GGame::input(const _XInputEvent &input)
{
	switch(input.type)
	{
	case EVENT_KEYBOARD:
		if(input.keyState == KEY_STATE_DOWN)
		{
			if(input.unicode != 0)
			{
				char temp[4] = {0};
				memcpy(temp,&input.unicode,2);
				if(temp[0] == 0)
				{
					temp[0] = temp[1];
					temp[1] = '\0';
				}
				printf("%s\n",temp);
			}
			switch(input.keyValue)
			{
			case XKEY_SPACE:
			//	SDL_GetClipboardText();
				printf("%s\n",getTextFromClipboard().c_str());
				break;
			case XKEY_R:
				setTextToClipboard("hello world!");
				break;
			}
		}
		break;
	}
}