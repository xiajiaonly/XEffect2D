#pragma once
#include "XEffeEngine.h"

class GFaceToolsUI
{
public:
	GFaceToolsUI(){}
	~GFaceToolsUI(){}
	bool createUI();
	bool disableAll();
	bool enableAll();
	bool visibleAll();
	bool hideAll();
	XE::XCheck m_isShowFeatureDataChk;
	XE::XCombo m_dataResourcesTypeCmb;
	XE::XButton m_startOrStopBtn;
	XE::XButton m_chooseFileBtn;
	XE::XText m_curFilenameTxt;
	XE::XText m_camIndexTxt;
	XE::XEdit m_camIndexEdt;
	XE::XText m_camWidthTxt;
	XE::XEdit m_camWidthEdt;
	XE::XText m_camHeightTxt;
	XE::XEdit m_camHeightEdt;
	XE::XCheck m_isShowCurDataChk;
	XE::XMultiText m_basicInfoMTxt;
	XE::XMultiText m_auInfoMTxt;
	XE::XMultiText m_suInfoMTxt;
	XE::XText m_oscSenderIPTxt;
	XE::XEdit m_oscSenderIPEdt;//OSC通讯的IP地址
	XE::XText m_oscSenderPortTxt;
	XE::XEdit m_oscSenderPortEdt;
	XE::XText m_oscRecvPortTxt;
	XE::XEdit m_oscRecvPortEdt;
};
