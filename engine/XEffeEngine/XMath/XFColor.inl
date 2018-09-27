INLINE XFColor XFColor::operator * (const XFColor& temp) const
{
	return XFColor(r * temp.r,g * temp.g,b * temp.b,a * temp.a);
}
INLINE XFColor XFColor::operator * (const float& temp) const
{
	return XFColor(r * temp,g * temp,b * temp,a * temp);
}
INLINE void XFColor::setR(float _r)
{
	r = XMath::clamp(_r, 0.0f, 1.0f);
}
INLINE void XFColor::setG(float _g)
{
	g = XMath::clamp(_g, 0.0f, 1.0f);
}
INLINE void XFColor::setB(float _b)
{
	b = XMath::clamp(_b, 0.0f, 1.0f);
}
INLINE void XFColor::setA(float _a)
{
	a = XMath::clamp(_a, 0.0f, 1.0f);
}
INLINE float XFColor::getR() const 
{
	return r;
}
INLINE float XFColor::getG() const 
{
	return g;
}
INLINE float XFColor::getB() const 
{
	return b;
}
INLINE float XFColor::getA() const 
{
	return a;
}
INLINE void XFColor::randColor()
{
	r = XRand::randomf(1.0f);
	g = XRand::randomf(1.0f);
	b = XRand::randomf(1.0f);
	a = XRand::randomf(1.0f);
}