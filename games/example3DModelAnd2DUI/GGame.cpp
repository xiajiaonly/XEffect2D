#include "GGame.h"
using namespace XE;
void GGame::ctrlEventProc(int id,int eventID)
{
	if(eventID != XButton::BTN_MOUSE_DOWN) return;
	if(id == m_updateBtn.getControlID())
	{//更新按钮
		updateData();
	}
}
bool GGame::init()
{
	int h = 10.0f;
	m_filenameTxt.init("文件路径:");
	m_filenameTxt.setPosition(10.0f,h);
	m_filenameEdt.initWithoutSkin(256.0f,"ResourcePack/pic/Frame/Caust_000.png");
	m_filenameEdt.setPosition(142.0f,h);
	h += 35.0f;
	m_radiusTxt.init("模糊半径:");
	m_radiusTxt.setPosition(10.0f,h);
	m_radiusEdt.initWithoutSkin(128.0f,"10");
	m_radiusEdt.setPosition(142.0f,h);
	h += 35.0f;
	m_updateBtn.initWithoutSkin("更新",128.0f);
	m_updateBtn.setPosition(10.0f,h);

	m_srcPic.createTexture(128,128,COLOR_RGB);
	m_dstPic.createTexture(128,128,COLOR_RGB);
	m_srcSprite.init(m_srcPic);
	m_dstSprite.init(m_dstPic);
	h += 35.0f;
	m_srcSprite.setPosition(10.0f,h);
	m_dstSprite.setPosition(142.0f,h);
	//下面分配内存空间
	m_srcData = XMem::createArrayMem<unsigned char>(128 * 128 * 3);
	m_dstData = XMem::createArrayMem<unsigned char>(128 * 128 * 3);
	m_mapData = XMem::createArrayMem<float>(128 * 128);
	m_tmpTerrain.init(m_mapData,NULL,128,128);

	m_backSprite.init("ResourcePack/pic/Down.png");
	h += 132.0f;
	m_backSprite.setPosition(10.0f,h);
	m_tmpShader.init("ResourcePack/LightShader.vrt","ResourcePack/LightShader.frg");
	m_tmpShader.connectTexture("Texture_00",NULL);
	m_tmpShader.connectTexture("Texture_01",&m_dstPic.texture.m_texture);
	m_backSprite.setShaderClass(&m_tmpShader);
	return true;
}
void GGame::updateData()
{
	XCurPixel tmpPix;
	if(!m_radiusEdt.getEdtIsNumber()) return;	//参数不合法
	int r = m_radiusEdt.getAsInt();
	if(r <= 0 || r >= 128) return;
	if(!tmpPix.load(m_filenameEdt.getString())) return;
	memcpy(m_srcData,tmpPix.getPixel(),128 * 128 * 3);
	m_srcPic.updateTexture(m_srcData);
	//下面进行均值模糊
	//float tmpData = 0.0f;
	//for(int w = 0;w < 128;++ w)
	//{
	//	for(int h = 0;h < 128;++ h)
	//	{
	//		tmpData = 0.0f;
	//		for(int ww = -r;ww <= r;++ ww)
	//		{
	//			for(int hh = -r;hh <= r;++ hh)
	//			{
	//				int x = (w + ww);
	//				int y = (h + hh);
	//				if(x < 0) x += 128;
	//				if(x >= 128) x -= 128;
	//				if(y < 0) y += 128;
	//				if(y >= 128) y -= 128;
	//				int offset = x + y * 128;
	//				tmpData += m_srcData[offset * 3];
	//			}
	//		}
	//		tmpData = tmpData / ((2 * r + 1) * (2 * r + 1));
	//		m_dstData[(w + h * 128) * 3] = tmpData;
	//		m_dstData[(w + h * 128) * 3 + 1] = tmpData;
	//		m_dstData[(w + h * 128) * 3 + 2] = tmpData;
	//	}
	//}
	//下面考虑进行高斯模糊
	double sigma = (double)r / 3.0;  
    double sigma2 = 2.0 * sigma * sigma;  
    double sigmap = sigma2 * PI;  
	
	static int oldR = 0;
	static float *tmpRate = NULL;
	if(oldR != r)
	{
		oldR = r;
		XMem::XDELETE_ARRAY(tmpRate);
		tmpRate = XMem::createArrayMem<float>((2 * r + 1) * (2 * r + 1));
		for(int ww = -r;ww <= r;++ ww)
		{
			for(int hh = -r;hh <= r;++ hh)
			{//原始算法
				tmpRate[r + ww + (r + hh) * (2 * r + 1)] = exp(-(double)(ww * ww + hh * hh) / sigma2) / sigmap; 
			}
		}
	}

	float tmpData = 0.0f;
	for(int w = 0;w < 128;++ w)
	{
		for(int h = 0;h < 128;++ h)
		{
			tmpData = 0.0f;
			for(int ww = -r;ww <= r;++ ww)
			{
				for(int hh = -r;hh <= r;++ hh)
				{
					int x = (w + ww);
					int y = (h + hh);
					if(x < 0) x += 128;
					if(x >= 128) x -= 128;
					if(y < 0) y += 128;
					if(y >= 128) y -= 128;
					int offset = x + y * 128;
					//原始算法
					tmpData += m_srcData[offset * 3] * tmpRate[r + ww + (r + hh) * (2 * r + 1)]; 
					//tmpData += m_srcData[offset * 3] * exp(-(double)(ww * ww + hh * hh) / sigma2) / sigmap; 
				}
			}
			m_dstData[(w + h * 128) * 3] = tmpData;
			m_dstData[(w + h * 128) * 3 + 1] = tmpData;
			m_dstData[(w + h * 128) * 3 + 2] = tmpData;
			m_mapData[w + h * 128] = tmpData;
		}
	}
	m_dstPic.updateTexture(m_dstData);
	m_tmpTerrain.updateHeight(m_mapData);
}
void GGame::move(float stepTime)
{
	if(!m_isPlay) return;
	m_nowTimer += stepTime;
	int nowFrame = (m_nowTimer / 100) % 32;
	static int oldData = 0;
	if(oldData != nowFrame)
	{
		oldData = nowFrame;
		std::string nowFilename = m_filenameEdt.getString();
		//下面更新数据
		int len = strlen(nowFilename.c_str());
		nowFilename[len - 6] = ((nowFrame / 10) % 10) + '0';
		nowFilename[len - 5] = (nowFrame% 10) + '0';
		m_filenameEdt.setString(nowFilename.c_str());
		updateData();
	}
}
void GGame::draw()
{
	m_srcSprite.draw();
	m_dstSprite.draw();
	m_backSprite.draw();
}
void GGame::draw3D(XBool withTex)
{
	XRender::drawOrigin();
	m_tmpTerrain.draw();
}
void GGame::input(const XInputEvent &event)
{
	if(event.type == EVENT_KEYBOARD
		&& event.keyState == KEY_STATE_DOWN)
	{
		switch(event.keyValue)
		{
		case XKEY_0: m_isPlay = !m_isPlay;break;
		}
	}
}
void GGame::release()
{
	XMem::XDELETE_ARRAY(m_srcData);
	XMem::XDELETE_ARRAY(m_dstData);
	XMem::XDELETE_ARRAY(m_mapData);
}