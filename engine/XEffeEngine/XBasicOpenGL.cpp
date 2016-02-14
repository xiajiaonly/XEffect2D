#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
//#include "stdio.h"
//#include "math.h"

#include "XBasicOpenGL.h"
//#include "XBasicFun.h"
#include "XPixelCommon.h"

//#include "XResourcePack.h"
//#include "XLogBook.h"
#include "XShaderGLSL.h"
#include "XResourceManager.h"
#include "XFile.h"
//#include "XMath\XVector4.h"

namespace XE{
#if TEX_DEBUG
int XTSum = 0;
#endif
#if TEX_DEBUG1
int CPSum = 0;
#endif
namespace XGL
{
	const int srcBlendMode[9] = {GL_ZERO,GL_ONE,GL_DST_COLOR,GL_ONE_MINUS_DST_COLOR,GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA,
			GL_DST_ALPHA,GL_ONE_MINUS_DST_ALPHA,GL_SRC_ALPHA_SATURATE};
	const int dstBlendMode[8] = {GL_ZERO,GL_ONE,GL_SRC_COLOR,GL_ONE_MINUS_SRC_COLOR,GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA,	
			GL_DST_ALPHA,GL_ONE_MINUS_DST_ALPHA};
//	bool blendFlag = false;
//	int blendSrc = GL_ZERO;
//	int blendDst = GL_ONE;
//	bool texture2DFlag = false;
//	unsigned int curTextureID[32];

	void printShaderInfoLog(GLuint obj)
	{
		int infologLength = 0;
		glGetShaderiv(obj, GL_INFO_LOG_LENGTH,&infologLength);
		if(infologLength <= 0) return;
		int charsWritten  = 0;
		char *infoLog = XMem::createArrayMem<char>(infologLength);
		if(infoLog == NULL) return;
		glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
		if(infoLog[0] != '\0') LogNull("%s",infoLog);
		XMem::XDELETE_ARRAY(infoLog);
	}
	void printProgramInfoLog(GLuint obj)
	{
		int infologLength = 0;
		glGetProgramiv(obj, GL_INFO_LOG_LENGTH,&infologLength);
		if(infologLength <= 0) return;

		int charsWritten  = 0;
		char *infoLog = XMem::createArrayMem<char>(infologLength);
		if(infoLog == NULL) return;
		glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
		if(infoLog[0] != '\0') LogNull("%s",infoLog);
		XMem::XDELETE_ARRAY(infoLog);
	}
	int printOglError(const char *file,int line)
	{
		int retCode = 0;
		while(glGetError() != GL_NO_ERROR)
		{
		//	LogNull("glError in file %s @ line %d: %s\n", file, line, gluErrorString(glErr));
 			LogNull("glError in file %s @ line %d",file,line);
			//printf("glError: 0x%x in file %s @ line %d\n",glErr,file,line);
			retCode = 1;
		}
		return retCode;
	}
	XBool setShader(const char* vertFile,const char* fragFile,int &pHandle,XResourcePosition resoursePosition)
	{
		if(vertFile == NULL || fragFile == NULL) return XFalse;
		char *vs = XFile::readFileToStr(vertFile,resoursePosition);
		char *fs = XFile::readFileToStr(fragFile,resoursePosition);
		if(vs == NULL || fs == NULL) return XFalse;
	
		GLuint v = glCreateShader(GL_VERTEX_SHADER);
		GLuint f = glCreateShader(GL_FRAGMENT_SHADER);
		//v = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
		//f = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);

		glShaderSource(v, 1, &vs,NULL);
		glShaderSource(f, 1, &fs,NULL);
		//glShaderSourceARB(v, 1, &vv,NULL);
		//glShaderSourceARB(f, 1, &ff,NULL);

		XMem::XDELETE_ARRAY(vs);
		XMem::XDELETE_ARRAY(fs);

		glCompileShader(v);
		glCompileShader(f);
		//glCompileShaderARB(v);
		//glCompileShaderARB(f);

		printShaderInfoLog(v);
		printShaderInfoLog(f);

		if(pHandle == 0) pHandle = glCreateProgram();	//如果没有建立则这里建立

		glAttachShader(pHandle,v);
		glAttachShader(pHandle,f);
		//pHandle = glCreateProgramObjectARB();
		//glAttachObjectARB(pHandle,v);
		//glAttachObjectARB(pHandle,f);

		glLinkProgram(pHandle);
		//glLinkProgramARB(pHandle);
		printProgramInfoLog(pHandle);
		return XTrue;
	}
	XBool setShaderFromStr(const char* vStr,const char *fStr,const char *gStr,XShaderHandle &pHandle)
	{
		if(vStr == NULL || fStr == NULL) return XFalse;
		if(pHandle.shaderV == 0) pHandle.shaderV = glCreateShader(GL_VERTEX_SHADER);
		if(pHandle.shaderF == 0) pHandle.shaderF = glCreateShader(GL_FRAGMENT_SHADER);
		if(gStr != NULL && pHandle.shaderG == 0)
			pHandle.shaderG = glCreateShader(GL_GEOMETRY_SHADER_EXT);

		glShaderSource(pHandle.shaderV, 1, &vStr,NULL);
		glShaderSource(pHandle.shaderF, 1, &fStr,NULL);
		if(gStr != NULL)
			 glShaderSource(pHandle.shaderG, 1, &gStr,NULL);

		glCompileShader(pHandle.shaderV);
		glCompileShader(pHandle.shaderF);
		if(gStr != NULL) glCompileShader(pHandle.shaderG);

		LogStr("vStr");
		printShaderInfoLog(pHandle.shaderV);
		LogStr("fStr");
		printShaderInfoLog(pHandle.shaderF);
		if(gStr != NULL)
		{
			LogStr("gStr");
			printShaderInfoLog(pHandle.shaderG);
		}

		if(pHandle.shaderHandle == 0) pHandle.shaderHandle = glCreateProgram();	//如果没有建立则这里建立

		glAttachShader(pHandle.shaderHandle,pHandle.shaderV);
		glAttachShader(pHandle.shaderHandle,pHandle.shaderF);
		if(gStr != NULL) glAttachShader(pHandle.shaderHandle,pHandle.shaderG);

		glLinkProgram(pHandle.shaderHandle);
		printProgramInfoLog(pHandle.shaderHandle);
		return XTrue;	
	}
	XBool setShader(const char* vertFile,const char* fragFile,const char* geomFile,XShaderHandle &pHandle,
							XResourcePosition resoursePosition)
	{
		if(vertFile == NULL || fragFile == NULL) return XFalse;
		char *vs = XFile::readFileToStr(vertFile,resoursePosition);
		char *fs = XFile::readFileToStr(fragFile,resoursePosition);
		if(vs == NULL || fs == NULL) return XFalse;
		char *gs = NULL;
		if(geomFile != NULL)
		{
			gs = XFile::readFileToStr(geomFile,resoursePosition);
			if(gs == NULL) return XFalse;
		}
		//1、简化版本
//		XBool ret = setShaderFromStr(vs,fs,gs,pHandle);
//		XMem::XDELETE_ARRAY(vs);
//		XMem::XDELETE_ARRAY(fs);
//		XMem::XDELETE_ARRAY(gs);
//		return ret;
		//2、原始版本
		if(pHandle.shaderV == 0) pHandle.shaderV = glCreateShader(GL_VERTEX_SHADER);
		if(pHandle.shaderF == 0) pHandle.shaderF = glCreateShader(GL_FRAGMENT_SHADER);
		if(geomFile != NULL && pHandle.shaderG == 0)
			pHandle.shaderG = glCreateShader(GL_GEOMETRY_SHADER_EXT);
		//v = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
		//f = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);

		glShaderSource(pHandle.shaderV, 1, &vs,NULL);
		glShaderSource(pHandle.shaderF, 1, &fs,NULL);
		if(geomFile != NULL)
			 glShaderSource(pHandle.shaderG, 1, &gs,NULL);
		//glShaderSourceARB(v, 1, &vv,NULL);
		//glShaderSourceARB(f, 1, &ff,NULL);

		XMem::XDELETE_ARRAY(vs);
		XMem::XDELETE_ARRAY(fs);
		XMem::XDELETE_ARRAY(gs);

		glCompileShader(pHandle.shaderV);
		glCompileShader(pHandle.shaderF);
		if(geomFile != NULL) glCompileShader(pHandle.shaderG);
		//glCompileShaderARB(v);
		//glCompileShaderARB(f);

		LogStr(vertFile);
		printShaderInfoLog(pHandle.shaderV);
		LogStr(fragFile);
		printShaderInfoLog(pHandle.shaderF);
		if(geomFile != NULL)
		{
			LogStr(geomFile);
			printShaderInfoLog(pHandle.shaderG);
		}

		if(pHandle.shaderHandle == 0) pHandle.shaderHandle = glCreateProgram();	//如果没有建立则这里建立

		glAttachShader(pHandle.shaderHandle,pHandle.shaderV);
		glAttachShader(pHandle.shaderHandle,pHandle.shaderF);
		if(geomFile != NULL) glAttachShader(pHandle.shaderHandle,pHandle.shaderG);
		//pHandle = glCreateProgramObjectARB();
		//glAttachObjectARB(pHandle,v);
		//glAttachObjectARB(pHandle,f);

		glLinkProgram(pHandle.shaderHandle);
		//glLinkProgramARB(pHandle);
		printProgramInfoLog(pHandle.shaderHandle);
		return XTrue;
	}
	/*void changeSize(int h, int w)
	{
		// Prevent a divide by zero, when window is too short
		// (you cant make a window of zero width).
		if(h == 0) h = 1;

		float ratio = 1.0f* w / h;

		// Reset the coordinate system before modifying
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		// Set the viewport to be the entire window
		glViewport(0, 0, w, h);

		// Set the correct perspective.
		//gluPerspective(90,ratio,1,1000);
		glMatrixMode(GL_MODELVIEW);
	
	}*/
	//XBool TextureLoad(GLuint &texture,const char *filename,int withAlpha,int *w,int *h,XResourcePosition resoursePosition)
	//{
	//	if(filename == NULL || strlen(filename) <= 0) return XFalse;
	//	if(withAlpha != 0) withAlpha = 1;
	//
	//	SDL_Surface *temp = NULL;
	//	SDL_Surface *image = NULL;
	//
	//	Uint32 rmask, gmask, bmask, amask;
	//
	//#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	//	rmask = 0xff000000; gmask = 0x00ff0000; bmask = 0x0000ff00;	amask = 0x000000ff;
	//#else
	//	rmask = 0x000000ff;	gmask = 0x0000ff00;	bmask = 0x00ff0000;	amask = 0xff000000;
	//#endif
	//	
	//	if((temp = (SDL_Surface *) loadImage(filename,withAlpha,resoursePosition)) != NULL) 
	//	{
	//		if((temp->w > XEE::maxTetureSize) || (temp->h > XEE::maxTetureSize)) 
	//		{
	//			fprintf(stderr, "Image size exceeds max texture size, which is %d pixels for each side\n",XEE::maxTetureSize);
	//			LogNull("Image size exceeds max texture size, which is %d pixels for each side\n",XEE::maxTetureSize);
	//			SDL_FreeSurface(temp);
	//			temp = NULL;
	//			return 0;
	//			//exit(-1);
	//		}
	//		//printf("Texture Max Size:%d x %d\n",max_size,max_size);
	//
	//		if(w != NULL && h != NULL)
	//		{
	//			(* w) = temp->w;
	//			(* h) = temp->h;
	//		}
	//
	//		// Check for NPOT textures 
	//		if (isNPOT(temp->w, temp->h)) //检查图片的尺寸是否符合2的n次方
	//		{//如果不符合2的n次方的图片下面进行特殊的处理(这个处理目前还存在问题,对于不符合2的n次方的透明通道贴图)
	//			GLint wreal = 0, hreal = 0;
	//			GLint xpad = 0, ypad = 0;
	//			
	//			wreal = (int)powf(2.0, ceilf(logf((float)temp->w)/logf(2.0f)));
	//			hreal = (int)powf(2.0, ceilf(logf((float)temp->h)/logf(2.0f)));
	//			xpad = (wreal - temp->w)/2;
	//			ypad = (hreal - temp->h)/2;
	//
	//			if(withAlpha != 0)
	//			{
	//				image = SDL_CreateRGBSurface(SDL_SWSURFACE, wreal, hreal, 32, rmask, gmask, bmask, amask);
	//			}else
	//			{
	//				image = SDL_CreateRGBSurface(SDL_SWSURFACE, wreal, hreal, 24, rmask, gmask, bmask, 0);
	//			}
	//			if(image == NULL)
	//			{
	//				SDL_FreeSurface(temp);
	//				temp = NULL;
	//				return XFalse;
	//			}
	//			SDL_Rect dst = {xpad, ypad, temp->w, temp->h};
	//			//当存在alpha通道时，这里这是不进行混合直接覆盖
	//			SDL_SetAlpha(temp,0,255);
	//			SDL_SetAlpha(image,0,255);
	//			SDL_BlitSurface(temp, 0, image, &dst);
	//			SDL_FreeSurface(temp);
	//			temp = NULL;
	//
	//			// OpenGL Texture creation	
	//			glGenTextures(1, &texture);
	//			XGL::BindTexture2D(texture);
	//			//效果好
	//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//#if WITHXSPRITE_EX
	//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//#endif			//速度快
	//			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//		//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	//		//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	//
	//		//	if(withAlpha != 0)
	//			if(image->format->BitsPerPixel == 32)
	//			{
	//			//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA4, image->w, image->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);
	//				if(image->format->Rshift == 0)
	//					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->w, image->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);
	//				else
	//					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->w, image->h, 0, GL_BGRA, GL_UNSIGNED_BYTE, image->pixels);
	//			}else
	//			if(image->format->BitsPerPixel == 24)
	//			{
	//			//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB4, image->w, image->h, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
	//				if(image->format->Rshift == 0)
	//					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->w, image->h, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
	//				else
	//					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->w, image->h, 0, GL_BGR, GL_UNSIGNED_BYTE, image->pixels);
	//			}
	//
	//			SDL_FreeSurface(image);
	//			image = NULL;
	//		}else
	//		{//符合2的n次方的图片
	//			// OpenGL Texture creation	
	//			glGenTextures(1, &texture);
	//			XGL::BindTexture2D(texture);
	//		//	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	//
	//			//效果好
	//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//#if WITHXSPRITE_EX
	//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//#endif			//速度快
	//			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//		//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	//		//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	//
	//			if(temp->format->BitsPerPixel == 32)
	//			{
	//			//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA4, temp->w, temp->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, temp->pixels);
	//				if(temp->format->Rshift == 0)
	//					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, temp->w, temp->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, temp->pixels);
	//				else
	//					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, temp->w, temp->h, 0, GL_BGRA, GL_UNSIGNED_BYTE, temp->pixels);
	//			}else
	//			if(temp->format->BitsPerPixel == 24)
	//			{
	//			//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB4, temp->w, temp->h, 0, GL_BGR, GL_UNSIGNED_BYTE, temp->pixels);
	//				if(temp->format->Rshift == 0)
	//					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, temp->w, temp->h, 0, GL_RGB, GL_UNSIGNED_BYTE, temp->pixels);
	//				else
	//					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, temp->w, temp->h, 0, GL_BGR, GL_UNSIGNED_BYTE, temp->pixels);
	//			}
	//
	//			SDL_FreeSurface(temp);
	//			temp = NULL;
	//		}
	//		
	////		// OpenGL Texture creation	
	////		glGenTextures(1, &texture);
	////		XGL::BindTexture2D(texture);
	////
	////		//效果好
	////	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	////	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	////#if WITHXSPRITE_EX
	////	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	////	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	////#endif
	////		//速度快
	////		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	////		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	////	//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	////	//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	////
	////		if(withAlpha != 0)
	////		{
	////			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->w, image->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);
	////			//glTexImage2D(GL_TEXTURE_2D, 0, GL_BGRA, image->w, image->h, 0, GL_BGRA, GL_UNSIGNED_BYTE, image->pixels);
	////		}else
	////		{
	////			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->w, image->h, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
	////		}
	////
	////		SDL_FreeSurface(image);
	////		image = NULL;
	//		return XTrue;
	//	}else
	//	{
	//		return XFalse;
	//	}
	////	return XTrue;
	//}
	//测试发现这个函数中的SDL_FreeSurface会存在内存IPR和FIM错误，具体需要SDL的代码实现来检测
	XBool TextureLoadEx(GLuint &texture,const char *filename,int *w,int *h,XResourcePosition resoursePosition)
	{
		if(filename == NULL || strlen(filename) <= 0) return XFalse;

		XPixels<XCurPixel> tmp;
		if(!tmp.load(filename,resoursePosition)) return XFalse;
		if((tmp.getWidth() > XEG.getMaxTextureSize() || tmp.getHeight() > XEG.getMaxTextureSize()) && XEG.getMaxTextureSize() > 0) 	//贴图最大尺寸的检测
		{
			fprintf(stderr, "Image size exceeds max texture size, which is %d pixels for each side\n", XEG.getMaxTextureSize());
			LogNull("Image size exceeds max texture size, which is %d pixels for each side\n", XEG.getMaxTextureSize());
			tmp.release();
			return XFalse;
		}
		if(w != NULL) (* w) = tmp.getWidth();
		if(h != NULL) (* h) = tmp.getHeight();
		tmp.fitNPot();	//适应2的n次方
		// OpenGL Texture creation	
		glGenTextures(1, &texture);
		XGL::BindTexture2D(texture);
	//	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
		//效果好
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	#if WITHXSPRITE_EX
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	#endif
		//速度快
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	//	float largest_supported_anisotropy;
	//	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT,&largest_supported_anisotropy);	//各异向性纹理过滤
	//	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAX_ANISOTROPY_EXT,largest_supported_anisotropy);

		switch(tmp.getColorMode())
		{
		case COLOR_RGBA:
			glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,tmp.getWidth(),tmp.getHeight(),0,GL_RGBA,GL_UNSIGNED_BYTE,tmp.getPixel());
			break;
		case COLOR_BGRA:
			glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,tmp.getWidth(),tmp.getHeight(),0,GL_BGRA,GL_UNSIGNED_BYTE,tmp.getPixel());
			break;
		case COLOR_RGB:
			glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,tmp.getWidth(),tmp.getHeight(),0,GL_RGB,GL_UNSIGNED_BYTE,tmp.getPixel());
			break;
		case COLOR_BGR:
			glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,tmp.getWidth(),tmp.getHeight(),0,GL_BGR,GL_UNSIGNED_BYTE,tmp.getPixel());
			break;
		case COLOR_GRAY:
			glTexImage2D(GL_TEXTURE_2D,0,GL_LUMINANCE,tmp.getWidth(),tmp.getHeight(),0,GL_LUMINANCE,GL_UNSIGNED_BYTE,tmp.getPixel());
			break;
		}
#if WITH_ENGINE_STATISTICS
		XEG.addStaticsticsTexInfo(texture);
#endif

		tmp.release();
		return XTrue;
	}
	XBool TextureLoadQuarter(GLuint &texture,const char *filename)
	{
		XPixels<XCurPixel> tmp;
		if(!tmp.load(filename)) return XFalse;
		int w = tmp.getWidth() >> 2;
		int h = tmp.getHeight() >> 2;
		unsigned char * tempPixel = XMem::createArrayMem<unsigned char>(w * h * 4);
		if(tempPixel == NULL)
		{
			tmp.release();
			return XFalse;
		}
		switch(tmp.getColorMode())
		{
		case COLOR_RGBA:
		case COLOR_BGRA:
			{
				int *p = (int *)tmp.getPixel();
				int *p1 = (int *)tempPixel;
				for(int i = 0; i < tmp.getHeight();i += 4)
					for(int j = 0;j < tmp.getWidth();j += 4)
						*(p1 + (i >> 2) * w + (j >> 2)) = *(p + i * tmp.getWidth() + j);
			}
			break;
		case COLOR_RGB:
		case COLOR_BGR:
			{
				unsigned char *p = tmp.getPixel();
				unsigned char *p1 = tempPixel;		//这里可以进行优化
				for(int j = 0;j < tmp.getHeight();j += 4)
					for(int i = 0; i < tmp.getWidth();i += 4)
					{
						*(p1 + i + j * w + 0) = *(p + (i << 2) + (j << 2) * tmp.getWidth() + 0);
						*(p1 + i + j * w + 1) = *(p + (i << 2) + (j << 2) * tmp.getWidth() + 1);
						*(p1 + i + j * w + 2) = *(p + (i << 2) + (j << 2) * tmp.getWidth() + 2);
						*(p1 + i + j * w + 3) = 255;
					}
			}
			break;
		default:	//其他格式不支持
			tmp.release();
			XMem::XDELETE_ARRAY(tempPixel);
			return XFalse;
		}
		glGenTextures(1, &texture);
		XGL::BindTexture2D(texture);
		//效果好
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	#if WITHXSPRITE_EX
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	#endif
		//速度快
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		switch(tmp.getColorMode())
		{
		case COLOR_RGBA:
			glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,w,h,0,GL_RGBA,GL_UNSIGNED_BYTE,tempPixel);
			break;
		case COLOR_BGRA:
			glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,w,h,0,GL_BGRA,GL_UNSIGNED_BYTE,tempPixel);
			break;
		case COLOR_RGB:
			glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,w,h,0,GL_RGB,GL_UNSIGNED_BYTE,tempPixel);
			break;
		case COLOR_BGR:
			glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,w,h,0,GL_BGR,GL_UNSIGNED_BYTE,tempPixel);
			break;
		default:	//其他格式不支持
			tmp.release();
			XMem::XDELETE_ARRAY(tempPixel);
			return XFalse;
		}
	#if WITH_ENGINE_STATISTICS
		XEG.addStaticsticsTexInfo(texture);
	#endif
		tmp.release();
		XMem::XDELETE_ARRAY(tempPixel);
		return XTrue;
	}
	void showGLInfoToLog()
	{
		GLint major, minor;  
		glGetIntegerv(GL_MAJOR_VERSION, &major);  
		glGetIntegerv(GL_MINOR_VERSION, &minor);  
		LogNull("GL Vendor    :%s",glGetString( GL_VENDOR ));
		LogNull("GL Renderer  :%s",glGetString( GL_RENDERER ));
		LogNull("GL Version (string)  :%s",glGetString( GL_VERSION ));
		LogNull("GL Version (integer) :%d.%d",major,minor);
		LogNull("GLSL Version :%s",glGetString( GL_SHADING_LANGUAGE_VERSION ));
	}
}
namespace XRender
{
	void drawLineAntiColor(float ax,float ay,float bx,float by,float width,XLineStyle type)
	{
		XGL::EnableBlend();
		XGL::SetBlendFunc(GL_ONE_MINUS_DST_COLOR,GL_ZERO);	//反色效果
		glColor4fv(XFColor::white);
		glLineWidth(width);
		XGL::DisableTexture2D();

		if (type == LS_SOLID)
		{//实线
			glBegin(GL_LINES);
			glVertex2f(ax,ay);
			glVertex2f(bx,by);
			glEnd();
		}else
		{//虚线
			glEnable(GL_LINE_STIPPLE);
			glLineStipple(1, type);
			glBegin(GL_LINES);
			glVertex2f(ax,ay);
			glVertex2f(bx,by);
			glEnd();
			glDisable(GL_LINE_STIPPLE);
		}
		XGL::DisableBlend();
	}
	void drawLine(float ax,float ay,float bx,float by,float width,float r,float g,float b)
	{
		glColor3f(r,g,b);
		glLineWidth(width);
		XGL::DisableTexture2D();
		glBegin(GL_LINES);
		glVertex2f(ax,ay);
		glVertex2f(bx,by);
		glEnd();
	}
	void drawLine(float ax, float ay, float bx, float by, float width, const XFColor &c, XLineStyle type)
	{
		XGL::EnableBlend();
		XGL::SetBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glColor4fv(c);
		glLineWidth(width);
		XGL::DisableTexture2D();
		if (type == LS_SOLID)
		{//实线
			glBegin(GL_LINES);
			glVertex2f(ax,ay);
			glVertex2f(bx,by);
			glEnd();
		}else
		{//虚线
			glEnable(GL_LINE_STIPPLE);
			glLineStipple(1, type);
			glBegin(GL_LINES);
			glVertex2f(ax,ay);
			glVertex2f(bx,by);
			glEnd();
			glDisable(GL_LINE_STIPPLE);
		}
		XGL::DisableBlend();
	} 
	void drawLinesVbo(unsigned int v,int pointSum,float w,float r,float g,float b)
	{
		glLineWidth(w);
		glColor3f(r,g,b);
		XGL::DisableTexture2D();
		glEnableClientState(GL_VERTEX_ARRAY);	
		glBindBufferARB(GL_ARRAY_BUFFER_ARB,v);
		glVertexPointer(2,GL_FLOAT,0,NULL);
		glDrawArrays(GL_LINE_STRIP,0,pointSum);
	
		glDisableClientState(GL_VERTEX_ARRAY);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB,0);
	}
	void drawLinesVbo(unsigned int v,int pointSum,float w,float r,float g,float b,float a)
	{
		XGL::EnableBlend();
		XGL::SetBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glLineWidth(w);
		glColor4f(r,g,b,a);
		XGL::DisableTexture2D();
		glEnableClientState(GL_VERTEX_ARRAY);	
		glBindBufferARB(GL_ARRAY_BUFFER_ARB,v);
		glVertexPointer(2,GL_FLOAT,0,NULL);
		glDrawArrays(GL_LINE_STRIP,0,pointSum);
		XGL::DisableBlend();
		glDisableClientState(GL_VERTEX_ARRAY);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB,0);
		//if(isMultiSampleSupport && isLineSmooth) XGL::DisableBlend();
	}
	//void drawLine(const XVector2& p,float angle,float length)
	//{
	//	drawLine(p,XVector2(p.x + length * cos(angle * DEGREE2RADIAN),p.y + length * sin(angle * DEGREE2RADIAN)));
	//}
	void drawRect(const XRect& rect, float width, const XFColor &c, XLineStyle type)
	{
		XGL::EnableBlend();
		XGL::SetBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);	//普通效果
		glColor4fv(c);
		glLineWidth(width);
		XGL::DisableTexture2D();

		if (type == LS_SOLID)
		{//实线
			drawRectS(rect);
		}else
		{//虚线
			glEnable(GL_LINE_STIPPLE);
			glLineStipple(1, type);
			drawRectS(rect);
			glDisable(GL_LINE_STIPPLE);
		}
		XGL::DisableBlend();
	}
	void drawRect(const XVector2& pos,const XVector2& size,float width, const XFColor &c)
	{
		XGL::EnableBlend();
		XGL::SetBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);	//普通效果
		glColor4fv(c);
		glLineWidth(width);
		XGL::DisableTexture2D();

		glBegin(GL_LINE_LOOP);
		glVertex2fv(pos - size);
		glVertex2f(pos.x + size.x,pos.y - size.y);
		glVertex2fv(pos + size);
		glVertex2f(pos.x - size.x,pos.y + size.y);
		glEnd();

		XGL::DisableBlend();
	}
	void drawRectAntiColor(const XRect& rect, float width, XLineStyle type)
	{
		XGL::EnableBlend();
		XGL::SetBlendFunc(GL_ONE_MINUS_DST_COLOR,GL_ZERO);	//反色效果
		glColor4fv(XFColor::white);
		glLineWidth(width);
		XGL::DisableTexture2D();
		if (type == LS_SOLID)
		{//实线
			drawRectS(rect);
		}else
		{//虚线
			glEnable(GL_LINE_STIPPLE);
			glLineStipple(1, type);
			drawRectS(rect);
			glDisable(GL_LINE_STIPPLE);
		}
		XGL::DisableBlend();
	}
	void drawBox(float x,float y,float sizeX,float sizeY,int w,const XFColor &c)
	{
		XGL::EnableBlend();
		XGL::SetBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glColor4fv(c);
		glLineWidth(w);
		XGL::DisableTexture2D();
		drawBoxS(x,y,sizeX,sizeY);
		XGL::DisableBlend();
	}
	void drawBox(float x,float y,float sizeX,float sizeY,float angle)
	{
		XVector2 P1(x - (sizeX * cos(angle) - sizeY * sin(angle)),y + (sizeX * sin(angle) + sizeY * cos(angle)));
		XVector2 P2(x - (sizeX * cos(angle) + sizeY * sin(angle)),y + (sizeX * sin(angle) - sizeY * cos(angle)));

		XVector2 P3(x + (sizeX * cos(angle) + sizeY * sin(angle)),y - (sizeX * sin(angle) - sizeY * cos(angle)));
		XVector2 P4(x + (sizeX * cos(angle) - sizeY * sin(angle)),y - (sizeX * sin(angle) + sizeY * cos(angle)));

		glColor3fv(XFColor::white);
		glLineWidth(1);
		XGL::DisableTexture2D();
		drawBoxS(P1,P2,P3,P4);
	}
	void drawBox(const XVector2& p0,const XVector2& p1,const XVector2& p2,const XVector2& p3,float width,float r,float g,float b)
	{
		glColor3f(r,g,b);
		glLineWidth(width);
		XGL::DisableTexture2D();
		drawBoxS(p0,p1,p3,p2);
	}
	void drawBox(const XVector2& p0,const XVector2& p1,const XVector2& p2,const XVector2& p3,float width,const XFColor &c)
	{
		XGL::EnableBlend();
		XGL::SetBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glColor4fv(c);
		glLineWidth(width);
		XGL::DisableTexture2D();
		drawBoxS(p0,p1,p3,p2);
		XGL::DisableBlend();
	}
	void drawTriangle(const XVector2& p0,const XVector2& p1,const XVector2& p2,float width,float r,float g,float b)
	{
		glColor3f(r,g,b);
		glLineWidth(width);
		XGL::DisableTexture2D();
		glBegin(GL_LINE_LOOP);
		glVertex2fv(p0);
		glVertex2fv(p1);
		glVertex2fv(p2);
		glEnd();
	}
	void drawTriangle(const XVector2& p,float size,float width,const XFColor &color)
	{
		XGL::EnableBlend();
		XGL::SetBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glColor4fv(color);
		glLineWidth(width);
		XGL::DisableTexture2D();
		glBegin(GL_LINE_LOOP);
		glVertex2f(p.x,p.y - size);
		glVertex2f(p.x - size * SQRT3 * 0.5f,p.y + size * 0.5f);
		glVertex2f(p.x + size * SQRT3 * 0.5f,p.y + size * 0.5f);
		glEnd();
		XGL::DisableBlend();
	}
	void drawPoint(float x,float y,float r,float g,float b,float a)
	{
		XGL::EnableBlend();
		XGL::SetBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(r,g,b,a);
		glPointSize(1.0f);
		XGL::DisableTexture2D();
		glBegin(GL_POINTS);
			glVertex2f(x,y);
		glEnd();
		XGL::DisableBlend();
	}
	void drawPoint(float x,float y,float size,float r,float g,float b,float a)
	{
		XGL::EnableBlend();
		XGL::SetBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
			glColor4f(r,g,b,a);
			glPointSize(size);
		XGL::DisableTexture2D();
		glBegin(GL_POINTS);
			glVertex2f(x,y);
		glEnd();
		XGL::DisableBlend();
	}
	void drawPointsVbo(unsigned int v,int pointSum,float s,float r,float g,float b)
	{
		glPointSize(s);
		glColor3f(r,g,b);
		XGL::DisableTexture2D();
		glEnableClientState(GL_VERTEX_ARRAY);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB,v);
		glVertexPointer(2,GL_FLOAT,0,NULL);
		glDrawArrays(GL_POINTS,0,pointSum);

		glDisableClientState(GL_VERTEX_ARRAY);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB,0);
	}
	void drawPointsVbo(unsigned int v,int pointSum,float s,float r,float g,float b,float a)
	{
		XGL::EnableBlend();
		XGL::SetBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glPointSize(s);
		glColor4f(r,g,b,a);
		XGL::DisableTexture2D();
		glEnableClientState(GL_VERTEX_ARRAY);	
		glBindBufferARB(GL_ARRAY_BUFFER_ARB,v);
		glVertexPointer(2,GL_FLOAT,0,NULL);
		glDrawArrays(GL_POINTS,0,pointSum);

		glDisableClientState(GL_VERTEX_ARRAY);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB,0);
		XGL::DisableBlend();
	}
	void drawCross(const XVector2& p,float size,float w,float r,float g,float b)
	{
		glColor3f(r,g,b);
		glLineWidth(w);
		XGL::DisableTexture2D();
		glBegin(GL_LINES);
		glVertex2f(p.x - size,p.y);
		glVertex2f(p.x + size,p.y);
		glVertex2f(p.x,p.y - size);
		glVertex2f(p.x,p.y + size);
		glEnd();
	}
	void drawCross(const XVector2& p,float size,float w,const XFColor& color)
	{
		XGL::EnableBlend();
		XGL::SetBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);	//普通效果
		glColor4fv(color);
		glLineWidth(w);
		XGL::DisableTexture2D();

		glBegin(GL_LINES);
		glVertex2f(p.x - size,p.y);
		glVertex2f(p.x + size,p.y);
		glVertex2f(p.x,p.y - size);
		glVertex2f(p.x,p.y + size);
		glEnd();

		XGL::DisableBlend();
	}
	void drawMaskRight(const XVector2& p,const XVector2& size,float width,const XFColor& color)
	{
		XGL::EnableBlend();
		XGL::SetBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glColor4fv(color);
		glLineWidth(width);
		XGL::DisableTexture2D();
		glBegin(GL_LINE_STRIP);
		glVertex2f(p.x - size.x,p.y);
		glVertex2f(p.x - size.x * 0.5f,p.y + size.y);
		glVertex2f(p.x + size.x,p.y - size.y);
		glEnd();
		XGL::DisableBlend();
	}
	void drawMaskWrong(const XVector2& p,const XVector2& size,float width,const XFColor& color)
	{
		XGL::EnableBlend();
		XGL::SetBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glColor4fv(color);
		glLineWidth(width);
		XGL::DisableTexture2D();
		glBegin(GL_LINES);
		glVertex2fv(p - size);
		glVertex2fv(p + size);
		glVertex2f(p.x + size.x,p.y - size.y);
		glVertex2f(p.x - size.x,p.y + size.y);
		glEnd();
		XGL::DisableBlend();
	}
	void drawLeft(const XVector2& p,const XVector2& size,float width,const XFColor& color)
	{
		XGL::EnableBlend();
		XGL::SetBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glColor4fv(color);
		glLineWidth(width);
		XGL::DisableTexture2D();
		glBegin(GL_LINE_STRIP);
		glVertex2f(p.x + size.x,p.y - size.y);
		glVertex2f(p.x - size.x,p.y);
		glVertex2f(p.x + size.x,p.y + size.y);
		glEnd();
		XGL::DisableBlend();
	}
	void drawRight(const XVector2& p,const XVector2& size,float width,const XFColor& color)
	{
		XGL::EnableBlend();
		XGL::SetBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glColor4fv(color);
		glLineWidth(width);
		XGL::DisableTexture2D();
		glBegin(GL_LINE_STRIP);
		glVertex2f(p.x - size.x,p.y - size.y);
		glVertex2f(p.x + size.x,p.y);
		glVertex2f(p.x - size.x,p.y + size.y);
		glEnd();
		XGL::DisableBlend();
	}
	void drawUp(const XVector2& p,const XVector2& size,float width,const XFColor& color)
	{
		XGL::EnableBlend();
		XGL::SetBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glColor4fv(color);
		glLineWidth(width);
		XGL::DisableTexture2D();
		glBegin(GL_LINE_STRIP);
		glVertex2f(p.x - size.x,p.y + size.y);
		glVertex2f(p.x,p.y - size.y);
		glVertex2f(p.x + size.x,p.y + size.y);
		glEnd();
		XGL::DisableBlend();
	}
	void drawDown(const XVector2& p,const XVector2& size,float width,const XFColor& color)
	{
		XGL::EnableBlend();
		XGL::SetBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glColor4fv(color);
		glLineWidth(width);
		XGL::DisableTexture2D();
		glBegin(GL_LINE_STRIP);
		glVertex2f(p.x - size.x,p.y - size.y);
		glVertex2f(p.x,p.y + size.y);
		glVertex2f(p.x + size.x,p.y - size.y);
		glEnd();
		XGL::DisableBlend();
	}
	void drawCircle(float px, float py, float sr, int an,float r,float g,float b,float a)
	{
		if(an <= 0 || sr <= 0.0f) return;
		XGL::EnableBlend();
		XGL::SetBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(r,g,b,a);
		r = (PI2)/an;	//为了优化计算
		XGL::DisableTexture2D();
		glBegin(GL_POLYGON);
			for(float i = 0;i < PI2; i += r)
			{
				glVertex2f(px + cosf(i) * sr,py + sinf(i) * sr);
			}
		glEnd();
		XGL::DisableBlend();
	}
	namespace XGLCircleData
	{
	#define CIRCLE_AN (60)
	bool isCircleInit = false;
	float circleSin[CIRCLE_AN];
	float circleCos[CIRCLE_AN];
	inline void circleInit()
	{
		if(isCircleInit) return;
		isCircleInit = true;
		float r = PI2 / CIRCLE_AN;
		for(int i = 0;i < CIRCLE_AN;++ i)
		{
			circleSin[i] = sinf(r * i);
			circleCos[i] = cosf(r * i);
		}
	}
	}
	void drawCircleEx(float px, float py, float sr,float r,float g,float b,float a)
	{
		if(sr <= 0.0f) return;
		XGLCircleData::circleInit();
		XGL::EnableBlend();
		XGL::SetBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(r,g,b,a);
		XGL::DisableTexture2D();
		glBegin(GL_POLYGON);
		for(int i = 0;i < CIRCLE_AN;++ i)
		{
			glVertex2f(px + XGLCircleData::circleCos[i] * sr,py + XGLCircleData::circleSin[i] * sr);
		}
		glEnd();
		XGL::DisableBlend();
	}
	void drawCircleLine(float px, float py, float sr, int an,float r,float g,float b,float a)
	{
		if(an <= 0 || sr <= 0.0f) return;
		XGL::EnableBlend();
		XGL::SetBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(r,g,b,a);
		r = (PI2)/an;	//为了优化计算
		XGL::DisableTexture2D();
		glBegin(GL_LINE_LOOP);
			for(float i = 0;i < PI2; i += r)
			{
				glVertex2f(px + cosf(i) * sr,py + sinf(i) * sr);
			}
		glEnd();
		XGL::DisableBlend();
	}
	void drawCircleLine(const XVector2 &pos, float sr, int an,float w,const XFColor &color)
	{
		if(an <= 0 || sr <= 0.0f) return;
		XGL::EnableBlend();
		XGL::SetBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glColor4fv(color);
		glLineWidth(w);
		float r = (PI2)/an;	//为了优化计算
		XGL::DisableTexture2D();
		glBegin(GL_LINE_LOOP);
			for(float i = 0;i < PI2; i += r)
			{
				glVertex2f(pos.x + cosf(i) * sr,pos.y + sinf(i) * sr);
			}
		glEnd();
		XGL::DisableBlend();
	}
	void drawCircleLineEx(float px, float py, float sr,float r,float g,float b,float a)
	{
		if(sr <= 0.0f) return;
		XGLCircleData::circleInit();
		XGL::EnableBlend();
		XGL::SetBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(r,g,b,a);
		XGL::DisableTexture2D();
		glBegin(GL_LINE_LOOP);
		for(int i = 0;i < CIRCLE_AN; ++ i)
		{
			glVertex2f(px + XGLCircleData::circleCos[i] * sr,py + XGLCircleData::circleSin[i] * sr);
		}
		glEnd();
		XGL::DisableBlend();
	}
	void drawCircleLineEx(const XVector2 &pos, float sr,float w,const XFColor &color)
	{
		if(sr <= 0.0f) return;
		XGLCircleData::circleInit();
		XGL::EnableBlend();
		XGL::SetBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glColor4fv(color);
		glLineWidth(w);
		XGL::DisableTexture2D();
		glBegin(GL_LINE_LOOP);
		for(int i = 0;i < CIRCLE_AN; ++ i)
		{
			glVertex2f(pos.x + XGLCircleData::circleCos[i] * sr,pos.y + XGLCircleData::circleSin[i] * sr);
		}
		glEnd();
		XGL::DisableBlend();
	}
	void drawFillRing(float x,float y,float r0,float r1,float angleS,float angleE,int an,float r,float g,float b,float a)
	{//测试通过，但是计算需要进一步优化
		if(angleE == angleS || r0 == r1 || an <= 0) return;
		//开始计算需要的点
		static std::vector<float> point;
		point.clear();
		float perAngle = (angleE - angleS) / an * DEGREE2RADIAN;
		angleS *= DEGREE2RADIAN;
		float s,c;
		for(int i = 0;i <= an;++ i)
		{
			s = sin(angleS);
			c = cos(angleS);
			point.push_back(x - r0 * s);
			point.push_back(y - r0 * c);
			point.push_back(x - r1 * s);
			point.push_back(y - r1 * c);
			angleS += perAngle;
		}
		XGL::EnableBlend();
		XGL::SetBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(r,g,b,a);
		XGL::DisableTexture2D();
		glBegin(GL_TRIANGLE_STRIP);
		for(unsigned int i = 0;i < point.size();i += 2)
		{
			glVertex2f(point[i],point[i + 1]);
		}
		glEnd();
		XGL::DisableBlend();
	}
	void drawFillRingEx(float x,float y,float r0,float r1,float angleS,float angleE,int an,float r,float g,float b,float a)
	{
		if(angleE == angleS || r0 == r1 || an <= 0) return;
		//下面开始计算需要的点
		static std::vector<float> point;
		float perAngle = (angleE - angleS) / an * DEGREE2RADIAN;
		angleS *= DEGREE2RADIAN;
		float s,c;
		for(int i = 0;i <= an;++ i)
		{
			s = sin(angleS);
			c = cos(angleS);
			point.push_back(x - r0 * s);
			point.push_back(y - r0 * c);
			point.push_back(x - r1 * s);
			point.push_back(y - r1 * c);
			angleS += perAngle;
		}
		XGL::EnableBlend();
		XGL::SetBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		perAngle = a / (float)(point.size()) * 2.0f;
		XGL::DisableTexture2D();
		glBegin(GL_TRIANGLE_STRIP);
		for(unsigned int i = 0;i < point.size();i += 2)
		{
			glColor4f(r,g,b,perAngle * (i >> 1));
			glVertex2f(point[i],point[i + 1]);
		}
		glEnd();
		XGL::DisableBlend();
	}
	//这里是关于圆角的定义
	namespace XGLCornerData
	{
		bool isRoundCornerInit = false;
		#define ROUND_CORNER_DENSITY (3)
		XVector2 roundCornerData[ROUND_CORNER_DENSITY * 4];	//10度一个弧
		float roundCornerR = 3.0f;	//圆角的半径
		float roundCornerD = 6.0f;	//圆角的直径
		inline void roundCornerInit()
		{
			if(!isRoundCornerInit)
			{
				float angle;
				for(int i = 0;i < ROUND_CORNER_DENSITY * 4;++ i)
				{
					angle = i * 360.0f / (ROUND_CORNER_DENSITY * 4.0f) * DEGREE2RADIAN;
					roundCornerData[i].set(roundCornerR * cos(angle),
						roundCornerR * sin(angle));
				}
				isRoundCornerInit = true;
			}
		}
		inline void drawCornerLine(const XVector2 &pos,const XVector2 &size)
		{
			glBegin(GL_LINE_STRIP);
			for(int i = 0;i <= ROUND_CORNER_DENSITY;++ i)
			{//右下角圆角
			//	glVertex2f(XGLCornerData::roundCornerData[i].x + size.x - XGLCornerData::roundCornerR + pos.x,
			//		XGLCornerData::roundCornerData[i].y + size.y - XGLCornerData::roundCornerR + pos.y);
				glVertex2fv(XGLCornerData::roundCornerData[i] + size + pos - XGLCornerData::roundCornerR);
			}
			for(int i = 0;i <= ROUND_CORNER_DENSITY;++ i)
			{//左下角圆角
			//	glVertex2f(XGLCornerData::roundCornerData[i + ROUND_CORNER_DENSITY].x + XGLCornerData::roundCornerR + pos.x,
			//		XGLCornerData::roundCornerData[i + ROUND_CORNER_DENSITY].y + size.y - XGLCornerData::roundCornerR + pos.y);
				glVertex2fv(XGLCornerData::roundCornerData[i + ROUND_CORNER_DENSITY] + pos + XVector2(XGLCornerData::roundCornerR,size.y - XGLCornerData::roundCornerR));
			}
			for(int i = 0;i <= ROUND_CORNER_DENSITY;++ i)
			{//左上角圆角
			//	glVertex2f(XGLCornerData::roundCornerData[i + (ROUND_CORNER_DENSITY << 1)].x + XGLCornerData::roundCornerR + pos.x,
			//		XGLCornerData::roundCornerData[i + (ROUND_CORNER_DENSITY << 1)].y + XGLCornerData::roundCornerR + pos.y);
				glVertex2fv(XGLCornerData::roundCornerData[i + (ROUND_CORNER_DENSITY << 1)] + pos + XGLCornerData::roundCornerR);
			}
			for(int i = 0;i < ROUND_CORNER_DENSITY;++ i)
			{//右上角圆角
			//	glVertex2f(XGLCornerData::roundCornerData[i + ROUND_CORNER_DENSITY * 3].x + size.x - XGLCornerData::roundCornerR + pos.x,
			//		XGLCornerData::roundCornerData[i + ROUND_CORNER_DENSITY * 3].y + XGLCornerData::roundCornerR + pos.y);
				glVertex2fv(XGLCornerData::roundCornerData[i + ROUND_CORNER_DENSITY * 3] + pos + XVector2(size.x - XGLCornerData::roundCornerR,XGLCornerData::roundCornerR));
			}
		//	glVertex2f(XGLCornerData::roundCornerData[0].x + size.x - XGLCornerData::roundCornerR + pos.x,
		//			XGLCornerData::roundCornerData[0].y + size.y - XGLCornerData::roundCornerR + pos.y);
			glVertex2fv(XGLCornerData::roundCornerData[0] + size + pos - XGLCornerData::roundCornerR);
			glEnd();
		}
	}
	void drawFillBox(const XVector2 &p0,const XFColor &c0,		//第一个点以及颜色
							const XVector2 &p1,const XFColor &c1,		//第二个点以及颜色
							const XVector2 &p2,const XFColor &c2,		//第三个点以及颜色
							const XVector2 &p3,const XFColor &c3)	//第四个点以及颜色
	{
		XGL::EnableBlend();
		XGL::SetBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		XGL::DisableTexture2D();
		glBegin(GL_QUADS);
		glColor4fv(c0);
		glVertex2fv(p0);
		glColor4fv(c1);
		glVertex2fv(p1);
		glColor4fv(c2);
		glVertex2fv(p2);
		glColor4fv(c3);
		glVertex2fv(p3);
		glEnd();
		XGL::DisableBlend();
	}
	void drawFillBox(float x,float y,float w,float h,const XFColor &c)	//描绘实心矩形
	{
		XGL::EnableBlend();
		XGL::SetBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		XGL::DisableTexture2D();
		glColor4fv(c);
		drawFillBoxS(x,y,w,h);
		XGL::DisableBlend();
	}
	void drawFillBox(const XVector2& pos,const XVector2& size,
					 float r,float g,float b,bool withLine)	//描绘实心矩形
	{
		XGL::DisableTexture2D();
		if(withLine)
		{//描绘边框
			glColor3f(0.15f,0.15f,0.15f);
			glLineWidth(1.0f);
			drawBox(pos,size);
		}
		glColor3f(r,g,b);
		drawFillBoxS(pos.x,pos.y,size.x,size.y);
	}
	void drawFillBoxA(const XVector2& pos,const XVector2& size,
		const XFColor &c,bool withLine)	//描绘实心矩形
	{
		XGL::EnableBlend();
		XGL::SetBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		XGL::DisableTexture2D();
		if(withLine)
		{//描绘边框
			glColor4f(0.15f,0.15f,0.15f,c.fA);
			glLineWidth(1.0f);
			drawBox(pos,size);
		}
		glColor4fv(c);
		drawFillBoxS(pos.x,pos.y,size.x,size.y);
		XGL::DisableBlend();
	}
	void drawFillTriangle(const XVector2& p0,const XVector2& p1,const XVector2& p2,
							float r,float g ,float b)
	{
		glColor3f(r,g,b);
		XGL::DisableTexture2D();
		glBegin(GL_TRIANGLES);
		glVertex2fv(p0);
		glVertex2fv(p1);
		glVertex2fv(p2);
		glEnd();
	}
	void drawFillTriangleEx(const XVector2& p0,const XVector2& p1,const XVector2& p2,
							float r,float g,float b)	//描绘实心三角形
	{
		float min = p0.y;
		float max = p0.y;
		if(p1.y < min) min = p1.y;
		if(p1.y > max) max = p1.y;
		if(p2.y < min) min = p2.y;
		if(p2.y > max) max = p2.y;
		float rate = 1.0f;
		XGL::DisableTexture2D();
		glBegin(GL_TRIANGLES);
		rate = XMath::maping1D(p0.y,min,max,0.9f,1.1f);
		glColor3f(r * rate,g * rate,b * rate);
		glVertex2fv(p0);
		rate = XMath::maping1D(p1.y,min,max,0.9f,1.1f);
		glColor3f(r * rate,g * rate,b * rate);
		glVertex2fv(p1);
		rate = XMath::maping1D(p2.y,min,max,0.9f,1.1f);
		glColor3f(r * rate,g * rate,b * rate);
		glVertex2fv(p2);
		glEnd();
	}
	void drawFillBoxEx(const XVector2& pos,const XVector2& size,
					 float r,float g,float b,
					 bool withLine,bool withRoundCorner,bool down)	//描绘实心矩形
	{
		XFColor lColor(r * 1.1f,g * 1.1f,b * 1.1f,1.0f);
		XFColor dColor(r * 0.9f,g * 0.9f,b * 0.9f,1.0f);
		XGL::DisableTexture2D();
		if(withRoundCorner && size.x >= XGLCornerData::roundCornerD && size.y >= XGLCornerData::roundCornerD)
		{
			XGLCornerData::roundCornerInit();
			if(withLine)
			{//描绘边框
				glColor3f(0.15f,0.15f,0.15f);
				glLineWidth(1.0f);
				XGLCornerData::drawCornerLine(pos,size);
			}
			//下半部分
			glBegin(GL_POLYGON);
			if(down) glColor3fv(lColor); 
			else glColor3fv(dColor);
			for(int i = 0;i <= ROUND_CORNER_DENSITY;++ i)	
			{//右下角圆角
			//	glVertex2f(XGLCornerData::roundCornerData[i].x + size.x - XGLCornerData::roundCornerR + pos.x,
			//		XGLCornerData::roundCornerData[i].y + size.y - XGLCornerData::roundCornerR + pos.y);
				glVertex2fv(XGLCornerData::roundCornerData[i] + size + pos - XGLCornerData::roundCornerR);
			}
			for(int i = 0;i <= ROUND_CORNER_DENSITY;++ i)
			{//左下角圆角
			//	glVertex2f(XGLCornerData::roundCornerData[i + ROUND_CORNER_DENSITY].x + XGLCornerData::roundCornerR + pos.x,
			//		XGLCornerData::roundCornerData[i + ROUND_CORNER_DENSITY].y + size.y - XGLCornerData::roundCornerR + pos.y);
				glVertex2fv(XGLCornerData::roundCornerData[i + ROUND_CORNER_DENSITY] + pos + XVector2(XGLCornerData::roundCornerR,size.y - XGLCornerData::roundCornerR));
			}
			glEnd();
			//上半部分
			glBegin(GL_POLYGON);
			if(down) glColor3fv(dColor);
			else glColor3fv(lColor);
			for(int i = 0;i <= ROUND_CORNER_DENSITY;++ i)
			{//左上角圆角
			//	glVertex2f(XGLCornerData::roundCornerData[i + (ROUND_CORNER_DENSITY << 1)].x + XGLCornerData::roundCornerR + pos.x,
			//		XGLCornerData::roundCornerData[i + (ROUND_CORNER_DENSITY << 1)].y + XGLCornerData::roundCornerR + pos.y);
				glVertex2fv(XGLCornerData::roundCornerData[i + (ROUND_CORNER_DENSITY << 1)] + pos + XGLCornerData::roundCornerR);
			}
			for(int i = 0;i < ROUND_CORNER_DENSITY;++ i)
			{//左下角圆角
			//	glVertex2f(XGLCornerData::roundCornerData[i + ROUND_CORNER_DENSITY * 3].x + size.x - XGLCornerData::roundCornerR + pos.x,
			//		XGLCornerData::roundCornerData[i + ROUND_CORNER_DENSITY * 3].y + XGLCornerData::roundCornerR + pos.y);
				glVertex2fv(XGLCornerData::roundCornerData[i + ROUND_CORNER_DENSITY * 3] + pos + XVector2(size.x - XGLCornerData::roundCornerR,XGLCornerData::roundCornerR));
			}
			//glVertex2f(XGLCornerData::roundCornerData[0].x + size.x - XGLCornerData::roundCornerR + pos.x,
			//	XGLCornerData::roundCornerData[0].y + XGLCornerData::roundCornerR + pos.y);
			glVertex2fv(XGLCornerData::roundCornerData[0] + pos + XVector2(size.x - XGLCornerData::roundCornerR,XGLCornerData::roundCornerR));
			glEnd();
			//中间
			glBegin(GL_POLYGON);
			if(down) glColor3fv(lColor); 
			else glColor3fv(dColor);
			//glVertex2f(XGLCornerData::roundCornerData[0].x + size.x - XGLCornerData::roundCornerR + pos.x,
			//	XGLCornerData::roundCornerData[0].y + size.y - XGLCornerData::roundCornerR + pos.y);
			glVertex2fv(XGLCornerData::roundCornerData[0] + size + pos - XGLCornerData::roundCornerR);
			//glVertex2f(XGLCornerData::roundCornerData[ROUND_CORNER_DENSITY << 1].x + XGLCornerData::roundCornerR + pos.x,
			//	XGLCornerData::roundCornerData[ROUND_CORNER_DENSITY << 1].y + size.y - XGLCornerData::roundCornerR + pos.y);
			glVertex2fv(XGLCornerData::roundCornerData[ROUND_CORNER_DENSITY << 1] + pos + XVector2(XGLCornerData::roundCornerR,size.y - XGLCornerData::roundCornerR));
			if(down) glColor3fv(dColor);
			else glColor3fv(lColor);
			//glVertex2f(XGLCornerData::roundCornerData[ROUND_CORNER_DENSITY << 1].x + XGLCornerData::roundCornerR + pos.x,
			//	XGLCornerData::roundCornerData[ROUND_CORNER_DENSITY << 1].y + XGLCornerData::roundCornerR + pos.y);
			glVertex2fv(XGLCornerData::roundCornerData[ROUND_CORNER_DENSITY << 1] + pos + XGLCornerData::roundCornerR);
			//glVertex2f(XGLCornerData::roundCornerData[0].x + size.x - XGLCornerData::roundCornerR + pos.x,
			//	XGLCornerData::roundCornerData[0].y + XGLCornerData::roundCornerR + pos.y);
			glVertex2fv(XGLCornerData::roundCornerData[0] + pos + XVector2(size.x - XGLCornerData::roundCornerR,XGLCornerData::roundCornerR));
			glEnd();
		}else
		{
			if(withLine)
			{//描绘边框
				glColor3f(0.15f,0.15f,0.15f);
				glLineWidth(1.0f);
				drawBox(pos,size);
			}
			glBegin(GL_QUADS);
			if(down) glColor3fv(dColor); 
			else glColor3fv(lColor);
			glVertex2fv(pos);
			glVertex2f(pos.x + size.x,pos.y);
			if(down) glColor3fv(lColor);
			else glColor3fv(dColor);
			glVertex2fv(pos + size);
			glVertex2f(pos.x,pos.y + size.y);
			glEnd();
		}
	}
	void drawFillBoxExA(const XVector2& pos,const XVector2& size,
					 float r,float g,float b,float a,
					 bool withLine,bool withRoundCorner,bool down)	//描绘实心矩形
	{
		XFColor lColor(r * 1.1f,g * 1.1f,b * 1.1f,a);
		XFColor dColor(r * 0.9f,g * 0.9f,b * 0.9f,a);
	
		XGL::DisableTexture2D();
		XGL::EnableBlend();
		XGL::SetBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		if(withRoundCorner && size.x >= XGLCornerData::roundCornerD && size.y >= XGLCornerData::roundCornerD)
		{
			XGLCornerData::roundCornerInit();
			if(withLine)
			{//描绘边框
				glColor4f(0.15f,0.15f,0.15f,a);
				glLineWidth(1.0f);
				XGLCornerData::drawCornerLine(pos,size);
			}
			//下半部分
			glBegin(GL_POLYGON);
			if(down) glColor3fv(lColor); 
			else glColor4fv(dColor);
			for(int i = 0;i <= ROUND_CORNER_DENSITY;++ i)	
			{//右下角圆角
			//	glVertex2f(XGLCornerData::roundCornerData[i].x + size.x - XGLCornerData::roundCornerR + pos.x,
			//		XGLCornerData::roundCornerData[i].y + size.y - XGLCornerData::roundCornerR + pos.y);
				glVertex2fv(XGLCornerData::roundCornerData[i] + size + pos - XGLCornerData::roundCornerR);
			}
			for(int i = 0;i <= ROUND_CORNER_DENSITY;++ i)
			{//左下角圆角
			//	glVertex2f(XGLCornerData::roundCornerData[i + ROUND_CORNER_DENSITY].x + XGLCornerData::roundCornerR + pos.x,
			//		XGLCornerData::roundCornerData[i + ROUND_CORNER_DENSITY].y + size.y - XGLCornerData::roundCornerR + pos.y);
				glVertex2fv(XGLCornerData::roundCornerData[i + ROUND_CORNER_DENSITY] + pos + XVector2(XGLCornerData::roundCornerR,size.y - XGLCornerData::roundCornerR));
			}
			glEnd();
			//上半部分
			glBegin(GL_POLYGON);
			if(down) glColor4fv(dColor);
			else glColor4fv(lColor);
			for(int i = 0;i <= ROUND_CORNER_DENSITY;++ i)
			{//左上角圆角
			//	glVertex2f(XGLCornerData::roundCornerData[i + (ROUND_CORNER_DENSITY << 1)].x + XGLCornerData::roundCornerR + pos.x,
			//		XGLCornerData::roundCornerData[i + (ROUND_CORNER_DENSITY << 1)].y + XGLCornerData::roundCornerR + pos.y);
				glVertex2fv(XGLCornerData::roundCornerData[i + (ROUND_CORNER_DENSITY << 1)] + pos + XGLCornerData::roundCornerR);
			}
			for(int i = 0;i < ROUND_CORNER_DENSITY;++ i)
			{//左下角圆角
			//	glVertex2f(XGLCornerData::roundCornerData[i + ROUND_CORNER_DENSITY * 3].x + size.x - XGLCornerData::roundCornerR + pos.x,
			//		XGLCornerData::roundCornerData[i + ROUND_CORNER_DENSITY * 3].y + XGLCornerData::roundCornerR + pos.y);
				glVertex2fv(XGLCornerData::roundCornerData[i + ROUND_CORNER_DENSITY * 3] + pos + XVector2(size.x - XGLCornerData::roundCornerR,XGLCornerData::roundCornerR));
			}
			//glVertex2f(XGLCornerData::roundCornerData[0].x + size.x - XGLCornerData::roundCornerR + pos.x,
			//	XGLCornerData::roundCornerData[0].y + XGLCornerData::roundCornerR + pos.y);
			glVertex2fv(XGLCornerData::roundCornerData[0] + pos + XVector2(size.x - XGLCornerData::roundCornerR,XGLCornerData::roundCornerR));
			glEnd();
			//中间
			glBegin(GL_POLYGON);
			if(down) glColor4fv(lColor); 
			else glColor4fv(dColor);
			//glVertex2f(XGLCornerData::roundCornerData[0].x + size.x - XGLCornerData::roundCornerR + pos.x,
			//	XGLCornerData::roundCornerData[0].y + size.y - XGLCornerData::roundCornerR + pos.y);
			glVertex2fv(XGLCornerData::roundCornerData[0] + pos + size - XGLCornerData::roundCornerR);
			//glVertex2f(XGLCornerData::roundCornerData[ROUND_CORNER_DENSITY << 1].x + XGLCornerData::roundCornerR + pos.x,
			//	XGLCornerData::roundCornerData[ROUND_CORNER_DENSITY << 1].y + size.y - XGLCornerData::roundCornerR + pos.y);
			glVertex2fv(XGLCornerData::roundCornerData[ROUND_CORNER_DENSITY << 1] + pos + XVector2(XGLCornerData::roundCornerR,size.y - XGLCornerData::roundCornerR));
			if(down) glColor4fv(dColor);
			else glColor4fv(lColor);
			//glVertex2f(XGLCornerData::roundCornerData[ROUND_CORNER_DENSITY << 1].x + XGLCornerData::roundCornerR + pos.x,
			//	XGLCornerData::roundCornerData[ROUND_CORNER_DENSITY << 1].y + XGLCornerData::roundCornerR + pos.y);
			glVertex2fv(XGLCornerData::roundCornerData[ROUND_CORNER_DENSITY << 1] + pos + XGLCornerData::roundCornerR);
			//glVertex2f(XGLCornerData::roundCornerData[0].x + size.x - XGLCornerData::roundCornerR + pos.x,
			//	XGLCornerData::roundCornerData[0].y + XGLCornerData::roundCornerR + pos.y);
			glVertex2fv(XGLCornerData::roundCornerData[0] + pos + XVector2(size.x - XGLCornerData::roundCornerR,XGLCornerData::roundCornerR));
			glEnd();
		}else
		{
			//调换绘制顺序，立体感会更强。
			glBegin(GL_QUADS);
			if(down) glColor4fv(dColor); 
			else glColor4fv(lColor);
			glVertex2fv(pos);
			glVertex2f(pos.x + size.x,pos.y);
			if(down) glColor4fv(lColor);
			else glColor4fv(dColor);
			glVertex2fv(pos + size);
			glVertex2f(pos.x,pos.y + size.y);
			glEnd();
			if(withLine)
			{
				//这里描绘高光
				glLineWidth(2.0f);
				if(down)
				{
					glColor4f(dColor.fR + 0.2f,dColor.fG + 0.2f,dColor.fB + 0.2f,dColor.fA); 
					glBegin(GL_LINES);
					glVertex2f(pos.x + size.x,pos.y + size.y - 1);
					glVertex2f(pos.x,pos.y + size.y - 1);
					glEnd();
				}else
				{
					glColor4f(lColor.fR + 0.2f,lColor.fG + 0.2f,lColor.fB + 0.2f,lColor.fA); 
					glBegin(GL_LINES);
					glVertex2fv(pos);
					glVertex2f(pos.x + size.x,pos.y);
					glEnd();
				}
				//描绘边框
				glColor4f(0.15f,0.15f,0.15f,a);
				glLineWidth(1.0f);
				drawBox(pos,size);
			}
		}
		XGL::DisableBlend();
	}
	void drawFillBoxExAS(const XVector2& pos,const XVector2& size,
					 float r,float g,float b,float a,
					 bool withLine,bool withRoundCorner,bool down)	//描绘实心矩形
	{
		XFColor lColor(r * 1.1f,g * 1.1f,b * 1.1f,a);
		XFColor dColor(r * 0.9f,g * 0.9f,b * 0.9f,a);

		XGL::DisableTexture2D();
		XGL::EnableBlend();
		XGL::SetBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		if(withRoundCorner && size.x >= XGLCornerData::roundCornerD && size.y >= XGLCornerData::roundCornerD)
		{
			XGLCornerData::roundCornerInit();
			if(withLine)
			{//描绘边框
				glColor4f(0.15f,0.15f,0.15f,a);
				glLineWidth(1.0f);
				XGLCornerData::drawCornerLine(pos,size);
			}
			//下半部分
			glBegin(GL_POLYGON);
			if(down) glColor3fv(lColor); 
			else glColor4fv(dColor);
			for(int i = 0;i <= ROUND_CORNER_DENSITY;++ i)	
			{//右下角圆角
			//	glVertex2f(XGLCornerData::roundCornerData[i].x + size.x - XGLCornerData::roundCornerR + pos.x,
			//		XGLCornerData::roundCornerData[i].y + size.y - XGLCornerData::roundCornerR + pos.y);
				glVertex2fv(XGLCornerData::roundCornerData[i] + pos + size - XGLCornerData::roundCornerR);
			}
			for(int i = 0;i <= ROUND_CORNER_DENSITY;++ i)
			{//左下角圆角
			//	glVertex2f(XGLCornerData::roundCornerData[i + ROUND_CORNER_DENSITY].x + XGLCornerData::roundCornerR + pos.x,
			//		XGLCornerData::roundCornerData[i + ROUND_CORNER_DENSITY].y + size.y - XGLCornerData::roundCornerR + pos.y);
				glVertex2fv(XGLCornerData::roundCornerData[i + ROUND_CORNER_DENSITY] + pos + XVector2(XGLCornerData::roundCornerR,size.y - XGLCornerData::roundCornerR));
			}
			glEnd();
			//上半部分
			glBegin(GL_POLYGON);
			if(down) glColor4fv(dColor);
			else glColor4fv(lColor);
			for(int i = 0;i <= ROUND_CORNER_DENSITY;++ i)
			{//左上角圆角
			//	glVertex2f(XGLCornerData::roundCornerData[i + (ROUND_CORNER_DENSITY << 1)].x + XGLCornerData::roundCornerR + pos.x,
			//		XGLCornerData::roundCornerData[i + (ROUND_CORNER_DENSITY << 1)].y + XGLCornerData::roundCornerR + pos.y);
				glVertex2fv(XGLCornerData::roundCornerData[i + (ROUND_CORNER_DENSITY << 1)] + pos + XGLCornerData::roundCornerR);
			}
			for(int i = 0;i < ROUND_CORNER_DENSITY;++ i)
			{//左下角圆角
			//	glVertex2f(XGLCornerData::roundCornerData[i + ROUND_CORNER_DENSITY * 3].x + size.x - XGLCornerData::roundCornerR + pos.x,
			//		XGLCornerData::roundCornerData[i + ROUND_CORNER_DENSITY * 3].y + XGLCornerData::roundCornerR + pos.y);
				glVertex2fv(XGLCornerData::roundCornerData[i + ROUND_CORNER_DENSITY * 3] + pos + XVector2(size.x - XGLCornerData::roundCornerR,XGLCornerData::roundCornerR));
			}
			//glVertex2f(XGLCornerData::roundCornerData[0].x + size.x - XGLCornerData::roundCornerR + pos.x,
			//	XGLCornerData::roundCornerData[0].y + XGLCornerData::roundCornerR + pos.y);
			glVertex2fv(XGLCornerData::roundCornerData[0] + pos + XVector2(size.x - XGLCornerData::roundCornerR,XGLCornerData::roundCornerR));
			glEnd();
			//中间
			glBegin(GL_POLYGON);
			if(down) glColor4fv(lColor); 
			else glColor4fv(dColor);
			//glVertex2f(XGLCornerData::roundCornerData[0].x + size.x - XGLCornerData::roundCornerR + pos.x,
			//	XGLCornerData::roundCornerData[0].y + size.y - XGLCornerData::roundCornerR + pos.y);
			glVertex2fv(XGLCornerData::roundCornerData[0] + pos + size - XGLCornerData::roundCornerR);
			//glVertex2f(XGLCornerData::roundCornerData[ROUND_CORNER_DENSITY << 1].x + XGLCornerData::roundCornerR + pos.x,
			//	XGLCornerData::roundCornerData[ROUND_CORNER_DENSITY << 1].y + size.y - XGLCornerData::roundCornerR + pos.y);
			glVertex2fv(XGLCornerData::roundCornerData[ROUND_CORNER_DENSITY << 1] + pos + XVector2(XGLCornerData::roundCornerR,size.y - XGLCornerData::roundCornerR));
			if(down) glColor4fv(dColor);
			else glColor4fv(lColor);
			//glVertex2f(XGLCornerData::roundCornerData[ROUND_CORNER_DENSITY << 1].x + XGLCornerData::roundCornerR + pos.x,
			//	XGLCornerData::roundCornerData[ROUND_CORNER_DENSITY << 1].y + XGLCornerData::roundCornerR + pos.y);
			glVertex2fv(XGLCornerData::roundCornerData[ROUND_CORNER_DENSITY << 1] + pos + XGLCornerData::roundCornerR);
			//glVertex2f(XGLCornerData::roundCornerData[0].x + size.x - XGLCornerData::roundCornerR + pos.x,
			//	XGLCornerData::roundCornerData[0].y + XGLCornerData::roundCornerR + pos.y);
			glVertex2fv(XGLCornerData::roundCornerData[0] + pos + XVector2(size.x - XGLCornerData::roundCornerR,XGLCornerData::roundCornerR));
			glEnd();
		}else
		{
			if(withLine)
			{//描绘边框
				glColor4f(0.15f,0.15f,0.15f,a);
				glLineWidth(1.0f);
				drawBox(pos,size);
			}
			glBegin(GL_QUADS);
			if(down) glColor4fv(dColor); 
			else glColor4fv(lColor);
			glVertex2fv(pos);
			glVertex2f(pos.x + size.x,pos.y);
			if(down) glColor4fv(lColor);
			else glColor4fv(dColor);
			glVertex2fv(pos + size);
			glVertex2f(pos.x,pos.y + size.y);
			glEnd();
		}
		XGL::DisableBlend();
	}
	inline void drawCorner(const XVector2& pos,const XVector2& size,int style,int index)
	{
		switch(style)
		{
		case 2:
			glBegin(GL_POLYGON);
			switch(index)
			{
			case 0:
				glVertex2fv(pos + size);
				for(int i = 0;i <= ROUND_CORNER_DENSITY;++ i)	
				{
					glVertex2fv(pos + size + XVector2(XGLCornerData::roundCornerR - XGLCornerData::roundCornerData[i].x,
						XGLCornerData::roundCornerData[i].y - XGLCornerData::roundCornerR));
				}
				break;
			case 1:
				glVertex2f(pos.x,pos.y + size.y);
				for(int i = ROUND_CORNER_DENSITY;i <= (ROUND_CORNER_DENSITY << 1);++ i)	
				{
					glVertex2f(pos.x - (XGLCornerData::roundCornerData[i].x + XGLCornerData::roundCornerR),
						pos.y + size.y + XGLCornerData::roundCornerData[i].y - XGLCornerData::roundCornerR);
				}
				break;
			case 2:
				glVertex2f(pos.x,pos.y);
				for(int i = (ROUND_CORNER_DENSITY << 1);i <= (ROUND_CORNER_DENSITY * 3);++ i)	
				{
					glVertex2f(pos.x - (XGLCornerData::roundCornerData[i].x + XGLCornerData::roundCornerR),
						pos.y + XGLCornerData::roundCornerData[i].y + XGLCornerData::roundCornerR);
				}
				break;
			case 3:
				glVertex2f(pos.x + size.x,pos.y);
				for(int i = (ROUND_CORNER_DENSITY * 3);i < (ROUND_CORNER_DENSITY << 2);++ i)	
				{
					glVertex2f(pos.x + size.x - (XGLCornerData::roundCornerData[i].x - XGLCornerData::roundCornerR),
						pos.y + XGLCornerData::roundCornerData[i].y + XGLCornerData::roundCornerR);
				}
				glVertex2f(pos.x + size.x - (XGLCornerData::roundCornerData[0].x - XGLCornerData::roundCornerR),
					pos.y + XGLCornerData::roundCornerData[0].y + XGLCornerData::roundCornerR);
				break;
			}
			glEnd();
			break;
		case 3:
			glBegin(GL_POLYGON);
			switch(index)
			{
			case 0:
				glVertex2fv(pos + size);
				for(int i = 0;i <= ROUND_CORNER_DENSITY;++ i)	
				{
					glVertex2f(pos.x + size.x + XGLCornerData::roundCornerData[i].x - XGLCornerData::roundCornerR,
						pos.y + size.y - (XGLCornerData::roundCornerData[i].y - XGLCornerData::roundCornerR));
				}
				break;
			case 1:
				glVertex2f(pos.x,pos.y + size.y);
				for(int i = ROUND_CORNER_DENSITY;i <= (ROUND_CORNER_DENSITY << 1);++ i)	
				{
					glVertex2f(pos.x + XGLCornerData::roundCornerData[i].x + XGLCornerData::roundCornerR,
						pos.y + size.y - (XGLCornerData::roundCornerData[i].y - XGLCornerData::roundCornerR));
				}
				break;
			case 2:
				glVertex2f(pos.x,pos.y);
				for(int i = (ROUND_CORNER_DENSITY << 1);i <= (ROUND_CORNER_DENSITY * 3);++ i)	
				{
					glVertex2f(pos.x + XGLCornerData::roundCornerData[i].x + XGLCornerData::roundCornerR,
						pos.y - (XGLCornerData::roundCornerData[i].y + XGLCornerData::roundCornerR));
				}
				glEnd();
				break;
			case 3:
				glVertex2f(pos.x + size.x,pos.y);
				for(int i = (ROUND_CORNER_DENSITY * 3);i < (ROUND_CORNER_DENSITY << 2);++ i)	
				{
					glVertex2f(pos.x + size.x + XGLCornerData::roundCornerData[i].x - XGLCornerData::roundCornerR,
						pos.y - (XGLCornerData::roundCornerData[i].y + XGLCornerData::roundCornerR));
				}
				glVertex2f(pos.x + size.x + XGLCornerData::roundCornerData[0].x - XGLCornerData::roundCornerR,
					pos.y - (XGLCornerData::roundCornerData[0].y + XGLCornerData::roundCornerR));
				break;
			}
			glEnd();
			break;
		}
	}
	inline void cornerData(const XVector2& pos,const XVector2& size,int style,int index)
	{
		switch(style)
		{
		case 1:
			switch(index)
			{
			case 0:
				for(int i = 0;i <= ROUND_CORNER_DENSITY;++ i)	
				{
					//glVertex2f(XGLCornerData::roundCornerData[i].x + size.x - XGLCornerData::roundCornerR + pos.x,
					//	XGLCornerData::roundCornerData[i].y + size.y - XGLCornerData::roundCornerR + pos.y);
					glVertex2fv(XGLCornerData::roundCornerData[i] + size + pos - XGLCornerData::roundCornerR);
				}
				break;
			case 1:
				for(int i = 0;i <= ROUND_CORNER_DENSITY;++ i)
				{
				//	glVertex2f(XGLCornerData::roundCornerData[i + ROUND_CORNER_DENSITY].x + XGLCornerData::roundCornerR + pos.x,
				//		XGLCornerData::roundCornerData[i + ROUND_CORNER_DENSITY].y + size.y - XGLCornerData::roundCornerR + pos.y);
					glVertex2fv(XGLCornerData::roundCornerData[i + ROUND_CORNER_DENSITY] + pos + XVector2(XGLCornerData::roundCornerR,size.y - XGLCornerData::roundCornerR));
				}
				break;
			case 2:
				for(int i = 0;i <= ROUND_CORNER_DENSITY;++ i)
				{//左上角圆角
				//	glVertex2f(XGLCornerData::roundCornerData[i + (ROUND_CORNER_DENSITY << 1)].x + XGLCornerData::roundCornerR + pos.x,
				//		XGLCornerData::roundCornerData[i + (ROUND_CORNER_DENSITY << 1)].y + XGLCornerData::roundCornerR + pos.y);
					glVertex2fv(XGLCornerData::roundCornerData[i + (ROUND_CORNER_DENSITY << 1)] + pos + XGLCornerData::roundCornerR);
				}
				break;
			case 3:
				for(int i = 0;i < ROUND_CORNER_DENSITY;++ i)
				{//左下角圆角
				//	glVertex2f(XGLCornerData::roundCornerData[i + ROUND_CORNER_DENSITY * 3].x + size.x - XGLCornerData::roundCornerR + pos.x,
				//		XGLCornerData::roundCornerData[i + ROUND_CORNER_DENSITY * 3].y + XGLCornerData::roundCornerR + pos.y);
					glVertex2fv(XGLCornerData::roundCornerData[i + ROUND_CORNER_DENSITY * 3] + pos + XVector2(size.x - XGLCornerData::roundCornerR,XGLCornerData::roundCornerR));
				}
				//glVertex2f(XGLCornerData::roundCornerData[0].x + size.x - XGLCornerData::roundCornerR + pos.x,
				//	XGLCornerData::roundCornerData[0].y + XGLCornerData::roundCornerR + pos.y);
				glVertex2fv(XGLCornerData::roundCornerData[0] + pos + XVector2(size.x - XGLCornerData::roundCornerR,XGLCornerData::roundCornerR));
				break;
			}
			break;
		default:
			switch(index)
			{
			case 0:
				glVertex2fv(pos + size + XGLCornerData::roundCornerData[0] - XGLCornerData::roundCornerR);
				glVertex2fv(pos + size);
				glVertex2fv(pos + size + XGLCornerData::roundCornerData[ROUND_CORNER_DENSITY] - XGLCornerData::roundCornerR);
				break;
			case 1:
				glVertex2fv(XGLCornerData::roundCornerData[ROUND_CORNER_DENSITY] + pos + XVector2(XGLCornerData::roundCornerR,size.y - XGLCornerData::roundCornerR));
				glVertex2f(pos.x,pos.y + size.y);
				glVertex2f(pos.x + XGLCornerData::roundCornerR + XGLCornerData::roundCornerData[ROUND_CORNER_DENSITY << 1].x,
					pos.y + size.y - XGLCornerData::roundCornerR + XGLCornerData::roundCornerData[ROUND_CORNER_DENSITY << 1].y);
				break;
			case 2:
				glVertex2fv(XGLCornerData::roundCornerData[ROUND_CORNER_DENSITY << 1] + pos + XGLCornerData::roundCornerR);
				glVertex2fv(pos);
				glVertex2fv(XGLCornerData::roundCornerData[ROUND_CORNER_DENSITY * 3] + pos + XGLCornerData::roundCornerR);
				break;
			case 3:
				glVertex2fv(XGLCornerData::roundCornerData[ROUND_CORNER_DENSITY * 3] + pos + XVector2(size.x - XGLCornerData::roundCornerR,XGLCornerData::roundCornerR));
				glVertex2f(pos.x + size.x,pos.y);
				glVertex2fv(XGLCornerData::roundCornerData[0] + pos + XVector2(size.x - XGLCornerData::roundCornerR,XGLCornerData::roundCornerR));
				break;
			}
			break;
		}
	}
	void drawFillBoxExEx(const XVector2& pos,const XVector2& size,
							float r,float g,float b,
							unsigned char lineStyle,unsigned char cornerStyle,bool down)
	{
		XFColor lColor(r * 1.1f,g * 1.1f,b * 1.1f,1.0f);
		XFColor dColor(r * 0.9f,g * 0.9f,b * 0.9f,1.0f);
		XGL::DisableTexture2D();
		if(cornerStyle != 0 && size.x >= XGLCornerData::roundCornerD && size.y >= XGLCornerData::roundCornerD)
		{//需要描绘圆角
			XGLCornerData::roundCornerInit();
			if(lineStyle != 0)
			{//描绘边框
				glColor3f(0.15f,0.15f,0.15f);
				glLineWidth(1.0f);
				if(lineStyle & 0x01)
				{//上边需要描绘
					glBegin(GL_LINE_STRIP);
					switch((cornerStyle & 0x30) >> 4)
					{
					case 0:glVertex2fv(pos);break;
					case 1:
						for(int i = ROUND_CORNER_DENSITY << 1;i <= ROUND_CORNER_DENSITY * 3;++ i)
						{//左上角圆角
						//	glVertex2f(pos.x + XGLCornerData::roundCornerR + XGLCornerData::roundCornerData[i].x,
						//		pos.y + XGLCornerData::roundCornerR + XGLCornerData::roundCornerData[i].y);
							glVertex2fv(pos + XGLCornerData::roundCornerData[i] + XGLCornerData::roundCornerR);
						}
						break;
					case 2:glVertex2f(pos.x - XGLCornerData::roundCornerR,pos.y);break;
					case 3:
						for(int i = ROUND_CORNER_DENSITY << 1;i <= ROUND_CORNER_DENSITY * 3;++ i)
						{//左上角圆角
							glVertex2f(pos.x + XGLCornerData::roundCornerR + XGLCornerData::roundCornerData[i].x,
								pos.y - (XGLCornerData::roundCornerR + XGLCornerData::roundCornerData[i].y));
						}
						break;
					}
					switch((cornerStyle & 0xc0) >> 6)
					{
					case 0:glVertex2f(pos.x + size.x,pos.y);break;
					case 1:glVertex2f(pos.x + size.x - XGLCornerData::roundCornerR,pos.y);break;
					case 2:glVertex2f(pos.x + size.x + XGLCornerData::roundCornerR,pos.y);break;
					case 3:
						for(int i = ROUND_CORNER_DENSITY * 3;i < ROUND_CORNER_DENSITY << 2;++ i)
						{//左上角圆角
							glVertex2f(pos.x + size.x - XGLCornerData::roundCornerR + XGLCornerData::roundCornerData[i].x,
								pos.y - (XGLCornerData::roundCornerR + XGLCornerData::roundCornerData[i].y));
						}
						glVertex2f(pos.x + size.x - XGLCornerData::roundCornerR + XGLCornerData::roundCornerData[0].x,
							pos.y - (XGLCornerData::roundCornerR + XGLCornerData::roundCornerData[0].y));
						break;
					}
					glEnd();
				}
				if(lineStyle & 0x02)
				{
					glBegin(GL_LINE_STRIP);
					switch((cornerStyle & 0xc0) >> 6)
					{
					case 0:glVertex2f(pos.x + size.x,pos.y);break;
					case 1:
						for(int i = ROUND_CORNER_DENSITY * 3;i < ROUND_CORNER_DENSITY << 2;++ i)
						{
							glVertex2fv(pos + XGLCornerData::roundCornerData[i] + XVector2(size.x - XGLCornerData::roundCornerR,XGLCornerData::roundCornerR));
						}
						//glVertex2f(pos.x + size.x - XGLCornerData::roundCornerR + XGLCornerData::roundCornerData[0].x,
						//	pos.y + XGLCornerData::roundCornerR + XGLCornerData::roundCornerData[0].y);
						glVertex2fv(pos + XGLCornerData::roundCornerData[0] + XVector2(size.x - XGLCornerData::roundCornerR,XGLCornerData::roundCornerR));
						break;
					case 2:					
						for(int i = ROUND_CORNER_DENSITY * 3;i < ROUND_CORNER_DENSITY << 2;++ i)
						{
							glVertex2f(pos.x + size.x - (XGLCornerData::roundCornerData[i].x - XGLCornerData::roundCornerR),
								pos.y + XGLCornerData::roundCornerR + XGLCornerData::roundCornerData[i].y);
						}
						glVertex2f(pos.x + size.x - (XGLCornerData::roundCornerData[0].x - XGLCornerData::roundCornerR),
							pos.y + XGLCornerData::roundCornerR + XGLCornerData::roundCornerData[0].y);
						break;
					case 3:glVertex2f(pos.x + size.x,pos.y - XGLCornerData::roundCornerR);break;
					}
					switch(cornerStyle & 0x03)
					{
					case 0:glVertex2fv(pos + size);break;
					case 1:glVertex2f(pos.x + size.x,pos.y + size.y - XGLCornerData::roundCornerR);break;
					case 2:					
						for(int i = 0;i <= ROUND_CORNER_DENSITY;++ i)
						{
							glVertex2f(pos.x + size.x - (XGLCornerData::roundCornerData[i].x - XGLCornerData::roundCornerR),
								pos.y + size.y - XGLCornerData::roundCornerR + XGLCornerData::roundCornerData[i].y);
						}
						break;
					case 3:glVertex2f(pos.x + size.x,pos.y + size.y + XGLCornerData::roundCornerR);break;
					}
					glEnd();
				}
				if(lineStyle & 0x04)
				{
					glBegin(GL_LINE_STRIP);
					switch(cornerStyle & 0x03)
					{
					case 0:glVertex2fv(pos + size);break;
					case 1:
						for(int i = 0;i <= ROUND_CORNER_DENSITY;++ i)
						{
							glVertex2fv(XGLCornerData::roundCornerData[i] + pos + size - XGLCornerData::roundCornerR);
						}
						break;
					case 2:glVertex2f(pos.x + size.x + XGLCornerData::roundCornerR,pos.y + size.y);break;					
					case 3:
						for(int i = 0;i <= ROUND_CORNER_DENSITY;++ i)
						{
							glVertex2f(pos.x + size.x + XGLCornerData::roundCornerData[i].x - XGLCornerData::roundCornerR,
								pos.y + size.y - (XGLCornerData::roundCornerData[i].y - XGLCornerData::roundCornerR));
						}
						break;
					}
					switch((cornerStyle & 0x0c) >> 2)
					{
					case 0:glVertex2f(pos.x,pos.y + size.y);break;
					case 1:glVertex2f(pos.x + XGLCornerData::roundCornerR,pos.y + size.y);break;
					case 2:glVertex2f(pos.x - XGLCornerData::roundCornerR,pos.y + size.y);break;					
					case 3:
						for(int i = ROUND_CORNER_DENSITY;i <= ROUND_CORNER_DENSITY << 1;++ i)
						{
							glVertex2f(pos.x + XGLCornerData::roundCornerData[i].x + XGLCornerData::roundCornerR,
								pos.y + size.y - (XGLCornerData::roundCornerData[i].y - XGLCornerData::roundCornerR));
						}
						break;
					}
					glEnd();
				}
				if(lineStyle & 0x08)
				{
					glBegin(GL_LINE_STRIP);
					switch((cornerStyle & 0x0c) >> 2)
					{
					case 0:glVertex2f(pos.x,pos.y + size.y);break;
					case 1:					
						for(int i = ROUND_CORNER_DENSITY;i <= ROUND_CORNER_DENSITY << 1;++ i)
						{
							glVertex2fv(XGLCornerData::roundCornerData[i] + pos + XVector2(XGLCornerData::roundCornerR,size.y - XGLCornerData::roundCornerR));
						}
						break;
					case 2:
						for(int i = ROUND_CORNER_DENSITY;i <= ROUND_CORNER_DENSITY << 1;++ i)
						{
							glVertex2f(pos.x - (XGLCornerData::roundCornerData[i].x + XGLCornerData::roundCornerR),
								pos.y + size.y + XGLCornerData::roundCornerData[i].y - XGLCornerData::roundCornerR);
						}
						break;
					case 3:glVertex2f(pos.x,pos.y + size.y + XGLCornerData::roundCornerR);break;
					}
					switch((cornerStyle & 0x30) >> 4)
					{
					case 0:glVertex2fv(pos);break;
					case 1:glVertex2f(pos.x,pos.y + XGLCornerData::roundCornerR);break;
					case 2:
						for(int i = ROUND_CORNER_DENSITY << 1;i <= ROUND_CORNER_DENSITY * 3;++ i)
						{
							glVertex2f(pos.x - (XGLCornerData::roundCornerR + XGLCornerData::roundCornerData[i].x),
								pos.y + XGLCornerData::roundCornerR + XGLCornerData::roundCornerData[i].y);
						}
						break;
					case 3:glVertex2f(pos.x,pos.y - XGLCornerData::roundCornerR);break;
					}
					glEnd();
				}
			}
			if(down) glColor3fv(lColor); 
			else glColor3fv(dColor);
			//下半部分
			if(down) glColor3fv(lColor); 
			else glColor3fv(dColor);
			glBegin(GL_POLYGON);
			cornerData(pos,size,cornerStyle & 0x03,0);	//右下角
			cornerData(pos,size,(cornerStyle & 0x0c) >> 2,1);	//左下角
			glEnd();
			drawCorner(pos,size,cornerStyle & 0x03,0);
			drawCorner(pos,size,(cornerStyle & 0x0c) >> 2,1);
			//上半部分
			if(down) glColor3fv(dColor);
			else glColor3fv(lColor);
			glBegin(GL_POLYGON);
			cornerData(pos,size,(cornerStyle & 0x30) >> 4,2);	//右下角
			cornerData(pos,size,(cornerStyle & 0xc0) >> 6,3);	//左下角
			glEnd();
			drawCorner(pos,size,(cornerStyle & 0x30) >> 4,2);	//左上角
			drawCorner(pos,size,(cornerStyle & 0xc0) >> 6,3);	//右上角
			//中间
			glBegin(GL_POLYGON);
			if(down) glColor3fv(lColor); 
			else glColor3fv(dColor);
			glVertex2fv(XGLCornerData::roundCornerData[0] + size + pos - XGLCornerData::roundCornerR);
			glVertex2fv(XGLCornerData::roundCornerData[ROUND_CORNER_DENSITY << 1] + pos + XVector2(XGLCornerData::roundCornerR,size.y - XGLCornerData::roundCornerR));
			if(down) glColor3fv(dColor);
			else glColor3fv(lColor);
			glVertex2fv(XGLCornerData::roundCornerData[ROUND_CORNER_DENSITY << 1] + pos + XGLCornerData::roundCornerR);
			glVertex2fv(XGLCornerData::roundCornerData[0] + pos + XVector2(size.x - XGLCornerData::roundCornerR,XGLCornerData::roundCornerR));
			glEnd();
		}else
		{//不需要描绘圆角
			if(lineStyle != 0)
			{//描绘边框
				glColor3f(0.15f,0.15f,0.15f);
				glLineWidth(1.0f);
				glBegin(GL_LINES);
				if(lineStyle & 0x01)
				{
					glVertex2fv(pos);
					glVertex2f(pos.x + size.x,pos.y);
				}
				if(lineStyle & 0x02)
				{
					glVertex2f(pos.x + size.x,pos.y);
					glVertex2fv(pos + size);
				}
				if(lineStyle & 0x04)
				{
					glVertex2fv(pos + size);
					glVertex2f(pos.x,pos.y + size.y);
				}
				if(lineStyle & 0x08)
				{
					glVertex2f(pos.x,pos.y + size.y);
					glVertex2fv(pos);
				}
				glEnd();
			}
			glBegin(GL_QUADS);
			if(down) glColor3fv(dColor); 
			else glColor3fv(lColor);
			glVertex2f(pos.x,pos.y + XGLCornerData::roundCornerR);
			glVertex2fv(pos);
			glVertex2f(pos.x + size.x,pos.y);
			glVertex2f(pos.x + size.x,pos.y + XGLCornerData::roundCornerR);
			glEnd();
			glBegin(GL_QUADS);
			if(down) glColor3fv(lColor);
			else glColor3fv(dColor);
			glVertex2f(pos.x + size.x,pos.y + size.y - XGLCornerData::roundCornerR);
			glVertex2f(pos.x + size.x,pos.y + size.y);
			glVertex2f(pos.x,pos.y + size.y);
			glVertex2f(pos.x,pos.y + size.y - XGLCornerData::roundCornerR);
			glEnd();
			glBegin(GL_QUADS);
			if(down) glColor3fv(dColor); 
			else glColor3fv(lColor);
			glVertex2f(pos.x,pos.y + XGLCornerData::roundCornerR);
			glVertex2f(pos.x + size.x,pos.y + XGLCornerData::roundCornerR);
			if(down) glColor3fv(lColor);
			else glColor3fv(dColor);
			glVertex2f(pos.x + size.x,pos.y + size.y - XGLCornerData::roundCornerR);
			glVertex2f(pos.x,pos.y + size.y - XGLCornerData::roundCornerR);
			glEnd();
		}
	}
	void drawFillBoxExExA(const XVector2& pos,const XVector2& size,
							float r,float g,float b,float a,
							unsigned char lineStyle,unsigned char cornerStyle,bool down)
	{
		XFColor lColor(r * 1.1f,g * 1.1f,b * 1.1f,a);
		XFColor dColor(r * 0.9f,g * 0.9f,b * 0.9f,a);
		XGL::DisableTexture2D();
		XGL::EnableBlend();
		XGL::SetBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		if(cornerStyle != 0 && size.x >= XGLCornerData::roundCornerD && size.y >= XGLCornerData::roundCornerD)
		{//需要描绘圆角
			XGLCornerData::roundCornerInit();
			if(lineStyle != 0)
			{//描绘边框
				glColor4f(0.15f,0.15f,0.15f,a);
				glLineWidth(1.0f);
				if(lineStyle & 0x01)
				{//上边需要描绘
					glBegin(GL_LINE_STRIP);
					switch((cornerStyle & 0x30) >> 4)
					{
					case 0:glVertex2fv(pos);break;
					case 1:
						for(int i = ROUND_CORNER_DENSITY << 1;i <= ROUND_CORNER_DENSITY * 3;++ i)
						{//左上角圆角
							glVertex2f(pos.x + XGLCornerData::roundCornerR + XGLCornerData::roundCornerData[i].x,
								pos.y + XGLCornerData::roundCornerR + XGLCornerData::roundCornerData[i].y);
						}
						break;
					case 2:glVertex2f(pos.x - XGLCornerData::roundCornerR,pos.y);break;
					case 3:
						for(int i = ROUND_CORNER_DENSITY << 1;i <= ROUND_CORNER_DENSITY * 3;++ i)
						{//左上角圆角
							glVertex2f(pos.x + XGLCornerData::roundCornerR + XGLCornerData::roundCornerData[i].x,
								pos.y - (XGLCornerData::roundCornerR + XGLCornerData::roundCornerData[i].y));
						}
						break;
					}
					switch((cornerStyle & 0xc0) >> 6)
					{
					case 0:glVertex2f(pos.x + size.x,pos.y);break;
					case 1:glVertex2f(pos.x + size.x - XGLCornerData::roundCornerR,pos.y);break;
					case 2:glVertex2f(pos.x + size.x + XGLCornerData::roundCornerR,pos.y);break;
					case 3:
						for(int i = ROUND_CORNER_DENSITY * 3;i < ROUND_CORNER_DENSITY << 2;++ i)
						{//左上角圆角
							glVertex2f(pos.x + size.x - XGLCornerData::roundCornerR + XGLCornerData::roundCornerData[i].x,
								pos.y - (XGLCornerData::roundCornerR + XGLCornerData::roundCornerData[i].y));
						}
						glVertex2f(pos.x + size.x - XGLCornerData::roundCornerR + XGLCornerData::roundCornerData[0].x,
							pos.y - (XGLCornerData::roundCornerR + XGLCornerData::roundCornerData[0].y));
						break;
					}
					glEnd();
				}
				if(lineStyle & 0x02)
				{
					glBegin(GL_LINE_STRIP);
					switch((cornerStyle & 0xc0) >> 6)
					{
					case 0:glVertex2f(pos.x + size.x,pos.y);break;
					case 1:
						for(int i = ROUND_CORNER_DENSITY * 3;i < ROUND_CORNER_DENSITY << 2;++ i)
						{
							glVertex2fv(XGLCornerData::roundCornerData[i] + pos + XVector2(size.x - XGLCornerData::roundCornerR,XGLCornerData::roundCornerR));
						}
						glVertex2fv(XGLCornerData::roundCornerData[0] + pos + XVector2(size.x - XGLCornerData::roundCornerR,XGLCornerData::roundCornerR));
						break;
					case 2:					
						for(int i = ROUND_CORNER_DENSITY * 3;i < ROUND_CORNER_DENSITY << 2;++ i)
						{
							glVertex2f(pos.x + size.x - (XGLCornerData::roundCornerData[i].x - XGLCornerData::roundCornerR),
								pos.y + XGLCornerData::roundCornerR + XGLCornerData::roundCornerData[i].y);
						}
						glVertex2f(pos.x + size.x - (XGLCornerData::roundCornerData[0].x - XGLCornerData::roundCornerR),
							pos.y + XGLCornerData::roundCornerR + XGLCornerData::roundCornerData[0].y);
						break;
					case 3:glVertex2f(pos.x + size.x,pos.y - XGLCornerData::roundCornerR);break;
					}
					switch(cornerStyle & 0x03)
					{
					case 0:glVertex2fv(pos + size);break;
					case 1:glVertex2f(pos.x + size.x,pos.y + size.y - XGLCornerData::roundCornerR);break;
					case 2:					
						for(int i = 0;i <= ROUND_CORNER_DENSITY;++ i)
						{
							glVertex2f(pos.x + size.x - (XGLCornerData::roundCornerData[i].x - XGLCornerData::roundCornerR),
								pos.y + size.y - XGLCornerData::roundCornerR + XGLCornerData::roundCornerData[i].y);
						}
						break;
					case 3:glVertex2f(pos.x + size.x,pos.y + size.y + XGLCornerData::roundCornerR);break;
					}
					glEnd();
				}
				if(lineStyle & 0x04)
				{
					glBegin(GL_LINE_STRIP);
					switch(cornerStyle & 0x03)
					{
					case 0:glVertex2fv(pos + size);break;
					case 1:
						for(int i = 0;i <= ROUND_CORNER_DENSITY;++ i)
						{
							glVertex2fv(XGLCornerData::roundCornerData[i] + pos + size - XGLCornerData::roundCornerR);
						}
						break;
					case 2:glVertex2f(pos.x + size.x + XGLCornerData::roundCornerR,pos.y + size.y);break;					
					case 3:
						for(int i = 0;i <= ROUND_CORNER_DENSITY;++ i)
						{
							glVertex2f(pos.x + size.x + XGLCornerData::roundCornerData[i].x - XGLCornerData::roundCornerR,
								pos.y + size.y - (XGLCornerData::roundCornerData[i].y - XGLCornerData::roundCornerR));
						}
						break;
					}
					switch((cornerStyle & 0x0c) >> 2)
					{
					case 0:glVertex2f(pos.x,pos.y + size.y);break;
					case 1:glVertex2f(pos.x + XGLCornerData::roundCornerR,pos.y + size.y);break;
					case 2:glVertex2f(pos.x - XGLCornerData::roundCornerR,pos.y + size.y);break;					
					case 3:
						for(int i = ROUND_CORNER_DENSITY;i <= ROUND_CORNER_DENSITY << 1;++ i)
						{
							glVertex2f(pos.x + XGLCornerData::roundCornerData[i].x + XGLCornerData::roundCornerR,
								pos.y + size.y - (XGLCornerData::roundCornerData[i].y - XGLCornerData::roundCornerR));
						}
						break;
					}
					glEnd();
				}
				if(lineStyle & 0x08)
				{
					glBegin(GL_LINE_STRIP);
					switch((cornerStyle & 0x0c) >> 2)
					{
					case 0:glVertex2f(pos.x,pos.y + size.y);break;
					case 1:					
						for(int i = ROUND_CORNER_DENSITY;i <= ROUND_CORNER_DENSITY << 1;++ i)
						{
							glVertex2fv(XGLCornerData::roundCornerData[i] + pos + XVector2(XGLCornerData::roundCornerR,size.y - XGLCornerData::roundCornerR));
						}
						break;
					case 2:
						for(int i = ROUND_CORNER_DENSITY;i <= ROUND_CORNER_DENSITY << 1;++ i)
						{
							glVertex2f(pos.x - (XGLCornerData::roundCornerData[i].x + XGLCornerData::roundCornerR),
								pos.y + size.y + XGLCornerData::roundCornerData[i].y - XGLCornerData::roundCornerR);
						}
						break;
					case 3:glVertex2f(pos.x,pos.y + size.y + XGLCornerData::roundCornerR);break;
					}
					switch((cornerStyle & 0x30) >> 4)
					{
					case 0:glVertex2fv(pos);break;
					case 1:glVertex2f(pos.x,pos.y + XGLCornerData::roundCornerR);break;
					case 2:
						for(int i = ROUND_CORNER_DENSITY << 1;i <= ROUND_CORNER_DENSITY * 3;++ i)
						{
							glVertex2f(pos.x - (XGLCornerData::roundCornerR + XGLCornerData::roundCornerData[i].x),
								pos.y + XGLCornerData::roundCornerR + XGLCornerData::roundCornerData[i].y);
						}
						break;
					case 3:glVertex2f(pos.x,pos.y - XGLCornerData::roundCornerR);break;
					}
					glEnd();
				}
			}
			if(down) glColor4fv(lColor); 
			else glColor4fv(dColor);
			//下半部分
			if(down) glColor4fv(lColor); 
			else glColor4fv(dColor);
			glBegin(GL_POLYGON);
			cornerData(pos,size,cornerStyle & 0x03,0);	//右下角
			cornerData(pos,size,(cornerStyle & 0x0c) >> 2,1);	//左下角
			glEnd();
			drawCorner(pos,size,cornerStyle & 0x03,0);
			drawCorner(pos,size,(cornerStyle & 0x0c) >> 2,1);
			//上半部分
			if(down) glColor4fv(dColor);
			else glColor4fv(lColor);
			glBegin(GL_POLYGON);
			cornerData(pos,size,(cornerStyle & 0x30) >> 4,2);	//右下角
			cornerData(pos,size,(cornerStyle & 0xc0) >> 6,3);	//左下角
			glEnd();
			drawCorner(pos,size,(cornerStyle & 0x30) >> 4,2);	//左上角
			drawCorner(pos,size,(cornerStyle & 0xc0) >> 6,3);	//右上角
			//中间
			glBegin(GL_POLYGON);
			if(down) glColor4fv(lColor); 
			else glColor4fv(dColor);
			glVertex2fv(XGLCornerData::roundCornerData[0] + size + pos - XGLCornerData::roundCornerR);
			glVertex2fv(XGLCornerData::roundCornerData[ROUND_CORNER_DENSITY << 1] + pos + XVector2(XGLCornerData::roundCornerR,size.y - XGLCornerData::roundCornerR));
			if(down) glColor4fv(dColor);
			else glColor4fv(lColor);
			glVertex2fv(XGLCornerData::roundCornerData[ROUND_CORNER_DENSITY << 1] + pos + XGLCornerData::roundCornerR);
			glVertex2fv(XGLCornerData::roundCornerData[0] + pos + XVector2(size.x - XGLCornerData::roundCornerR,XGLCornerData::roundCornerR));
			glEnd();
		}else
		{//不需要描绘圆角
			if(lineStyle != 0)
			{//描绘边框
				glColor4f(0.15f,0.15f,0.15f,a);
				glLineWidth(1.0f);
				glBegin(GL_LINES);
				if(lineStyle & 0x01)
				{
					glVertex2fv(pos);
					glVertex2f(pos.x + size.x,pos.y);
				}
				if(lineStyle & 0x02)
				{
					glVertex2f(pos.x + size.x,pos.y);
					glVertex2fv(pos + size);
				}
				if(lineStyle & 0x04)
				{
					glVertex2fv(pos + size);
					glVertex2f(pos.x,pos.y + size.y);
				}
				if(lineStyle & 0x08)
				{
					glVertex2f(pos.x,pos.y + size.y);
					glVertex2fv(pos);
				}
				glEnd();
			}
			glBegin(GL_QUADS);
			if(down) glColor4fv(dColor); 
			else glColor4fv(lColor);
			glVertex2f(pos.x,pos.y + XGLCornerData::roundCornerR);
			glVertex2fv(pos);
			glVertex2f(pos.x + size.x,pos.y);
			glVertex2f(pos.x + size.x,pos.y + XGLCornerData::roundCornerR);
			glEnd();
			glBegin(GL_QUADS);
			if(down) glColor4fv(lColor);
			else glColor4fv(dColor);
			glVertex2f(pos.x + size.x,pos.y + size.y - XGLCornerData::roundCornerR);
			glVertex2f(pos.x + size.x,pos.y + size.y);
			glVertex2f(pos.x,pos.y + size.y);
			glVertex2f(pos.x,pos.y + size.y - XGLCornerData::roundCornerR);
			glEnd();
			glBegin(GL_QUADS);
			if(down) glColor4fv(dColor); 
			else glColor4fv(lColor);
			glVertex2f(pos.x,pos.y + XGLCornerData::roundCornerR);
			glVertex2f(pos.x + size.x,pos.y + XGLCornerData::roundCornerR);
			if(down) glColor4fv(lColor);
			else glColor4fv(dColor);
			glVertex2f(pos.x + size.x,pos.y + size.y - XGLCornerData::roundCornerR);
			glVertex2f(pos.x,pos.y + size.y - XGLCornerData::roundCornerR);
			glEnd();
		}
		XGL::DisableBlend();
	}
	void drawFillBox(const XVector2& p0,const XVector2& p1,const XVector2& p2,const XVector2& p3,
					 float r,float g,float b)
	{
		glColor3f(r,g,b);
		XGL::DisableTexture2D();
		glBegin(GL_QUADS);
		glVertex2fv(p0);
		glVertex2fv(p1);
		glVertex2fv(p2);
		glVertex2fv(p3);
		glEnd();
	}
	void drawFillBoxEx(const XVector2& p0,const XVector2& p1,const XVector2& p2,const XVector2& p3,
					 float r,float g,float b,
					 bool withLine)
	{
		XGL::DisableTexture2D();
		if(withLine)
		{//描绘边框
			glColor3f(0.15f,0.15f,0.15f);
			glLineWidth(1.0f);
			drawBoxS(p0,p1,p2,p3);
		}
		glBegin(GL_QUADS);
		glColor3f(r * 1.1f,g * 1.1f,b * 1.1f);
		glVertex2fv(p0);
		glVertex2fv(p1);
		glColor3f(r * 0.9f,g * 0.9f,b * 0.9f);
		glVertex2fv(p2);
		glVertex2fv(p3);
		glEnd();
	}
	void drawFillPolygon(const XVector2 *p,int sum,float r,float g,float b)
	{
		glColor3f(r,g,b);
		XGL::DisableTexture2D();
		glBegin(GL_POLYGON);
		for(int i = 0;i < sum;++ i)
		{
			glVertex2fv(p[i]);
		}
		glEnd();
	}
	void drawFillPolygon(const XVector2 *p,int sum,const XVector2 &pos,float angle,float r,float g,float b)
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glTranslatef(pos.x,pos.y, 0);
		glRotatef(angle,0,0,1);

		glColor3f(r,g,b);
		XGL::DisableTexture2D();
		glBegin(GL_POLYGON);
		for(int i = 0;i < sum;++ i)
		{
			glVertex2fv(p[i]);
		}
		glEnd();
		glPopMatrix();
	}
	void drawFillPolygonEx(const XVector2 *p,const float *c,int sum,float r,float g,float b,bool withLine)
	{
		XGL::DisableTexture2D();
		if(withLine)
		{//描绘边框
			glColor3f(0.15f,0.15f,0.15f);
			glLineWidth(1.0f);
			glBegin(GL_LINE_STRIP);
			for(int i = 0;i < sum;++ i)
			{
				glVertex2fv(p[i]);
			}
			glEnd();
		}
		glBegin(GL_POLYGON);
		for(int i = 0;i < sum;++ i)
		{
			glColor3f(r * c[i],g * c[i],b * c[i]);
			glVertex2fv(p[i]);
		}
		glEnd();
	}
	void drawFillPolygonExA(const XVector2 *p,const float *c,int sum,float r,float g,float b,float a,bool withLine)
	{
		XGL::DisableTexture2D();
		XGL::EnableBlend();
		XGL::SetBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		if(withLine)
		{//描绘边框
			glColor4f(0.15f,0.15f,0.15f,a);
			glLineWidth(1.0f);
			glBegin(GL_LINE_STRIP);
			for(int i = 0;i < sum;++ i)
			{
				glVertex2fv(p[i]);
			}
			glEnd();
		}
		glBegin(GL_POLYGON);
		for(int i = 0;i < sum;++ i)
		{
			glColor4f(r * c[i],g * c[i],b * c[i],a);
			glVertex2fv(p[i]);
		}
		glEnd();
		XGL::DisableBlend();
	}
	void drawFillBoxBW(const XVector2& pos,const XVector2& size,const XVector2& cSize)
	{
		XGL::DisableTexture2D();
		glColor3f(1.0f,1.0f,1.0f);
		//先描绘一个白色底
		drawFillBoxS(pos.x,pos.y,size.x,size.y);
		int w = size.x / cSize.x;
		int h = size.y / cSize.y;
		XVector2 tempPos;
		glColor3f(0.0f,0.0f,0.0f);
		//方法1
		glBegin(GL_QUADS);
		for(int i = 0;i < w;++ i)
		{
			tempPos.x = pos.x + i * cSize.x;
			for(int j = 0;j < h;++ j)
			{
				if((i + j)%2 == 0)
				{
					tempPos.y = pos.y + j * cSize.y;
					glVertex2fv(tempPos);
					glVertex2f(tempPos.x + cSize.x,tempPos.y);
					glVertex2f(tempPos.x + cSize.x,tempPos.y + cSize.y);
					glVertex2f(tempPos.x,tempPos.y + cSize.y);
				}
			}
		}
		glEnd();
		//方法2
		//int arraySize = w * (h >> 1) * 4;
		//XVector2 *vPoint = XMem::createArrayMem<XVector2>(arraySize);
		//int index = 0;
		//for(int i = 0;i < w;++ i)
		//{
		//	tempPos.x = pos.x + i * cSize.x;
		//	for(int j = 0;j < h;++ j)
		//	{
		//		if((i + j)%2 == 0)
		//		{
		//			tempPos.y = pos.y + j * cSize.y;
		//			vPoint[(index << 2)] = tempPos;
		//			vPoint[(index << 2) + 1].set(tempPos.x + cSize.x,tempPos.y);
		//			vPoint[(index << 2) + 2].set(tempPos.x + cSize.x,tempPos.y + cSize.y);
		//			vPoint[(index << 2) + 3].set(tempPos.x,tempPos.y + cSize.y);
		//			++ index;
		//		}
		//	}
		//}
		////下面开始描绘黑色的方块
		//glEnableClientState(GL_VERTEX_ARRAY);		//表示启用顶点数组
		//glVertexPointer(2, GL_FLOAT, 0, vPoint);	//2表示每个顶点由两个量组成，GL_FLOAT表示每个量都是	
		////glEnableClientState(GL_TEXTURE_COORD_ARRAY);//启用颜色数组
		////glTexCoordPointer(2, GL_FLOAT, 0, m_uPoint);
		//glDrawArrays(GL_QUADS,0,arraySize);
		//XGL::DisableTexture2D();
		//XMem::XDELETE_ARRAY(vPoint);
	}
	void drawTexture(XTexture &tex,const XVector2 &pos,int cW,int cH,
		XVector2 *vArray,XVector2 *uArray,int arrayW,int arrayH)
	{
		if(vArray == NULL || uArray == NULL) return;
		int tW = tex.m_w;
		int tH = tex.m_h;
		int ux = (tW - cW) >> 1,uy = (tH - cH) >> 1;
		int vx = -(cW >> 1),vy = -(cH >> 1);

		XGL::EnableTexture2D();
		XGL::DisableBlend();
	#if WITHXSPRITE_EX
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);   
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
	#endif
		glMatrixMode(GL_TEXTURE);
		glPushMatrix();
		glLoadIdentity();
		glOrtho(0,tW << 1, 0,tH << 1, -1, 1);
			
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glTranslatef(pos.x + (cW >> 1),pos.y + (cH >> 1), 0);

		XGL::BindTexture2D(tex.m_texture);
		//glActiveTexture(GL_TEXTURE0);
		glColor4f(1.0f,1.0f,1.0f,1.0f);

		for(int j = 0;j < arrayH - 1;++ j)
		{
			glBegin(GL_TRIANGLE_STRIP);
			for(int i = 0;i < arrayW;++ i)
			{	//左上
				int offsetTemp = i + j * arrayW;
				glTexCoord2f(ux + uArray[offsetTemp].x,uy + uArray[offsetTemp].y);	//u
				glVertex2f(vx + vArray[offsetTemp].x,vy + vArray[offsetTemp].y);	//v

				glTexCoord2f(ux + uArray[offsetTemp + arrayW].x,
					uy + uArray[offsetTemp + arrayW].y);	//u
				glVertex2f(vx + vArray[offsetTemp + arrayW].x,
					vy + vArray[offsetTemp + arrayW].y);	//v
			}
			glEnd();
		}

		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
		glMatrixMode(GL_TEXTURE);
		glPopMatrix();
	#if WITHXSPRITE_EX
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);   
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
	#endif
		XGL::DisableBlend();
	}
	void drawTextureEx(unsigned int tex,const XVector2 &pos,const XVector2 &size,
						XVector2 *vArray,XVector2 *uArray,int arrayW,int arrayH,bool blend)
	{
		XGL::EnableTexture2D();
		if(blend)
		{
			XGL::EnableBlend();
			XGL::SetBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}else
		{
			XGL::DisableBlend();
		}
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glTranslatef(pos.x,pos.y,0);
		glScalef(size.x,size.y,1.0);

		XGL::BindTexture2D(tex);
		glColor4f(1.0f,1.0f,1.0f,1.0f);

		for(int j = 0;j < arrayH - 1;++ j)
		{
			glBegin(GL_TRIANGLE_STRIP);
			for(int i = 0;i < arrayW;++ i)
			{	//左上
				int offsetTemp = i + j * arrayW;
				glTexCoord2fv(uArray[offsetTemp]);	//u
				glVertex2fv(vArray[offsetTemp]);	//v

				glTexCoord2fv(uArray[offsetTemp + arrayW]);	//u
				glVertex2fv(vArray[offsetTemp + arrayW]);	//v
			}
			glEnd();
		}

		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
		XGL::DisableBlend();
	}
	void drawTexture(unsigned int tex,const XVector2 &pos,int texW,int texH,bool blend)
	{
		XGL::EnableTexture2D();
		if(blend)
		{
			XGL::EnableBlend();
			XGL::SetBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}else
		{
			XGL::DisableBlend();
		}
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glTranslatef(pos.x,pos.y,0);
		//glScalef(0.5f,0.5f,1.0);

		XGL::BindTexture2D(tex);
		glColor4f(1.0f,1.0f,1.0f,1.0f);

		glBegin(GL_QUADS);
			glTexCoord2f(0,0);	//u
			glVertex2f(0,0);	//v
			glTexCoord2f(1,0);	//u
			glVertex2f(texW,0);	//v
			glTexCoord2f(1,1);	//u
			glVertex2f(texW,texH);	//v
			glTexCoord2f(0,1);	//u
			glVertex2f(0,texH);	//v
		glEnd();

		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
		XGL::DisableBlend();
	}
}
namespace XGL
{
	GLuint emptyTexture(int x,int y,XColorMode mode)
	{
		GLuint textureID;
		glGenTextures(1, &textureID);
		XGL::BindTexture2D(textureID);		
		//效果好
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	#if WITHXSPRITE_EX
		if(mode == COLOR_DEPTH)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		}else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}
	#endif
		//速度快
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		switch(mode)
		{
		case COLOR_RGBA:	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,x,y,0,GL_RGBA,GL_UNSIGNED_BYTE,0);	break;
		case COLOR_RGB:		glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,x,y,0,GL_RGB,GL_UNSIGNED_BYTE,0);	break;
	//	case COLOR_GRAY:	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,x,y,0,GL_RGB,GL_UNSIGNED_BYTE,0);	break;	//感觉存在问题
		case COLOR_GRAY:	glTexImage2D(GL_TEXTURE_2D,0,GL_LUMINANCE,x,y,0,GL_LUMINANCE,GL_UNSIGNED_BYTE,0);break;	//修改
		case COLOR_RGBA_F:	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA32F,x,y,0,GL_RGBA,GL_FLOAT,0);		break;
	//	case COLOR_GRAY_F:	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB32F,x,y,0,GL_RGB,GL_FLOAT,0);		break;	//感觉存在问题
		case COLOR_GRAY_F:	glTexImage2D(GL_TEXTURE_2D,0,GL_R32F,x,y,0,GL_RED,GL_FLOAT,0);			break;	//修改
		case COLOR_BGRA:	glTexImage2D(GL_TEXTURE_2D,0,GL_BGRA,x,y,0,GL_BGRA,GL_UNSIGNED_BYTE,0);	break;
		case COLOR_BGR:		glTexImage2D(GL_TEXTURE_2D,0,GL_BGR,x,y,0,GL_BGR,GL_UNSIGNED_BYTE,0);	break;
		case COLOR_DEPTH:	glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT,x,y,0,GL_DEPTH_COMPONENT,GL_UNSIGNED_BYTE,0);break;
		case COLOR_RED:		glTexImage2D(GL_TEXTURE_2D,0,GL_RED,x,y,0,GL_RED,GL_UNSIGNED_BYTE,0);		break;	//修改
		}
#if WITH_ENGINE_STATISTICS
		if (gFrameworkData.pFrameWork != NULL)
			gFrameworkData.pFrameWork->addStaticsticsTexInfo(textureID);
#endif
		//glClear(GL_COLOR_BUFFER_BIT);
		//初始化像素
		//glClearTexImage();	//这个貌似要GL3.0才支持，目前的版本好像不支持
		XGL::BindTexture2D(0);
		return textureID;
	}
}
void XTexture::updateTexture(const XPBO &pbo)		//使用指定数据更新贴图
{
	if(!m_isLoaded) return;
	pbo.bind();
	updateTextureX(NULL);
	//pbo.unbind();
	glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB,0);
}
void XTexture::updateTextureR2B(const XPBO &pbo)		//使用指定数据更新贴图
{
	if(!m_isLoaded) return;
	pbo.bind();
	updateTextureR2BX(NULL);
	//pbo.unbind();
	glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB,0);
}
void XTexture::updateTextureEx(const void *data)		//初步测试这个性能会比下面的性能好
{
	if(!m_isLoaded || data == NULL) return;	//PBO的时候可以为 NULL
	assert(glIsTexture(m_texture));
	//if(!glIsTexture(m_texture)) return;
	XGL::EnableTexture2D();
	XGL::BindTexture2D(m_texture);
	switch(m_mode)
	{
	case COLOR_RGBA:	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,m_w,m_h,0,GL_RGBA,GL_UNSIGNED_BYTE,data);	break;
	case COLOR_RGB:		glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,m_w,m_h,0,GL_RGB,GL_UNSIGNED_BYTE,data);	break;
	case COLOR_GRAY:	glTexImage2D(GL_TEXTURE_2D,0,GL_LUMINANCE,m_w,m_h,0,GL_LUMINANCE,GL_UNSIGNED_BYTE,data);	break;
	case COLOR_RGBA_F:	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA32F,m_w,m_h,0,GL_RGBA,GL_FLOAT,data);		break;
	case COLOR_GRAY_F:	glTexImage2D(GL_TEXTURE_2D,0,GL_R32F,m_w,m_h,0,GL_RED,GL_FLOAT,data);			break;
	case COLOR_BGRA:	glTexImage2D(GL_TEXTURE_2D,0,GL_BGRA,m_w,m_h,0,GL_BGRA,GL_UNSIGNED_BYTE,data);	break;
	case COLOR_BGR:		glTexImage2D(GL_TEXTURE_2D,0,GL_BGR,m_w,m_h,0,GL_BGR,GL_UNSIGNED_BYTE,data);	break;
	case COLOR_RED:		glTexImage2D(GL_TEXTURE_2D,0,GL_RED,m_w,m_h,0,GL_RED,GL_UNSIGNED_BYTE,data);	break;
	default:	//其他格式不支持
		break;
	}
}
void XTexture::updateTextureR2BEx(const void *data)	//初步测试这个性能会比下面的性能好
{
	if(!m_isLoaded || data == NULL) return;	//PBO的时候可以为 NULL
	assert(glIsTexture(m_texture));
	//if(!glIsTexture(m_texture)) return;
	XGL::EnableTexture2D();
	XGL::BindTexture2D(m_texture);
	switch(m_mode)
	{
	case COLOR_RGBA:	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,m_w,m_h,0,GL_BGRA,GL_UNSIGNED_BYTE,data);	break;
	case COLOR_RGB:		glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,m_w,m_h,0,GL_BGR,GL_UNSIGNED_BYTE,data);	break;
	case COLOR_GRAY:	glTexImage2D(GL_TEXTURE_2D,0,GL_LUMINANCE,m_w,m_h,0,GL_LUMINANCE,GL_UNSIGNED_BYTE,data);	break;
	case COLOR_RGBA_F:	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA32F,m_w,m_h,0,GL_BGRA,GL_FLOAT,data);		break;
	case COLOR_GRAY_F:	glTexImage2D(GL_TEXTURE_2D,0,GL_R32F,m_w,m_h,0,GL_RED,GL_FLOAT,data);			break;
	case COLOR_BGRA:	glTexImage2D(GL_TEXTURE_2D,0,GL_BGRA,m_w,m_h,0,GL_RGBA,GL_UNSIGNED_BYTE,data);	break;
	case COLOR_BGR:		glTexImage2D(GL_TEXTURE_2D,0,GL_BGR,m_w,m_h,0,GL_RGB,GL_UNSIGNED_BYTE,data);	break;
	case COLOR_RED:		glTexImage2D(GL_TEXTURE_2D,0,GL_RED,m_w,m_h,0,GL_RED,GL_UNSIGNED_BYTE,data);	break;
	default:	//其他格式不支持
		break;
	}
}
void XTexture::updateTextureX(const void *data,int w,int h,int x,int y)
{
	assert(glIsTexture(m_texture));
	//if(!glIsTexture(m_texture)) return;
	if(w <= 0) w = m_w;
	if(h <= 0) h = m_h;
	XGL::EnableTexture2D();
	XGL::BindTexture2D(m_texture);
	switch(m_mode)
	{
	case COLOR_RGBA:	glTexSubImage2D(GL_TEXTURE_2D,0,x,y,w,h,GL_RGBA,GL_UNSIGNED_BYTE,data);			break;
	case COLOR_RGB:		glTexSubImage2D(GL_TEXTURE_2D,0,x,y,w,h,GL_RGB,GL_UNSIGNED_BYTE,data);			break;
	case COLOR_GRAY:	glTexSubImage2D(GL_TEXTURE_2D,0,x,y,w,h,GL_LUMINANCE,GL_UNSIGNED_BYTE,data);	break;
	case COLOR_RGBA_F:	glTexSubImage2D(GL_TEXTURE_2D,0,x,y,w,h,GL_RGBA,GL_FLOAT,data);					break;
	case COLOR_GRAY_F:	glTexSubImage2D(GL_TEXTURE_2D,0,x,y,w,h,GL_RED,GL_FLOAT,data);					break;
	case COLOR_BGRA:	glTexSubImage2D(GL_TEXTURE_2D,0,x,y,w,h,GL_BGRA,GL_UNSIGNED_BYTE,data);			break;
	case COLOR_BGR:		glTexSubImage2D(GL_TEXTURE_2D,0,x,y,w,h,GL_BGR,GL_UNSIGNED_BYTE,data);			break;
	case COLOR_RED:		glTexSubImage2D(GL_TEXTURE_2D,0,x,y,w,h,GL_RED,GL_UNSIGNED_BYTE,data);			break;
	default:	//其他格式不支持
		break;
	}
}
void XTexture::updateTextureR2BX(const void *data,int w,int h,int x,int y)
{
	assert(glIsTexture(m_texture));
	//if(!glIsTexture(m_texture)) return;
	if(w <= 0) w = m_w;
	if(h <= 0) h = m_h;
	XGL::EnableTexture2D();
	XGL::BindTexture2D(m_texture);
	switch(m_mode)
	{
	case COLOR_RGBA:	glTexSubImage2D(GL_TEXTURE_2D,0,x,y,w,h,GL_BGRA,GL_UNSIGNED_BYTE,data);		break;
	case COLOR_RGB:		glTexSubImage2D(GL_TEXTURE_2D,0,x,y,w,h,GL_BGR,GL_UNSIGNED_BYTE,data);		break;
	case COLOR_GRAY:	glTexSubImage2D(GL_TEXTURE_2D,0,x,y,w,h,GL_LUMINANCE,GL_UNSIGNED_BYTE,data);break;
	case COLOR_RGBA_F:	glTexSubImage2D(GL_TEXTURE_2D,0,x,y,w,h,GL_RGBA,GL_FLOAT,data);				break;
	case COLOR_GRAY_F:	glTexSubImage2D(GL_TEXTURE_2D,0,x,y,w,h,GL_RED,GL_FLOAT,data);				break;
	case COLOR_BGRA:	glTexSubImage2D(GL_TEXTURE_2D,0,x,y,w,h,GL_RGBA,GL_UNSIGNED_BYTE,data);		break;
	case COLOR_BGR:		glTexSubImage2D(GL_TEXTURE_2D,0,x,y,w,h,GL_RGB,GL_UNSIGNED_BYTE,data);		break;
	case COLOR_RED:		glTexSubImage2D(GL_TEXTURE_2D,0,x,y,w,h,GL_RED,GL_UNSIGNED_BYTE,data);		break;
	default:	//其他格式不支持
		break;
	}
}
XBool XTexture::createTexture(int w,int h,XColorMode mode)
{
	if(m_isLoaded) return XTrue;	//如果资源已经载入了，则不能重复载入
	if(w <= 0 ||
		h <= 0) return XFalse;
	try
	{
#if TEX_DEBUG1
		++ CPSum;
		printf("%d-\n",CPSum);
#endif
		m_cp = new XSCounter;
		if(m_cp == NULL) return XFalse;
	}catch(...)
	{
		return XFalse;
	}
	m_mode = mode;

	m_w = w;
	m_h = h;
	m_texture = XGL::emptyTexture(m_w,m_h,m_mode);

	m_isLoaded = XTrue;
	return XTrue;
}
XBool XTexture::createWithTexture(int w,int h,XColorMode mode,unsigned int tex)
{
	if(m_isLoaded) return XTrue;	//如果资源已经载入了，则不能重复载入
	if(w <= 0 ||
		h <= 0) return XFalse;
	try
	{
#if TEX_DEBUG1
		++ CPSum;
		printf("%d-\n",CPSum);
#endif
		m_cp = new XSCounter;
		if(m_cp == NULL) return XFalse;
	}catch(...)
	{
		return XFalse;
	}
	m_mode = mode;

	m_w = w;
	m_h = h;
	m_texture = tex;

	m_isLoaded = XTrue;
	return XTrue;
}
XBool XTexture::load(const char *filename,XResourcePosition resoursePosition)//从指定文件中载入图片资源
{
	if(m_isLoaded) return XTrue;	//如果资源已经载入了，则不能重复载入
	try
	{
#if TEX_DEBUG1
		++ CPSum;
		printf("%d-\n",CPSum);
#endif
		m_cp = new XSCounter;
		if(m_cp == NULL) return XFalse;
	}catch(...)
	{
		return XFalse;
	}
	m_resInfo = XResManager.loadResource(filename,RESOURCE_TYPE_TEXTURE,resoursePosition);
	if(m_resInfo != NULL)
	{
		XResourceTex *tempResTex = (XResourceTex *)m_resInfo->m_pointer;
		m_texture = tempResTex->m_texID;
		m_w = tempResTex->m_width;
		m_h = tempResTex->m_height;
#if TEX_DEBUG
		++ XTSum;
		printf("%d:%d+%s\n",XTSum,m_texture,filename);
#endif
		m_isLoaded = XTrue;
		return XTrue;
	}
	XMem::XDELETE(m_cp);
	m_isLoaded = XFalse;
	return XFalse;
}
XBool XTexture::reset()
{
	int sum = m_w * m_h;
	switch(m_mode)
	{
	case COLOR_RGBA:
	case COLOR_BGRA:sum = (sum << 2);	break;
	case COLOR_RGB:
	case COLOR_BGR:	sum = sum * 3;		break;
	case COLOR_GRAY:
	case COLOR_RED:	break;
	case COLOR_RGBA_F:sum = (sum << 4);break;
	case COLOR_GRAY_F:sum = (sum << 2);break;
	default: return XFalse;	//其他格式不支持该操作
	}
	unsigned char *pData = XMem::createArrayMem<unsigned char>(sum);
	if(pData == NULL) return XFalse;
	memset(pData,0,sum);
	updateTexture(pData);
	XMem::XDELETE_ARRAY(pData);
	return XTrue;
}
void XTexture::release()	//释放图片资源
{
	if(!m_isLoaded) return;
#if TEX_DEBUG
	-- XTSum;
	printf("%d-%d\n",XTSum,m_texture);
#endif
	//经过资源管理器之后其自身不能释放资源
	if(m_resInfo == NULL)
	{
		if(glIsTexture(m_texture)) 
		{
#if WITH_ENGINE_STATISTICS
			XEG.decStaticsticsTexInfo(m_texture);
#endif
			LogNull("delete texture:%d",m_texture);
			glDeleteTextures(1, &m_texture);
		}
	}else
	{
		if(XResManager.releaseResource(m_resInfo))
			m_resInfo = NULL;
	}
	m_isLoaded = XFalse;
}
XTexture::XTexture(const XTexture& temp)	//拷贝构造函数
{
	if(this == &temp) return;		//防止自生拷贝
	if(temp.m_cp != NULL) ++temp.m_cp->m_counter;
	m_texture = temp.m_texture;
	m_w = temp.m_w;
	m_h = temp.m_h;
	m_isLoaded = temp.m_isLoaded;
	m_cp = temp.m_cp;

	m_resInfo = XResManager.copyResource(temp.m_resInfo);
	return;	
}
XTexture& XTexture::operator = (const XTexture &temp)
{
	if(this == &temp) return *this;		//防止自生拷贝
	if(temp.m_cp != NULL) ++temp.m_cp->m_counter;
	if(m_cp != NULL && -- m_cp->m_counter <= 0)
	{
		release();	//引用计数结束，释放资源
#if TEX_DEBUG1
		-- CPSum;
		printf("%d-\n",CPSum);
#endif
		XMem::XDELETE(m_cp);
	}
	m_texture = temp.m_texture;
	m_w = temp.m_w;
	m_h = temp.m_h;
	m_isLoaded = temp.m_isLoaded;
	m_cp = temp.m_cp;
	if(m_resInfo != NULL) -- m_resInfo->m_counter;
	m_resInfo = XResManager.copyResource(temp.m_resInfo);
	return *this;	
}
}