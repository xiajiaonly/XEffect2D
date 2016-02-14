INLINE  void XWindowTitle::draw()
{
	//drawFillBox(XVector2(0.0f,-MIN_FONT_CTRL_SIZE),XVector2(XEE::windowData.sceneW - 129.0f,MIN_FONT_CTRL_SIZE),0.5f,0.5f,0.5f);
	XRender::drawFillBox(XVector2(0.0f,-MIN_FONT_CTRL_SIZE),XFColor(0.35f,0.35f,0.35f,1.0f),
		XVector2(0.0f + XEG.m_windowData.sceneW - MIN_FONT_CTRL_SIZE * 3.0f, -MIN_FONT_CTRL_SIZE), XFColor(0.55f, 0.55f, 0.55f, 1.0f),
		XVector2(0.0f + XEG.m_windowData.sceneW - MIN_FONT_CTRL_SIZE * 3.0f, 0.0f), XFColor(0.55f, 0.55f, 0.55f, 1.0f),
		XVector2(0.0f,0.0f),XFColor(0.35f,0.35f,0.35f,1.0f));
	m_titleFont.draw();
}
INLINE void XWindowTitle::setCallbackFun(void (*winExitFun)(void *),void (*winMinFun)(void *),void * pClass)
{
	m_winExitFun = winExitFun;
	m_winMinFun = winMinFun;
	m_pClass = pClass;
}
INLINE void XWindowTitle::setWithText(bool b)
{
	if(b) m_titleFont.setVisible();
	else m_titleFont.disVisible();
}