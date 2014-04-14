//inline 函数的定义
inline float _XLine::getLineLength() const
{
	return m_startPosition.getLength(m_endPosition);
}
inline float _XLine::getLineAngle() const
{
	return m_angle;
}
inline _XVector2 _XLine::getStartPosition() const
{
	return m_startPosition;
}
inline _XVector2 _XLine::getEndPosition() const
{
	return m_endPosition;
}