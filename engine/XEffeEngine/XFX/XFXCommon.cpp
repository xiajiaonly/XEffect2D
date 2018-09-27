#include "XStdHead.h"
#include "XFXCommon.h"
namespace XE
{
namespace XFX
{
std::string gCommonVrt = "varying vec2 TexCoord;\n\
void main(void)\n\
{\n\
	TexCoord = gl_MultiTexCoord0.st;\n\
	gl_Position = ftransform();\n\
}";
//std::string commonVrt = STRINGIFY(varying vec2 TexCoord;
//void main(void)
//{
//	TexCoord = gl_MultiTexCoord0.st;
//	gl_Position = ftransform();
//});
}
}