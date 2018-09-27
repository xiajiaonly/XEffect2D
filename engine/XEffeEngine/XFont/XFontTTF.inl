INLINE int XFontTTF::releaseTTFFile(int order)	//释放某个字体库文件
{
	if(order < 0 || order >= MAX_TTF_FONT_FILE_SUM ||
		!m_fontInfo[order].isEnable) return 0;

	TTF_CloseFont(m_fontInfo[order].font);
	m_fontInfo[order].isEnable = XFalse;
	m_fontInfo[order].font = NULL;
	-- m_curLoadTTFFontFileSum;
	return 1;
}
INLINE void XFontTTF::setAttitude(int fontOrder,XTTFRenderType type,SDL_Color color)	//设置字体的渲染参数
{//这里调用会造成逻辑上的不完整
	if(fontOrder >= 0 && fontOrder < MAX_TTF_FONT_FILE_SUM && m_fontInfo[fontOrder].isEnable) 
	{
		m_fontInfo[fontOrder].setType(type);
		m_fontInfo[fontOrder].setColor(color);
	}
//	m_renderType = type;
//	m_forecol = color;
}
INLINE void XFontTTF::setOutline(int fontOrder,int outLine)
{
	if(fontOrder >= 0 && fontOrder < MAX_TTF_FONT_FILE_SUM && 
		m_fontInfo[fontOrder].isEnable) 
	{
		TTF_SetFontOutline(m_fontInfo[fontOrder].font,outLine);
	}
}
INLINE void XFontTTF::setStyle(int fontOrder,int style)
{
	if(fontOrder >= 0 && fontOrder < MAX_TTF_FONT_FILE_SUM && 
		m_fontInfo[fontOrder].isEnable) 
	{
		TTF_SetFontStyle(m_fontInfo[fontOrder].font,style);
	}
}
INLINE void XFontTTF::release()	//释放资源
{
	for(int i = 0;i < MAX_TTF_FONT_FILE_SUM;++ i)
	{
		if(!m_fontInfo[i].isEnable) continue;
		TTF_CloseFont(m_fontInfo[i].font);
		m_fontInfo[i].isEnable = XFalse;
		m_fontInfo[i].font = NULL;
	}
	m_curLoadTTFFontFileSum = 0;
}