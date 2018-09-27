#include "XStdHead.h"
#include "XBlur.h"
namespace XE
{
namespace XFX
{
std::string gBlurFrg = "#version 130 \n\
#extension GL_ARB_gpu_shader_fp64 : enable\n\
uniform sampler2D Texture_00;	\n\
varying vec2 TexCoord;\n\
uniform int power;\n\
uniform int isH;	//是否为水平方向\n\
void main()\n\
{\n\
	vec2 size = textureSize(Texture_00,0);\n\
	if(power < 2/* || TexCoord.y < 0.5*/)\n\
	{\n\
		gl_FragColor = texture2D(Texture_00,TexCoord).rgba;\n\
		return;\n\
	}\n\
	vec4 color = vec4(0.0);\n\
	float SQRT2PI = sqrt(2.0 * 3.1415926);\n\
	float sigma2 = 2.0 * power * power / 9.0;\n\
	float sigmap = power * SQRT2PI / 3.0;\n\
	color += texture2D(Texture_00,TexCoord).rgba / sigmap;\n\
	if(isH == 1)\n\
	{\n\
		for(int i = 1;i <= power;++ i)\n\
		{\n\
			float kernel = exp(-(i * i) / sigma2) / sigmap;\n\
			color += texture2D(Texture_00,vec2(TexCoord.x + i / size.x,TexCoord.y)).rgba * kernel;\n\
			color += texture2D(Texture_00,vec2(TexCoord.x - i / size.x,TexCoord.y)).rgba * kernel;\n\
		}\n\
	}else\n\
	{\n\
		for(int i = 1;i <= power;++ i)\n\
		{\n\
			float kernel = exp(-(i * i) / sigma2) / sigmap;\n\
			color += texture2D(Texture_00,vec2(TexCoord.x,TexCoord.y + i / size.y)).rgba * kernel;\n\
			color += texture2D(Texture_00,vec2(TexCoord.x,TexCoord.y - i / size.y)).rgba * kernel;\n\
		}\n\
	}\n\
	gl_FragColor = color;\n\
}";
}
}