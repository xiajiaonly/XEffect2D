#include "XStdHead.h"
#include "XVirtualMTouch.h"
//#define TEST_MODE
namespace XE {
bool XVirMTouch::init(int maxPointsSum)
{
	if (m_isInited) return true;
	if (maxPointsSum <= 0 || maxPointsSum > MAX_TOUCH_COUNT) return false;
	m_maxPointSum = maxPointsSum;
	if (!InitializeTouchInjection(m_maxPointSum, TOUCH_FEEDBACK_NONE)) return false;
	//if (!InitializeTouchInjection(m_maxPointSum, TOUCH_FEEDBACK_INDIRECT)) return false;
	m_contacts.resize(m_maxPointSum);
	for (auto it = m_contacts.begin(); it != m_contacts.end(); ++it)
	{
		memset(&(*it), 0, sizeof(POINTER_TOUCH_INFO));

		it->pointerInfo.pointerType = PT_TOUCH; //we're sending touch input
		it->touchFlags = TOUCH_FLAG_NONE;
		it->touchMask = TOUCH_MASK_CONTACTAREA | TOUCH_MASK_ORIENTATION | TOUCH_MASK_PRESSURE;
	//	it->orientation = 90;
		it->pressure = 1024;
	}

	m_isInited = true;
	return true;
}
void XVirMTouch::release()
{
	if (!m_isInited) return;

	m_isInited = false;
}
bool XVirMTouch::pointDown(float x, float y, int id)
{
	if (!m_isInited || id < 0 || id >= m_maxPointSum) return false;
	POINTER_TOUCH_INFO& ref = m_contacts[0];
	ref.pointerInfo.pointerId = id;          //contact 0
	ref.pointerInfo.ptPixelLocation.x = x;
	ref.pointerInfo.ptPixelLocation.y = y;
	ref.rcContact.top = y - 2;
	ref.rcContact.bottom = y + 2;
	ref.rcContact.left = x - 2;
	ref.rcContact.right = x + 2;
	ref.pointerInfo.pointerFlags = POINTER_FLAG_DOWN | POINTER_FLAG_INRANGE | POINTER_FLAG_INCONTACT;
	return InjectTouchInput(1, &ref);
}
bool XVirMTouch::pointUp(float x, float y, int id)
{
	if (!m_isInited || id < 0 || id >= m_maxPointSum) return false;
	POINTER_TOUCH_INFO& ref = m_contacts[0];
	ref.pointerInfo.pointerId = id;          //contact 0
	ref.pointerInfo.ptPixelLocation.x = x;
	ref.pointerInfo.ptPixelLocation.y = y;
	ref.rcContact.top = y - 2;
	ref.rcContact.bottom = y + 2;
	ref.rcContact.left = x - 2;
	ref.rcContact.right = x + 2;
	ref.pointerInfo.pointerFlags = POINTER_FLAG_UP;
	return InjectTouchInput(1, &ref);
}
bool XVirMTouch::pointMove(float x, float y, int id)
{
	if (!m_isInited || id < 0 || id >= m_maxPointSum) return false;
	POINTER_TOUCH_INFO& ref = m_contacts[0];
	ref.pointerInfo.pointerId = id;          //contact 0
	ref.pointerInfo.ptPixelLocation.x = x;
	ref.pointerInfo.ptPixelLocation.y = y;
	ref.rcContact.top = y - 2;
	ref.rcContact.bottom = y + 2;
	ref.rcContact.left = x - 2;
	ref.rcContact.right = x + 2;
	ref.pointerInfo.pointerFlags = POINTER_FLAG_UPDATE | POINTER_FLAG_INRANGE | POINTER_FLAG_INCONTACT;
	return InjectTouchInput(1, &ref);
}
bool XVirMTouch::pointsDown(const std::vector<XVTouchInfo>& info)
{
	if (!m_isInited) return false;
	int sum = info.size();
	if (sum > m_maxPointSum) sum = m_maxPointSum;
	if (sum <= 0) return true;
#ifdef TEST_MODE
	printf("Down:");
#endif
	for (int i = 0; i < sum; ++i)
	{
		POINTER_TOUCH_INFO& ref = m_contacts[i];
		const XVTouchInfo& refX = info[i];
		ref.pointerInfo.pointerId = refX.id;          //contact 0
		ref.pointerInfo.ptPixelLocation.x = refX.x;
		ref.pointerInfo.ptPixelLocation.y = refX.y;
		ref.rcContact.top = refX.y - 2;
		ref.rcContact.bottom = refX.y + 2;
		ref.rcContact.left = refX.x - 2;
		ref.rcContact.right = refX.x + 2;
		ref.pointerInfo.pointerFlags = POINTER_FLAG_DOWN | POINTER_FLAG_INRANGE | POINTER_FLAG_INCONTACT;
#ifdef TEST_MODE
		printf("%d,", refX.id);
#endif
	}
#ifdef TEST_MODE
	printf("\n");
#endif
	return InjectTouchInput(sum, &m_contacts[0]);
}
bool XVirMTouch::pointsUp(const std::vector<XVTouchInfo>& info)
{
	if (!m_isInited) return false;
	int sum = info.size();
	if (sum > m_maxPointSum) sum = m_maxPointSum;
	if (sum <= 0) return true;
#ifdef TEST_MODE
	printf("Up:");
#endif
	for (int i = 0; i < sum; ++i)
	{
		POINTER_TOUCH_INFO& ref = m_contacts[i];
		const XVTouchInfo& refX = info[i];
		ref.pointerInfo.pointerId = refX.id;          //contact 0
		ref.pointerInfo.ptPixelLocation.x = refX.x;
		ref.pointerInfo.ptPixelLocation.y = refX.y;
		ref.rcContact.top = refX.y - 2;
		ref.rcContact.bottom = refX.y + 2;
		ref.rcContact.left = refX.x - 2;
		ref.rcContact.right = refX.x + 2;
		ref.pointerInfo.pointerFlags = POINTER_FLAG_UP;
#ifdef TEST_MODE
		printf("%d,", refX.id);
#endif
	}
#ifdef TEST_MODE
	printf("\n");
#endif
	return InjectTouchInput(sum, &m_contacts[0]);
}
bool XVirMTouch::pointsMove(const std::vector<XVTouchInfo>& info)
{
	if (!m_isInited) return false;
	int sum = info.size();
	if (sum > m_maxPointSum) sum = m_maxPointSum;
	if (sum <= 0) return true;
#ifdef TEST_MODE
	printf("Move:");
#endif
	for (int i = 0; i < sum; ++i)
	{
		POINTER_TOUCH_INFO& ref = m_contacts[i];
		const XVTouchInfo& refX = info[i];
		ref.pointerInfo.pointerId = refX.id;          //contact 0
		ref.pointerInfo.ptPixelLocation.x = refX.x;
		ref.pointerInfo.ptPixelLocation.y = refX.y;
		ref.rcContact.top = refX.y - 2;
		ref.rcContact.bottom = refX.y + 2;
		ref.rcContact.left = refX.x - 2;
		ref.rcContact.right = refX.x + 2;
		ref.pointerInfo.pointerFlags = POINTER_FLAG_UPDATE | POINTER_FLAG_INRANGE | POINTER_FLAG_INCONTACT;
#ifdef TEST_MODE
		printf("%d,", refX.id);
#endif
	}
#ifdef TEST_MODE
	printf("\n");
#endif
	return InjectTouchInput(sum, &m_contacts[0]);
}
bool XVirMTouch::upDateData(const std::vector<XVTouchInfo>& info)
{
	if (!m_isInited) return false;
	int sum = info.size();
	if (sum > m_maxPointSum) sum = m_maxPointSum;
	if (sum <= 0) return true;
#ifdef TEST_MODE
	printf("Update:");
#endif
	for (int i = 0; i < sum; ++i)
	{
		POINTER_TOUCH_INFO& ref = m_contacts[i];
		const XVTouchInfo& refX = info[i];
		ref.pointerInfo.pointerId = refX.id;          //contact 0
		ref.pointerInfo.ptPixelLocation.x = refX.x;
		ref.pointerInfo.ptPixelLocation.y = refX.y;
		ref.rcContact.top = refX.y - 2;
		ref.rcContact.bottom = refX.y + 2;
		ref.rcContact.left = refX.x - 2;
		ref.rcContact.right = refX.x + 2;
		switch (refX.state)
		{
		case VT_STATE_DOWN:
			ref.pointerInfo.pointerFlags = POINTER_FLAG_DOWN | POINTER_FLAG_INRANGE | POINTER_FLAG_INCONTACT;
#ifdef TEST_MODE
			printf("d");
#endif
			break;
		case VT_STATE_MOVE:
			ref.pointerInfo.pointerFlags = POINTER_FLAG_UPDATE | POINTER_FLAG_INRANGE | POINTER_FLAG_INCONTACT;
#ifdef TEST_MODE
			printf("m");
#endif
			break;
		case VT_STATE_UP:
			ref.pointerInfo.pointerFlags = POINTER_FLAG_UP;
#ifdef TEST_MODE
			printf("u");
#endif
			break;
		}
#ifdef TEST_MODE
		printf("%d,", refX.id);
#endif
	}
#ifdef TEST_MODE
	printf("\n");
#endif
	return InjectTouchInput(sum, &m_contacts[0]);
}
}