#include "XStdHead.h"
#include "XPixelCore.h"
namespace XE{
bool XPixelCore::setColorMode(XColorMode mode)	//进行像素格式转换
{//这个函数尚未经过严格的测试
	if(!m_isInited) return false;
	switch (m_colorMode)
	{
	case COLOR_RGBA:
		switch (mode)
		{
		case COLOR_RGBA: return true;
		case COLOR_RGB:
			{
				int pixleSum = w * h;
				unsigned char *tmp = m_pPixels;
				m_pPixels = XMem::createArrayMem<unsigned char>(pixleSum * 3);
				if(m_pPixels == NULL)
				{
					m_pPixels = tmp;
					return false;
				}
				for(int i = 0,index = 0;i < pixleSum * 4;i += 4,index += 3)
				{
					m_pPixels[index] = tmp[i];
					m_pPixels[index + 1] = tmp[i + 1];
					m_pPixels[index + 2] = tmp[i + 2];
					//memcpy(m_pPixels + index,tmp + i,3);
				}
				XMem::XDELETE_ARRAY(tmp);
			}
			break;
		case COLOR_GRAY:
			{
				int pixleSum = w * h;
				unsigned char *tmp = m_pPixels;
				m_pPixels = XMem::createArrayMem<unsigned char>(pixleSum);
				if(m_pPixels == NULL)
				{
					m_pPixels = tmp;
					return false;
				}
				memset(m_pPixels,0,pixleSum);	//全部设置为黑色
				for(int i = 0,index = 0;i < pixleSum ;++ i,index += 4)
				{
					//这里暂时不考虑alpha混合的情况，较好的方式应该是原图与黑色图进行alpha混合之后在最终转成灰度图
					if(tmp[index + 3] > 0)
					{
						m_pPixels[i] = (tmp[index] + tmp[index + 1] + tmp[index + 2]) / 3;
					}
				}
				XMem::XDELETE_ARRAY(tmp);
			}
			break;
		case COLOR_RGBA_F:return false;	//暂时不支持
		case COLOR_RGB_F:return false;	//暂时不支持
		case COLOR_GRAY_F:return false;	//暂时不支持
		case COLOR_BGRA:
			{
				int pixleSum = w * h;
				//unsigned char tmp;
				for(int i = 0,index = 0;i < pixleSum ;++ i,index += 4)
				{
					//tmp = m_pPixels[index];
					//m_pPixels[index] = m_pPixels[index + 2];
					//m_pPixels[index + 2] = tmp;
					XMath::swapTwoSum(m_pPixels[index],m_pPixels[index + 2]);
				}
			}
			break;
		case COLOR_BGR:
			{
				int pixleSum = w * h;
				unsigned char *tmp = m_pPixels;
				m_pPixels = XMem::createArrayMem<unsigned char>(pixleSum * 3);
				if(m_pPixels == NULL)
				{
					m_pPixels = tmp;
					return false;
				}
				for(int i = 0,index = 0;i < pixleSum * 4;i += 4,index += 3)
				{
					m_pPixels[index] = tmp[i + 2];
					m_pPixels[index + 1] = tmp[i + 1];
					m_pPixels[index + 2] = tmp[i];
					//memcpy(m_pPixels + index,tmp + i,3);
				}
				XMem::XDELETE_ARRAY(tmp);
			}
			break;
		case COLOR_DEPTH:return false;
		case COLOR_RED:return false;	//暂时不支持
		default:return false;
		}
		break;
	case COLOR_RGB:
		switch (mode)
		{
		case COLOR_RGBA:
			{
				int pixleSum = w * h;
				unsigned char *tmp = m_pPixels;
				m_pPixels = XMem::createArrayMem<unsigned char>(pixleSum * 4);
				if(m_pPixels == NULL)
				{
					m_pPixels = tmp;
					return false;
				}
				memset(m_pPixels,255,pixleSum * 4);
				for(int i = 0,index = 0;i < pixleSum * 3;i += 3,index += 4)
				{
					m_pPixels[index] = tmp[i];
					m_pPixels[index + 1] = tmp[i + 1];
					m_pPixels[index + 2] = tmp[i + 2];
					//memcpy(m_pPixels + index,tmp + i,3);
				}
				XMem::XDELETE_ARRAY(tmp);
			}
			break;
		case COLOR_RGB:return true;
		case COLOR_GRAY:
			{
				int pixleSum = w * h;
				unsigned char *tmp = m_pPixels;
				m_pPixels = XMem::createArrayMem<unsigned char>(pixleSum);
				if(m_pPixels == NULL)
				{
					m_pPixels = tmp;
					return false;
				}
				for(int i = 0,index = 0;i < pixleSum ;++ i,index += 3)
				{
					m_pPixels[i] = (tmp[index] + tmp[index + 1] + tmp[index + 2]) / 3;
				}
				XMem::XDELETE_ARRAY(tmp);
			}
			break;
		case COLOR_RGBA_F:return false;	//暂时不支持
		case COLOR_RGB_F:return false;	//暂时不支持
		case COLOR_GRAY_F:return false;	//暂时不支持
		case COLOR_BGRA:
			{
				int pixleSum = w * h;
				unsigned char *tmp = m_pPixels;
				m_pPixels = XMem::createArrayMem<unsigned char>(pixleSum * 4);
				if(m_pPixels == NULL)
				{
					m_pPixels = tmp;
					return false;
				}
				memset(m_pPixels,255,pixleSum * 4);
				for(int i = 0,index = 0;i < pixleSum * 3;i += 3,index += 4)
				{
					m_pPixels[index] = tmp[i + 2];
					m_pPixels[index + 1] = tmp[i + 1];
					m_pPixels[index + 2] = tmp[i];
					//memcpy(m_pPixels + index,tmp + i,3);
				}
				XMem::XDELETE_ARRAY(tmp);
			}
			break;
		case COLOR_BGR:
			{
				int pixleSum = w * h;
				//unsigned char tmp;
				for(int i = 0,index = 0;i < pixleSum ;++ i,index += 3)
				{
					//tmp = m_pPixels[index];
					//m_pPixels[index] = m_pPixels[index + 2];
					//m_pPixels[index + 2] = tmp;
					XMath::swapTwoSum(m_pPixels[index],m_pPixels[index + 2]);
				}
			}
			break;
		case COLOR_DEPTH:return false;
		case COLOR_RED:return false;
		default:return false;
		}
		break;
	case COLOR_GRAY:
		switch (mode)
		{
		case COLOR_RGBA:
		case COLOR_BGRA:
			{
				int pixleSum = w * h;
				unsigned char *tmp = m_pPixels;
				m_pPixels = XMem::createArrayMem<unsigned char>(pixleSum * 4);
				if(m_pPixels == NULL)
				{
					m_pPixels = tmp;
					return false;
				}
				memset(m_pPixels,255,pixleSum * 4);
				for(int i = 0,index = 0;i < pixleSum;++ i,index += 4)
				{
					m_pPixels[index] = tmp[i];
					m_pPixels[index + 1] = tmp[i];
					m_pPixels[index + 2] = tmp[i];
				}
				XMem::XDELETE_ARRAY(tmp);
			}
			break;
		case COLOR_RGB:
		case COLOR_BGR:
			{
				int pixleSum = w * h;
				unsigned char *tmp = m_pPixels;
				m_pPixels = XMem::createArrayMem<unsigned char>(pixleSum * 3);
				if(m_pPixels == NULL)
				{
					m_pPixels = tmp;
					return false;
				}
				for(int i = 0,index = 0;i < pixleSum;++ i,index += 3)
				{
					m_pPixels[index] = tmp[i];
					m_pPixels[index + 1] = tmp[i];
					m_pPixels[index + 2] = tmp[i];
				}
				XMem::XDELETE_ARRAY(tmp);
			}
			break;
		case COLOR_GRAY: return true;
		case COLOR_RGBA_F:return false;
		case COLOR_RGB_F:return false;
		case COLOR_GRAY_F:return false;
		case COLOR_DEPTH:return false;
		case COLOR_RED:return false;
		default:return false;
		}
		break;
	case COLOR_RGBA_F:return false;
	case COLOR_RGB_F:return false;
	case COLOR_GRAY_F:return false;
	case COLOR_BGRA:
		switch (mode)
		{
		case COLOR_RGBA:
			{
				int pixleSum = w * h;
				//unsigned char tmp;
				for(int i = 0,index = 0;i < pixleSum ;++ i,index += 4)
				{
					//tmp = m_pPixels[index];
					//m_pPixels[index] = m_pPixels[index + 2];
					//m_pPixels[index + 2] = tmp;
					XMath::swapTwoSum(m_pPixels[index],m_pPixels[index + 2]);
				}
			}
			break;
		case COLOR_RGB:
			{
				int pixleSum = w * h;
				unsigned char *tmp = m_pPixels;
				m_pPixels = XMem::createArrayMem<unsigned char>(pixleSum * 3);
				if(m_pPixels == NULL)
				{
					m_pPixels = tmp;
					return false;
				}
				for(int i = 0,index = 0;i < pixleSum * 4;i += 4,index += 3)
				{
					m_pPixels[index] = tmp[i + 2];
					m_pPixels[index + 1] = tmp[i + 1];
					m_pPixels[index + 2] = tmp[i];
					//memcpy(m_pPixels + index,tmp + i,3);
				}
				XMem::XDELETE_ARRAY(tmp);
			}
			break;
		case COLOR_GRAY:
			{
				int pixleSum = w * h;
				unsigned char *tmp = m_pPixels;
				m_pPixels = XMem::createArrayMem<unsigned char>(pixleSum);
				if(m_pPixels == NULL)
				{
					m_pPixels = tmp;
					return false;
				}
				memset(m_pPixels,0,pixleSum);	//全部设置为黑色
				for(int i = 0,index = 0;i < pixleSum ;++ i,index += 4)
				{
					//这里暂时不考虑alpha混合的情况，较好的方式应该是原图与黑色图进行alpha混合之后在最终转成灰度图
					if(tmp[index + 3] > 0)
					{
						m_pPixels[i] = (tmp[index] + tmp[index + 1] + tmp[index + 2]) / 3;
					}
				}
				XMem::XDELETE_ARRAY(tmp);
			}
			break;
		case COLOR_RGBA_F:return false;
		case COLOR_RGB_F:return false;
		case COLOR_GRAY_F:return false;
		case COLOR_BGRA: return true;
		case COLOR_BGR:
			{
				int pixleSum = w * h;
				unsigned char *tmp = m_pPixels;
				m_pPixels = XMem::createArrayMem<unsigned char>(pixleSum * 3);
				if(m_pPixels == NULL)
				{
					m_pPixels = tmp;
					return false;
				}
				for(int i = 0,index = 0;i < pixleSum * 4;i += 4,index += 3)
				{
					m_pPixels[index] = tmp[i];
					m_pPixels[index + 1] = tmp[i + 1];
					m_pPixels[index + 2] = tmp[i + 2];
					//memcpy(m_pPixels + index,tmp + i,3);
				}
				XMem::XDELETE_ARRAY(tmp);
			}
			break;
		case COLOR_DEPTH:return false;
		case COLOR_RED:return false;
		default:return false;
		}
		break;
	case COLOR_BGR:
		switch (mode)
		{
		case COLOR_RGBA:
			{
				int pixleSum = w * h;
				unsigned char *tmp = m_pPixels;
				m_pPixels = XMem::createArrayMem<unsigned char>(pixleSum * 4);
				if(m_pPixels == NULL)
				{
					m_pPixels = tmp;
					return false;
				}
				memset(m_pPixels,255,pixleSum * 4);
				for(int i = 0,index = 0;i < pixleSum * 3;i += 3,index += 4)
				{
					m_pPixels[index] = tmp[i + 2];
					m_pPixels[index + 1] = tmp[i + 1];
					m_pPixels[index + 2] = tmp[i];
					//memcpy(m_pPixels + index,tmp + i,3);
				}
				XMem::XDELETE_ARRAY(tmp);
			}
			break;
		case COLOR_RGB:
			{
				int pixleSum = w * h;
				//unsigned char tmp;
				for(int i = 0,index = 0;i < pixleSum ;++ i,index += 3)
				{
					//tmp = m_pPixels[index];
					//m_pPixels[index] = m_pPixels[index + 2];
					//m_pPixels[index + 2] = tmp;
					XMath::swapTwoSum(m_pPixels[index],m_pPixels[index + 2]);
				}
			}
			break;
		case COLOR_GRAY:
			{
				int pixleSum = w * h;
				unsigned char *tmp = m_pPixels;
				m_pPixels = XMem::createArrayMem<unsigned char>(pixleSum);
				if(m_pPixels == NULL)
				{
					m_pPixels = tmp;
					return false;
				}
				for(int i = 0,index = 0;i < pixleSum ;++ i,index += 3)
				{
					m_pPixels[i] = (tmp[index] + tmp[index + 1] + tmp[index + 2]) / 3;
				}
				XMem::XDELETE_ARRAY(tmp);
			}
			break;
		case COLOR_RGBA_F:return false;
		case COLOR_RGB_F:return false;
		case COLOR_GRAY_F:return false;
		case COLOR_BGRA:
			{
				int pixleSum = w * h;
				unsigned char *tmp = m_pPixels;
				m_pPixels = XMem::createArrayMem<unsigned char>(pixleSum * 4);
				if(m_pPixels == NULL)
				{
					m_pPixels = tmp;
					return false;
				}
				memset(m_pPixels,255,pixleSum * 4);
				for(int i = 0,index = 0;i < pixleSum * 3;i += 3,index += 4)
				{
					m_pPixels[index] = tmp[i];
					m_pPixels[index + 1] = tmp[i + 1];
					m_pPixels[index + 2] = tmp[i + 2];
					//memcpy(m_pPixels + index,tmp + i,3);
				}
				XMem::XDELETE_ARRAY(tmp);
			}
			break;
		case COLOR_BGR: return true;
		case COLOR_DEPTH:return false;
		case COLOR_RED:return false;
		default:return false;
		}
		break;
	case COLOR_DEPTH:return false;
	case COLOR_RED:return false;
	default:return false;
	}
	m_colorMode = mode;
	return true;
}
}