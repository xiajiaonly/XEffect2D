#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	¼ÖÊ¤»ª(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XFColor.h"
#include "XCColor.h"
namespace XE{
XFColor::XFColor(const XCColor &temp)
{
	fR = temp.r / 255.0f;
	fG = temp.g / 255.0f;
	fB = temp.b / 255.0f;
	fA = temp.a / 255.0f;
}
XFColor& XFColor::operator = (const XCColor& temp)
{
	fR = temp.r / 255.0f;
	fG = temp.g / 255.0f;
	fB = temp.b / 255.0f;
	fA = temp.a / 255.0f;
	return *this;
}
void XFColor::setColor(float r,float g,float b,float a)
{
    if(r > 1.0f) r = 1.0f;
    if(g > 1.0f) g = 1.0f;
    if(b > 1.0f) b = 1.0f;
    if(a > 1.0f) a = 1.0f;
    if(r < 0.0f) r = 0.0f;
    if(g < 0.0f) g = 0.0f;
    if(b < 0.0f) b = 0.0f;
    if(a < 0.0f) a = 0.0f;
    fR = r;
    fG = g;
    fB = b;
    fA = a;
}
namespace XColor{
XFColor colorMix(const XFColor &color1,const XFColor &color2,float rate1,float rate2)
{
	float w1 = max(color1.fR,max(color1.fG,color1.fB));
	float w2 = max(color2.fR,max(color2.fG,color2.fB));
	XFColor ret;
	ret.fR = color1.fR * rate1 + color2.fR * rate2;
	ret.fG = color1.fG * rate1 + color2.fG * rate2;
	ret.fB = color1.fB * rate1 + color2.fB * rate2;
	float w3 = w1 * rate1 + w2 * rate2;
	//
	float max = max(ret.fR,max(ret.fG,ret.fB));
	if(max > 0.0f)
	{
		max = w3 / max;
		ret.fR = ret.fR * max;
		ret.fG = ret.fG * max;
		ret.fB = ret.fB * max;
	}
	return ret;
}
XFColor colorMix(const XFColor &color1,const XFColor &color2,const XFColor &color3,const XFColor &color4,
						 float rate1,float rate2,float rate3,float rate4)
{
	float w1 = max(color1.fR,max(color1.fG,color1.fB));
	float w2 = max(color2.fR,max(color2.fG,color2.fB));
	float w3 = max(color3.fR,max(color3.fG,color3.fB));
	float w4 = max(color4.fR,max(color4.fG,color4.fB));
	XFColor ret;
	ret.fR = color1.fR * rate1 + color2.fR * rate2 + color3.fR * rate3 + color4.fR * rate4;
	ret.fG = color1.fG * rate1 + color2.fG * rate2 + color3.fG * rate3 + color4.fG * rate4;
	ret.fB = color1.fB * rate1 + color2.fB * rate2 + color3.fB * rate3 + color4.fB * rate4;
	float w5 = w1 * rate1 + w2 * rate2 + w3 * rate3 + w4 * rate4;
	//
	float max = max(ret.fR,max(ret.fG,ret.fB));
	if(max > 0.0f)
	{
		max = w5 / max;
		ret.fR = ret.fR * max;
		ret.fG = ret.fG * max;
		ret.fB = ret.fB * max;
	}
	return ret;
}
XFColor colorMixEx(const XFColor &color1,const XFColor &color2,const XFColor &color3,const XFColor &color4,
						 float rate1,float rate2,float rate3,float rate4)
{
	XFColor ret;
	ret.fR = color1.fR * rate1 + color2.fR * rate2 + color3.fR * rate3 + color4.fR * rate4;
	ret.fG = color1.fG * rate1 + color2.fG * rate2 + color3.fG * rate3 + color4.fG * rate4;
	ret.fB = color1.fB * rate1 + color2.fB * rate2 + color3.fB * rate3 + color4.fB * rate4;
	ret.fW = color1.fW * rate1 + color2.fW * rate2 + color3.fW * rate3 + color4.fW * rate4;
	float max = max(ret.fR,max(ret.fG,ret.fB));
	if(max > 0.0f)
	{
		max = ret.fW / max;
		ret.fR = ret.fR * max;
		ret.fG = ret.fG * max;
		ret.fB = ret.fB * max;
	}
	return ret;
}
XFColor colorMix(const XFColor *color,const float *rate,int mixSum)
{
	XFColor ret(0,0,0,0);
	float w;	//Ã÷¶È
	for(int i = 0;i < mixSum;++ i)
	{
		ret.fR += color[i].fR * rate[i];
		ret.fG += color[i].fG * rate[i];
		ret.fB += color[i].fB * rate[i];
		w = max(color[i].fR,max(color[i].fG,color[i].fB));
		ret.fW += w * rate[i];
	}
	float max = max(ret.fR,max(ret.fG,ret.fB));
	if(max > 0.0f)
	{
		max = ret.fW / max;
		ret.fR = ret.fR * max;
		ret.fG = ret.fG * max;
		ret.fB = ret.fB * max;
	}
	return ret;
}
XFColor colorMixEx(const XFColor *color,const float *rate,int mixSum)
{
	XFColor ret(0,0,0,0);
	for(int i = 0;i < mixSum;++ i)
	{
		ret.fR += color[i].fR * rate[i];
		ret.fG += color[i].fG * rate[i];
		ret.fB += color[i].fB * rate[i];
		ret.fW += color[i].fW * rate[i];
	}
	float max = max(ret.fR,max(ret.fG,ret.fB));
	if(max > 0.0f)
	{
		max = ret.fW / max;
		ret.fR = ret.fR * max;
		ret.fG = ret.fG * max;
		ret.fB = ret.fB * max;
	}
	return ret;
}
}
const XFColor XFColor::gray(1.0f / 2, 1.0f / 2, 1.0f / 2);
const XFColor XFColor::white(1.0f, 1.0f, 1.0f);
const XFColor XFColor::red(1.0f, 0, 0);
const XFColor XFColor::green(0, 1.0f, 0);
const XFColor XFColor::blue(0, 0, 1.0f);
const XFColor XFColor::cyan(0, 1.0f, 1.0f);
const XFColor XFColor::magenta(1.0f, 0, 1.0f);
const XFColor XFColor::yellow(1.0f, 1.0f, 0);
const XFColor XFColor::black(0, 0, 0);
const XFColor XFColor::aliceBlue(0.941176f * 1.0f,0.972549f * 1.0f,1.0f * 1.0f);
const XFColor XFColor::antiqueWhite(0.980392f * 1.0f,0.921569f * 1.0f,0.843137f * 1.0f);
const XFColor XFColor::aqua(0.0f * 1.0f,1.0f * 1.0f,1.0f * 1.0f);
const XFColor XFColor::aquamarine(0.498039f * 1.0f,1.0f * 1.0f,0.831373f * 1.0f);
const XFColor XFColor::azure(0.941176f * 1.0f,1.0f * 1.0f,1.0f * 1.0f);
const XFColor XFColor::beige(0.960784f * 1.0f,0.960784f * 1.0f,0.862745f * 1.0f);
const XFColor XFColor::bisque(1.0f * 1.0f,0.894118f * 1.0f,0.768627f * 1.0f);
const XFColor XFColor::blanchedAlmond(1.0f * 1.0f,0.921569f * 1.0f,0.803922f * 1.0f);
const XFColor XFColor::blueViolet(0.541176f * 1.0f,0.168627f * 1.0f,0.886275f * 1.0f);
const XFColor XFColor::brown(0.647059f * 1.0f,0.164706f * 1.0f,0.164706f * 1.0f);
const XFColor XFColor::burlyWood(0.870588f * 1.0f,0.721569f * 1.0f,0.529412f * 1.0f);
const XFColor XFColor::cadetBlue(0.372549f * 1.0f,0.619608f * 1.0f,0.627451f * 1.0f);
const XFColor XFColor::chartreuse(0.498039f * 1.0f,1.0f * 1.0f,0.0f * 1.0f);
const XFColor XFColor::chocolate(0.823529f * 1.0f,0.411765f * 1.0f,0.117647f * 1.0f);
const XFColor XFColor::coral(1.0f * 1.0f,0.498039f * 1.0f,0.313726f * 1.0f);
const XFColor XFColor::cornflowerBlue(0.392157f * 1.0f,0.584314f * 1.0f,0.929412f * 1.0f);
const XFColor XFColor::cornsilk(1.0f * 1.0f,0.972549f * 1.0f,0.862745f * 1.0f);
const XFColor XFColor::crimson(0.862745f * 1.0f,0.0784314f * 1.0f,0.235294f * 1.0f);
const XFColor XFColor::darkBlue(0.0f * 1.0f,0.0f * 1.0f,0.545098f * 1.0f);
const XFColor XFColor::darkCyan(0.0f * 1.0f,0.545098f * 1.0f,0.545098f * 1.0f);
const XFColor XFColor::darkGoldenRod(0.721569f * 1.0f,0.52549f * 1.0f,0.0431373f * 1.0f);
const XFColor XFColor::darkGray(0.662745f * 1.0f,0.662745f * 1.0f,0.662745f * 1.0f);
const XFColor XFColor::darkGrey(0.662745f * 1.0f,0.662745f * 1.0f,0.662745f * 1.0f);
const XFColor XFColor::darkGreen(0.0f * 1.0f,0.392157f * 1.0f,0.0f * 1.0f);
const XFColor XFColor::darkKhaki(0.741176f * 1.0f,0.717647f * 1.0f,0.419608f * 1.0f);
const XFColor XFColor::darkMagenta(0.545098f * 1.0f,0.0f * 1.0f,0.545098f * 1.0f);
const XFColor XFColor::darkOliveGreen(0.333333f * 1.0f,0.419608f * 1.0f,0.184314f * 1.0f);
const XFColor XFColor::darkorange(1.0f * 1.0f,0.54902f * 1.0f,0.0f * 1.0f);
const XFColor XFColor::darkOrchid(0.6f * 1.0f,0.196078f * 1.0f,0.8f * 1.0f);
const XFColor XFColor::darkRed(0.545098f * 1.0f,0.0f * 1.0f,0.0f * 1.0f);
const XFColor XFColor::darkSalmon(0.913725f * 1.0f,0.588235f * 1.0f,0.478431f * 1.0f);
const XFColor XFColor::darkSeaGreen(0.560784f * 1.0f,0.737255f * 1.0f,0.560784f * 1.0f);
const XFColor XFColor::darkSlateBlue(0.282353f * 1.0f,0.239216f * 1.0f,0.545098f * 1.0f);
const XFColor XFColor::darkSlateGray(0.184314f * 1.0f,0.309804f * 1.0f,0.309804f * 1.0f);
const XFColor XFColor::darkSlateGrey(0.184314f * 1.0f,0.309804f * 1.0f,0.309804f * 1.0f);
const XFColor XFColor::darkTurquoise(0.0f * 1.0f,0.807843f * 1.0f,0.819608f * 1.0f);
const XFColor XFColor::darkViolet(0.580392f * 1.0f,0.0f * 1.0f,0.827451f * 1.0f);
const XFColor XFColor::deepPink(1.0f * 1.0f,0.0784314f * 1.0f,0.576471f * 1.0f);
const XFColor XFColor::deepSkyBlue(0.0f * 1.0f,0.74902f * 1.0f,1.0f * 1.0f);
const XFColor XFColor::dimGray(0.411765f * 1.0f,0.411765f * 1.0f,0.411765f * 1.0f);
const XFColor XFColor::dimGrey(0.411765f * 1.0f,0.411765f * 1.0f,0.411765f * 1.0f);
const XFColor XFColor::dodgerBlue(0.117647f * 1.0f,0.564706f * 1.0f,1.0f * 1.0f);
const XFColor XFColor::fireBrick(0.698039f * 1.0f,0.133333f * 1.0f,0.133333f * 1.0f);
const XFColor XFColor::floralWhite(1.0f * 1.0f,0.980392f * 1.0f,0.941176f * 1.0f);
const XFColor XFColor::forestGreen(0.133333f * 1.0f,0.545098f * 1.0f,0.133333f * 1.0f);
const XFColor XFColor::fuchsia(1.0f * 1.0f,0.0f * 1.0f,1.0f * 1.0f);
const XFColor XFColor::gainsboro(0.862745f * 1.0f,0.862745f * 1.0f,0.862745f * 1.0f);
const XFColor XFColor::ghostWhite(0.972549f * 1.0f,0.972549f * 1.0f,1.0f * 1.0f);
const XFColor XFColor::gold(1.0f * 1.0f,0.843137f * 1.0f,0.0f * 1.0f);
const XFColor XFColor::goldenRod(0.854902f * 1.0f,0.647059f * 1.0f,0.12549f * 1.0f);
const XFColor XFColor::grey(0.501961f * 1.0f,0.501961f * 1.0f,0.501961f * 1.0f);
const XFColor XFColor::greenYellow(0.678431f * 1.0f,1.0f * 1.0f,0.184314f * 1.0f);
const XFColor XFColor::honeyDew(0.941176f * 1.0f,1.0f * 1.0f,0.941176f * 1.0f);
const XFColor XFColor::hotPink(1.0f * 1.0f,0.411765f * 1.0f,0.705882f * 1.0f);
const XFColor XFColor::indianRed (0.803922f * 1.0f,0.360784f * 1.0f,0.360784f * 1.0f);
const XFColor XFColor::indigo (0.294118f * 1.0f,0.0f * 1.0f,0.509804f * 1.0f);
const XFColor XFColor::ivory(1.0f * 1.0f,1.0f * 1.0f,0.941176f * 1.0f);
const XFColor XFColor::khaki(0.941176f * 1.0f,0.901961f * 1.0f,0.54902f * 1.0f);
const XFColor XFColor::lavender(0.901961f * 1.0f,0.901961f * 1.0f,0.980392f * 1.0f);
const XFColor XFColor::lavenderBlush(1.0f * 1.0f,0.941176f * 1.0f,0.960784f * 1.0f);
const XFColor XFColor::lawnGreen(0.486275f * 1.0f,0.988235f * 1.0f,0.0f * 1.0f);
const XFColor XFColor::lemonChiffon(1.0f * 1.0f,0.980392f * 1.0f,0.803922f * 1.0f);
const XFColor XFColor::lightBlue(0.678431f * 1.0f,0.847059f * 1.0f,0.901961f * 1.0f);
const XFColor XFColor::lightCoral(0.941176f * 1.0f,0.501961f * 1.0f,0.501961f * 1.0f);
const XFColor XFColor::lightCyan(0.878431f * 1.0f,1.0f * 1.0f,1.0f * 1.0f);
const XFColor XFColor::lightGoldenRodYellow(0.980392f * 1.0f,0.980392f * 1.0f,0.823529f * 1.0f);
const XFColor XFColor::lightGray(0.827451f * 1.0f,0.827451f * 1.0f,0.827451f * 1.0f);
const XFColor XFColor::lightGreen(0.564706f * 1.0f,0.933333f * 1.0f,0.564706f * 1.0f);
const XFColor XFColor::lightPink(1.0f * 1.0f,0.713726f * 1.0f,0.756863f * 1.0f);
const XFColor XFColor::lightSalmon(1.0f * 1.0f,0.627451f * 1.0f,0.478431f * 1.0f);
const XFColor XFColor::lightSeaGreen(0.12549f * 1.0f,0.698039f * 1.0f,0.666667f * 1.0f);
const XFColor XFColor::lightSkyBlue(0.529412f * 1.0f,0.807843f * 1.0f,0.980392f * 1.0f);
const XFColor XFColor::lightSlateGray(0.466667f * 1.0f,0.533333f * 1.0f,0.6f * 1.0f);
const XFColor XFColor::lightSlateGrey(0.466667f * 1.0f,0.533333f * 1.0f,0.6f * 1.0f);
const XFColor XFColor::lightSteelBlue(0.690196f * 1.0f,0.768627f * 1.0f,0.870588f * 1.0f);
const XFColor XFColor::lightYellow(1.0f * 1.0f,1.0f * 1.0f,0.878431f * 1.0f);
const XFColor XFColor::lime(0.0f * 1.0f,1.0f * 1.0f,0.0f * 1.0f);
const XFColor XFColor::limeGreen(0.196078f * 1.0f,0.803922f * 1.0f,0.196078f * 1.0f);
const XFColor XFColor::linen(0.980392f * 1.0f,0.941176f * 1.0f,0.901961f * 1.0f);
const XFColor XFColor::maroon(0.501961f * 1.0f,0.0f * 1.0f,0.0f * 1.0f);
const XFColor XFColor::mediumAquaMarine(0.4f * 1.0f,0.803922f * 1.0f,0.666667f * 1.0f);
const XFColor XFColor::mediumBlue(0.0f * 1.0f,0.0f * 1.0f,0.803922f * 1.0f);
const XFColor XFColor::mediumOrchid(0.729412f * 1.0f,0.333333f * 1.0f,0.827451f * 1.0f);
const XFColor XFColor::mediumPurple(0.576471f * 1.0f,0.439216f * 1.0f,0.858824f * 1.0f);
const XFColor XFColor::mediumSeaGreen(0.235294f * 1.0f,0.701961f * 1.0f,0.443137f * 1.0f);
const XFColor XFColor::mediumSlateBlue(0.482353f * 1.0f,0.407843f * 1.0f,0.933333f * 1.0f);
const XFColor XFColor::mediumSpringGreen(0.0f * 1.0f,0.980392f * 1.0f,0.603922f * 1.0f);
const XFColor XFColor::mediumTurquoise(0.282353f * 1.0f,0.819608f * 1.0f,0.8f * 1.0f);
const XFColor XFColor::mediumVioletRed(0.780392f * 1.0f,0.0823529f * 1.0f,0.521569f * 1.0f);
const XFColor XFColor::midnightBlue(0.0980392f * 1.0f,0.0980392f * 1.0f,0.439216f * 1.0f);
const XFColor XFColor::mintCream(0.960784f * 1.0f,1.0f * 1.0f,0.980392f * 1.0f);
const XFColor XFColor::mistyRose(1.0f * 1.0f,0.894118f * 1.0f,0.882353f * 1.0f);
const XFColor XFColor::moccasin(1.0f * 1.0f,0.894118f * 1.0f,0.709804f * 1.0f);
const XFColor XFColor::navajoWhite(1.0f * 1.0f,0.870588f * 1.0f,0.678431f * 1.0f);
const XFColor XFColor::navy(0.0f * 1.0f,0.0f * 1.0f,0.501961f * 1.0f);
const XFColor XFColor::oldLace(0.992157f * 1.0f,0.960784f * 1.0f,0.901961f * 1.0f);
const XFColor XFColor::olive(0.501961f * 1.0f,0.501961f * 1.0f,0.0f * 1.0f);
const XFColor XFColor::oliveDrab(0.419608f * 1.0f,0.556863f * 1.0f,0.137255f * 1.0f);
const XFColor XFColor::orange(1.0f * 1.0f,0.647059f * 1.0f,0.0f * 1.0f);
const XFColor XFColor::orangeRed(1.0f * 1.0f,0.270588f * 1.0f,0.0f * 1.0f);
const XFColor XFColor::orchid(0.854902f * 1.0f,0.439216f * 1.0f,0.839216f * 1.0f);
const XFColor XFColor::paleGoldenRod(0.933333f * 1.0f,0.909804f * 1.0f,0.666667f * 1.0f);
const XFColor XFColor::paleGreen(0.596078f * 1.0f,0.984314f * 1.0f,0.596078f * 1.0f);
const XFColor XFColor::paleTurquoise(0.686275f * 1.0f,0.933333f * 1.0f,0.933333f * 1.0f);
const XFColor XFColor::paleVioletRed(0.858824f * 1.0f,0.439216f * 1.0f,0.576471f * 1.0f);
const XFColor XFColor::papayaWhip(1.0f * 1.0f,0.937255f * 1.0f,0.835294f * 1.0f);
const XFColor XFColor::peachPuff(1.0f * 1.0f,0.854902f * 1.0f,0.72549f * 1.0f);
const XFColor XFColor::peru(0.803922f * 1.0f,0.521569f * 1.0f,0.247059f * 1.0f);
const XFColor XFColor::pink(1.0f * 1.0f,0.752941f * 1.0f,0.796078f * 1.0f);
const XFColor XFColor::plum(0.866667f * 1.0f,0.627451f * 1.0f,0.866667f * 1.0f);
const XFColor XFColor::powderBlue(0.690196f * 1.0f,0.878431f * 1.0f,0.901961f * 1.0f);
const XFColor XFColor::purple(0.501961f * 1.0f,0.0f * 1.0f,0.501961f * 1.0f);
const XFColor XFColor::rosyBrown(0.737255f * 1.0f,0.560784f * 1.0f,0.560784f * 1.0f);
const XFColor XFColor::royalBlue(0.254902f * 1.0f,0.411765f * 1.0f,0.882353f * 1.0f);
const XFColor XFColor::saddleBrown(0.545098f * 1.0f,0.270588f * 1.0f,0.0745098f * 1.0f);
const XFColor XFColor::salmon(0.980392f * 1.0f,0.501961f * 1.0f,0.447059f * 1.0f);
const XFColor XFColor::sandyBrown(0.956863f * 1.0f,0.643137f * 1.0f,0.376471f * 1.0f);
const XFColor XFColor::seaGreen(0.180392f * 1.0f,0.545098f * 1.0f,0.341176f * 1.0f);
const XFColor XFColor::seaShell(1.0f * 1.0f,0.960784f * 1.0f,0.933333f * 1.0f);
const XFColor XFColor::sienna(0.627451f * 1.0f,0.321569f * 1.0f,0.176471f * 1.0f);
const XFColor XFColor::silver(0.752941f * 1.0f,0.752941f * 1.0f,0.752941f * 1.0f);
const XFColor XFColor::skyBlue(0.529412f * 1.0f,0.807843f * 1.0f,0.921569f * 1.0f);
const XFColor XFColor::slateBlue(0.415686f * 1.0f,0.352941f * 1.0f,0.803922f * 1.0f);
const XFColor XFColor::slateGray(0.439216f * 1.0f,0.501961f * 1.0f,0.564706f * 1.0f);
const XFColor XFColor::slateGrey(0.439216f * 1.0f,0.501961f * 1.0f,0.564706f * 1.0f);
const XFColor XFColor::snow(1.0f * 1.0f,0.980392f * 1.0f,0.980392f * 1.0f);
const XFColor XFColor::springGreen(0.0f * 1.0f,1.0f * 1.0f,0.498039f * 1.0f);
const XFColor XFColor::steelBlue(0.27451f * 1.0f,0.509804f * 1.0f,0.705882f * 1.0f);
const XFColor XFColor::tan(0.823529f * 1.0f,0.705882f * 1.0f,0.54902f * 1.0f);
const XFColor XFColor::teal(0.0f * 1.0f,0.501961f * 1.0f,0.501961f * 1.0f);
const XFColor XFColor::thistle(0.847059f * 1.0f,0.74902f * 1.0f,0.847059f * 1.0f);
const XFColor XFColor::tomato(1.0f * 1.0f,0.388235f * 1.0f,0.278431f * 1.0f);
const XFColor XFColor::turquoise(0.25098f * 1.0f,0.878431f * 1.0f,0.815686f * 1.0f);
const XFColor XFColor::violet(0.933333f * 1.0f,0.509804f * 1.0f,0.933333f * 1.0f);
const XFColor XFColor::wheat(0.960784f * 1.0f,0.870588f * 1.0f,0.70196f * 1.0f);
const XFColor XFColor::whiteSmoke(0.960784f * 1.0f,0.960784f * 1.0f,0.960784f * 1.0f);
const XFColor XFColor::yellowGreen(0.603922f * 1.0f,0.803922f * 1.0f,0.196078f * 1.0f);
#if !WITH_INLINE_FILE
#include "XFColor.inl"
#endif
}