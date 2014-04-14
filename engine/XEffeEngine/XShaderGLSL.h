#ifndef _JIA_XSHADER_GLSL_
#define _JIA_XSHADER_GLSL_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2014.1.1
//--------------------------------
//#include "glew.h"
#include "XBasicFun.h"
//#include "XBasicOpenGL.h"
//数据类型
enum _XShaderDataType		//GLSL传递的数据类型
{
	DATA_TYPE_INT,			//整形
	DATA_TYPE_FLOAT,		//单精度浮点型
	DATA_TYPE_INT_ARRAY,	//1D整形
	DATA_TYPE_FLOAT_ARRAY,	//1D单精度
	DATA_TYPE_2INT_ARRAY,	//2D整形
	DATA_TYPE_2FLOAT_ARRAY,	//2D单精度
	DATA_TYPE_3INT_ARRAY,	//3D整形
	DATA_TYPE_3FLOAT_ARRAY,	//3D单精度
	DATA_TYPE_3FLOAT_MATRIX,//3x3矩阵
	DATA_TYPE_4FLOAT_MATRIX,//4x4矩阵
};
#define MAX_SHADER_GLSL_DATA_SUM (256)	//可以连接的数据的最大数量
class _XShaderDataInfo
{
public:
	int m_handle;				//数据的句柄
	//char m_name[256];			//数据的名称
	_XShaderDataType m_type;	//数据的类型
	void *m_p;					//数据的指针
	int m_size;

	void updateData();	//更新数据
};
//下面是关于UBO的封装
class _XUBO
{
private:
	_XBool m_isInited;
	unsigned int m_uboHandle;
public:
	_XUBO()
		:m_isInited(XFalse)
	{
	}
	~_XUBO() {release();}
	void release();
	_XBool init(int size,const void * p);
	void useUBO(unsigned int index);	//index为对应的shader中结构的索引
};
class _XShaderUBOData
{
public:
	unsigned int m_handle;
	char m_name[256];
	int m_offset[MAX_SHADER_GLSL_DATA_SUM];
	unsigned int m_index[MAX_SHADER_GLSL_DATA_SUM];
	int m_size;
};
class _XShaderUBOInfo
{
public:
	unsigned int m_handle;
	_XUBO *m_pUBO;

	void updateData() {m_pUBO->useUBO(m_handle);}
};
class _XShaderGLSL
{//目前对UBO的支持有限
private:
	_XBool m_isInited;		//是否初始化
	int m_shaderHandle;	//shader的句柄
	int m_dataSum;			//数据的数量
	_XShaderDataInfo m_dataInfo[MAX_SHADER_GLSL_DATA_SUM];	//链接数据的句柄
	int m_texSum;
	unsigned int *m_texture[MAX_SHADER_GLSL_DATA_SUM];
	int m_texHandle[MAX_SHADER_GLSL_DATA_SUM];
	int m_texType[MAX_SHADER_GLSL_DATA_SUM];

	int m_UBOSum;
	_XShaderUBOInfo m_UBOInfo[MAX_SHADER_GLSL_DATA_SUM];
public:
	_XBool init(const char* vertFile,const char* fragFile,
		_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	_XBool connectData(const char *name,_XShaderDataType type,int size,void *p);//链接数据
	_XBool connectTexture(const char *name,unsigned int * tex,int type = 0);//链接贴图
	void useShader(bool withTex0 = false);	//使用shader,参数withTex0适用于设置是否需要绑定第一张贴图，
											//如果需要绑定第一张贴图则设置为true,因为存在公用第一张贴图的情况所以有这个设置
	void useShaderEx(unsigned int tex0,int type = 0);	//调用的时候才传入第一张贴图
	void disShader();
	int getShaderHandle() { return m_shaderHandle;}
	_XShaderGLSL()
		:m_isInited(XFalse)
	{}
	//下面是为了提升性能而增加的接口
	_XBool connectUBO(const char *uboName,_XUBO *ubo);
	//从shader中读取UBO的相关信息
	_XBool getUBOInfo(_XShaderUBOData &uboData,int valueSum,const char *uboName,const char **valueNames);	//连接UBO
};

//使用UBO代替直接的GLSL的数据传递，以提升效率
//下面是关于使用UBO的例子
//GLuint blockIndex = glGetUniformBlockIndex(m_shaderHandle,"BlobSettings");  //获取结构的编号
//GLint blockSize;  
//glGetActiveUniformBlockiv(m_shaderHandle,  
//                            blockIndex,  
//                            GL_UNIFORM_BLOCK_DATA_SIZE,  
//                            &blockSize);					//获取结构的尺寸
//GLubyte *blockBuffer = (GLubyte*)malloc(blockSize);		//分配一块内存空间
//const GLchar *names[] = {"BlobSettings.InnerColor","BlobSettings.OuterColor",  
//                            "BlobSettings.RadiusInner","BlobSettings.RadiusOuter"};  
//GLuint indices[4];  
//glGetUniformIndices(m_shaderHandle, 4, names, indices);  //获取每个变量的标签
//  
//GLint offset[4];  
//glGetActiveUniformsiv(m_shaderHandle, 4, indices, GL_UNIFORM_OFFSET, offset);	//获取每个变量的偏移
//  
//GLfloat outerColor[] = {0.0f, 0.0f, 0.0f, 0.0f};  
//GLfloat innerColor[] = {1.0f, 1.0f, 0.75f, 1.0f};  
//GLfloat innerRadius = 0.25f, outerRadius = 0.45f;	//初始化相应的变量
//  
//memcpy(blockBuffer + offset[0],innerColor,4 * sizeof(float));  
//memcpy(blockBuffer + offset[1],outerColor,4 * sizeof(float));  
//memcpy(blockBuffer + offset[2],&innerRadius,sizeof(float));  
//memcpy(blockBuffer + offset[3],&outerRadius,sizeof(float));		//复制数据
////create OpenGL buffer UBO to store the data  
//GLuint uboHandle;  
//glGenBuffers(1,&uboHandle);  
//glBindBuffer(GL_UNIFORM_BUFFER,uboHandle);  
//glBufferData(GL_UNIFORM_BUFFER,blockSize,blockBuffer,GL_DYNAMIC_DRAW);			//建立UBO                   
////bind the UBO th the uniform block  
//glBindBufferBase(GL_UNIFORM_BUFFER,blockIndex,uboHandle);		//绑定UBO
//free(blockBuffer);												//释放临时的内存空间
////GLSL中的代码
//layout(std140) uniform BlobSettings
//{  
//    vec4 InnerColor;  
//    vec4 OuterColor;  
//    float RadiusInner;  
//    float RadiusOuter;  
//}Blob;//Use an instance name with a uniform block  
//下面使用PBO的方式读取屏幕数据期望以提升性能
#ifndef GL_FRONT
#define GL_FRONT (0x4040)
#endif
enum _XPboMode
{
	PBO_MODE_UNPACK,
	PBO_MODE_PACK,
};
enum _XColorMode;
//说明：在使用PBO来更新贴图的时候只有当需要将RGB转换成BGR的时候回来带有价值的性能提升外
//其他形式完全没有必要
class _XPBO	
{
private:
	_XBool m_isInited;
	unsigned int m_pboID[2];	//使用奇偶轮换的方式
	int m_nowPBOIndex;
	int m_size;
	_XColorMode m_colorType;
	int m_w;	//屏幕宽度
	int m_h;	//屏幕高度
	int m_px;	//截取图像的左上角点
	int m_py;
	int m_wx;	//截取图像的宽度
	int m_wy;
	int m_dataSize;
	_XPboMode m_mode;
public:
	_XPBO()
		:m_isInited(0)
		,m_nowPBOIndex(0)
		,m_mode(PBO_MODE_PACK)
	{
	}
	~_XPBO()
	{
		release();
	}
	void release();
	_XBool init(int w,int h,int px,int py,int wx,int wy,_XColorMode colorType,_XPboMode mode = PBO_MODE_PACK);
	_XBool getPixel(unsigned char * buff,int target = GL_FRONT);
	_XBool setPixel(unsigned char * buff);
	void bind() const;
};
////++++++++++++++++++++++++++++++++++++++++++++++
////下面是关于PBO的
////建立
//GLuint pboID;
//unsigned char *dataBuff;
//glGenBuffersARB(1,&pboID);
//glBindBufferARB(GL_PIXEL_PACK_BUFFER_ARB,pboID);
//glBufferDataARB(GL_PIXEL_PACK_BUFFER_ARB,1280 * 720 * 3,0,GL_STREAM_READ_ARB);
//glBindBufferARB(GL_PIXEL_PACK_BUFFER_ARB,0);
////使用
//glBindBufferARB(GL_PIXEL_PACK_BUFFER_ARB,pboID);
//glReadPixels(0,0,1280,720,GL_RGB,GL_UNSIGNED_BYTE,0);
////glBindBufferARB(GL_PIXEL_PACK_BUFFER_ARB, pboIds[nextIndex]); //不知道这个是干啥的
//GLubyte* src = (GLubyte*)glMapBufferARB(GL_PIXEL_PACK_BUFFER_ARB,GL_READ_ONLY_ARB);
//if(src)
//{//已经获取数据
//	//这里将数据取出
//	memcpy(dataBuff,src,1280 * 720 * 3);
//	glUnmapBufferARB(GL_PIXEL_PACK_BUFFER_ARB);     // release pointer to the mapped buffer
//}
//glBindBufferARB(GL_PIXEL_PACK_BUFFER_ARB,0);
////释放
//glDeleteBuffersARB(1,&pboID);
////--------------------------------------------------
//下面使用VBO的方式提升性能
class _XVBO
{
private:
	_XBool m_isInited;
	_XBool m_withV;	//顶点
	_XBool m_withT;	//贴图
	_XBool m_withN;	//法线
	_XBool m_withC;	//颜色
	_XBool m_withI;	//索引
	unsigned int m_v;
	unsigned int m_t;
	unsigned int m_n;
	unsigned int m_c;
	unsigned int m_i;
	int m_size;
	int m_indexSize;	//索引的大小
public:
	_XBool init(int size,const float *v,const float *t = NULL,const float *n = NULL,const float *c = NULL,
		int iSize = 0,const unsigned int *i = NULL);
	_XBool updateDate(int size,const float *v,const float *t = NULL,const float *n = NULL,const float *c = NULL,
		int iSize = 0,const unsigned int *i = NULL);
	_XBool updateDataV(int size,const float *v);
	_XBool updateDataT(int size,const float *t);
	_XBool updateDataN(int size,const float *n);
	_XBool updateDataC(int size,const float *c);
	_XBool updateDataI(int size,const unsigned int *i);
	void use(_XBool withTex = XTrue);
	void disuse();
	void release();
	int getSize() {return m_size;}
	int getIndexSize() {return m_indexSize;}
	//temp:从属的对象
	//v,n,c,t:是否有这些属性
	//cv,cn,cc,ct:属性是否从从属对象中获取
	_XBool setAsSubject(const _XVBO &temp,_XBool v,_XBool n,_XBool c,_XBool t,_XBool i,_XBool cv,_XBool cn,_XBool cc,_XBool ct,_XBool ci);
	_XBool m_isSubject;
	_XBool m_subjectV;
	_XBool m_subjectN;
	_XBool m_subjectC;
	_XBool m_subjectT;
	_XBool m_subjectI;

	_XVBO()
		:m_isInited(XFalse)
		,m_withV(XFalse)
		,m_withT(XFalse)
		,m_withN(XFalse)
		,m_withC(XFalse)
		,m_isSubject(XFalse)
		,m_v(0)
		,m_n(0)
		,m_t(0)
		,m_c(0)
		,m_i(0)
	{
	}
	~_XVBO()
	{
		release();
	}
};
#define MAX_FBO_TEXTURE_SUM (256)
enum _XFBOTextureType
{
	TEXTURE_RGB,
	TEXTURE_RGBA,	//普通的RGBA贴图
	TEXTURE_RGBA_F,	//RGBA都是float数据
	TEXTURE_DEEPTH,	//深度贴图
};
extern _XBool isFBOSupported();		//判断底层是否支持FBO
extern _XBool isFramebufferReady();	//判断FBO状态是否就绪
//下面是对FBO的支持
class _XFBO
{
private:
	unsigned int m_fboId;								//FBO的ID
	unsigned int m_rboId;							//RBO的ID
	unsigned int m_textureId[MAX_FBO_TEXTURE_SUM];    //FBO对应贴图的ID
	int m_nowFBOTextureSum;
	int m_maxTextureSum;	//一次能同时使用的贴图的数量
	//int m_upTextureSum;		//上次绑定的贴图数量
public:
	_XBool init(int w,int h,_XFBOTextureType type = TEXTURE_RGBA); //w和h是纹理的尺寸，返回初始化是否成功
	void useFBO(bool newSize = false,int w = 0,int h = 0);	//参数为是否使用新的视口矩阵
	void useFBO3D();	//针对3D的情况额外封装一下
	void attachTex(int order = 0);	//需要在useFBO之后调用
	_XBool attachTexs(int sum,int index,...);	//需要在useFBO之后调用
	void removeFBO();
	void removeFBO3D();
	void addOneTexture(int w,int h,_XFBOTextureType type = TEXTURE_RGBA);
	unsigned int getTexture(int order)	//存在问题
	{
		if(order >= 0 && order < m_nowFBOTextureSum)
		{
			return m_textureId[order];
		}
		return 0;
	}
	void release();
	_XFBO()
		:m_nowFBOTextureSum(0)
	{}
	~_XFBO() {release();}
};

//为了解决shader与FBO的结合而定义的函数
extern void drawBlankPlane(int w,int h,unsigned int tex,_XShaderGLSL *pShader = NULL);
#endif //_JIA_XSHADER_GLSL_