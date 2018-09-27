#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	¼ÖÊ¤»ª(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XFColor.h"
#include "XCColor.h"
#include "XColorSpace.h"
namespace XE {
XFColor::XFColor(const XCColor &temp)
{
	r = temp.r / 255.0f;
	g = temp.g / 255.0f;
	b = temp.b / 255.0f;
	a = temp.a / 255.0f;
}
XFColor& XFColor::operator = (const XCColor& temp)
{
	r = temp.r / 255.0f;
	g = temp.g / 255.0f;
	b = temp.b / 255.0f;
	a = temp.a / 255.0f;
	return *this;
}
void XFColor::set(float _r, float _g, float _b, float _a)
{
	r = XMath::clamp(_r, 0.0f, 1.0f);
	g = XMath::clamp(_g, 0.0f, 1.0f);
	b = XMath::clamp(_b, 0.0f, 1.0f);
	a = XMath::clamp(_a, 0.0f, 1.0f);
}
void XFColor::set(float _l, float _a)
{
	r = g = b = XMath::clamp(_l, 0.0f, 1.0f);
	a = XMath::clamp(_a, 0.0f, 1.0f);
}
namespace XColor {
	XFColor colorMix(const XFColor& color1, const XFColor& color2, float rate1, float rate2)
	{
		float w1 = (std::max)(color1.r, (std::max)(color1.g, color1.b));
		float w2 = (std::max)(color2.r, (std::max)(color2.g, color2.b));
		XFColor ret;
		ret.r = color1.r * rate1 + color2.r * rate2;
		ret.g = color1.g * rate1 + color2.g * rate2;
		ret.b = color1.b * rate1 + color2.b * rate2;
		float w3 = w1 * rate1 + w2 * rate2;
		//
		float max = (std::max)(ret.r, (std::max)(ret.g, ret.b));
		if (max > 0.0f)
		{
			max = w3 / max;
			ret.r = ret.r * max;
			ret.g = ret.g * max;
			ret.b = ret.b * max;
		}
		return ret;
	}
	XFColor colorMix(const XFColor& color1, const XFColor& color2, const XFColor& color3, const XFColor& color4,
		float rate1, float rate2, float rate3, float rate4)
	{
		float w1 = (std::max)(color1.r, (std::max)(color1.g, color1.b));
		float w2 = (std::max)(color2.r, (std::max)(color2.g, color2.b));
		float w3 = (std::max)(color3.r, (std::max)(color3.g, color3.b));
		float w4 = (std::max)(color4.r, (std::max)(color4.g, color4.b));
		XFColor ret;
		ret.r = color1.r * rate1 + color2.r * rate2 + color3.r * rate3 + color4.r * rate4;
		ret.g = color1.g * rate1 + color2.g * rate2 + color3.g * rate3 + color4.g * rate4;
		ret.b = color1.b * rate1 + color2.b * rate2 + color3.b * rate3 + color4.b * rate4;
		float w5 = w1 * rate1 + w2 * rate2 + w3 * rate3 + w4 * rate4;
		//
		float max = (std::max)(ret.r, (std::max)(ret.g, ret.b));
		if (max > 0.0f)
		{
			max = w5 / max;
			ret.r = ret.r * max;
			ret.g = ret.g * max;
			ret.b = ret.b * max;
		}
		return ret;
	}
	XFColor colorMixEx(const XFColor& color1, const XFColor& color2, const XFColor& color3, const XFColor& color4,
		float rate1, float rate2, float rate3, float rate4)
	{
		XFColor ret;
		ret.r = color1.r * rate1 + color2.r * rate2 + color3.r * rate3 + color4.r * rate4;
		ret.g = color1.g * rate1 + color2.g * rate2 + color3.g * rate3 + color4.g * rate4;
		ret.b = color1.b * rate1 + color2.b * rate2 + color3.b * rate3 + color4.b * rate4;
		ret.w = color1.w * rate1 + color2.w * rate2 + color3.w * rate3 + color4.w * rate4;
		float max = (std::max)(ret.r, (std::max)(ret.g, ret.b));
		if (max > 0.0f)
		{
			max = ret.w / max;
			ret.r = ret.r * max;
			ret.g = ret.g * max;
			ret.b = ret.b * max;
		}
		return ret;
	}
	XFColor colorMix(const XFColor *color, const float *rate, int mixSum)
	{
		XFColor ret(0.0f, 0.0f);
		float w;	//Ã÷¶È
		for (int i = 0; i < mixSum; ++i)
		{
			ret.r += color[i].r * rate[i];
			ret.g += color[i].g * rate[i];
			ret.b += color[i].b * rate[i];
			w = (std::max)(color[i].r, (std::max)(color[i].g, color[i].b));
			ret.w += w * rate[i];
		}
		float max = (std::max)(ret.r, (std::max)(ret.g, ret.b));
		if (max > 0.0f)
		{
			max = ret.w / max;
			ret.r = ret.r * max;
			ret.g = ret.g * max;
			ret.b = ret.b * max;
		}
		return ret;
	}
	XFColor colorMixEx(const XFColor *color, const float *rate, int mixSum)
	{
		XFColor ret(0.0f, 0.0f);
		for (int i = 0; i < mixSum; ++i)
		{
			ret.r += color[i].r * rate[i];
			ret.g += color[i].g * rate[i];
			ret.b += color[i].b * rate[i];
			ret.w += color[i].w * rate[i];
		}
		float max = (std::max)(ret.r, (std::max)(ret.g, ret.b));
		if (max > 0.0f)
		{
			max = ret.w / max;
			ret.r = ret.r * max;
			ret.g = ret.g * max;
			ret.b = ret.b * max;
		}
		return ret;
	}
	XFColor colorMixEx(const XFColor& color1, const XFColor& color2)
	{
		float h0, s0, l0;
		XColorSpace::RGB2HSL(color1.r * 255.0f, color1.g * 255.0f, color1.b * 255.0f, h0, s0, l0);
		float h1, s1, l1;
		XColorSpace::RGB2HSL(color2.r * 255.0f, color2.g * 255.0f, color2.b * 255.0f, h1, s1, l1);
		h0 = (h0 + h1) * 0.5f + 0.5f;
		if (h0 > 1.0f) h0 -= 1.0f;
		s0 = (s0 + s1) * 0.5f;
		l0 = (l0 + l1) * 0.5f;
		float r, g, b;
		XColorSpace::HSL2RGB(r, g, b, h0, s0, l0);
		return XFColor(r / 255.0f, g / 255.0f, b / 255.0f);
	}
}
const XFColor XFColor::zero(0.0f, 0.0f);
const XFColor XFColor::gray(0.5f, 1.0f);
const XFColor XFColor::white(1.0f, 1.0f);
const XFColor XFColor::red(1.0f, 0, 0);
const XFColor XFColor::green(0, 1.0f, 0);
const XFColor XFColor::blue(0, 0, 1.0f);
const XFColor XFColor::cyan(0, 1.0f, 1.0f);
const XFColor XFColor::magenta(1.0f, 0, 1.0f);
const XFColor XFColor::yellow(1.0f, 1.0f, 0);
const XFColor XFColor::black(0.0f, 1.0f);
const XFColor XFColor::aliceBlue(0.941176f, 0.972549f, 1.0f);
const XFColor XFColor::antiqueWhite(0.980392f, 0.921569f, 0.843137f);
const XFColor XFColor::aqua(0.0f, 1.0f, 1.0f);
const XFColor XFColor::aquamarine(0.498039f, 1.0f, 0.831373f);
const XFColor XFColor::azure(0.941176f, 1.0f, 1.0f);
const XFColor XFColor::beige(0.960784f, 0.960784f, 0.862745f);
const XFColor XFColor::bisque(1.0f, 0.894118f, 0.768627f);
const XFColor XFColor::blanchedAlmond(1.0f, 0.921569f, 0.803922f);
const XFColor XFColor::blueViolet(0.541176f, 0.168627f, 0.886275f);
const XFColor XFColor::brown(0.647059f, 0.164706f, 0.164706f);
const XFColor XFColor::burlyWood(0.870588f, 0.721569f, 0.529412f);
const XFColor XFColor::cadetBlue(0.372549f, 0.619608f, 0.627451f);
const XFColor XFColor::chartreuse(0.498039f, 1.0f, 0.0f);
const XFColor XFColor::chocolate(0.823529f, 0.411765f, 0.117647f);
const XFColor XFColor::coral(1.0f, 0.498039f, 0.313726f);
const XFColor XFColor::cornflowerBlue(0.392157f, 0.584314f, 0.929412f);
const XFColor XFColor::cornsilk(1.0f, 0.972549f, 0.862745f);
const XFColor XFColor::crimson(0.862745f, 0.0784314f, 0.235294f);
const XFColor XFColor::darkBlue(0.0f, 0.0f, 0.545098f);
const XFColor XFColor::darkCyan(0.0f, 0.545098f, 0.545098f);
const XFColor XFColor::darkGoldenRod(0.721569f, 0.52549f, 0.0431373f);
const XFColor XFColor::darkGray(0.662745f, 0.662745f, 0.662745f);
const XFColor XFColor::darkGrey(0.662745f, 0.662745f, 0.662745f);
const XFColor XFColor::darkGreen(0.0f, 0.392157f, 0.0f);
const XFColor XFColor::darkKhaki(0.741176f, 0.717647f, 0.419608f);
const XFColor XFColor::darkMagenta(0.545098f, 0.0f, 0.545098f);
const XFColor XFColor::darkOliveGreen(0.333333f, 0.419608f, 0.184314f);
const XFColor XFColor::darkorange(1.0f, 0.54902f, 0.0f);
const XFColor XFColor::darkOrchid(0.6f, 0.196078f, 0.8f);
const XFColor XFColor::darkRed(0.545098f, 0.0f, 0.0f);
const XFColor XFColor::darkSalmon(0.913725f, 0.588235f, 0.478431f);
const XFColor XFColor::darkSeaGreen(0.560784f, 0.737255f, 0.560784f);
const XFColor XFColor::darkSlateBlue(0.282353f, 0.239216f, 0.545098f);
const XFColor XFColor::darkSlateGray(0.184314f, 0.309804f, 0.309804f);
const XFColor XFColor::darkSlateGrey(0.184314f, 0.309804f, 0.309804f);
const XFColor XFColor::darkTurquoise(0.0f, 0.807843f, 0.819608f);
const XFColor XFColor::darkViolet(0.580392f, 0.0f, 0.827451f);
const XFColor XFColor::deepPink(1.0f, 0.0784314f, 0.576471f);
const XFColor XFColor::deepSkyBlue(0.0f, 0.74902f, 1.0f);
const XFColor XFColor::dimGray(0.411765f, 0.411765f, 0.411765f);
const XFColor XFColor::dimGrey(0.411765f, 0.411765f, 0.411765f);
const XFColor XFColor::dodgerBlue(0.117647f, 0.564706f, 1.0f);
const XFColor XFColor::fireBrick(0.698039f, 0.133333f, 0.133333f);
const XFColor XFColor::floralWhite(1.0f, 0.980392f, 0.941176f);
const XFColor XFColor::forestGreen(0.133333f, 0.545098f, 0.133333f);
const XFColor XFColor::fuchsia(1.0f, 0.0f, 1.0f);
const XFColor XFColor::gainsboro(0.862745f, 0.862745f, 0.862745f);
const XFColor XFColor::ghostWhite(0.972549f, 0.972549f, 1.0f);
const XFColor XFColor::gold(1.0f, 0.843137f, 0.0f);
const XFColor XFColor::goldenRod(0.854902f, 0.647059f, 0.12549f);
const XFColor XFColor::grey(0.501961f, 0.501961f, 0.501961f);
const XFColor XFColor::greenYellow(0.678431f, 1.0f, 0.184314f);
const XFColor XFColor::honeyDew(0.941176f, 1.0f, 0.941176f);
const XFColor XFColor::hotPink(1.0f, 0.411765f, 0.705882f);
const XFColor XFColor::indianRed(0.803922f, 0.360784f, 0.360784f);
const XFColor XFColor::indigo(0.294118f, 0.0f, 0.509804f);
const XFColor XFColor::ivory(1.0f, 1.0f, 0.941176f);
const XFColor XFColor::khaki(0.941176f, 0.901961f, 0.54902f);
const XFColor XFColor::lavender(0.901961f, 0.901961f, 0.980392f);
const XFColor XFColor::lavenderBlush(1.0f, 0.941176f, 0.960784f);
const XFColor XFColor::lawnGreen(0.486275f, 0.988235f, 0.0f);
const XFColor XFColor::lemonChiffon(1.0f, 0.980392f, 0.803922f);
const XFColor XFColor::lightBlue(0.678431f, 0.847059f, 0.901961f);
const XFColor XFColor::lightCoral(0.941176f, 0.501961f, 0.501961f);
const XFColor XFColor::lightCyan(0.878431f, 1.0f, 1.0f);
const XFColor XFColor::lightGoldenRodYellow(0.980392f, 0.980392f, 0.823529f);
const XFColor XFColor::lightGray(0.827451f, 0.827451f, 0.827451f);
const XFColor XFColor::lightGreen(0.564706f, 0.933333f, 0.564706f);
const XFColor XFColor::lightPink(1.0f, 0.713726f, 0.756863f);
const XFColor XFColor::lightSalmon(1.0f, 0.627451f, 0.478431f);
const XFColor XFColor::lightSeaGreen(0.12549f, 0.698039f, 0.666667f);
const XFColor XFColor::lightSkyBlue(0.529412f, 0.807843f, 0.980392f);
const XFColor XFColor::lightSlateGray(0.466667f, 0.533333f, 0.6f);
const XFColor XFColor::lightSlateGrey(0.466667f, 0.533333f, 0.6f);
const XFColor XFColor::lightSteelBlue(0.690196f, 0.768627f, 0.870588f);
const XFColor XFColor::lightYellow(1.0f, 1.0f, 0.878431f);
const XFColor XFColor::lime(0.0f, 1.0f, 0.0f);
const XFColor XFColor::limeGreen(0.196078f, 0.803922f, 0.196078f);
const XFColor XFColor::linen(0.980392f, 0.941176f, 0.901961f);
const XFColor XFColor::maroon(0.501961f, 0.0f, 0.0f);
const XFColor XFColor::mediumAquaMarine(0.4f, 0.803922f, 0.666667f);
const XFColor XFColor::mediumBlue(0.0f, 0.0f, 0.803922f);
const XFColor XFColor::mediumOrchid(0.729412f, 0.333333f, 0.827451f);
const XFColor XFColor::mediumPurple(0.576471f, 0.439216f, 0.858824f);
const XFColor XFColor::mediumSeaGreen(0.235294f, 0.701961f, 0.443137f);
const XFColor XFColor::mediumSlateBlue(0.482353f, 0.407843f, 0.933333f);
const XFColor XFColor::mediumSpringGreen(0.0f, 0.980392f, 0.603922f);
const XFColor XFColor::mediumTurquoise(0.282353f, 0.819608f, 0.8f);
const XFColor XFColor::mediumVioletRed(0.780392f, 0.0823529f, 0.521569f);
const XFColor XFColor::midnightBlue(0.0980392f, 0.0980392f, 0.439216f);
const XFColor XFColor::mintCream(0.960784f, 1.0f, 0.980392f);
const XFColor XFColor::mistyRose(1.0f, 0.894118f, 0.882353f);
const XFColor XFColor::moccasin(1.0f, 0.894118f, 0.709804f);
const XFColor XFColor::navajoWhite(1.0f, 0.870588f, 0.678431f);
const XFColor XFColor::navy(0.0f, 0.0f, 0.501961f);
const XFColor XFColor::oldLace(0.992157f, 0.960784f, 0.901961f);
const XFColor XFColor::olive(0.501961f, 0.501961f, 0.0f);
const XFColor XFColor::oliveDrab(0.419608f, 0.556863f, 0.137255f);
const XFColor XFColor::orange(1.0f, 0.647059f, 0.0f);
const XFColor XFColor::orangeRed(1.0f, 0.270588f, 0.0f);
const XFColor XFColor::orchid(0.854902f, 0.439216f, 0.839216f);
const XFColor XFColor::paleGoldenRod(0.933333f, 0.909804f, 0.666667f);
const XFColor XFColor::paleGreen(0.596078f, 0.984314f, 0.596078f);
const XFColor XFColor::paleTurquoise(0.686275f, 0.933333f, 0.933333f);
const XFColor XFColor::paleVioletRed(0.858824f, 0.439216f, 0.576471f);
const XFColor XFColor::papayaWhip(1.0f, 0.937255f, 0.835294f);
const XFColor XFColor::peachPuff(1.0f, 0.854902f, 0.72549f);
const XFColor XFColor::peru(0.803922f, 0.521569f, 0.247059f);
const XFColor XFColor::pink(1.0f, 0.752941f, 0.796078f);
const XFColor XFColor::plum(0.866667f, 0.627451f, 0.866667f);
const XFColor XFColor::powderBlue(0.690196f, 0.878431f, 0.901961f);
const XFColor XFColor::purple(0.501961f, 0.0f, 0.501961f);
const XFColor XFColor::rosyBrown(0.737255f, 0.560784f, 0.560784f);
const XFColor XFColor::royalBlue(0.254902f, 0.411765f, 0.882353f);
const XFColor XFColor::saddleBrown(0.545098f, 0.270588f, 0.0745098f);
const XFColor XFColor::salmon(0.980392f, 0.501961f, 0.447059f);
const XFColor XFColor::sandyBrown(0.956863f, 0.643137f, 0.376471f);
const XFColor XFColor::seaGreen(0.180392f, 0.545098f, 0.341176f);
const XFColor XFColor::seaShell(1.0f, 0.960784f, 0.933333f);
const XFColor XFColor::sienna(0.627451f, 0.321569f, 0.176471f);
const XFColor XFColor::silver(0.752941f, 0.752941f, 0.752941f);
const XFColor XFColor::skyBlue(0.529412f, 0.807843f, 0.921569f);
const XFColor XFColor::slateBlue(0.415686f, 0.352941f, 0.803922f);
const XFColor XFColor::slateGray(0.439216f, 0.501961f, 0.564706f);
const XFColor XFColor::slateGrey(0.439216f, 0.501961f, 0.564706f);
const XFColor XFColor::snow(1.0f, 0.980392f, 0.980392f);
const XFColor XFColor::springGreen(0.0f, 1.0f, 0.498039f);
const XFColor XFColor::steelBlue(0.27451f, 0.509804f, 0.705882f);
const XFColor XFColor::tan(0.823529f, 0.705882f, 0.54902f);
const XFColor XFColor::teal(0.0f, 0.501961f, 0.501961f);
const XFColor XFColor::thistle(0.847059f, 0.74902f, 0.847059f);
const XFColor XFColor::tomato(1.0f, 0.388235f, 0.278431f);
const XFColor XFColor::turquoise(0.25098f, 0.878431f, 0.815686f);
const XFColor XFColor::violet(0.933333f, 0.509804f, 0.933333f);
const XFColor XFColor::wheat(0.960784f, 0.870588f, 0.70196f);
const XFColor XFColor::whiteSmoke(0.960784f, 0.960784f, 0.960784f);
const XFColor XFColor::yellowGreen(0.603922f, 0.803922f, 0.196078f);
#if !WITH_INLINE_FILE
#include "XFColor.inl"
#endif
}