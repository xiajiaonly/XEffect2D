inline _XFColor _XFColor::operator * (const _XFColor& temp) const
{
	return _XFColor(fR * temp.fR,fG * temp.fG,fB * temp.fB,fA * temp.fA);
}
inline void _XFColor::setR(float r)
{
	if(r < 0.0f) r = 0.0f;
	if(r > 1.0f) r = 1.0f;
	fR = r;
}
inline void _XFColor::setG(float g)
{
	if(g < 0.0f) g = 0.0f;
	if(g > 1.0f) g = 1.0f;
	fG = g;
}
inline void _XFColor::setB(float b)
{
	if(b < 0.0f) b = 0.0f;
	if(b > 1.0f) b = 1.0f;
	fB = b;
}
inline void _XFColor::setA(float a)
{
	if(a < 0.0f) a = 0.0f;
	if(a > 1.0f) a = 1.0f;
	fA = a;
}
inline float _XFColor::getR() const 
{
	return fR;
}
inline float _XFColor::getG() const 
{
	return fG;
}
inline float _XFColor::getB() const 
{
	return fB;
}
inline float _XFColor::getA() const 
{
	return fA;
}
inline void _XFColor::randColor()
{
	fR = randomf(1);
	fG = randomf(1);
	fB = randomf(1);
	fA = randomf(1);
}