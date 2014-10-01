#ifndef _JIA_XCONTROLS_
#define _JIA_XCONTROLS_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//使用这些控件，必须遵守下面的默认约束
//1、控件必须要有自己的图片资源，这些资源必须要存在否则控件将会报告错误而不能操作
//2、控件的尺寸和控件的贴图需要自己进行匹配，程序不会动态的匹配这两个参数，因为如下考虑：
//	(1)、过分的缩放贴图将会造成非常严重的美观问题
//	(2)、缩放贴图适应控件的区域将会导致不必要的计算量的浪费
//----------------------------------------------------------
#include "XButton.h"		//按钮控件
#include "XButtonEx.h"		//异形按钮
#include "XSlider.h"		//滑动条
#include "XProgress.h"		//进度条
#include "XCheck.h"			//单选框
#include "XRadios.h"		//复选框
#include "XMouseRightButtonMenu.h"	//右键菜单
#include "XEdit.h"			//当行输入框
#include "XMultiText.h"		//多行文本框
#include "XMultiEdit.h"		//多行输入框(尚未实现)
#include "XMultiList.h"		//多列列表框
#include "XMultiListBasic.h"	//带选择的多列列表框
#include "XCombo.h"			//下拉菜单
#include "XPasswordPad.h"	//密码输入板
#include "XDirectoryList.h"	//路径列表
#include "XGroup.h"			//群组控件
#include "XPointCtrl.h"		//点控件
#include "XLineCtrl.h"		//线控件
#include "XSliderEx.h"		//可微调的滑动条
#include "XImageList.h"		//图片列表
#include "XChart.h"			//数据显示控件
#include "XTab.h"			//分页控件
#include "XSubWindow.h"		//子窗口控件	//尚未实现	子窗口
#include "XProgressRing.h"	//环形进度条控件
#include "XSimpleLine.h"	//分割线控件
#include "XText.h"			//静态文本控件
#include "XCalendar.h"		//日历控件
#include "XMenu.h"			//菜单的控件	//尚未实现
							//工具栏的控件(需要添加)
#include "XImageCtrl.h"
#include "XComment.h"		//控件说明的类
#include "XButtonBar.h"		//按钮条
#include "XToolBar.h"		//工具条

#endif
