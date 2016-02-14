INLINE XFColor XFColor::operator * (const XFColor& temp) const
{
	return XFColor(fR * temp.fR,fG * temp.fG,fB * temp.fB,fA * temp.fA);
}
INLINE void XFColor::setR(float r)
{
	if(r < 0.0f) r = 0.0f;
	if(r > 1.0f) r = 1.0f;
	fR = r;
}
INLINE void XFColor::setG(float g)
{
	if(g < 0.0f) g = 0.0f;
	if(g > 1.0f) g = 1.0f;
	fG = g;
}
INLINE void XFColor::setB(float b)
{
	if(b < 0.0f) b = 0.0f;
	if(b > 1.0f) b = 1.0f;
	fB = b;
}
INLINE void XFColor::setA(float a)
{
	if(a < 0.0f) a = 0.0f;
	if(a > 1.0f) a = 1.0f;
	fA = a;
}
INLINE float XFColor::getR() const 
{
	return fR;
}
INLINE float XFColor::getG() const 
{
	return fG;
}
INLINE float XFColor::getB() const 
{
	return fB;
}
INLINE float XFColor::getA() const 
{
	return fA;
}
INLINE void XFColor::randColor()
{
	fR = XRand::randomf(1);
	fG = XRand::randomf(1);
	fB = XRand::randomf(1);
	fA = XRand::randomf(1);
}