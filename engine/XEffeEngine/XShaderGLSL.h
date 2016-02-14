#ifndef _JIA_XSHADER_GLSL_
#define _JIA_XSHADER_GLSL_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2014.1.1
//--------------------------------
//#include "XBasicFun.h"
//#include "XBasicOpenGL.h"
#include <vector>
//
#include "XEngineCommon.h"
namespace XE{
//数据类型
enum XShaderDataType		//GLSL传递的数据类型
{
	DATA_TYPE_INT,			//整形
	DATA_TYPE_FLOAT,		//单精度浮点型float
	DATA_TYPE_INT_ARRAY,	//1D整形
	DATA_TYPE_FLOAT_ARRAY,	//1D单精度
	DATA_TYPE_2INT_ARRAY,	//2D整形
	DATA_TYPE_2FLOAT_ARRAY,	//2D单精度
	DATA_TYPE_3INT_ARRAY,	//3D整形
	DATA_TYPE_3FLOAT_ARRAY,	//3D单精度
	DATA_TYPE_3FLOAT_MATRIX,//3x3矩阵
	DATA_TYPE_4FLOAT_MATRIX,//4x4矩阵
	DATA_TYPE_2INT,			//整形
	DATA_TYPE_3INT,			//整形
	DATA_TYPE_2FLOAT,		//单精度浮点型vec2
	DATA_TYPE_3FLOAT,		//单精度浮点型vec3
	DATA_TYPE_4FLOAT,		//单精度浮点型vec4
};
class XShaderDataInfo
{
public:
	int m_handle;				//数据的句柄
	//char m_name[256];			//数据的名称
	XShaderDataType m_type;	//数据的类型
	void *m_p;					//数据的指针
	int m_size;

	void updateData();	//更新数据
};
//下面是关于UBO的封装
class XUBO
{
private:
	XBool m_isInited;
	unsigned int m_uboHandle;
public:
	XUBO()
		:m_isInited(XFalse)
		,m_uboHandle(0)
	{}
	~XUBO() {release();}
	void release();
	XBool init(int size,const void * p);
	void useUBO(unsigned int index);	//index为对应的shader中结构的索引
};
class XShaderUBOInfo
{
public:
	unsigned int m_handle;
	XUBO *m_pUBO;

	void updateData() {m_pUBO->useUBO(m_handle);}
};
enum XShaderTexType
{
	TEX_TYPE_2D,
	TEX_TYPE_CUBE,
};
struct XSaderTexInfo
{
	unsigned int * texID;
	int texHandle;
	XShaderTexType texType;
};
class XShaderUBOData;
class XShaderGLSL
{//目前对UBO的支持有限
public:
	static const int m_maxShaderGLSLDataSum = 256;	//可以连接的数据的最大数量
private:
	XBool m_isInited;		//是否初始化
	XShaderHandle m_shaderHandle;
	std::vector<XShaderDataInfo> m_dataInfos;
	std::vector<XSaderTexInfo> m_texInfos;
	std::vector<XShaderUBOInfo> m_UBOInfos;
	void release();
public:
	void updateData();
	XBool init(const char* vertFile,const char* fragFile,
		XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE,const char* geomFile = NULL);
	XBool initFromStr(const char *vStr,const char *fStr,const char * gStr = NULL);
	XBool connectData(const char *name,XShaderDataType type,int size,void *p);//链接数据
	XBool connectTexture(const char *name,unsigned int * tex,XShaderTexType type = TEX_TYPE_2D);//链接贴图
	void useShader(bool withTex0 = false);	//使用shader,参数withTex0适用于设置是否需要绑定第一张贴图，
											//如果需要绑定第一张贴图则设置为true,因为存在公用第一张贴图的情况所以有这个设置
	void useShaderEx(unsigned int tex0,XShaderTexType type = TEX_TYPE_2D);	//调用的时候才传入第一张贴图
	//++++++++++++++++++++++++++++++++++++++++++++++
	//geometryShader相关的接口
	void setGeometryInfo(unsigned int inType,unsigned int outType,int outSum);
	int getGeometryMaxOutputCount() const;
	//----------------------------------------------
	void disShader();
	unsigned int getShaderHandle()const{ return m_shaderHandle.shaderHandle;}
	XShaderGLSL()
		:m_isInited(XFalse)
	{}
	~XShaderGLSL() {release();}
	//下面是为了提升性能而增加的接口
	XBool connectUBO(const char *uboName,XUBO *ubo);
	//从shader中读取UBO的相关信息
	XBool getUBOInfo(XShaderUBOData &uboData,int valueSum,const char *uboName,const char **valueNames);	//连接UBO
};
class XShaderUBOData
{
public:
	unsigned int m_handle;
	char m_name[256];
	int m_offset[XShaderGLSL::m_maxShaderGLSLDataSum];
	unsigned int m_index[XShaderGLSL::m_maxShaderGLSLDataSum];
	int m_size;
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
enum XPboMode
{
	PBO_MODE_UNPACK,
	PBO_MODE_PACK,
};
enum XColorMode;
//说明：在使用PBO来更新贴图的时候只有当需要将RGB转换成BGR的时候回来带有价值的性能提升外
//其他形式完全没有必要
class XPBO	
{
private:
	XBool m_isInited;
	unsigned int m_pboID[2];	//使用奇偶轮换的方式
	int m_curPBOIndex;
	int m_size;
	XColorMode m_colorType;
	int m_w;	//屏幕宽度
	int m_h;	//屏幕高度
	int m_px;	//截取图像的左上角点
	int m_py;
	int m_wx;	//截取图像的宽度
	int m_wy;
	int m_dataSize;
	XPboMode m_mode;
public:
	XPBO()
		:m_isInited(XFalse)
		,m_curPBOIndex(0)
		,m_mode(PBO_MODE_PACK)
	{}
	~XPBO(){release();}
	void release();
	XBool init(int w,int h,int px,int py,int wx,int wy,XColorMode colorType,XPboMode mode = PBO_MODE_PACK);
	XBool getPixel(unsigned char * buff,int target = GL_FRONT);
	XBool setPixel(unsigned char * buff);
	void bind() const;
	void unbind() const;
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
//下面使用VBO的方式提升性能（如果vbo显示不出来，请切换到release模式下试一试）
//VBO使用技巧
//1、太频繁的更新VBO将大大的影响性能，比如说在粒子系统中，每个粒子就是一个VBO的话，
//频繁的更新VBO将会造成性能大大折扣，此时应该进行合并操作，将小的vbo合并成大的VBO(合并是否带来性能提升，尚未经过测试)
//2、当数据量比较小的时候，BufferSubData是远远超过MapBuffer的效率的，但是当数据足够大的时候，
//MapBuffer会压倒性的超越BufferSubData，据测试这个值大概是32k，具体还是根据驱动来决定。(尚未经过验证)
class XVBO
{
private:
	XBool m_withVAO;	//使用使用VAO(使用VAO没有明显性能提升，不过代码可以变得更简洁)
	XBool m_isInited;
	XBool m_withV;	//顶点
	XBool m_withT;	//贴图
	XBool m_withN;	//法线
	XBool m_withC;	//颜色
	XBool m_withI;	//索引
	unsigned int m_vao;
	unsigned int m_v;
	unsigned int m_t;
	unsigned int m_n;
	unsigned int m_c;
	unsigned int m_i;
	int m_size;			//VBO中数据的数量，一般是顶点的数量
	int m_indexSize;	//索引的数量，一般来说就是模型渲染中依次填入的顶点的数量
	int m_vSize;	//描述一个顶点的数据数量，3D顶点为3，2D顶点为2
	XBool initX(int size,const float *v,const float *t = NULL,const float *n = NULL,const float *c = NULL,
		int iSize = 0,const unsigned int *i = NULL,bool withVAO = false);
	XBool updateDateX(int size,const float *v,const float *t = NULL,const float *n = NULL,const float *c = NULL,
		int iSize = 0,const unsigned int *i = NULL);
public:
	//size为顶点的数量，实际的v的尺寸为size * 3，因为3D空间中一个顶点由3个浮点数确定
	//t的尺寸为size * 2,n的尺寸为size * 3,c的尺寸为size * 4；
	//iSize 为描绘面的数量，(由于这里默认为三角形)，所以i的尺寸为iSize * 3	//这项约定由问题，需要改正
	XBool init(int size,const float *v,const float *t = NULL,const float *n = NULL,const float *c = NULL,
		int iSize = 0,const unsigned int *i = NULL,bool withVAO = false)
	{
		m_vSize = 3;
		return initX(size,v,t,n,c,iSize,i,withVAO);
	}
	XBool updateDate(int size,const float *v,const float *t = NULL,const float *n = NULL,const float *c = NULL,
		int iSize = 0,const unsigned int *i = NULL)
	{
		if(!m_isInited) return init(size,v,t,n,c,iSize,i);//如果尚未进行初始化，则这里直接调用初始化函数
		return updateDateX(size,v,t,n,c,iSize,i);
	}
	XBool init2D(int size,const float *v,const float *t = NULL,const float *n = NULL,const float *c = NULL,
		int iSize = 0,const unsigned int *i = NULL,bool withVAO = false)
	{
		m_vSize = 2;
		return initX(size,v,t,n,c,iSize,i,withVAO);
	}
	XBool updateDate2D(int size,const float *v,const float *t = NULL,const float *n = NULL,const float *c = NULL,
		int iSize = 0,const unsigned int *i = NULL)
	{
		if(!m_isInited) return init2D(size,v,t,n,c,iSize,i);//如果尚未进行初始化，则这里直接调用初始化函数
		return updateDateX(size,v,t,n,c,iSize,i);
	}
	XBool updateDataV(int size,const float *v);	//size为顶点的数量，默认一个顶点由3个浮点数描述
	XBool updateDataT(int size,const float *t);	//size为顶点的数量，默认一个顶点的t由2个浮点数描述
	XBool updateDataN(int size,const float *n);	//size为顶点的数量，默认一个顶点的n由3个浮点数描述
	XBool updateDataC(int size,const float *c);	//size为顶点的数量，默认一个顶点的c由4个浮点数描述
	XBool updateDataI(int size,const unsigned int *i);	//size为索引的数量，
	void use(XBool withTex = XTrue);	//参数用于描述是否显示贴图
	void disuse();	//取消索引
	void drawByIndex(unsigned int type,XBool withTex = XTrue);
	void drawByIndex(unsigned int type,int size,unsigned int indexType,void *pIndex,XBool withTex = XTrue);
	void drawByArray(unsigned int type,XBool withTex = XTrue);
	void drawByArray(unsigned int type,int index,int size,XBool withTex);
	void release();
	int getSize()const{return m_size;}
	int getIndexSize()const{return m_indexSize;}
	//当一个VBO中的数据与另一个VBO数据相同时，不需要使用两份数据，而是设置一个VBO从属于另一个VBO，数据直接从另一个VBO中获取即可
	//temp:从属的对象
	//v,n,c,t:是否有这些属性
	//cv,cn,cc,ct:属性是否从从属对象中获取
	XBool setAsSubject(const XVBO &temp,XBool v,XBool n,XBool c,XBool t,XBool i,XBool cv,XBool cn,XBool cc,XBool ct,XBool ci);
	XBool m_isSubject;
	XBool m_subjectV;
	XBool m_subjectN;
	XBool m_subjectC;
	XBool m_subjectT;
	XBool m_subjectI;

	XVBO()
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
		,m_vSize(3)
		,m_withVAO(false)
	{}
	~XVBO(){release();}
};
//VBO的使用方法如下
//tmpVBO.use();	//启用VBO
//第一个参数是描绘的类型和OpenGL支持的类型一致
//第二个参数为模型中描绘的所以值的数量
//第三个参数为索引值的类型
//第四个参数为索引值的指针
//如果VBO中已经包含索引数据，则这里可以为NULL
//目前的VBO封装封装中默认支持的描绘方式为三角形，所以对于非三角形会有问题
//glDrawElements(GL_QUADS,150 * 150 * 4,GL_UNSIGNED_INT,index);	//描绘动作
//tmpVBO.disuse();	//停止VBO的使用

namespace XGL
{
	extern XBool getIsFramebufferReady();	//判断FBO状态是否就绪
}
//下面是对FBO的支持
class XFBOEx;
class XFBO
{
private:
	unsigned int m_fboId;					//FBO的ID
	unsigned int m_rboId;					//RBO的ID
	std::vector<XTexture *> m_pTextures;
	XColorMode m_type;
	int m_zeroTexIndex;	//绑定在0号位置的贴图编号
public:
	XBool init(int w,int h,XColorMode type = COLOR_RGBA); //w和h是纹理的尺寸，返回初始化是否成功
	void useFBO(bool newSize = false,int w = 0,int h = 0);	//参数为是否使用新的视口矩阵
	void bind();	//针对3D的情况额外封装一下
	void attachTex(unsigned int order = 0);	//需要在useFBO之后调用
	XBool attachTexs(int sum,int index,...);	//需要在useFBO之后调用
	void removeFBO();
	void unbind();
	void addOneTexture(int w,int h,XColorMode type = COLOR_RGBA);
	void updateTexture(void *p,unsigned int index);
	unsigned int getTexture(unsigned int order) const;	//存在问题
	int getWidth(unsigned int order) const;
	int getHeight(unsigned int order) const;
	void release();
	XFBO()
		:m_zeroTexIndex(-1)
		, m_fboId(0)
		, m_rboId(0)
		, m_type(COLOR_RGBA)
	{}
	~XFBO() {release();}
	//下面是用于测试的接口
	unsigned int getFboID() const {return m_fboId;}
	void getPixelFromFboEx(const XFBOEx& ex);
	int getW() const;
	int getH() const;
};
//这个是支持多重采样的FBO的实现
//这是fbo比较正规的用法，但是由于之前的FBO封装已经用的比较顺手，所以这里做额外的封装
class XFBOEx
{
private:
	unsigned int m_fboId;
	unsigned int m_rboId;
	unsigned int m_rbdId;
	int m_w;
	int m_h;

	bool m_isInited;
public:
	bool init(int w,int h,XColorMode type = COLOR_RGBA);
	void useFBO(bool newSize = false,int w = 0,int h = 0);	//参数为是否使用新的视口矩阵
	void removeFBO();
	XFBOEx()
		:m_isInited(false)
		, m_fboId(0)
		, m_rboId(0)
		, m_rbdId(0)
		, m_w(0)
		, m_h(0)
	{}
	~XFBOEx(){release();}
	void release();
	unsigned int getFboID() const {return m_fboId;}
	int getW() const{return m_w;}
	int getH() const{return m_h;}
};
//为了解决shader与FBO的结合而定义的函数
namespace XRender
{
extern void drawBlankPlane(int w,int h,unsigned int tex,XShaderGLSL *pShader = NULL);
}
#if WITH_INLINE_FILE
#include "XShaderGLSL.inl"
#endif
}
#endif //_JIA_XSHADER_GLSL_
