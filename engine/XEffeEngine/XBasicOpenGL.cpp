//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "glew.h"
//#include <gl/GLAux.h>
#include "stdio.h"
#include "math.h"

#include "XBasicOpenGL.h"
#include "XBasicFun.h"
#include "XPixelCommon.h"

#include "XResourcePack.h"
#include "XLogBook.h"
#include "XSavePNG.h"
#if TEX_DEBUG
int XTSum = 0;
#endif
#if TEX_DEBUG1
int CPSum = 0;
#endif

unsigned int srcBlendMode[9] = {GL_ZERO,GL_ONE,GL_DST_COLOR,GL_ONE_MINUS_DST_COLOR,GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA,
		GL_DST_ALPHA,GL_ONE_MINUS_DST_ALPHA,GL_SRC_ALPHA_SATURATE};
unsigned int dstBlendMode[8] = {GL_ZERO,GL_ONE,GL_SRC_COLOR,GL_ONE_MINUS_SRC_COLOR,GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA,	
		GL_DST_ALPHA,GL_ONE_MINUS_DST_ALPHA};

void printShaderInfoLog(GLuint obj)
{
    int infologLength = 0;
    int charsWritten  = 0;
    static char *infoLog = NULL;

    glGetShaderiv(obj, GL_INFO_LOG_LENGTH,&infologLength);

    if(infologLength > 0)
    {
        infoLog = createArrayMem<char>(infologLength);
		if(infoLog == NULL) return;
        glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
		LogNull("%s",infoLog);
        XDELETE_ARRAY(infoLog);
    }
}
void printProgramInfoLog(GLuint obj)
{
    int infologLength = 0;
    int charsWritten  = 0;
    static char *infoLog = NULL;

    glGetProgramiv(obj, GL_INFO_LOG_LENGTH,&infologLength);

    if(infologLength > 0)
    {
        infoLog = createArrayMem<char>(infologLength);
		if(infoLog == NULL) return;
        glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
		LogNull("%s",infoLog);
        XDELETE_ARRAY(infoLog);
    }
}
int printOglError(const char *file,int line)
{
    static GLenum glErr;
    int retCode = 0;

    glErr = glGetError();
    while(glErr != GL_NO_ERROR)
    {
	//	LogNull("glError in file %s @ line %d: %s\n", file, line, gluErrorString(glErr));
 		LogNull("glError in file %s @ line %d",file,line);
		//printf("glError: 0x%x in file %s @ line %d\n",glErr,file,line);
        retCode = 1;
        glErr = glGetError();
    }
    return retCode;
}
char *textFileRead(const char *filename,_XResourcePosition resoursePosition) 
{
	if(resoursePosition == RESOURCE_SYSTEM_DEFINE) resoursePosition = XEE::defaultResourcePosition;
	if(resoursePosition == RESOURCE_LOCAL_PACK)
	{
	//	unsigned char *content = _XResourcePack::GetInstance().getFileData(filename);
	//	return (char *)content;
		return (char *)_XResourcePack::GetInstance().getFileData(filename);
	}else
	if(resoursePosition == RESOURCE_LOCAL_FOLDER)
	{
		FILE *fp;
		char *content = NULL;
		int count = 0;
		if(filename != NULL && (fp = fopen(filename,"rt")) != NULL) 
		{
			fseek(fp, 0, SEEK_END);
			count = ftell(fp);
			rewind(fp);

			if(count > 0) 
			{
				//content = (char *)malloc(sizeof(char) * (count + 1));
				content = createArrayMem<char>(sizeof(char) * (count + 1));
				count = fread(content,sizeof(char),count,fp);
				content[count] = '\0';
			}
			fclose(fp);
		}
		return content;
	}
	return NULL;
}
int textFileWrite(const char *filename,const char *s) 
{
	if(filename == NULL ||
		s == NULL) return 0;
	FILE *fp = NULL;
	int status = 0;

	if((fp = fopen(filename,"w")) != NULL) 
	{	
		if(fwrite(s,sizeof(char),strlen(s),fp) == strlen(s)) status = 1;
		fclose(fp);
	}
	return(status);
}
_XBool setShader(const char* vertFile,const char* fragFile,int &pHandle,_XResourcePosition resoursePosition)
{
	if(vertFile == NULL || fragFile == NULL) return XFalse;
    char *vs = NULL;
	char *fs = NULL;
	GLuint v,f;

	v = glCreateShader(GL_VERTEX_SHADER);
    f = glCreateShader(GL_FRAGMENT_SHADER);
	//v = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
    //f = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);

	vs = textFileRead(vertFile,resoursePosition);
    fs = textFileRead(fragFile,resoursePosition);
	if(vs == NULL || fs == NULL) return XFalse;

	const char * vv = vs;
    const char * ff = fs;

	glShaderSource(v, 1, &vv,NULL);
    glShaderSource(f, 1, &ff,NULL);
	//glShaderSourceARB(v, 1, &vv,NULL);
    //glShaderSourceARB(f, 1, &ff,NULL);

	XDELETE_ARRAY(vs);
	XDELETE_ARRAY(fs);

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
_XBool setShader(const char* vertFile,const char* fragFile,const char* geomFile,_XShaderHandle &pHandle,
						_XResourcePosition resoursePosition)
{
	if(vertFile == NULL || fragFile == NULL) return XFalse;
    char *vs = NULL;
	char *fs = NULL;
	char *gs = NULL;

	if(pHandle.shaderV == 0) pHandle.shaderV = glCreateShader(GL_VERTEX_SHADER);
    if(pHandle.shaderF == 0) pHandle.shaderF = glCreateShader(GL_FRAGMENT_SHADER);
	if(geomFile != NULL && pHandle.shaderG == 0)
		pHandle.shaderG = glCreateShader(GL_GEOMETRY_SHADER_EXT);
	//v = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
    //f = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);

	vs = textFileRead(vertFile,resoursePosition);
    fs = textFileRead(fragFile,resoursePosition);
	if(vs == NULL || fs == NULL) return XFalse;
	if(geomFile != NULL)
	{
		gs = textFileRead(geomFile,resoursePosition);
		if(gs == NULL) return XFalse;
	}

	const char * vv = vs;
    const char * ff = fs;
    const char * gg = gs;

	glShaderSource(pHandle.shaderV, 1, &vv,NULL);
    glShaderSource(pHandle.shaderF, 1, &ff,NULL);
	if(geomFile != NULL)
		 glShaderSource(pHandle.shaderG, 1, &gg,NULL);
	//glShaderSourceARB(v, 1, &vv,NULL);
    //glShaderSourceARB(f, 1, &ff,NULL);

	XDELETE_ARRAY(vs);
	XDELETE_ARRAY(fs);
	XDELETE_ARRAY(gs);

	glCompileShader(pHandle.shaderV);
	glCompileShader(pHandle.shaderF);
	if(geomFile != NULL)
		glCompileShader(pHandle.shaderG);
	//glCompileShaderARB(v);
	//glCompileShaderARB(f);

	printShaderInfoLog(pHandle.shaderV);
    printShaderInfoLog(pHandle.shaderF);
	if(geomFile != NULL) printShaderInfoLog(pHandle.shaderG);

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
//_XBool TextureLoad(GLuint &texture,const char *filename,int withAlpha,int *w,int *h,_XResourcePosition resoursePosition)
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
//			glBindTexture(GL_TEXTURE_2D, texture);
//			//效果好
//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//#if WITH_XSPRITE_EX
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
//			glBindTexture(GL_TEXTURE_2D, texture);
//		//	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
//
//			//效果好
//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//#if WITH_XSPRITE_EX
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
////		glBindTexture(GL_TEXTURE_2D, texture);
////
////		//效果好
////	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
////	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
////#if WITH_XSPRITE_EX
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
_XBool TextureLoadEx(GLuint &texture,const char *filename,int *w,int *h,_XResourcePosition resoursePosition)
{
	if(filename == NULL || strlen(filename) <= 0) return XFalse;

	_XPixels<_XCurPixel> tmp;
	if(!tmp.load(filename,resoursePosition)) return XFalse;
	if((tmp.getWidth() > XEE::maxTetureSize || tmp.getHeight() > XEE::maxTetureSize) && XEE::maxTetureSize > 0) 	//贴图最大尺寸的检测
	{
		fprintf(stderr, "Image size exceeds max texture size, which is %d pixels for each side\n", XEE::maxTetureSize);
		LogNull("Image size exceeds max texture size, which is %d pixels for each side\n", XEE::maxTetureSize);
		tmp.release();
		return XFalse;
	}
	if(w != NULL) (* w) = tmp.getWidth();
	if(h != NULL) (* h) = tmp.getHeight();
	tmp.fitNPot();	//适应2的n次方
	// OpenGL Texture creation	
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
//	glPixelStorei(GL_UNPACK_ALIGNMENT,1);

	//效果好
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
#if WITH_XSPRITE_EX
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

	tmp.release();
	return XTrue;
}
_XBool TextureLoadQuarter(GLuint &texture,const char *filename)
{
	_XPixels<_XCurPixel> tmp;
	if(!tmp.load(filename)) return XFalse;
	int w = tmp.getWidth() >> 2;
	int h = tmp.getHeight() >> 2;
	unsigned char * tempPixel = createArrayMem<unsigned char>(w * h * 4);
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
			unsigned char *p1 = tempPixel;
			for(int i = 0; i < tmp.getWidth();i += 4)
				for(int j = 0;j < tmp.getHeight();j += 4)
				{
					*(p1 + i + j * w + 0) = *(p + (i << 2) + (j << 2) * tmp.getWidth() + 0);
					*(p1 + i + j * w + 1) = *(p + (i << 2) + (j << 2) * tmp.getWidth() + 1);
					*(p1 + i + j * w + 2) = *(p + (i << 2) + (j << 2) * tmp.getWidth() + 2);
					*(p1 + i + j * w + 3) = 255;
				}
		}
		break;
	case COLOR_GRAY:
		{//尚未实现
		}
		break;
	}
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	//效果好
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
#if WITH_XSPRITE_EX
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
	case COLOR_GRAY:
		//尚未实现
		break;
	}
	tmp.release();
	XDELETE_ARRAY(tempPixel);
	return XTrue;
}
void initWindowMatrix()
{
	switch(XEE::windowData.windowType)
	{
	case WINDOW_TYPE_EX:
		glMatrixMode(GL_PROJECTION);					//设置当前矩阵模式 （对投影矩阵应用之后的矩阵操作）
		glLoadIdentity();								//变换坐标系函数
		glViewport(0, 0, XEE::windowData.w, XEE::windowData.h);
		switch(XEE::windowData.mode)
		{
		case WINDOW_SIZE_MODE_CLIP_LP:
			XEE::sceneX = XEE::windowData.positionX;
			XEE::sceneY = XEE::windowData.positionY;
			XEE::sceneWidth = XEE::windowData.w;
			XEE::sceneHeight = XEE::windowData.h;
			XEE::sceneSizeX = 1.0f;
			XEE::sceneSizeY = 1.0f;
			break;
		case WINDOW_SIZE_MODE_CLIP_MID:
			XEE::sceneX = ((XEE::windowData.sceneW - XEE::windowData.w) >> 1) + XEE::windowData.positionX;
			XEE::sceneY = ((XEE::windowData.sceneH - XEE::windowData.h) >> 1) + XEE::windowData.positionY;
			XEE::sceneWidth = XEE::windowData.w;
			XEE::sceneHeight = XEE::windowData.h;
			XEE::sceneSizeX = 1.0f;
			XEE::sceneSizeY = 1.0f;
			break;
		case WINDOW_SIZE_MODE_CLIP_RESIZE:
			XEE::sceneSizeX = (float)(XEE::windowData.sceneW) / (float)(XEE::windowData.w);
			XEE::sceneSizeY = (float)(XEE::windowData.sceneH) / (float)(XEE::windowData.h);
			if(XEE::sceneSizeX <= XEE::sceneSizeY)
			{
				XEE::sceneWidth = XEE::windowData.sceneW;
				XEE::sceneHeight = XEE::windowData.h * XEE::sceneSizeX;
				XEE::sceneX = XEE::windowData.positionX;
				XEE::sceneY = ((XEE::windowData.sceneH - XEE::sceneHeight) >> 1) + XEE::windowData.positionY;
			}else
			{
				XEE::sceneWidth = XEE::windowData.w * XEE::sceneSizeY;
				XEE::sceneHeight = XEE::windowData.sceneH;
				XEE::sceneX = ((XEE::windowData.sceneW - XEE::sceneWidth) >> 1) + XEE::windowData.positionX;
				XEE::sceneY = XEE::windowData.positionY;
			}
			break;
		case WINDOW_SIZE_MODE_RESIZE_CLIP:
			XEE::sceneSizeX = (float)(XEE::windowData.sceneW) / (float)(XEE::windowData.w);
			XEE::sceneSizeY = (float)(XEE::windowData.sceneH) / (float)(XEE::windowData.h);
			if(XEE::sceneSizeX >= XEE::sceneSizeY)
			{
				XEE::sceneWidth = XEE::windowData.sceneW;
				XEE::sceneHeight = XEE::windowData.h * XEE::sceneSizeX;
				XEE::sceneX = XEE::windowData.positionX;
				XEE::sceneY = ((XEE::windowData.sceneH - XEE::sceneHeight) >> 1) + XEE::windowData.positionY;
			}else
			{
				XEE::sceneWidth = XEE::windowData.w * XEE::sceneSizeY;
				XEE::sceneHeight = XEE::windowData.sceneH;
				XEE::sceneX = ((XEE::windowData.sceneW - XEE::sceneWidth) >> 1) + XEE::windowData.positionX;
				XEE::sceneY = XEE::windowData.positionY;
			}
			break;
		case WINDOW_SIZE_MODE_RESIZE:
			XEE::sceneX = XEE::windowData.positionX;
			XEE::sceneY = XEE::windowData.positionY;
			XEE::sceneWidth = XEE::windowData.sceneW;
			XEE::sceneHeight = XEE::windowData.sceneH;
			XEE::sceneSizeX = (float)(XEE::windowData.sceneW) / (float)(XEE::windowData.w);
			XEE::sceneSizeY = (float)(XEE::windowData.sceneH) / (float)(XEE::windowData.h);
			break;
		}
		switch(XEE::windowData.rotateMode)
		{
		case WINDOW_ROTATE_MODE_0:
			switch(XEE::windowData.turnOverMode)
			{
			case WINDOW_TURNOVER_MODE_NULL:
				glOrtho(XEE::sceneX,XEE::sceneWidth + XEE::sceneX,
					XEE::sceneHeight + XEE::sceneY,XEE::sceneY,-1,1);
				break;
			case WINDOW_TURNOVER_MODE_LTR:
				glOrtho(XEE::sceneWidth + XEE::sceneX,XEE::sceneX,
					XEE::sceneHeight + XEE::sceneY,XEE::sceneY,-1,1);
				break;
			case WINDOW_TURNOVER_MODE_TTD:
				glOrtho(XEE::sceneX,XEE::sceneWidth + XEE::sceneX,
					XEE::sceneY,XEE::sceneHeight + XEE::sceneY,-1,1);
				break;
			case WINDOW_TURNOVER_MODE_LTR_TTD:
				glOrtho(XEE::sceneWidth + XEE::sceneX,XEE::sceneX,
					XEE::sceneY,XEE::sceneHeight + XEE::sceneY,-1,1);
				break;
			}
			break;
		case WINDOW_ROTATE_MODE_90:
			switch(XEE::windowData.turnOverMode)
			{
			case WINDOW_TURNOVER_MODE_NULL:
				glOrtho(XEE::sceneHeight + XEE::sceneY,XEE::sceneY,
					XEE::sceneX,XEE::sceneWidth + XEE::sceneX,-1,1);
				break;
			case WINDOW_TURNOVER_MODE_LTR:
				glOrtho(XEE::sceneHeight + XEE::sceneY,XEE::sceneY,
					XEE::sceneWidth + XEE::sceneX,XEE::sceneX,-1,1);
				break;
			case WINDOW_TURNOVER_MODE_TTD:
				glOrtho(XEE::sceneY,XEE::sceneHeight + XEE::sceneY,
					XEE::sceneX,XEE::sceneWidth + XEE::sceneX,-1,1);
				break;
			case WINDOW_TURNOVER_MODE_LTR_TTD:
				glOrtho(XEE::sceneY,XEE::sceneHeight + XEE::sceneY,
					XEE::sceneWidth + XEE::sceneX,XEE::sceneX,-1,1);
				break;
			}
			break;
		case WINDOW_ROTATE_MODE_180:
			switch(XEE::windowData.turnOverMode)
			{
			case WINDOW_TURNOVER_MODE_NULL:
				glOrtho(XEE::sceneWidth + XEE::sceneX,XEE::sceneX,
					XEE::sceneY,XEE::sceneHeight + XEE::sceneY,-1,1);
				break;
			case WINDOW_TURNOVER_MODE_LTR:
				glOrtho(XEE::sceneX,XEE::sceneWidth + XEE::sceneX,
					XEE::sceneY,XEE::sceneHeight + XEE::sceneY,-1,1);
				break;
			case WINDOW_TURNOVER_MODE_TTD:
				glOrtho(XEE::sceneWidth + XEE::sceneX,XEE::sceneX,
					XEE::sceneHeight + XEE::sceneY,XEE::sceneY,-1,1);
				break;
			case WINDOW_TURNOVER_MODE_LTR_TTD:
				glOrtho(XEE::sceneX,XEE::sceneWidth + XEE::sceneX,
					XEE::sceneHeight + XEE::sceneY,XEE::sceneY,-1,1);
				break;
			}
			break;
		case WINDOW_ROTATE_MODE_270:
			switch(XEE::windowData.turnOverMode)
			{
			case WINDOW_TURNOVER_MODE_NULL:
				glOrtho(XEE::sceneY,XEE::sceneHeight + XEE::sceneY,
					XEE::sceneWidth + XEE::sceneX,XEE::sceneX,-1,1);
				break;
			case WINDOW_TURNOVER_MODE_LTR:
				glOrtho(XEE::sceneY,XEE::sceneHeight + XEE::sceneY,
					XEE::sceneX,XEE::sceneWidth + XEE::sceneX,-1,1);
				break;
			case WINDOW_TURNOVER_MODE_TTD:
				glOrtho(XEE::sceneHeight + XEE::sceneY,XEE::sceneY,
					XEE::sceneWidth + XEE::sceneX,XEE::sceneX,-1,1);
				break;
			case WINDOW_TURNOVER_MODE_LTR_TTD:
				glOrtho(XEE::sceneHeight + XEE::sceneY,XEE::sceneY,
					XEE::sceneX,XEE::sceneWidth + XEE::sceneX,-1,1);
				break;
			}
			break;
		}
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		switch(XEE::windowData.rotateMode)
		{
		case WINDOW_ROTATE_MODE_0: break;	//do nothing
		case WINDOW_ROTATE_MODE_90: 
			switch(XEE::windowData.mode)
			{
			case WINDOW_SIZE_MODE_CLIP_LP:
				glTranslatef(XEE::windowData.h,0,0);
				glRotatef(90,0,0,1);
				break;
			case WINDOW_SIZE_MODE_CLIP_MID:
			case WINDOW_SIZE_MODE_CLIP_RESIZE:
			case WINDOW_SIZE_MODE_RESIZE_CLIP:
			case WINDOW_SIZE_MODE_RESIZE:
				glTranslatef(XEE::windowData.sceneH,0,0);
				glRotatef(90,0,0,1);
				break;
			}
			break;
		case WINDOW_ROTATE_MODE_180: break;	//do nothing
		case WINDOW_ROTATE_MODE_270: 
			switch(XEE::windowData.mode)
			{
			case WINDOW_SIZE_MODE_CLIP_LP:
				glTranslatef(XEE::windowData.h,0,0);
				glRotatef(90,0,0,1);
				break;
			case WINDOW_SIZE_MODE_CLIP_MID:
			case WINDOW_SIZE_MODE_CLIP_RESIZE:
			case WINDOW_SIZE_MODE_RESIZE_CLIP:
			case WINDOW_SIZE_MODE_RESIZE:
				glTranslatef(XEE::windowData.sceneH,0,0);
				glRotatef(90,0,0,1);
				break;
			}
			break;
		}
		break;
	case WINDOW_TYPE_NORMAL:
		glMatrixMode(GL_PROJECTION);					//设置当前矩阵模式 （对投影矩阵应用之后的矩阵操作）
		glLoadIdentity();								//变换坐标系函数
		glViewport(0, 0, XEE::windowData.w, XEE::windowData.h);
		glOrtho(0,XEE::windowData.w,XEE::windowData.h,0,-1,1);					//only for 2D的原始数据
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		XEE::sceneX = 0;
		XEE::sceneY = 0;
		XEE::sceneWidth = XEE::windowData.w;
		XEE::sceneHeight = XEE::windowData.h;
		XEE::sceneSizeX = 1.0f;
		XEE::sceneSizeY = 1.0f;
		break;
	case WINDOW_TYPE_3D:
		glMatrixMode(GL_PROJECTION);					//设置当前矩阵模式 （对投影矩阵应用之后的矩阵操作）
		glLoadIdentity();								//变换坐标系函数
		glViewport(0, 0, XEE::windowData.w, XEE::windowData.h);
		gluPerspective(45.0f,(float)(XEE::windowData.w) / (float)(XEE::windowData.h),1.0f,10000.0f);		//最后两个数值需要与之前的吻合，不然会出现模型破损的问题
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		XEE::sceneX = 0;
		XEE::sceneY = 0;
		XEE::sceneWidth = XEE::windowData.w;
		XEE::sceneHeight = XEE::windowData.h;
		XEE::sceneSizeX = 1.0f;
		XEE::sceneSizeY = 1.0f;
		break;
	}
}
#pragma comment(lib,"glu32.lib")
inline void initGlew()
{
	if(glewInit() != 0)
	{//初始化失败
		LogStr("Glew init error!");
		XEE::isOpenGL2Support = 0;
	}else
	{
		//记录显卡的最大贴图尺寸
		glGetIntegerv(GL_MAX_TEXTURE_SIZE,&XEE::maxTetureSize);	
		//检查硬件是否支持OpenGL2.0
		if(glewIsSupported("GL_VERSION_2_0"))
		{
			LogStr("Ready for OpenGL 2.0");
			XEE::isOpenGL2Support = 1;
		}else
		{
			LogStr("OpenGL 2.0 not supported");
			XEE::isOpenGL2Support = 0;
		}
		//开启多重采样抗锯齿
		//glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB |GLUT_MULTISAMPLE);//，初始化的时候开启；2）
		GLint bufSum = 0;
		GLint samplesSum = 0;
		glGetIntegerv(GL_SAMPLE_BUFFERS, &bufSum);
		glGetIntegerv(GL_SAMPLES, &samplesSum);//可以检查多重采样是否可用，前者返回1，后者返回大于1表示可用; 3）
		if(bufSum == 1 && samplesSum > 1)
		{
			XEE::isMultiSampleSupport = 1;
			glEnable(GL_MULTISAMPLE);	//这里生效需要在显卡的控制面板中设置为最佳图像质量
		}else
			XEE::isMultiSampleSupport = 0;
	}
}
_XBool initOpenGL3D()
{
	initWindowMatrix();	//初始化矩阵
	initGlew();			//初始化glew
	glShadeModel(GL_SMOOTH);		//启动反走样
	glClearColor(XEE::defaultBGColor.fR,XEE::defaultBGColor.fG,XEE::defaultBGColor.fB,XEE::defaultBGColor.fA);				//清除颜色
	glHint(GL_POLYGON_SMOOTH,GL_NICEST);
	glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);
	glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);//建议：优化掉这一步，不用每次都调用
	if(XEE::isMultiSampleSupport)
	{
		glEnable(GL_POLYGON_SMOOTH);			//开启这里会造成贴图破损
		glEnable(GL_POINT_SMOOTH);		//开启各种抗锯齿功能
		if(XEE::isLineSmooth) glEnable(GL_LINE_SMOOTH);
	}
	//3D
	glClearDepth(1.0f);									//Depth Buffer Setup
	glClearStencil(0);
//	glEnable(GL_DEPTH);	
	glEnable(GL_DEPTH_TEST);							//Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								//The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);	//Really Nice Perspective Calculations
	glEnable(GL_TEXTURE_2D);							// Enable 2D Texture Mapping
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);	// Set Up Sphere Mapping
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);	// Set Up Sphere Mapping

	glAlphaFunc(GL_GREATER,0.1f);						//允许进行alpha测试
	glEnable(GL_ALPHA_TEST); 
	//下面的代码是为了解决背光面的问题
	//glEnable(GL_CULL_FACE);	//只允许渲染正面

	glEnable(GL_NORMALIZE);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	//+++++++++++++++++++++++++++++
	//注意这里，颜色与法线贴图有冲突
	glEnable(GL_COLOR_MATERIAL);	//开启这里会造成法线贴图颜色太暗的问题,不开启这个会造成颜色不生效
	//-----------------------------
	glMaterialfv(GL_FRONT,GL_SPECULAR,_XVector4(1.0f,1.0f,1.0f,1.0f));
	glMaterialf(GL_FRONT,GL_SHININESS,128.0f);

	//glDisable(GL_LINE_SMOOTH);
	//glCullFace(GL_BACK);
	//glEnable(GL_CULL_FACE);
	return XTrue;
}
_XBool initOpenGL2D()
{
	initWindowMatrix();	//初始化矩阵
	initGlew();			//初始化glew
	glShadeModel(GL_SMOOTH);						// Enable Smooth Shading
	glClearColor(XEE::defaultBGColor.fR,XEE::defaultBGColor.fG,XEE::defaultBGColor.fB,XEE::defaultBGColor.fA);			//清除颜色
	glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);
	glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);//建议：优化掉这一步，不用每次都调用
	glHint(GL_POLYGON_SMOOTH,GL_NICEST);
	if(XEE::isMultiSampleSupport)
	{
		glEnable(GL_POLYGON_SMOOTH);			//开启这里会造成贴图破损
		glEnable(GL_POINT_SMOOTH);		//开启各种抗锯齿功能
		if(XEE::isLineSmooth) glEnable(GL_LINE_SMOOTH);
	}
	//初始化GL的相关数据
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	//glDisable(GL_DEPTH);							//2D项目不需要深度测试
	glDisable(GL_DEPTH_TEST);						//2D项目不需要深度测试
	//glEnalbe(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	return XTrue;
}
void drawLine(float ax,float ay,float bx,float by,float width,float r,float g,float b)
{
	//if(XEE::isMultiSampleSupport && XEE::isLineSmooth)
	//{
	//	glEnable(GL_BLEND);
	//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//}
	glDisable(GL_TEXTURE_2D);
	glColor3f(r,g,b);
	glLineWidth(width);
	glBegin(GL_LINES);
	glVertex2f(ax,ay);
	glVertex2f(bx,by);
	glEnd();
	glEnable(GL_TEXTURE_2D);
	//if(XEE::isMultiSampleSupport && XEE::isLineSmooth) glDisable(GL_BLEND);
}
void drawLine(float ax, float ay, float bx, float by,float width, float r, float g, float b, float a,int type)
{
    glDisable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glColor4f(r,g,b,a);
	glLineWidth(width);
	if(type == 0)
	{//实线
		glBegin(GL_LINES);
		glVertex2f(ax,ay);
		glVertex2f(bx,by);
		glEnd();
	}else
	{//虚线
		glEnable(GL_LINE_STIPPLE);
		glLineStipple(1,0x00ff);
		glBegin(GL_LINES);
		glVertex2f(ax,ay);
		glVertex2f(bx,by);
		glEnd();
		glDisable(GL_LINE_STIPPLE);
	}

    glDisable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
} 
void drawLinesVbo(unsigned int v,int pointSum,float w,float r,float g,float b)
{
	glDisable(GL_TEXTURE_2D);
	//if(XEE::isMultiSampleSupport && XEE::isLineSmooth)
	//{
	//	glEnable(GL_BLEND);
	//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//}

	glLineWidth(w);
	glEnableClientState(GL_VERTEX_ARRAY);	

	glColor3f(r,g,b);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB,v);
	glVertexPointer(2,GL_FLOAT,0,NULL);
	glDrawArrays(GL_LINE_STRIP,0,pointSum);

	glEnable(GL_TEXTURE_2D);
	glDisableClientState(GL_VERTEX_ARRAY);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB,0);
	//if(XEE::isMultiSampleSupport && XEE::isLineSmooth) glDisable(GL_BLEND);
}
void drawLinesVbo(unsigned int v,int pointSum,float w,float r,float g,float b,float a)
{
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glLineWidth(w);
	glEnableClientState(GL_VERTEX_ARRAY);	

	glColor4f(r,g,b,a);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB,v);
	glVertexPointer(2,GL_FLOAT,0,NULL);
	glDrawArrays(GL_LINE_STRIP,0,pointSum);

	glEnable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glDisableClientState(GL_VERTEX_ARRAY);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB,0);
	//if(XEE::isMultiSampleSupport && XEE::isLineSmooth) glDisable(GL_BLEND);
}
//void drawLine(const _XVector2& p,float angle,float length)
//{
//	drawLine(p,_XVector2(p.x + length * cos(angle * DEGREE2RADIAN),p.y + length * sin(angle * DEGREE2RADIAN)));
//}
void drawRect(const _XRect& rect,float width,float r,float g,float b,float a,int type)
{
	glDisable(GL_TEXTURE_2D);

    glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	//普通效果
	glColor4f( r, g, b, a);
	if(type == 0)
	{//实线
		glLineWidth(width);
		drawRectS(rect);
	}else
	{//虚线
		glEnable(GL_LINE_STIPPLE);
		glLineStipple(1,0x00ff);
		glLineWidth(width);
		drawRectS(rect);
		glDisable(GL_LINE_STIPPLE);
	}

    glDisable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
}
void drawRectAntiColor(const _XRect& rect,float width,int type)
{
	glDisable(GL_TEXTURE_2D);

    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ZERO);	//反色效果
	glColor4fv(_XFColor::white);
	if(type == 0)
	{//实线
		glLineWidth(width);
		drawRectS(rect);
	}else
	{//虚线
		glEnable(GL_LINE_STIPPLE);
		glLineStipple(1,0x00ff);
		glLineWidth(width);
		drawRectS(rect);
		glDisable(GL_LINE_STIPPLE);
	}

    glDisable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
}
void drawBox(float x,float y,float sizeX,float sizeY,int w,float r,float g,float b,float a)
{
	glDisable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glColor4f(r,g,b,a);
	glLineWidth(w);
	drawBoxS(x,y,sizeX,sizeY);
	glDisable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
}
void drawBox(float x,float y,float sizeX,float sizeY,float angle)
{
	_XVector2 P1(x - (sizeX * cos(angle) - sizeY * sin(angle)),y + (sizeX * sin(angle) + sizeY * cos(angle)));
	_XVector2 P2(x - (sizeX * cos(angle) + sizeY * sin(angle)),y + (sizeX * sin(angle) - sizeY * cos(angle)));

	_XVector2 P3(x + (sizeX * cos(angle) + sizeY * sin(angle)),y - (sizeX * sin(angle) - sizeY * cos(angle)));
	_XVector2 P4(x + (sizeX * cos(angle) - sizeY * sin(angle)),y - (sizeX * sin(angle) + sizeY * cos(angle)));

	glDisable(GL_TEXTURE_2D);
	//if(XEE::isMultiSampleSupport && XEE::isLineSmooth)
	//{
	//	glEnable(GL_BLEND);
	//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//}
	glColor3fv(_XFColor::white);
	glLineWidth(1);
	drawBoxS(P1,P2,P3,P4);
	//if(XEE::isMultiSampleSupport && XEE::isLineSmooth) glDisable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
}
void drawBox(const _XVector2& p0,const _XVector2& p1,const _XVector2& p2,const _XVector2& p3,float width,float r,float g,float b)
{
	glDisable(GL_TEXTURE_2D);
	//if(XEE::isMultiSampleSupport && XEE::isLineSmooth)
	//{
	//	glEnable(GL_BLEND);
	//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//}
	glColor3f(r,g,b);
	glLineWidth(width);
	drawBoxS(p0,p1,p3,p2);
	//if(XEE::isMultiSampleSupport && XEE::isLineSmooth) glDisable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
}
void drawBox(const _XVector2& p0,const _XVector2& p1,const _XVector2& p2,const _XVector2& p3,float width,float r,float g,float b,float a)
{
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(r,g,b,a);
	glLineWidth(width);
	drawBoxS(p0,p1,p3,p2);
	glDisable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
}
void drawTriangle(const _XVector2& p0,const _XVector2& p1,const _XVector2& p2,float width,float r,float g,float b)
{
	glDisable(GL_TEXTURE_2D);
	//if(XEE::isMultiSampleSupport && XEE::isLineSmooth)
	//{
	//	glEnable(GL_BLEND);
	//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//}
	glColor3f(r,g,b);
	glLineWidth(width);
	glBegin(GL_LINE_STRIP);
	glVertex2fv(p0);
	glVertex2fv(p1);
	glVertex2fv(p2);
	glVertex2fv(p0);
	glEnd();
	//if(XEE::isMultiSampleSupport && XEE::isLineSmooth) glDisable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
}
void drawPoint(float x,float y,float r,float g,float b,float a)
{
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glColor4f(r,g,b,a);
	glPointSize(1.0f);
	glBegin(GL_POINTS);
		glVertex2f(x,y);
	glEnd();

    glDisable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
}
void drawPoint(float x,float y,float size,float r,float g,float b,float a)
{
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glColor4f(r,g,b,a);
	glPointSize(size);
	glBegin(GL_POINTS);
		glVertex2f(x,y);
	glEnd();

    glDisable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
}
void drawPointsVbo(unsigned int v,int pointSum,float s,float r,float g,float b)
{
	glDisable(GL_TEXTURE_2D);

	glPointSize(s);
	glEnableClientState(GL_VERTEX_ARRAY);	

	glColor3f(r,g,b);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB,v);
	glVertexPointer(2,GL_FLOAT,0,NULL);
	glDrawArrays(GL_POINTS,0,pointSum);

	glEnable(GL_TEXTURE_2D);
	glDisableClientState(GL_VERTEX_ARRAY);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB,0);
}
void drawPointsVbo(unsigned int v,int pointSum,float s,float r,float g,float b,float a)
{
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPointSize(s);
	glEnableClientState(GL_VERTEX_ARRAY);	

	glColor3f(r,g,b);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB,v);
	glVertexPointer(2,GL_FLOAT,0,NULL);
	glDrawArrays(GL_POINTS,0,pointSum);

	glEnable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glDisableClientState(GL_VERTEX_ARRAY);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB,0);
}
void drawCross(const _XVector2& p,float size,float w,float r,float g,float b)
{
	glDisable(GL_TEXTURE_2D);
	glColor3f(r,g,b);
	glLineWidth(w);
	glBegin(GL_LINES);
	glVertex2f(p.x - size,p.y);
	glVertex2f(p.x + size,p.y);
	glVertex2f(p.x,p.y - size);
	glVertex2f(p.x,p.y + size);
	glEnd();
	glEnable(GL_TEXTURE_2D);
}
void drawCircle(float px, float py, float sr, int an,float r,float g,float b,float a)
{
	if(an <= 0 || sr <= 0.0f) return;
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(r,g,b,a);
	r = (PI2)/an;	//为了优化计算
	glBegin(GL_POLYGON);
		for(float i = 0;i < PI2; i += r)
		{
			glVertex2f(px + cosf(i) * sr,py + sinf(i) * sr);
		}
	glEnd();
    glDisable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
}
void drawCircleLine(float px, float py, float sr, int an,float r,float g,float b,float a)
{
	if(an <= 0 || sr <= 0.0f) return;
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(r,g,b,a);
	r = (PI2)/an;	//为了优化计算
	glBegin(GL_LINE_STRIP);
		for(float i = 0;i < PI2; i += r)
		{
			glVertex2f(px + cosf(i) * sr,py + sinf(i) * sr);
		}
	glEnd();
    glDisable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
}
void drawFillRing(float x,float y,float r0,float r1,float angleS,float angleE,int an,float r,float g,float b,float a)
{//测试通过，但是计算需要进一步优化
	if(angleE == angleS || r0 == r1 || an <= 0) return;
	//喜爱按开始计算需要的点
	std::vector<float> point;
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
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(r,g,b,a);
	glBegin(GL_TRIANGLE_STRIP);
	for(int i = 0;i < point.size();i += 2)
	{
		glVertex2f(point[i],point[i + 1]);
	}
	glEnd();
    glDisable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
}
void drawFillRingEx(float x,float y,float r0,float r1,float angleS,float angleE,int an,float r,float g,float b,float a)
{
	if(angleE == angleS || r0 == r1 || an <= 0) return;
	//喜爱按开始计算需要的点
	std::vector<float> point;
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
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	perAngle = a / point.size() * 2.0f;
	glBegin(GL_TRIANGLE_STRIP);
	for(int i = 0;i < point.size();i += 2)
	{
		glColor4f(r,g,b,perAngle * (i >> 1));
		glVertex2f(point[i],point[i + 1]);
	}
	glEnd();
    glDisable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
}
//这里是关于圆角的定义
namespace _XGLCornerData
{
	bool isRoundCornerInit = false;
	#define ROUND_CORNER_DENSITY (3)
	_XVector2 roundCornerData[ROUND_CORNER_DENSITY * 4];	//10度一个弧
	float roundCornerR = 10.0f;	//圆角的半径
	float roundCornerD = 20.0f;	//圆角的直径
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
	inline void drawCornerLine(const _XVector2 &pos,const _XVector2 &size)
	{
		glBegin(GL_LINE_STRIP);
		for(int i = 0;i <= ROUND_CORNER_DENSITY;++ i)
		{//右下角圆角
			glVertex2f(_XGLCornerData::roundCornerData[i].x + size.x - _XGLCornerData::roundCornerR + pos.x,
				_XGLCornerData::roundCornerData[i].y + size.y - _XGLCornerData::roundCornerR + pos.y);
		}
		for(int i = 0;i <= ROUND_CORNER_DENSITY;++ i)
		{//左下角圆角
			glVertex2f(_XGLCornerData::roundCornerData[i + ROUND_CORNER_DENSITY].x + _XGLCornerData::roundCornerR + pos.x,
				_XGLCornerData::roundCornerData[i + ROUND_CORNER_DENSITY].y + size.y - _XGLCornerData::roundCornerR + pos.y);
		}
		for(int i = 0;i <= ROUND_CORNER_DENSITY;++ i)
		{//左上角圆角
			glVertex2f(_XGLCornerData::roundCornerData[i + (ROUND_CORNER_DENSITY << 1)].x + _XGLCornerData::roundCornerR + pos.x,
				_XGLCornerData::roundCornerData[i + (ROUND_CORNER_DENSITY << 1)].y + _XGLCornerData::roundCornerR + pos.y);
		}
		for(int i = 0;i < ROUND_CORNER_DENSITY;++ i)
		{//右上角圆角
			glVertex2f(_XGLCornerData::roundCornerData[i + ROUND_CORNER_DENSITY * 3].x + size.x - _XGLCornerData::roundCornerR + pos.x,
				_XGLCornerData::roundCornerData[i + ROUND_CORNER_DENSITY * 3].y + _XGLCornerData::roundCornerR + pos.y);
		}
		glVertex2f(_XGLCornerData::roundCornerData[0].x + size.x - _XGLCornerData::roundCornerR + pos.x,
				_XGLCornerData::roundCornerData[0].y + size.y - _XGLCornerData::roundCornerR + pos.y);
		glEnd();
	}
}
void drawFillBox(const _XVector2 &p0,const _XFColor &c0,		//第一个点以及颜色
						const _XVector2 &p1,const _XFColor &c1,		//第二个点以及颜色
						const _XVector2 &p2,const _XFColor &c2,		//第三个点以及颜色
						const _XVector2 &p3,const _XFColor &c3)	//第四个点以及颜色
{
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

    glDisable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
}
void drawFillBox(float x,float y,float w,float h,
						float r,float g,float b,float a)	//描绘实心矩形
{
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glColor4f(r,g,b,a);
	drawFillBoxS(x,y,w,h);

    glDisable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
}
void drawFillBox(const _XVector2& pos,const _XVector2& size,
				 float r,float g,float b,bool withLine)	//描绘实心矩形
{
	//drawLine(pos.x,pos.y + size.y * 0.5f,pos.x + size.x,pos.y + size.y * 0.5f,size.y,r,g,b);
	glDisable(GL_TEXTURE_2D);
	if(withLine)
	{//描绘边框
		glColor3f(0.15f,0.15f,0.15f);
		glLineWidth(1.0f);
		drawBox(pos,size);
	}
	glColor3f(r,g,b);
	drawFillBoxS(pos.x,pos.y,size.x,size.y);
	glEnable(GL_TEXTURE_2D);
}
void drawFillBoxA(const _XVector2& pos,const _XVector2& size,
				 float r,float g,float b,float a,bool withLine)	//描绘实心矩形
{
	//drawLine(pos.x,pos.y + size.y * 0.5f,pos.x + size.x,pos.y + size.y * 0.5f,size.y,r,g,b);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if(withLine)
	{//描绘边框
		glColor4f(0.15f,0.15f,0.15f,a);
		glLineWidth(1.0f);
		drawBox(pos,size);
	}
	glColor4f(r,g,b,a);
	drawFillBoxS(pos.x,pos.y,size.x,size.y);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
}
void drawFillTriangle(const _XVector2& p0,const _XVector2& p1,const _XVector2& p2,
						float r,float g ,float b)
{
	glDisable(GL_TEXTURE_2D);
	glColor3f(r,g,b);
	glBegin(GL_TRIANGLES);
	glVertex2fv(p0);
	glVertex2fv(p1);
	glVertex2fv(p2);
	glEnd();
	glEnable(GL_TEXTURE_2D);
}
void drawFillTriangleEx(const _XVector2& p0,const _XVector2& p1,const _XVector2& p2,
						float r,float g,float b)	//描绘实心三角形
{
	float min = p0.y;
	float max = p0.y;
	if(p1.y < min) min = p1.y;
	if(p1.y > max) max = p1.y;
	if(p2.y < min) min = p2.y;
	if(p2.y > max) max = p2.y;
	float rate = 1.0f;
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_TRIANGLES);
	rate = maping1D(p0.y,min,max,0.9f,1.1f);
	glColor3f(r * rate,g * rate,b * rate);
	glVertex2fv(p0);
	rate = maping1D(p1.y,min,max,0.9f,1.1f);
	glColor3f(r * rate,g * rate,b * rate);
	glVertex2fv(p1);
	rate = maping1D(p2.y,min,max,0.9f,1.1f);
	glColor3f(r * rate,g * rate,b * rate);
	glVertex2fv(p2);
	glEnd();
	glEnable(GL_TEXTURE_2D);
}
void drawFillBoxEx(const _XVector2& pos,const _XVector2& size,
				 float r,float g,float b,
				 bool withLine,bool withRoundCorner,bool down)	//描绘实心矩形
{
	_XFColor lColor(r * 1.1f,g * 1.1f,b * 1.1f,1.0f);
	_XFColor dColor(r * 0.9f,g * 0.9f,b * 0.9f,1.0f);
	glDisable(GL_TEXTURE_2D);
	if(withRoundCorner && size.x >= _XGLCornerData::roundCornerD && size.y >= _XGLCornerData::roundCornerD)
	{
		_XGLCornerData::roundCornerInit();
		if(withLine)
		{//描绘边框
			glColor3f(0.15f,0.15f,0.15f);
			glLineWidth(1.0f);
			_XGLCornerData::drawCornerLine(pos,size);
		}
		//下半部分
		glBegin(GL_POLYGON);
		if(down) glColor3fv(lColor); 
		else glColor3fv(dColor);
		for(int i = 0;i <= ROUND_CORNER_DENSITY;++ i)	
		{//右下角圆角
			glVertex2f(_XGLCornerData::roundCornerData[i].x + size.x - _XGLCornerData::roundCornerR + pos.x,
				_XGLCornerData::roundCornerData[i].y + size.y - _XGLCornerData::roundCornerR + pos.y);
		}
		for(int i = 0;i <= ROUND_CORNER_DENSITY;++ i)
		{//左下角圆角
			glVertex2f(_XGLCornerData::roundCornerData[i + ROUND_CORNER_DENSITY].x + _XGLCornerData::roundCornerR + pos.x,
				_XGLCornerData::roundCornerData[i + ROUND_CORNER_DENSITY].y + size.y - _XGLCornerData::roundCornerR + pos.y);
		}
		glEnd();
		//上半部分
		glBegin(GL_POLYGON);
		if(down) glColor3fv(dColor);
		else glColor3fv(lColor);
		for(int i = 0;i <= ROUND_CORNER_DENSITY;++ i)
		{//左上角圆角
			glVertex2f(_XGLCornerData::roundCornerData[i + (ROUND_CORNER_DENSITY << 1)].x + _XGLCornerData::roundCornerR + pos.x,
				_XGLCornerData::roundCornerData[i + (ROUND_CORNER_DENSITY << 1)].y + _XGLCornerData::roundCornerR + pos.y);
		}
		for(int i = 0;i < ROUND_CORNER_DENSITY;++ i)
		{//左下角圆角
			glVertex2f(_XGLCornerData::roundCornerData[i + ROUND_CORNER_DENSITY * 3].x + size.x - _XGLCornerData::roundCornerR + pos.x,
				_XGLCornerData::roundCornerData[i + ROUND_CORNER_DENSITY * 3].y + _XGLCornerData::roundCornerR + pos.y);
		}
		glVertex2f(_XGLCornerData::roundCornerData[0].x + size.x - _XGLCornerData::roundCornerR + pos.x,
			_XGLCornerData::roundCornerData[0].y + _XGLCornerData::roundCornerR + pos.y);
		glEnd();
		//中间
		glBegin(GL_POLYGON);
		if(down) glColor3fv(lColor); 
		else glColor3fv(dColor);
		glVertex2f(_XGLCornerData::roundCornerData[0].x + size.x - _XGLCornerData::roundCornerR + pos.x,
			_XGLCornerData::roundCornerData[0].y + size.y - _XGLCornerData::roundCornerR + pos.y);
		glVertex2f(_XGLCornerData::roundCornerData[ROUND_CORNER_DENSITY << 1].x + _XGLCornerData::roundCornerR + pos.x,
			_XGLCornerData::roundCornerData[ROUND_CORNER_DENSITY << 1].y + size.y - _XGLCornerData::roundCornerR + pos.y);
		if(down) glColor3fv(dColor);
		else glColor3fv(lColor);
		glVertex2f(_XGLCornerData::roundCornerData[ROUND_CORNER_DENSITY << 1].x + _XGLCornerData::roundCornerR + pos.x,
			_XGLCornerData::roundCornerData[ROUND_CORNER_DENSITY << 1].y + _XGLCornerData::roundCornerR + pos.y);
		glVertex2f(_XGLCornerData::roundCornerData[0].x + size.x - _XGLCornerData::roundCornerR + pos.x,
			_XGLCornerData::roundCornerData[0].y + _XGLCornerData::roundCornerR + pos.y);
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
		glVertex2f(pos.x + size.x,pos.y + size.y);
		glVertex2f(pos.x,pos.y + size.y);
		glEnd();
	}
	glEnable(GL_TEXTURE_2D);
}
//void drawFillBoxExA(const _XVector2& pos,const _XVector2& size,
//				 float r,float g,float b,float a,
//				 bool withLine,bool withRoundCorner,bool down)	//描绘实心矩形
//{
//	_XFColor lColor(r * 1.1f,g * 1.1f,b * 1.1f,a);
//	_XFColor dColor(r * 0.9f,g * 0.9f,b * 0.9f,a);
//
//	glDisable(GL_TEXTURE_2D);
//	glEnable(GL_BLEND);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//	if(withRoundCorner && size.x >= _XGLCornerData::roundCornerD && size.y >= _XGLCornerData::roundCornerD)
//	{
//		_XGLCornerData::roundCornerInit();
//		if(withLine)
//		{//描绘边框
//			glColor4f(0.15f,0.15f,0.15f,a);
//			glLineWidth(1.0f);
//			_XGLCornerData::drawCornerLine(pos,size);
//		}
//		//下半部分
//		glBegin(GL_POLYGON);
//		if(down) glColor3fv(lColor); 
//		else glColor4fv(dColor);
//		for(int i = 0;i <= ROUND_CORNER_DENSITY;++ i)	
//		{//右下角圆角
//			glVertex2f(_XGLCornerData::roundCornerData[i].x + size.x - _XGLCornerData::roundCornerR + pos.x,
//				_XGLCornerData::roundCornerData[i].y + size.y - _XGLCornerData::roundCornerR + pos.y);
//		}
//		for(int i = 0;i <= ROUND_CORNER_DENSITY;++ i)
//		{//左下角圆角
//			glVertex2f(_XGLCornerData::roundCornerData[i + ROUND_CORNER_DENSITY].x + _XGLCornerData::roundCornerR + pos.x,
//				_XGLCornerData::roundCornerData[i + ROUND_CORNER_DENSITY].y + size.y - _XGLCornerData::roundCornerR + pos.y);
//		}
//		glEnd();
//		//上半部分
//		glBegin(GL_POLYGON);
//		if(down) glColor4fv(dColor);
//		else glColor4fv(lColor);
//		for(int i = 0;i <= ROUND_CORNER_DENSITY;++ i)
//		{//左上角圆角
//			glVertex2f(_XGLCornerData::roundCornerData[i + (ROUND_CORNER_DENSITY << 1)].x + _XGLCornerData::roundCornerR + pos.x,
//				_XGLCornerData::roundCornerData[i + (ROUND_CORNER_DENSITY << 1)].y + _XGLCornerData::roundCornerR + pos.y);
//		}
//		for(int i = 0;i < ROUND_CORNER_DENSITY;++ i)
//		{//左下角圆角
//			glVertex2f(_XGLCornerData::roundCornerData[i + ROUND_CORNER_DENSITY * 3].x + size.x - _XGLCornerData::roundCornerR + pos.x,
//				_XGLCornerData::roundCornerData[i + ROUND_CORNER_DENSITY * 3].y + _XGLCornerData::roundCornerR + pos.y);
//		}
//		glVertex2f(_XGLCornerData::roundCornerData[0].x + size.x - _XGLCornerData::roundCornerR + pos.x,
//			_XGLCornerData::roundCornerData[0].y + _XGLCornerData::roundCornerR + pos.y);
//		glEnd();
//		//中间
//		glBegin(GL_POLYGON);
//		if(down) glColor4fv(lColor); 
//		else glColor4fv(dColor);
//		glVertex2f(_XGLCornerData::roundCornerData[0].x + size.x - _XGLCornerData::roundCornerR + pos.x,
//			_XGLCornerData::roundCornerData[0].y + size.y - _XGLCornerData::roundCornerR + pos.y);
//		glVertex2f(_XGLCornerData::roundCornerData[ROUND_CORNER_DENSITY << 1].x + _XGLCornerData::roundCornerR + pos.x,
//			_XGLCornerData::roundCornerData[ROUND_CORNER_DENSITY << 1].y + size.y - _XGLCornerData::roundCornerR + pos.y);
//		if(down) glColor4fv(dColor);
//		else glColor4fv(lColor);
//		glVertex2f(_XGLCornerData::roundCornerData[ROUND_CORNER_DENSITY << 1].x + _XGLCornerData::roundCornerR + pos.x,
//			_XGLCornerData::roundCornerData[ROUND_CORNER_DENSITY << 1].y + _XGLCornerData::roundCornerR + pos.y);
//		glVertex2f(_XGLCornerData::roundCornerData[0].x + size.x - _XGLCornerData::roundCornerR + pos.x,
//			_XGLCornerData::roundCornerData[0].y + _XGLCornerData::roundCornerR + pos.y);
//		glEnd();
//	}else
//	{
//		//调换绘制顺序，立体感会更强。
//		glBegin(GL_QUADS);
//		if(down) glColor4fv(dColor); 
//		else glColor4fv(lColor);
//		glVertex2fv(pos);
//		glVertex2f(pos.x + size.x,pos.y);
//		if(down) glColor4fv(lColor);
//		else glColor4fv(dColor);
//		glVertex2f(pos.x + size.x,pos.y + size.y);
//		glVertex2f(pos.x,pos.y + size.y);
//		glEnd();
//		if(withLine)
//		{
//			//这里描绘高光
//			if(down)
//			{
//				glColor4f(dColor.fR + 0.2f,dColor.fG + 0.2f,dColor.fB + 0.2f,dColor.fA); 
//				glLineWidth(2.0f);
//				glBegin(GL_LINES);
//				glVertex2f(pos.x + size.x,pos.y + size.y - 1);
//				glVertex2f(pos.x,pos.y + size.y - 1);
//				glEnd();
//			}else
//			{
//				glColor4f(lColor.fR + 0.2f,lColor.fG + 0.2f,lColor.fB + 0.2f,lColor.fA); 
//				glLineWidth(2.0f);
//				glBegin(GL_LINES);
//				glVertex2fv(pos);
//				glVertex2f(pos.x + size.x,pos.y);
//				glEnd();
//			}
//			//描绘边框
//			glColor4f(0.15f,0.15f,0.15f,a);
//			glLineWidth(1.0f);
//			drawBox(pos,size);
//		}
//	}
//	glDisable(GL_BLEND);
//	glEnable(GL_TEXTURE_2D);
//}
void drawFillBoxExAS(const _XVector2& pos,const _XVector2& size,
				 float r,float g,float b,float a,
				 bool withLine,bool withRoundCorner,bool down)	//描绘实心矩形
{
	_XFColor lColor(r * 1.1f,g * 1.1f,b * 1.1f,a);
	_XFColor dColor(r * 0.9f,g * 0.9f,b * 0.9f,a);

	glDisable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	if(withRoundCorner && size.x >= _XGLCornerData::roundCornerD && size.y >= _XGLCornerData::roundCornerD)
	{
		_XGLCornerData::roundCornerInit();
		if(withLine)
		{//描绘边框
			glColor4f(0.15f,0.15f,0.15f,a);
			glLineWidth(1.0f);
			_XGLCornerData::drawCornerLine(pos,size);
		}
		//下半部分
		glBegin(GL_POLYGON);
		if(down) glColor3fv(lColor); 
		else glColor4fv(dColor);
		for(int i = 0;i <= ROUND_CORNER_DENSITY;++ i)	
		{//右下角圆角
			glVertex2f(_XGLCornerData::roundCornerData[i].x + size.x - _XGLCornerData::roundCornerR + pos.x,
				_XGLCornerData::roundCornerData[i].y + size.y - _XGLCornerData::roundCornerR + pos.y);
		}
		for(int i = 0;i <= ROUND_CORNER_DENSITY;++ i)
		{//左下角圆角
			glVertex2f(_XGLCornerData::roundCornerData[i + ROUND_CORNER_DENSITY].x + _XGLCornerData::roundCornerR + pos.x,
				_XGLCornerData::roundCornerData[i + ROUND_CORNER_DENSITY].y + size.y - _XGLCornerData::roundCornerR + pos.y);
		}
		glEnd();
		//上半部分
		glBegin(GL_POLYGON);
		if(down) glColor4fv(dColor);
		else glColor4fv(lColor);
		for(int i = 0;i <= ROUND_CORNER_DENSITY;++ i)
		{//左上角圆角
			glVertex2f(_XGLCornerData::roundCornerData[i + (ROUND_CORNER_DENSITY << 1)].x + _XGLCornerData::roundCornerR + pos.x,
				_XGLCornerData::roundCornerData[i + (ROUND_CORNER_DENSITY << 1)].y + _XGLCornerData::roundCornerR + pos.y);
		}
		for(int i = 0;i < ROUND_CORNER_DENSITY;++ i)
		{//左下角圆角
			glVertex2f(_XGLCornerData::roundCornerData[i + ROUND_CORNER_DENSITY * 3].x + size.x - _XGLCornerData::roundCornerR + pos.x,
				_XGLCornerData::roundCornerData[i + ROUND_CORNER_DENSITY * 3].y + _XGLCornerData::roundCornerR + pos.y);
		}
		glVertex2f(_XGLCornerData::roundCornerData[0].x + size.x - _XGLCornerData::roundCornerR + pos.x,
			_XGLCornerData::roundCornerData[0].y + _XGLCornerData::roundCornerR + pos.y);
		glEnd();
		//中间
		glBegin(GL_POLYGON);
		if(down) glColor4fv(lColor); 
		else glColor4fv(dColor);
		glVertex2f(_XGLCornerData::roundCornerData[0].x + size.x - _XGLCornerData::roundCornerR + pos.x,
			_XGLCornerData::roundCornerData[0].y + size.y - _XGLCornerData::roundCornerR + pos.y);
		glVertex2f(_XGLCornerData::roundCornerData[ROUND_CORNER_DENSITY << 1].x + _XGLCornerData::roundCornerR + pos.x,
			_XGLCornerData::roundCornerData[ROUND_CORNER_DENSITY << 1].y + size.y - _XGLCornerData::roundCornerR + pos.y);
		if(down) glColor4fv(dColor);
		else glColor4fv(lColor);
		glVertex2f(_XGLCornerData::roundCornerData[ROUND_CORNER_DENSITY << 1].x + _XGLCornerData::roundCornerR + pos.x,
			_XGLCornerData::roundCornerData[ROUND_CORNER_DENSITY << 1].y + _XGLCornerData::roundCornerR + pos.y);
		glVertex2f(_XGLCornerData::roundCornerData[0].x + size.x - _XGLCornerData::roundCornerR + pos.x,
			_XGLCornerData::roundCornerData[0].y + _XGLCornerData::roundCornerR + pos.y);
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
		glVertex2f(pos.x + size.x,pos.y + size.y);
		glVertex2f(pos.x,pos.y + size.y);
		glEnd();
	}
	glDisable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
}
inline void drawCorner(const _XVector2& pos,const _XVector2& size,int style,int index)
{
	switch(style)
	{
	case 2:
		switch(index)
		{
		case 0:
			glBegin(GL_POLYGON);
			glVertex2f(pos.x + size.x,pos.y + size.y);
			for(int i = 0;i <= ROUND_CORNER_DENSITY;++ i)	
			{
				glVertex2f(pos.x + size.x - (_XGLCornerData::roundCornerData[i].x - _XGLCornerData::roundCornerR),
					pos.y + size.y + _XGLCornerData::roundCornerData[i].y - _XGLCornerData::roundCornerR);
			}
			glEnd();
			break;
		case 1:
			glBegin(GL_POLYGON);
			glVertex2f(pos.x,pos.y + size.y);
			for(int i = ROUND_CORNER_DENSITY;i <= (ROUND_CORNER_DENSITY << 1);++ i)	
			{
				glVertex2f(pos.x - (_XGLCornerData::roundCornerData[i].x + _XGLCornerData::roundCornerR),
					pos.y + size.y + _XGLCornerData::roundCornerData[i].y - _XGLCornerData::roundCornerR);
			}
			glEnd();
			break;
		case 2:
			glBegin(GL_POLYGON);
			glVertex2f(pos.x,pos.y);
			for(int i = (ROUND_CORNER_DENSITY << 1);i <= (ROUND_CORNER_DENSITY * 3);++ i)	
			{
				glVertex2f(pos.x - (_XGLCornerData::roundCornerData[i].x + _XGLCornerData::roundCornerR),
					pos.y + _XGLCornerData::roundCornerData[i].y + _XGLCornerData::roundCornerR);
			}
			glEnd();
			break;
		case 3:
			glBegin(GL_POLYGON);
			glVertex2f(pos.x + size.x,pos.y);
			for(int i = (ROUND_CORNER_DENSITY * 3);i < (ROUND_CORNER_DENSITY << 2);++ i)	
			{
				glVertex2f(pos.x + size.x - (_XGLCornerData::roundCornerData[i].x - _XGLCornerData::roundCornerR),
					pos.y + _XGLCornerData::roundCornerData[i].y + _XGLCornerData::roundCornerR);
			}
			glVertex2f(pos.x + size.x - (_XGLCornerData::roundCornerData[0].x - _XGLCornerData::roundCornerR),
				pos.y + _XGLCornerData::roundCornerData[0].y + _XGLCornerData::roundCornerR);
			glEnd();
			break;
		}
		break;
	case 3:
		switch(index)
		{
		case 0:
			glBegin(GL_POLYGON);
			glVertex2f(pos.x + size.x,pos.y + size.y);
			for(int i = 0;i <= ROUND_CORNER_DENSITY;++ i)	
			{
				glVertex2f(pos.x + size.x + _XGLCornerData::roundCornerData[i].x - _XGLCornerData::roundCornerR,
					pos.y + size.y - (_XGLCornerData::roundCornerData[i].y - _XGLCornerData::roundCornerR));
			}
			glEnd();
			break;
		case 1:
			glBegin(GL_POLYGON);
			glVertex2f(pos.x,pos.y + size.y);
			for(int i = ROUND_CORNER_DENSITY;i <= (ROUND_CORNER_DENSITY << 1);++ i)	
			{
				glVertex2f(pos.x + _XGLCornerData::roundCornerData[i].x + _XGLCornerData::roundCornerR,
					pos.y + size.y - (_XGLCornerData::roundCornerData[i].y - _XGLCornerData::roundCornerR));
			}
			glEnd();
			break;
		case 2:
			glBegin(GL_POLYGON);
			glVertex2f(pos.x,pos.y);
			for(int i = (ROUND_CORNER_DENSITY << 1);i <= (ROUND_CORNER_DENSITY * 3);++ i)	
			{
				glVertex2f(pos.x + _XGLCornerData::roundCornerData[i].x + _XGLCornerData::roundCornerR,
					pos.y - (_XGLCornerData::roundCornerData[i].y + _XGLCornerData::roundCornerR));
			}
			glEnd();
			break;
		case 3:
			glBegin(GL_POLYGON);
			glVertex2f(pos.x + size.x,pos.y);
			for(int i = (ROUND_CORNER_DENSITY * 3);i < (ROUND_CORNER_DENSITY << 2);++ i)	
			{
				glVertex2f(pos.x + size.x + _XGLCornerData::roundCornerData[i].x - _XGLCornerData::roundCornerR,
					pos.y - (_XGLCornerData::roundCornerData[i].y + _XGLCornerData::roundCornerR));
			}
			glVertex2f(pos.x + size.x + _XGLCornerData::roundCornerData[0].x - _XGLCornerData::roundCornerR,
				pos.y - (_XGLCornerData::roundCornerData[0].y + _XGLCornerData::roundCornerR));
			glEnd();
			break;
		}
		break;
	}
}
inline void cornerData(const _XVector2& pos,const _XVector2& size,int style,int index)
{
	switch(style)
	{
	case 1:
		switch(index)
		{
		case 0:
			for(int i = 0;i <= ROUND_CORNER_DENSITY;++ i)	
			{
				glVertex2f(_XGLCornerData::roundCornerData[i].x + size.x - _XGLCornerData::roundCornerR + pos.x,
					_XGLCornerData::roundCornerData[i].y + size.y - _XGLCornerData::roundCornerR + pos.y);
			}
			break;
		case 1:
			for(int i = 0;i <= ROUND_CORNER_DENSITY;++ i)
			{
				glVertex2f(_XGLCornerData::roundCornerData[i + ROUND_CORNER_DENSITY].x + _XGLCornerData::roundCornerR + pos.x,
					_XGLCornerData::roundCornerData[i + ROUND_CORNER_DENSITY].y + size.y - _XGLCornerData::roundCornerR + pos.y);
			}
			break;
		case 2:
			for(int i = 0;i <= ROUND_CORNER_DENSITY;++ i)
			{//左上角圆角
				glVertex2f(_XGLCornerData::roundCornerData[i + (ROUND_CORNER_DENSITY << 1)].x + _XGLCornerData::roundCornerR + pos.x,
					_XGLCornerData::roundCornerData[i + (ROUND_CORNER_DENSITY << 1)].y + _XGLCornerData::roundCornerR + pos.y);
			}
			break;
		case 3:
			for(int i = 0;i < ROUND_CORNER_DENSITY;++ i)
			{//左下角圆角
				glVertex2f(_XGLCornerData::roundCornerData[i + ROUND_CORNER_DENSITY * 3].x + size.x - _XGLCornerData::roundCornerR + pos.x,
					_XGLCornerData::roundCornerData[i + ROUND_CORNER_DENSITY * 3].y + _XGLCornerData::roundCornerR + pos.y);
			}
			glVertex2f(_XGLCornerData::roundCornerData[0].x + size.x - _XGLCornerData::roundCornerR + pos.x,
				_XGLCornerData::roundCornerData[0].y + _XGLCornerData::roundCornerR + pos.y);
			break;
		}
		break;
	default:
		switch(index)
		{
		case 0:
			glVertex2f(pos.x + size.x - _XGLCornerData::roundCornerR + _XGLCornerData::roundCornerData[0].x,
				pos.y + size.y - _XGLCornerData::roundCornerR + _XGLCornerData::roundCornerData[0].y);
			glVertex2f(pos.x + size.x,pos.y + size.y);
			glVertex2f(pos.x + size.x - _XGLCornerData::roundCornerR + _XGLCornerData::roundCornerData[ROUND_CORNER_DENSITY].x,
				pos.y + size.y - _XGLCornerData::roundCornerR + _XGLCornerData::roundCornerData[ROUND_CORNER_DENSITY].y);
			break;
		case 1:
			glVertex2f(pos.x + _XGLCornerData::roundCornerR + _XGLCornerData::roundCornerData[ROUND_CORNER_DENSITY].x,
				pos.y + size.y - _XGLCornerData::roundCornerR + _XGLCornerData::roundCornerData[ROUND_CORNER_DENSITY].y);
			glVertex2f(pos.x,pos.y + size.y);
			glVertex2f(pos.x + _XGLCornerData::roundCornerR + _XGLCornerData::roundCornerData[ROUND_CORNER_DENSITY << 1].x,
				pos.y + size.y - _XGLCornerData::roundCornerR + _XGLCornerData::roundCornerData[ROUND_CORNER_DENSITY << 1].y);
			break;
		case 2:
			glVertex2f(pos.x + _XGLCornerData::roundCornerR + _XGLCornerData::roundCornerData[ROUND_CORNER_DENSITY << 1].x,
				pos.y + _XGLCornerData::roundCornerR + _XGLCornerData::roundCornerData[ROUND_CORNER_DENSITY << 1].y);
			glVertex2f(pos.x,pos.y);
			glVertex2f(pos.x + _XGLCornerData::roundCornerR + _XGLCornerData::roundCornerData[ROUND_CORNER_DENSITY * 3].x,
				pos.y + _XGLCornerData::roundCornerR + _XGLCornerData::roundCornerData[ROUND_CORNER_DENSITY * 3].y);
			break;
		case 3:
			glVertex2f(pos.x + size.x - _XGLCornerData::roundCornerR + _XGLCornerData::roundCornerData[ROUND_CORNER_DENSITY * 3].x,
				pos.y + _XGLCornerData::roundCornerR + _XGLCornerData::roundCornerData[ROUND_CORNER_DENSITY * 3].y);
			glVertex2f(pos.x + size.x,pos.y);
			glVertex2f(pos.x + size.x - _XGLCornerData::roundCornerR + _XGLCornerData::roundCornerData[0].x,
				pos.y + _XGLCornerData::roundCornerR + _XGLCornerData::roundCornerData[0].y);
			break;
		}
		break;
	}
}
void drawFillBoxExEx(const _XVector2& pos,const _XVector2& size,
						float r,float g,float b,
						unsigned char lineStyle,unsigned char cornerStyle,bool down)
{
	_XFColor lColor(r * 1.1f,g * 1.1f,b * 1.1f,1.0f);
	_XFColor dColor(r * 0.9f,g * 0.9f,b * 0.9f,1.0f);
	glDisable(GL_TEXTURE_2D);
	if(cornerStyle != 0 && size.x >= _XGLCornerData::roundCornerD && size.y >= _XGLCornerData::roundCornerD)
	{//需要描绘圆角
		_XGLCornerData::roundCornerInit();
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
						glVertex2f(pos.x + _XGLCornerData::roundCornerR + _XGLCornerData::roundCornerData[i].x,
							pos.y + _XGLCornerData::roundCornerR + _XGLCornerData::roundCornerData[i].y);
					}
					break;
				case 2:glVertex2f(pos.x - _XGLCornerData::roundCornerR,pos.y);break;
				case 3:
					for(int i = ROUND_CORNER_DENSITY << 1;i <= ROUND_CORNER_DENSITY * 3;++ i)
					{//左上角圆角
						glVertex2f(pos.x + _XGLCornerData::roundCornerR + _XGLCornerData::roundCornerData[i].x,
							pos.y - (_XGLCornerData::roundCornerR + _XGLCornerData::roundCornerData[i].y));
					}
					break;
				}
				switch((cornerStyle & 0xc0) >> 6)
				{
				case 0:glVertex2f(pos.x + size.x,pos.y);break;
				case 1:glVertex2f(pos.x + size.x - _XGLCornerData::roundCornerR,pos.y);break;
				case 2:glVertex2f(pos.x + size.x + _XGLCornerData::roundCornerR,pos.y);break;
				case 3:
					for(int i = ROUND_CORNER_DENSITY * 3;i < ROUND_CORNER_DENSITY << 2;++ i)
					{//左上角圆角
						glVertex2f(pos.x + size.x - _XGLCornerData::roundCornerR + _XGLCornerData::roundCornerData[i].x,
							pos.y - (_XGLCornerData::roundCornerR + _XGLCornerData::roundCornerData[i].y));
					}
					glVertex2f(pos.x + size.x - _XGLCornerData::roundCornerR + _XGLCornerData::roundCornerData[0].x,
						pos.y - (_XGLCornerData::roundCornerR + _XGLCornerData::roundCornerData[0].y));
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
						glVertex2f(pos.x + size.x - _XGLCornerData::roundCornerR + _XGLCornerData::roundCornerData[i].x,
							pos.y + _XGLCornerData::roundCornerR + _XGLCornerData::roundCornerData[i].y);
					}
					glVertex2f(pos.x + size.x - _XGLCornerData::roundCornerR + _XGLCornerData::roundCornerData[0].x,
						pos.y + _XGLCornerData::roundCornerR + _XGLCornerData::roundCornerData[0].y);
					break;
				case 2:					
					for(int i = ROUND_CORNER_DENSITY * 3;i < ROUND_CORNER_DENSITY << 2;++ i)
					{
						glVertex2f(pos.x + size.x - (_XGLCornerData::roundCornerData[i].x - _XGLCornerData::roundCornerR),
							pos.y + _XGLCornerData::roundCornerR + _XGLCornerData::roundCornerData[i].y);
					}
					glVertex2f(pos.x + size.x - (_XGLCornerData::roundCornerData[0].x - _XGLCornerData::roundCornerR),
						pos.y + _XGLCornerData::roundCornerR + _XGLCornerData::roundCornerData[0].y);
					break;
				case 3:glVertex2f(pos.x + size.x,pos.y - _XGLCornerData::roundCornerR);break;
				}
				switch(cornerStyle & 0x03)
				{
				case 0:glVertex2f(pos.x + size.x,pos.y + size.y);break;
				case 1:glVertex2f(pos.x + size.x,pos.y + size.y - _XGLCornerData::roundCornerR);break;
				case 2:					
					for(int i = 0;i <= ROUND_CORNER_DENSITY;++ i)
					{
						glVertex2f(pos.x + size.x - (_XGLCornerData::roundCornerData[i].x - _XGLCornerData::roundCornerR),
							pos.y + size.y - _XGLCornerData::roundCornerR + _XGLCornerData::roundCornerData[i].y);
					}
					break;
				case 3:glVertex2f(pos.x + size.x,pos.y + size.y + _XGLCornerData::roundCornerR);break;
				}
				glEnd();
			}
			if(lineStyle & 0x04)
			{
				glBegin(GL_LINE_STRIP);
				switch(cornerStyle & 0x03)
				{
				case 0:glVertex2f(pos.x + size.x,pos.y + size.y);break;
				case 1:
					for(int i = 0;i <= ROUND_CORNER_DENSITY;++ i)
					{
						glVertex2f(pos.x + size.x - _XGLCornerData::roundCornerR + _XGLCornerData::roundCornerData[i].x,
							pos.y + size.y - _XGLCornerData::roundCornerR + _XGLCornerData::roundCornerData[i].y);
					}
					break;
				case 2:glVertex2f(pos.x + size.x + _XGLCornerData::roundCornerR,pos.y + size.y);break;					
				case 3:
					for(int i = 0;i <= ROUND_CORNER_DENSITY;++ i)
					{
						glVertex2f(pos.x + size.x + _XGLCornerData::roundCornerData[i].x - _XGLCornerData::roundCornerR,
							pos.y + size.y - (_XGLCornerData::roundCornerData[i].y - _XGLCornerData::roundCornerR));
					}
					break;
				}
				switch((cornerStyle & 0x0c) >> 2)
				{
				case 0:glVertex2f(pos.x,pos.y + size.y);break;
				case 1:glVertex2f(pos.x + _XGLCornerData::roundCornerR,pos.y + size.y);break;
				case 2:glVertex2f(pos.x - _XGLCornerData::roundCornerR,pos.y + size.y);break;					
				case 3:
					for(int i = ROUND_CORNER_DENSITY;i <= ROUND_CORNER_DENSITY << 1;++ i)
					{
						glVertex2f(pos.x + _XGLCornerData::roundCornerData[i].x + _XGLCornerData::roundCornerR,
							pos.y + size.y - (_XGLCornerData::roundCornerData[i].y - _XGLCornerData::roundCornerR));
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
						glVertex2f(pos.x + _XGLCornerData::roundCornerData[i].x + _XGLCornerData::roundCornerR,
							pos.y + size.y + _XGLCornerData::roundCornerData[i].y - _XGLCornerData::roundCornerR);
					}
					break;
				case 2:
					for(int i = ROUND_CORNER_DENSITY;i <= ROUND_CORNER_DENSITY << 1;++ i)
					{
						glVertex2f(pos.x - (_XGLCornerData::roundCornerData[i].x + _XGLCornerData::roundCornerR),
							pos.y + size.y + _XGLCornerData::roundCornerData[i].y - _XGLCornerData::roundCornerR);
					}
					break;
				case 3:glVertex2f(pos.x,pos.y + size.y + _XGLCornerData::roundCornerR);break;
				}
				switch((cornerStyle & 0x30) >> 4)
				{
				case 0:glVertex2fv(pos);break;
				case 1:glVertex2f(pos.x,pos.y + _XGLCornerData::roundCornerR);break;
				case 2:
					for(int i = ROUND_CORNER_DENSITY << 1;i <= ROUND_CORNER_DENSITY * 3;++ i)
					{
						glVertex2f(pos.x - (_XGLCornerData::roundCornerR + _XGLCornerData::roundCornerData[i].x),
							pos.y + _XGLCornerData::roundCornerR + _XGLCornerData::roundCornerData[i].y);
					}
					break;
				case 3:glVertex2f(pos.x,pos.y - _XGLCornerData::roundCornerR);break;
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
		glVertex2f(_XGLCornerData::roundCornerData[0].x + size.x - _XGLCornerData::roundCornerR + pos.x,
			_XGLCornerData::roundCornerData[0].y + size.y - _XGLCornerData::roundCornerR + pos.y);
		glVertex2f(_XGLCornerData::roundCornerData[ROUND_CORNER_DENSITY << 1].x + _XGLCornerData::roundCornerR + pos.x,
			_XGLCornerData::roundCornerData[ROUND_CORNER_DENSITY << 1].y + size.y - _XGLCornerData::roundCornerR + pos.y);
		if(down) glColor3fv(dColor);
		else glColor3fv(lColor);
		glVertex2f(_XGLCornerData::roundCornerData[ROUND_CORNER_DENSITY << 1].x + _XGLCornerData::roundCornerR + pos.x,
			_XGLCornerData::roundCornerData[ROUND_CORNER_DENSITY << 1].y + _XGLCornerData::roundCornerR + pos.y);
		glVertex2f(_XGLCornerData::roundCornerData[0].x + size.x - _XGLCornerData::roundCornerR + pos.x,
			_XGLCornerData::roundCornerData[0].y + _XGLCornerData::roundCornerR + pos.y);
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
				glVertex2f(pos.x + size.x,pos.y + size.y);
			}
			if(lineStyle & 0x04)
			{
				glVertex2f(pos.x + size.x,pos.y + size.y);
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
		glVertex2f(pos.x,pos.y + _XGLCornerData::roundCornerR);
		glVertex2fv(pos);
		glVertex2f(pos.x + size.x,pos.y);
		glVertex2f(pos.x + size.x,pos.y + _XGLCornerData::roundCornerR);
		glEnd();
		glBegin(GL_QUADS);
		if(down) glColor3fv(lColor);
		else glColor3fv(dColor);
		glVertex2f(pos.x + size.x,pos.y + size.y - _XGLCornerData::roundCornerR);
		glVertex2f(pos.x + size.x,pos.y + size.y);
		glVertex2f(pos.x,pos.y + size.y);
		glVertex2f(pos.x,pos.y + size.y - _XGLCornerData::roundCornerR);
		glEnd();
		glBegin(GL_QUADS);
		if(down) glColor3fv(dColor); 
		else glColor3fv(lColor);
		glVertex2f(pos.x,pos.y + _XGLCornerData::roundCornerR);
		glVertex2f(pos.x + size.x,pos.y + _XGLCornerData::roundCornerR);
		if(down) glColor3fv(lColor);
		else glColor3fv(dColor);
		glVertex2f(pos.x + size.x,pos.y + size.y - _XGLCornerData::roundCornerR);
		glVertex2f(pos.x,pos.y + size.y - _XGLCornerData::roundCornerR);
		glEnd();
	}
	glEnable(GL_TEXTURE_2D);
}
void drawFillBoxExExA(const _XVector2& pos,const _XVector2& size,
						float r,float g,float b,float a,
						unsigned char lineStyle,unsigned char cornerStyle,bool down)
{
	_XFColor lColor(r * 1.1f,g * 1.1f,b * 1.1f,a);
	_XFColor dColor(r * 0.9f,g * 0.9f,b * 0.9f,a);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	if(cornerStyle != 0 && size.x >= _XGLCornerData::roundCornerD && size.y >= _XGLCornerData::roundCornerD)
	{//需要描绘圆角
		_XGLCornerData::roundCornerInit();
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
						glVertex2f(pos.x + _XGLCornerData::roundCornerR + _XGLCornerData::roundCornerData[i].x,
							pos.y + _XGLCornerData::roundCornerR + _XGLCornerData::roundCornerData[i].y);
					}
					break;
				case 2:glVertex2f(pos.x - _XGLCornerData::roundCornerR,pos.y);break;
				case 3:
					for(int i = ROUND_CORNER_DENSITY << 1;i <= ROUND_CORNER_DENSITY * 3;++ i)
					{//左上角圆角
						glVertex2f(pos.x + _XGLCornerData::roundCornerR + _XGLCornerData::roundCornerData[i].x,
							pos.y - (_XGLCornerData::roundCornerR + _XGLCornerData::roundCornerData[i].y));
					}
					break;
				}
				switch((cornerStyle & 0xc0) >> 6)
				{
				case 0:glVertex2f(pos.x + size.x,pos.y);break;
				case 1:glVertex2f(pos.x + size.x - _XGLCornerData::roundCornerR,pos.y);break;
				case 2:glVertex2f(pos.x + size.x + _XGLCornerData::roundCornerR,pos.y);break;
				case 3:
					for(int i = ROUND_CORNER_DENSITY * 3;i < ROUND_CORNER_DENSITY << 2;++ i)
					{//左上角圆角
						glVertex2f(pos.x + size.x - _XGLCornerData::roundCornerR + _XGLCornerData::roundCornerData[i].x,
							pos.y - (_XGLCornerData::roundCornerR + _XGLCornerData::roundCornerData[i].y));
					}
					glVertex2f(pos.x + size.x - _XGLCornerData::roundCornerR + _XGLCornerData::roundCornerData[0].x,
						pos.y - (_XGLCornerData::roundCornerR + _XGLCornerData::roundCornerData[0].y));
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
						glVertex2f(pos.x + size.x - _XGLCornerData::roundCornerR + _XGLCornerData::roundCornerData[i].x,
							pos.y + _XGLCornerData::roundCornerR + _XGLCornerData::roundCornerData[i].y);
					}
					glVertex2f(pos.x + size.x - _XGLCornerData::roundCornerR + _XGLCornerData::roundCornerData[0].x,
						pos.y + _XGLCornerData::roundCornerR + _XGLCornerData::roundCornerData[0].y);
					break;
				case 2:					
					for(int i = ROUND_CORNER_DENSITY * 3;i < ROUND_CORNER_DENSITY << 2;++ i)
					{
						glVertex2f(pos.x + size.x - (_XGLCornerData::roundCornerData[i].x - _XGLCornerData::roundCornerR),
							pos.y + _XGLCornerData::roundCornerR + _XGLCornerData::roundCornerData[i].y);
					}
					glVertex2f(pos.x + size.x - (_XGLCornerData::roundCornerData[0].x - _XGLCornerData::roundCornerR),
						pos.y + _XGLCornerData::roundCornerR + _XGLCornerData::roundCornerData[0].y);
					break;
				case 3:glVertex2f(pos.x + size.x,pos.y - _XGLCornerData::roundCornerR);break;
				}
				switch(cornerStyle & 0x03)
				{
				case 0:glVertex2f(pos.x + size.x,pos.y + size.y);break;
				case 1:glVertex2f(pos.x + size.x,pos.y + size.y - _XGLCornerData::roundCornerR);break;
				case 2:					
					for(int i = 0;i <= ROUND_CORNER_DENSITY;++ i)
					{
						glVertex2f(pos.x + size.x - (_XGLCornerData::roundCornerData[i].x - _XGLCornerData::roundCornerR),
							pos.y + size.y - _XGLCornerData::roundCornerR + _XGLCornerData::roundCornerData[i].y);
					}
					break;
				case 3:glVertex2f(pos.x + size.x,pos.y + size.y + _XGLCornerData::roundCornerR);break;
				}
				glEnd();
			}
			if(lineStyle & 0x04)
			{
				glBegin(GL_LINE_STRIP);
				switch(cornerStyle & 0x03)
				{
				case 0:glVertex2f(pos.x + size.x,pos.y + size.y);break;
				case 1:
					for(int i = 0;i <= ROUND_CORNER_DENSITY;++ i)
					{
						glVertex2f(pos.x + size.x - _XGLCornerData::roundCornerR + _XGLCornerData::roundCornerData[i].x,
							pos.y + size.y - _XGLCornerData::roundCornerR + _XGLCornerData::roundCornerData[i].y);
					}
					break;
				case 2:glVertex2f(pos.x + size.x + _XGLCornerData::roundCornerR,pos.y + size.y);break;					
				case 3:
					for(int i = 0;i <= ROUND_CORNER_DENSITY;++ i)
					{
						glVertex2f(pos.x + size.x + _XGLCornerData::roundCornerData[i].x - _XGLCornerData::roundCornerR,
							pos.y + size.y - (_XGLCornerData::roundCornerData[i].y - _XGLCornerData::roundCornerR));
					}
					break;
				}
				switch((cornerStyle & 0x0c) >> 2)
				{
				case 0:glVertex2f(pos.x,pos.y + size.y);break;
				case 1:glVertex2f(pos.x + _XGLCornerData::roundCornerR,pos.y + size.y);break;
				case 2:glVertex2f(pos.x - _XGLCornerData::roundCornerR,pos.y + size.y);break;					
				case 3:
					for(int i = ROUND_CORNER_DENSITY;i <= ROUND_CORNER_DENSITY << 1;++ i)
					{
						glVertex2f(pos.x + _XGLCornerData::roundCornerData[i].x + _XGLCornerData::roundCornerR,
							pos.y + size.y - (_XGLCornerData::roundCornerData[i].y - _XGLCornerData::roundCornerR));
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
						glVertex2f(pos.x + _XGLCornerData::roundCornerData[i].x + _XGLCornerData::roundCornerR,
							pos.y + size.y + _XGLCornerData::roundCornerData[i].y - _XGLCornerData::roundCornerR);
					}
					break;
				case 2:
					for(int i = ROUND_CORNER_DENSITY;i <= ROUND_CORNER_DENSITY << 1;++ i)
					{
						glVertex2f(pos.x - (_XGLCornerData::roundCornerData[i].x + _XGLCornerData::roundCornerR),
							pos.y + size.y + _XGLCornerData::roundCornerData[i].y - _XGLCornerData::roundCornerR);
					}
					break;
				case 3:glVertex2f(pos.x,pos.y + size.y + _XGLCornerData::roundCornerR);break;
				}
				switch((cornerStyle & 0x30) >> 4)
				{
				case 0:glVertex2fv(pos);break;
				case 1:glVertex2f(pos.x,pos.y + _XGLCornerData::roundCornerR);break;
				case 2:
					for(int i = ROUND_CORNER_DENSITY << 1;i <= ROUND_CORNER_DENSITY * 3;++ i)
					{
						glVertex2f(pos.x - (_XGLCornerData::roundCornerR + _XGLCornerData::roundCornerData[i].x),
							pos.y + _XGLCornerData::roundCornerR + _XGLCornerData::roundCornerData[i].y);
					}
					break;
				case 3:glVertex2f(pos.x,pos.y - _XGLCornerData::roundCornerR);break;
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
		glVertex2f(_XGLCornerData::roundCornerData[0].x + size.x - _XGLCornerData::roundCornerR + pos.x,
			_XGLCornerData::roundCornerData[0].y + size.y - _XGLCornerData::roundCornerR + pos.y);
		glVertex2f(_XGLCornerData::roundCornerData[ROUND_CORNER_DENSITY << 1].x + _XGLCornerData::roundCornerR + pos.x,
			_XGLCornerData::roundCornerData[ROUND_CORNER_DENSITY << 1].y + size.y - _XGLCornerData::roundCornerR + pos.y);
		if(down) glColor4fv(dColor);
		else glColor4fv(lColor);
		glVertex2f(_XGLCornerData::roundCornerData[ROUND_CORNER_DENSITY << 1].x + _XGLCornerData::roundCornerR + pos.x,
			_XGLCornerData::roundCornerData[ROUND_CORNER_DENSITY << 1].y + _XGLCornerData::roundCornerR + pos.y);
		glVertex2f(_XGLCornerData::roundCornerData[0].x + size.x - _XGLCornerData::roundCornerR + pos.x,
			_XGLCornerData::roundCornerData[0].y + _XGLCornerData::roundCornerR + pos.y);
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
				glVertex2f(pos.x + size.x,pos.y + size.y);
			}
			if(lineStyle & 0x04)
			{
				glVertex2f(pos.x + size.x,pos.y + size.y);
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
		glVertex2f(pos.x,pos.y + _XGLCornerData::roundCornerR);
		glVertex2fv(pos);
		glVertex2f(pos.x + size.x,pos.y);
		glVertex2f(pos.x + size.x,pos.y + _XGLCornerData::roundCornerR);
		glEnd();
		glBegin(GL_QUADS);
		if(down) glColor4fv(lColor);
		else glColor4fv(dColor);
		glVertex2f(pos.x + size.x,pos.y + size.y - _XGLCornerData::roundCornerR);
		glVertex2f(pos.x + size.x,pos.y + size.y);
		glVertex2f(pos.x,pos.y + size.y);
		glVertex2f(pos.x,pos.y + size.y - _XGLCornerData::roundCornerR);
		glEnd();
		glBegin(GL_QUADS);
		if(down) glColor4fv(dColor); 
		else glColor4fv(lColor);
		glVertex2f(pos.x,pos.y + _XGLCornerData::roundCornerR);
		glVertex2f(pos.x + size.x,pos.y + _XGLCornerData::roundCornerR);
		if(down) glColor4fv(lColor);
		else glColor4fv(dColor);
		glVertex2f(pos.x + size.x,pos.y + size.y - _XGLCornerData::roundCornerR);
		glVertex2f(pos.x,pos.y + size.y - _XGLCornerData::roundCornerR);
		glEnd();
	}
	glDisable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
}
void drawFillBox(const _XVector2& p0,const _XVector2& p1,const _XVector2& p2,const _XVector2& p3,
				 float r,float g,float b)
{
	glDisable(GL_TEXTURE_2D);
	glColor3f(r,g,b);
	glBegin(GL_QUADS);
	glVertex2fv(p0);
	glVertex2fv(p1);
	glVertex2fv(p2);
	glVertex2fv(p3);
	glEnd();
	glEnable(GL_TEXTURE_2D);
}
void drawFillBoxEx(const _XVector2& p0,const _XVector2& p1,const _XVector2& p2,const _XVector2& p3,
				 float r,float g,float b,
				 bool withLine)
{
	glDisable(GL_TEXTURE_2D);
	if(withLine)
	{//描绘边框
		//if(XEE::isMultiSampleSupport && XEE::isLineSmooth)
		//{
		//	glEnable(GL_BLEND);
		//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//}
		glColor3f(0.15f,0.15f,0.15f);
		glLineWidth(1.0f);
		drawBoxS(p0,p1,p2,p3);
		//if(XEE::isMultiSampleSupport && XEE::isLineSmooth) glDisable(GL_BLEND);
	}
	glBegin(GL_QUADS);
	glColor3f(r * 1.1f,g * 1.1f,b * 1.1f);
	glVertex2fv(p0);
	glVertex2fv(p1);
	glColor3f(r * 0.9f,g * 0.9f,b * 0.9f);
	glVertex2fv(p2);
	glVertex2fv(p3);
	glEnd();
	glEnable(GL_TEXTURE_2D);
}
void drawFillPolygon(const _XVector2 *p,int sum,float r,float g,float b)
{
	glDisable(GL_TEXTURE_2D);
	glColor3f(r,g,b);
	glBegin(GL_POLYGON);
	for(int i = 0;i < sum;++ i)
	{
		glVertex2fv(p[i]);
	}
	glEnd();
	glEnable(GL_TEXTURE_2D);
}
void drawFillPolygon(const _XVector2 *p,int sum,const _XVector2 &pos,float angle,float r,float g,float b)
{
	glDisable(GL_TEXTURE_2D);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(pos.x,pos.y, 0);
	glRotatef(angle,0,0,1);

	glColor3f(r,g,b);
	glBegin(GL_POLYGON);
	for(int i = 0;i < sum;++ i)
	{
		glVertex2fv(p[i]);
	}
	glEnd();
	glPopMatrix();
	glEnable(GL_TEXTURE_2D);
}
void drawFillPolygonEx(const _XVector2 *p,const float *c,int sum,float r,float g,float b,bool withLine)
{
	glDisable(GL_TEXTURE_2D);
	if(withLine)
	{//描绘边框
		//if(XEE::isMultiSampleSupport && XEE::isLineSmooth)
		//{
		//	glEnable(GL_BLEND);
		//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//}
		glColor3f(0.15f,0.15f,0.15f);
		glLineWidth(1.0f);
		glBegin(GL_LINE_STRIP);
		for(int i = 0;i < sum;++ i)
		{
			glVertex2fv(p[i]);
		}
		glEnd();
		//if(XEE::isMultiSampleSupport && XEE::isLineSmooth) glDisable(GL_BLEND);
	}
	glBegin(GL_POLYGON);
	for(int i = 0;i < sum;++ i)
	{
		glColor3f(r * c[i],g * c[i],b * c[i]);
		glVertex2fv(p[i]);
	}
	glEnd();
	glEnable(GL_TEXTURE_2D);
}
void drawFillPolygonExA(const _XVector2 *p,const float *c,int sum,float r,float g,float b,float a,bool withLine)
{
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if(withLine)
	{//描绘边框
		//if(XEE::isMultiSampleSupport && XEE::isLineSmooth)
		//{
		//	glEnable(GL_BLEND);
		//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//}
		glColor4f(0.15f,0.15f,0.15f,a);
		glLineWidth(1.0f);
		glBegin(GL_LINE_STRIP);
		for(int i = 0;i < sum;++ i)
		{
			glVertex2fv(p[i]);
		}
		glEnd();
		//if(XEE::isMultiSampleSupport && XEE::isLineSmooth) glDisable(GL_BLEND);
	}
	glBegin(GL_POLYGON);
	for(int i = 0;i < sum;++ i)
	{
		glColor4f(r * c[i],g * c[i],b * c[i],a);
		glVertex2fv(p[i]);
	}
	glEnd();
	glDisable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
}
void drawFillBoxBW(const _XVector2& pos,const _XVector2& size,const _XVector2& cSize)
{
	glDisable(GL_TEXTURE_2D);
	glColor3f(1.0f,1.0f,1.0f);
	//先描绘一个白色底
	drawFillBoxS(pos.x,pos.y,size.x,size.y);
	int w = size.x / cSize.x;
	int h = size.y / cSize.y;
	_XVector2 tempPos;
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
	glEnable(GL_TEXTURE_2D);
	//方法2
	//int arraySize = w * (h >> 1) * 4;
	//_XVector2 *vPoint = createArrayMem<_XVector2>(arraySize);
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
	//glEnable(GL_TEXTURE_2D);
	//XDELETE_ARRAY(vPoint);
}
void drawTexture(_XTexture &tex,const _XVector2 &pos,int cW,int cH,
	_XVector2 *vArray,_XVector2 *uArray,int arrayW,int arrayH)
{
	if(vArray == NULL || uArray == NULL) return;
	int tW = tex.m_w;
	int tH = tex.m_h;
	int ux = (tW - cW) >> 1,uy = (tH - cH) >> 1;
	int vx = -(cW >> 1),vy = -(cH >> 1);

	glEnable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
#if WITH_XSPRITE_EX
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

	glBindTexture(GL_TEXTURE_2D,tex.m_texture);
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
#if WITH_XSPRITE_EX
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);   
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
#endif
	glDisable(GL_BLEND);
}
void drawTextureEx(unsigned int tex,const _XVector2 &pos,const _XVector2 &size,
				   _XVector2 *vArray,_XVector2 *uArray,int arrayW,int arrayH,bool blend)
{
	glEnable(GL_TEXTURE_2D);
	if(blend)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}else
	{
		glDisable(GL_BLEND);
	}
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(pos.x,pos.y,0);
	glScalef(size.x,size.y,1.0);

	glBindTexture(GL_TEXTURE_2D,tex);
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
	glDisable(GL_BLEND);
}
void drawTexture(unsigned int tex,const _XVector2 &pos,int texW,int texH,bool blend)
{
	glEnable(GL_TEXTURE_2D);
	if(blend)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}else
	{
		glDisable(GL_BLEND);
	}
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(pos.x,pos.y,0);
	//glScalef(0.5f,0.5f,1.0);

	glBindTexture(GL_TEXTURE_2D,tex);
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
	glDisable(GL_BLEND);
}
GLuint emptyTexture(int x,int y,_XColorMode mode)
{
	GLuint txtnumber;

	glGenTextures(1, &txtnumber);
	glBindTexture(GL_TEXTURE_2D, txtnumber);		

	//效果好
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
#if WITH_XSPRITE_EX
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
#endif
	//速度快
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	switch(mode)
	{
	case COLOR_RGBA:glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,x,y,0,GL_RGBA,GL_UNSIGNED_BYTE,0);break;
	case COLOR_RGB:glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,x,y,0,GL_RGB,GL_UNSIGNED_BYTE,0);break;
//	case COLOR_GRAY:glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,x,y,0,GL_RGB,GL_UNSIGNED_BYTE,0);break;	//感觉存在问题
	case COLOR_GRAY:glTexImage2D(GL_TEXTURE_2D,0,GL_LUMINANCE,x,y,0,GL_LUMINANCE,GL_UNSIGNED_BYTE,0);break;	//修改
	case COLOR_RGBA_F:glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA32F,x,y,0,GL_RGBA,GL_FLOAT,0);break;
//	case COLOR_GRAY_F:glTexImage2D(GL_TEXTURE_2D,0,GL_RGB32F,x,y,0,GL_RGB,GL_FLOAT,0);break;	//感觉存在问题
	case COLOR_GRAY_F:glTexImage2D(GL_TEXTURE_2D,0,GL_LUMINANCE,x,y,0,GL_LUMINANCE,GL_FLOAT,0);break;	//修改
	}
	//glClear(GL_COLOR_BUFFER_BIT);
	//初始化像素
	//glClearTexImage();	//这个貌似要GL3.0才支持，目前的版本好像不支持

	return txtnumber;
}

struct _XScreenShotData
{
	char fileName[MAX_FILE_NAME_LENGTH];
	int w;
	int h;
	unsigned int *data;
	_XScreenShotData()
		:data(NULL)
	{}
};
_XScreenShotData *screenShotData = NULL;
//由于这里使用了多线程，这里需要进行数据互斥锁，尚未加入
#ifdef XEE_OS_WINDOWS
DWORD WINAPI saveDataToPNG(void *)
#endif
#ifdef XEE_OS_LINUX
static void *saveDataToPNG(void * pParam)
#endif
{
	if(screenShotData == NULL || screenShotData->data == NULL) 
	{
#ifdef XEE_OS_WINDOWS
		return 1;
#endif
#ifdef XEE_OS_LINUX
		return;
#endif
	}
	//将数据保存成图片
	unsigned char *pData = createArrayMem<unsigned char>(screenShotData->w * screenShotData->h * 4);
	int w = screenShotData->w << 2;
	unsigned char *pd = pData + screenShotData->w * screenShotData->h * 4 - w;
	unsigned char *ps = (unsigned char *)screenShotData->data;
	for(int i = 0;i < screenShotData->h;++ i)	//上下翻转
	{
		memcpy(pd,ps,w);
		ps += w;
		pd -= w;
	}
//	printf("%d\n",getCurrentTicks());
	savePNG(screenShotData->fileName,pData,screenShotData->w,screenShotData->h,COLOR_RGBA,Z_BEST_COMPRESSION);
//	printf("%d\n",getCurrentTicks());
	//释放资源
	XDELETE_ARRAY(pData);
	XDELETE_ARRAY(screenShotData->data);
	XDELETE(screenShotData);
#ifdef XEE_OS_WINDOWS
	return 1;
#endif
}
//由于这里使用了多线程，这里需要进行数据互斥锁，尚未加入
_XBool gameShot(const char *fileName,int w,int h,bool withThread,float x,float y)
{
	if(fileName == NULL) return XFalse;
	if(x < 0.0f || y < 0.0f) return XFalse;
	if(w <= 0 || h <= 0) return XFalse;
	if(screenShotData != NULL) return XFalse;
	screenShotData = createMem<_XScreenShotData>();
	if(screenShotData == NULL) return XFalse;

	screenShotData->h = h;
	screenShotData->w = w;
	strcpy(screenShotData->fileName,fileName);
	screenShotData->data = createArrayMem<unsigned int>(w * h);
	if(screenShotData->data == NULL)
	{
		XDELETE(screenShotData);
		return XFalse;
	}

	//读取数据
	//glReadPixels(0,0,w,h,GL_RGBA,GL_BYTE,screenShotData->data);
	glReadPixels(x,y,w,h,GL_RGBA,GL_UNSIGNED_BYTE,screenShotData->data);
	if(withThread)
	{//优化之后直接开启一个线程来保存图片
#ifdef XEE_OS_WINDOWS
		DWORD tempThreadP;
		if(CreateThread(0,0,saveDataToPNG,NULL,0,&tempThreadP) == 0)
		{
			return XFalse;
		}
#endif
#ifdef XEE_OS_LINUX
		pthread_t tempThreadP;
			if(pthread_create(&tempThreadP, NULL, &(saveDataToPNG), NULL) != 0)
			{
			return XFalse;
		}
#endif
	}else
	{
		saveDataToPNG(NULL);
	}
	return XTrue;
}
#include "XShaderGLSL.h"
void _XTexture::updateTexture(const _XPBO &pbo)		//使用指定数据更新贴图
{
	pbo.bind();
	updateTexture(NULL);
	//glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB,0);
}
void _XTexture::updateTextureR2B(const _XPBO &pbo)		//使用指定数据更新贴图
{
	pbo.bind();
	updateTextureR2B(NULL);
	//glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB,0);
}
void _XTexture::updateTexture(const void *data,int w,int h,int x,int y)
{
	if(!m_isLoaded) return;
	//if(data == NULL) return;	//PBO的时候可以为 NULL
	if(w <= 0) w = m_w;
	if(h <= 0) h = m_h;
	if(glIsTexture(m_texture))
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,m_texture);
		switch(m_mode)
		{
		case COLOR_RGBA:
			glTexSubImage2D(GL_TEXTURE_2D,0,x,y,w,h,GL_RGBA,GL_UNSIGNED_BYTE,data);
			break;
		case COLOR_RGB:
			glTexSubImage2D(GL_TEXTURE_2D,0,x,y,w,h,GL_RGB,GL_UNSIGNED_BYTE,data);
			break;
		case COLOR_GRAY:
			glTexSubImage2D(GL_TEXTURE_2D,0,x,y,w,h,GL_LUMINANCE,GL_UNSIGNED_BYTE,data);
			break;
		case COLOR_RGBA_F:
			glTexSubImage2D(GL_TEXTURE_2D,0,x,y,w,h,GL_RGBA,GL_FLOAT,data);
			break;
		case COLOR_GRAY_F:
			glTexSubImage2D(GL_TEXTURE_2D,0,x,y,w,h,GL_LUMINANCE,GL_FLOAT,data);
			break;
		}

		glDisable(GL_TEXTURE_2D);
	}
}
void _XTexture::updateTextureR2B(const void *data,int w,int h,int x,int y)
{
	if(!m_isLoaded) return;
	//if(data == NULL) return; //PBO时为null
	if(w <= 0) w = m_w;
	if(h <= 0) h = m_h;
	if(glIsTexture(m_texture))
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,m_texture);
		switch(m_mode)
		{
		case COLOR_RGBA:
			glTexSubImage2D(GL_TEXTURE_2D,0,x,y,w,h,GL_BGRA,GL_UNSIGNED_BYTE,data);
			break;
		case COLOR_RGB:
			glTexSubImage2D(GL_TEXTURE_2D,0,x,y,w,h,GL_BGR,GL_UNSIGNED_BYTE,data);
			break;
		case COLOR_GRAY:
			glTexSubImage2D(GL_TEXTURE_2D,0,x,y,w,h,GL_LUMINANCE,GL_UNSIGNED_BYTE,data);
			break;
		case COLOR_RGBA_F:
			glTexSubImage2D(GL_TEXTURE_2D,0,x,y,w,h,GL_RGBA,GL_FLOAT,data);
			break;
		case COLOR_GRAY_F:
			glTexSubImage2D(GL_TEXTURE_2D,0,x,y,w,h,GL_LUMINANCE,GL_FLOAT,data);
			break;
		}

		glDisable(GL_TEXTURE_2D);
	}
}
//void _XTexture::updateTextureGray(const unsigned char *data)
//{
//	if(!m_isLoaded) return;
//	if(data == NULL) return;
//	if(glIsTexture(m_texture))
//	{
//		glEnable(GL_TEXTURE_2D);
//		glBindTexture(GL_TEXTURE_2D,m_texture);
//		if(m_mode == 0) glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_w, m_h, GL_R,GL_UNSIGNED_BYTE,data);
//		else glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_w, m_h, GL_R,GL_UNSIGNED_BYTE,data);
//		glDisable(GL_TEXTURE_2D);
//	}
//}
_XBool _XTexture::createTexture(int w,int h,_XColorMode mode)
{
	if(m_isLoaded) return XTrue;	//如果资源已经载入了，则不能重复载入
	if(w <= 0 ||
		h <= 0) return XFalse;
	try
	{
#if TEX_DEBUG1
		CPSum ++;
		printf("%d-\n",CPSum);
#endif
		m_cp = new _XSCounter;
		if(m_cp == NULL) return XFalse;
	}catch(...)
	{
		return XFalse;
	}
//	m_fileName = NULL;	//没有文件名
	m_mode = mode;

	m_w = w;
	m_h = h;
	m_texture = emptyTexture(m_w,m_h,m_mode);

	m_isLoaded = XTrue;
	return XTrue;
}
_XBool _XTexture::createWithTexture(int w,int h,_XColorMode mode,unsigned int tex)
{
	if(m_isLoaded) return XTrue;	//如果资源已经载入了，则不能重复载入
	if(w <= 0 ||
		h <= 0) return XFalse;
	try
	{
#if TEX_DEBUG1
		CPSum ++;
		printf("%d-\n",CPSum);
#endif
		m_cp = new _XSCounter;
		if(m_cp == NULL) return XFalse;
	}catch(...)
	{
		return XFalse;
	}
//	m_fileName = NULL;	//没有文件名
	m_mode = mode;

	m_w = w;
	m_h = h;
	m_texture = tex;

	m_isLoaded = XTrue;
	return XTrue;
}
#include "XResourceManager.h"
_XBool _XTexture::load(const char *filename,_XResourcePosition resoursePosition)//从指定文件中载入图片资源
{
	if(m_isLoaded) return XTrue;	//如果资源已经载入了，则不能重复载入
	try
	{
#if TEX_DEBUG1
		CPSum ++;
		printf("%d-\n",CPSum);
#endif
		m_cp = new _XSCounter;
		if(m_cp == NULL) return XFalse;
	}catch(...)
	{
		return XFalse;
	}
//	m_fileName = createArrayMem<char>(MAX_FILE_NAME_LENGTH);
//	if(m_fileName == NULL)
//	{
//		XDELETE(m_cp);
//		return XFalse;
//	}
//	strcpy(m_fileName,filename);
	m_resInfo = _XResourceManager::GetInstance().loadResource(filename,RESOURCE_TYPE_TEXTURE,resoursePosition);
	if(m_resInfo != NULL)
	{
		_XResourceTex *tempResTex = (_XResourceTex *)m_resInfo->m_pointer;
		m_texture = tempResTex->m_texID;
		m_w = tempResTex->m_width;
		m_h = tempResTex->m_height;
	//if(TextureLoadEx(m_texture,filename,&m_w,&m_h,resoursePosition))
	//{
	//	printf("%d\n",m_texture);
#if TEX_DEBUG
		++ XTSum;
		printf("%d:%d+%s\n",XTSum,m_texture,filename);
#endif
		m_isLoaded = XTrue;
		return XTrue;
	}
	XDELETE(m_cp);
//	XDELETE_ARRAY(m_fileName);
	m_isLoaded = XFalse;
	return XFalse;
}
_XBool _XTexture::reset()
{
	int sum = m_w * m_h;
	switch(m_mode)
	{
	case COLOR_RGBA:
	case COLOR_BGRA:
		sum = (sum << 2);
		break;
	case COLOR_RGB:
	case COLOR_BGR:
		sum = sum * 3;
		break;
	case COLOR_GRAY:
		break;
	case COLOR_RGBA_F:
		sum = (sum << 4);
		break;
	case COLOR_GRAY_F:
		sum = (sum << 2);
		break;
	}
	unsigned char *pData = createArrayMem<unsigned char>(sum);
	if(pData == NULL) return XFalse;
	memset(pData,0,sum);
	updateTexture(pData);
	XDELETE_ARRAY(pData);
	return XTrue;
}
void _XTexture::release()	//释放图片资源
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
			LogNull("delete texture:%d",m_texture);
			glDeleteTextures(1, &m_texture);
		}
	}else
	{
		if(_XResourceManager::GetInstance().releaseResource(m_resInfo))
			m_resInfo = NULL;
	}
	m_isLoaded = XFalse;
}
_XTexture::_XTexture(const _XTexture& temp)	//拷贝构造函数
{
	if(this == &temp) return;		//防止自生拷贝
	if(temp.m_cp != NULL) ++temp.m_cp->m_counter;
	m_texture = temp.m_texture;
	m_w = temp.m_w;
	m_h = temp.m_h;
	m_isLoaded = temp.m_isLoaded;
	m_cp = temp.m_cp;
//	m_fileName = temp.m_fileName;

	m_resInfo = _XResourceMng.copyResource(temp.m_resInfo);
	return;	
}
_XTexture& _XTexture::operator = (const _XTexture &temp)
{
	if(this == &temp) return *this;		//防止自生拷贝
	if(temp.m_cp != NULL) ++temp.m_cp->m_counter;
	if(m_cp != NULL && -- m_cp->m_counter <= 0)
	{
		release();	//引用计数结束，释放资源
#if TEX_DEBUG1
		CPSum --;
		printf("%d-\n",CPSum);
#endif
		XDELETE(m_cp);
//		XDELETE_ARRAY(m_fileName);
	}
	m_texture = temp.m_texture;
	m_w = temp.m_w;
	m_h = temp.m_h;
	m_isLoaded = temp.m_isLoaded;
	m_cp = temp.m_cp;
//	m_fileName = temp.m_fileName;
	if(m_resInfo != NULL) -- m_resInfo->m_counter;
	m_resInfo = _XResourceMng.copyResource(temp.m_resInfo);
	return *this;	
}