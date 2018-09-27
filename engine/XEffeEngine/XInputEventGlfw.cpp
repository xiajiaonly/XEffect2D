#include "XStdHead.h"
#include "XInputEventSDL.h"
#include "XWindowCommon.h"
#include "XTimer.h"
#if CREATE_WINDOW_METHOD == 2
#include "XWindowGlfw.h"
#include "glfw\glfw3.h"
namespace XE{
bool getInputEventGlfw(XInputEvent &event)
{
	glfwPollEvents();
	XWindowGlfw& ref = *(XWindowGlfw*)(gFrameworkData.pWindowCore);
	if (ref.m_events.size() == 0) return false;
	event = ref.m_events[0];
	ref.m_events.pop_front();
	return true;
}
}
#endif