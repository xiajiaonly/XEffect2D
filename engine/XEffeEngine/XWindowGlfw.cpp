#include "XStdHead.h"
#if CREATE_WINDOW_METHOD == 2
#include "XWindowGlfw.h"
#include "glfw\glfw3.h"
#ifdef _WIN64
#pragma comment(lib, "glfw/glfw3_64.lib")
#else
#pragma comment(lib, "glfw/glfw3.lib")
#endif
namespace XE {
	bool XWindowGlfw::createWindow(int width, int height, const char *windowTitle, 
		bool isFullScreen, bool withFrame)
	{
		if (!glfwInit()) return false;

		glfwDefaultWindowHints();
		glfwWindowHint(GLFW_RED_BITS, 8);
		glfwWindowHint(GLFW_GREEN_BITS, 8);
		glfwWindowHint(GLFW_BLUE_BITS, 8);
		glfwWindowHint(GLFW_ALPHA_BITS, 8);

		glfwWindowHint(GLFW_DEPTH_BITS, 24);
		glfwWindowHint(GLFW_STENCIL_BITS, 8);
		//glfwWindowHint(GLFW_STEREO, settings.stereo);
		glfwWindowHint(GLFW_VISIBLE, GL_TRUE);

		glfwWindowHint(GLFW_SAMPLES, 4);
		glfwWindowHint(GLFW_RESIZABLE, 0);
		//glfwWindowHint(GLFW_DECORATED, settings.decorated);
		glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
		if (isFullScreen)
		{//这里可以选择在那个显示器上面全屏
			int count;
			GLFWmonitor** monitors = glfwGetMonitors(&count);
			m_window = glfwCreateWindow(width, height, XString::ANSI2UTF8(windowTitle).c_str(), monitors[0], NULL);
		}
		else
			m_window = glfwCreateWindow(width, height, XString::ANSI2UTF8(windowTitle).c_str(), NULL, NULL);
		if (!m_window)
		{
			glfwTerminate();
			return false;
		}
		//if (!withFrame)
		//{//尚未实现
		//}
		glfwSetWindowUserPointer(m_window, this);	//设置回调函数的指针
		glfwMakeContextCurrent(m_window);
		//判断窗口是否处于放大模式下
		int framebufferW, framebufferH;
		glfwGetFramebufferSize(m_window, &framebufferW, &framebufferH);

		//this lets us detect if the window is running in a retina mode
		if (framebufferW != width)
		{
			m_pixelScreenCoordScale = framebufferW / width;

			XVec2 position = getWindowPosition();
			setWindowShape(XVec2(width, height));
			setWindowPosition(position);
		}
		glfwSwapInterval(1);
		setVerticalSync(true);
		//下面设置回调函数
		glfwSetMouseButtonCallback(m_window, mouse_cb);
		glfwSetKeyCallback(m_window, keyboard_cb);
		glfwSetCursorEnterCallback(m_window, entry_cb);
		glfwSetCursorPosCallback(m_window, motion_cb);
		glfwSetWindowSizeCallback(m_window, resize_cb);
		glfwSetWindowCloseCallback(m_window, exit_cb);
		glfwSetScrollCallback(m_window, scroll_cb);
		glfwSetDropCallback(m_window, drop_cb);

		return true;
	}
	void XWindowGlfw::setWindowTitle(const std::string& title)
	{
		glfwSetWindowTitle(m_window, XString::ANSI2UTF8(title.c_str()).c_str());
	}
	unsigned char *XWindowGlfw::getWindowBuff()
	{
		return nullptr;
	}
	void XWindowGlfw::setCurcor(bool flag)
	{
		//	ShowCursor(flag);
		if (flag) glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		else glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	}
	bool XWindowGlfw::getCurcor()
	{
		return glfwGetInputMode(m_window, GLFW_CURSOR) != GLFW_CURSOR_HIDDEN;
	}
	void XWindowGlfw::release()
	{
		glfwDestroyWindow(m_window);
		glfwTerminate();
	}
	void XWindowGlfw::update()
	{
		glfwSwapBuffers(m_window);
		//glfwPollEvents();
	}
	int XWindowGlfw::mapKey(int key)
	{
		return 0;
	}
	XVec2 XWindowGlfw::getWindowPosition()
	{
		int x, y;
		glfwGetWindowPos(m_window, &x, &y);

		x *= m_pixelScreenCoordScale;
		y *= m_pixelScreenCoordScale;

		return XVec2(x, y);
	}
	void XWindowGlfw::setWindowShape(const XVec2& size)
	{
		glfwSetWindowSize(m_window, size.x / m_pixelScreenCoordScale, size.y / m_pixelScreenCoordScale);
	}
	void XWindowGlfw::setWindowPosition(const XVec2& pos)
	{
		glfwSetWindowPos(m_window, pos.x / m_pixelScreenCoordScale, pos.y / m_pixelScreenCoordScale);
	}
	void XWindowGlfw::setVerticalSync(bool bVerticalSync)
	{
		if (bVerticalSync) glfwSwapInterval(1);
		else glfwSwapInterval(0);
	}
	void XWindowGlfw::mouse_cb(GLFWwindow* windowP_, int button, int state, int mods)
	{
		XWindowGlfw* pRef = (XWindowGlfw*)glfwGetWindowUserPointer(windowP_);
		if (pRef == nullptr) return;
		XInputEvent tmp;
		tmp.type = EVENT_MOUSE;
		switch (state)
		{
		case GLFW_PRESS:
			switch (button)
			{
			case GLFW_MOUSE_BUTTON_LEFT:
				tmp.mouseState = MOUSE_LEFT_BUTTON_DOWN;
				break;
			case GLFW_MOUSE_BUTTON_RIGHT:
				tmp.mouseState = MOUSE_RIGHT_BUTTON_DOWN;
				break;
			case GLFW_MOUSE_BUTTON_MIDDLE:
				tmp.mouseState = MOUSE_MIDDLE_BUTTON_DOWN;
				break;
			}
			break;
		case GLFW_RELEASE:
			switch (button)
			{
			case GLFW_MOUSE_BUTTON_LEFT:
				tmp.mouseState = MOUSE_LEFT_BUTTON_UP;
				break;
			case GLFW_MOUSE_BUTTON_RIGHT:
				tmp.mouseState = MOUSE_RIGHT_BUTTON_UP;
				break;
			case GLFW_MOUSE_BUTTON_MIDDLE:
				tmp.mouseState = MOUSE_MIDDLE_BUTTON_UP;
				break;
			}
			break;
		}
		tmp.mouseX = pRef->m_curMousePos.x;
		tmp.mouseY = pRef->m_curMousePos.y;
		pRef->m_events.push_back(tmp);
	}
	void XWindowGlfw::motion_cb(GLFWwindow* windowP_, double x, double y)
	{
		XWindowGlfw* pRef = (XWindowGlfw*)glfwGetWindowUserPointer(windowP_);
		if (pRef == nullptr) return;
		pRef->m_curMousePos.x = x * pRef->m_pixelScreenCoordScale;
		pRef->m_curMousePos.y = y * pRef->m_pixelScreenCoordScale;

		XInputEvent tmp;
		tmp.type = EVENT_MOUSE;
		tmp.mouseState = MOUSE_MOVE;
		tmp.mouseX = pRef->m_curMousePos.x;
		tmp.mouseY = pRef->m_curMousePos.y;
		pRef->m_events.push_back(tmp);
	}
	void XWindowGlfw::entry_cb(GLFWwindow* windowP_, int entered)
	{
		//printf("entry_cb\n");
	}
	void XWindowGlfw::keyboard_cb(GLFWwindow* windowP_, int key, int scancode, int action, int mods)
	{
		XWindowGlfw* pRef = (XWindowGlfw*)glfwGetWindowUserPointer(windowP_);
		if (pRef == nullptr) return;
		XInputEvent tmp;
		tmp.type = EVENT_KEYBOARD;
		switch (action)
		{
		case GLFW_PRESS:
			tmp.keyState = KEY_STATE_DOWN;
			break;
		case GLFW_RELEASE:
			tmp.keyState = KEY_STATE_UP;
			break;
		}

		switch (key)
		{
		case GLFW_KEY_SPACE: tmp.keyValue = XKEY_SPACE; break;
		case GLFW_KEY_APOSTROPHE: tmp.keyValue = XKEY_QUOTE; break;
		case GLFW_KEY_COMMA: tmp.keyValue = XKEY_COMMA; break;
		case GLFW_KEY_MINUS: tmp.keyValue = XKEY_MINUS; break;
		case GLFW_KEY_PERIOD: tmp.keyValue = XKEY_PERIOD; break;
		case GLFW_KEY_SLASH: tmp.keyValue = XKEY_SLASH; break;
		case GLFW_KEY_0: tmp.keyValue = XKEY_0; break;
		case GLFW_KEY_1: tmp.keyValue = XKEY_1; break;
		case GLFW_KEY_2: tmp.keyValue = XKEY_2; break;
		case GLFW_KEY_3: tmp.keyValue = XKEY_3; break;
		case GLFW_KEY_4: tmp.keyValue = XKEY_4; break;
		case GLFW_KEY_5: tmp.keyValue = XKEY_5; break;
		case GLFW_KEY_6: tmp.keyValue = XKEY_6; break;
		case GLFW_KEY_7: tmp.keyValue = XKEY_7; break;
		case GLFW_KEY_8: tmp.keyValue = XKEY_8; break;
		case GLFW_KEY_9: tmp.keyValue = XKEY_9; break;
		case GLFW_KEY_SEMICOLON: tmp.keyValue = XKEY_SEMICOLON; break;
		case GLFW_KEY_EQUAL: tmp.keyValue = XKEY_EQUALS; break;
		case GLFW_KEY_A: tmp.keyValue = XKEY_A; break;
		case GLFW_KEY_B: tmp.keyValue = XKEY_B; break;
		case GLFW_KEY_C: tmp.keyValue = XKEY_C; break;
		case GLFW_KEY_D: tmp.keyValue = XKEY_D; break;
		case GLFW_KEY_E: tmp.keyValue = XKEY_E; break;
		case GLFW_KEY_F: tmp.keyValue = XKEY_F; break;
		case GLFW_KEY_G: tmp.keyValue = XKEY_G; break;
		case GLFW_KEY_H: tmp.keyValue = XKEY_H; break;
		case GLFW_KEY_I: tmp.keyValue = XKEY_I; break;
		case GLFW_KEY_J: tmp.keyValue = XKEY_J; break;
		case GLFW_KEY_K: tmp.keyValue = XKEY_K; break;
		case GLFW_KEY_L: tmp.keyValue = XKEY_L; break;
		case GLFW_KEY_M: tmp.keyValue = XKEY_M; break;
		case GLFW_KEY_N: tmp.keyValue = XKEY_N; break;
		case GLFW_KEY_O: tmp.keyValue = XKEY_O; break;
		case GLFW_KEY_P: tmp.keyValue = XKEY_P; break;
		case GLFW_KEY_Q: tmp.keyValue = XKEY_Q; break;
		case GLFW_KEY_R: tmp.keyValue = XKEY_R; break;
		case GLFW_KEY_S: tmp.keyValue = XKEY_S; break;
		case GLFW_KEY_T: tmp.keyValue = XKEY_T; break;
		case GLFW_KEY_U: tmp.keyValue = XKEY_U; break;
		case GLFW_KEY_V: tmp.keyValue = XKEY_V; break;
		case GLFW_KEY_W: tmp.keyValue = XKEY_W; break;
		case GLFW_KEY_X: tmp.keyValue = XKEY_X; break;
		case GLFW_KEY_Y: tmp.keyValue = XKEY_Y; break;
		case GLFW_KEY_Z: tmp.keyValue = XKEY_Z; break;
		case GLFW_KEY_LEFT_BRACKET: tmp.keyValue = XKEY_LEFTBRACKET; break;
		case GLFW_KEY_BACKSLASH: tmp.keyValue = XKEY_BACKSLASH; break;
		case GLFW_KEY_RIGHT_BRACKET: tmp.keyValue = XKEY_RIGHTBRACKET; break;
		case GLFW_KEY_GRAVE_ACCENT: tmp.keyValue = XKEY_DELETE; break;
		case GLFW_KEY_WORLD_1: tmp.keyValue = XKEY_UNKNOWN; break;
		case GLFW_KEY_WORLD_2: tmp.keyValue = XKEY_UNKNOWN; break;

		case GLFW_KEY_ESCAPE: tmp.keyValue = XKEY_ESCAPE; break;
		case GLFW_KEY_ENTER: tmp.keyValue = XKEY_RETURN; break;
		case GLFW_KEY_TAB: tmp.keyValue = XKEY_TAB; break;
		case GLFW_KEY_BACKSPACE: tmp.keyValue = XKEY_BACKSPACE; break;
		case GLFW_KEY_INSERT: tmp.keyValue = XKEY_INSERT; break;
		case GLFW_KEY_DELETE: tmp.keyValue = XKEY_DELETE; break;
		case GLFW_KEY_RIGHT: tmp.keyValue = XKEY_RIGHT; break;
		case GLFW_KEY_LEFT: tmp.keyValue = XKEY_LEFT; break;
		case GLFW_KEY_DOWN: tmp.keyValue = XKEY_DOWN; break;
		case GLFW_KEY_UP: tmp.keyValue = XKEY_UP; break;
		case GLFW_KEY_PAGE_UP: tmp.keyValue = XKEY_PAGEUP; break;
		case GLFW_KEY_PAGE_DOWN: tmp.keyValue = XKEY_PAGEDOWN; break;
		case GLFW_KEY_HOME: tmp.keyValue = XKEY_HOME; break;
		case GLFW_KEY_END: tmp.keyValue = XKEY_END; break;
		case GLFW_KEY_CAPS_LOCK: tmp.keyValue = XKEY_CAPSLOCK; break;
		case GLFW_KEY_SCROLL_LOCK: tmp.keyValue = XKEY_SCROLLOCK; break;
		case GLFW_KEY_NUM_LOCK: tmp.keyValue = XKEY_NUMLOCK; break;
		case GLFW_KEY_PRINT_SCREEN: tmp.keyValue = XKEY_PRINT; break;
		case GLFW_KEY_PAUSE: tmp.keyValue = XKEY_PAUSE; break;
		case GLFW_KEY_F1: tmp.keyValue = XKEY_F1; break;
		case GLFW_KEY_F2: tmp.keyValue = XKEY_F2; break;
		case GLFW_KEY_F3: tmp.keyValue = XKEY_F3; break;
		case GLFW_KEY_F4: tmp.keyValue = XKEY_F4; break;
		case GLFW_KEY_F5: tmp.keyValue = XKEY_F5; break;
		case GLFW_KEY_F6: tmp.keyValue = XKEY_F6; break;
		case GLFW_KEY_F7: tmp.keyValue = XKEY_F7; break;
		case GLFW_KEY_F8: tmp.keyValue = XKEY_F8; break;
		case GLFW_KEY_F9: tmp.keyValue = XKEY_F9; break;
		case GLFW_KEY_F10: tmp.keyValue = XKEY_F10; break;
		case GLFW_KEY_F11: tmp.keyValue = XKEY_F11; break;
		case GLFW_KEY_F12: tmp.keyValue = XKEY_F12; break;
		case GLFW_KEY_F13: tmp.keyValue = XKEY_F13; break;
		case GLFW_KEY_F14: tmp.keyValue = XKEY_F14; break;
		case GLFW_KEY_F15: tmp.keyValue = XKEY_F15; break;
		case GLFW_KEY_F16: tmp.keyValue = XKEY_UNKNOWN; break;
		case GLFW_KEY_F17: tmp.keyValue = XKEY_UNKNOWN; break;
		case GLFW_KEY_F18: tmp.keyValue = XKEY_UNKNOWN; break;
		case GLFW_KEY_F19: tmp.keyValue = XKEY_UNKNOWN; break;
		case GLFW_KEY_F20: tmp.keyValue = XKEY_UNKNOWN; break;
		case GLFW_KEY_F21: tmp.keyValue = XKEY_UNKNOWN; break;
		case GLFW_KEY_F22: tmp.keyValue = XKEY_UNKNOWN; break;
		case GLFW_KEY_F23: tmp.keyValue = XKEY_UNKNOWN; break;
		case GLFW_KEY_F24: tmp.keyValue = XKEY_UNKNOWN; break;
		case GLFW_KEY_F25: tmp.keyValue = XKEY_UNKNOWN; break;
		case GLFW_KEY_KP_0: tmp.keyValue = XKEY_N0; break;
		case GLFW_KEY_KP_1: tmp.keyValue = XKEY_N1; break;
		case GLFW_KEY_KP_2: tmp.keyValue = XKEY_N2; break;
		case GLFW_KEY_KP_3: tmp.keyValue = XKEY_N3; break;
		case GLFW_KEY_KP_4: tmp.keyValue = XKEY_N4; break;
		case GLFW_KEY_KP_5: tmp.keyValue = XKEY_N5; break;
		case GLFW_KEY_KP_6: tmp.keyValue = XKEY_N6; break;
		case GLFW_KEY_KP_7: tmp.keyValue = XKEY_N7; break;
		case GLFW_KEY_KP_8: tmp.keyValue = XKEY_N8; break;
		case GLFW_KEY_KP_9: tmp.keyValue = XKEY_N9; break;
		case GLFW_KEY_KP_DECIMAL: tmp.keyValue = XKEY_N_PERIOD; break;
		case GLFW_KEY_KP_DIVIDE: tmp.keyValue = XKEY_N_DIVIDE; break;
		case GLFW_KEY_KP_MULTIPLY: tmp.keyValue = XKEY_N_MULTIPLY; break;
		case GLFW_KEY_KP_SUBTRACT: tmp.keyValue = XKEY_N_MINUS; break;
		case GLFW_KEY_KP_ADD: tmp.keyValue = XKEY_N_PLUS; break;
		case GLFW_KEY_KP_ENTER: tmp.keyValue = XKEY_N_ENTER; break;
		case GLFW_KEY_KP_EQUAL: tmp.keyValue = XKEY_N_EQUALS; break;
		case GLFW_KEY_LEFT_SHIFT: tmp.keyValue = XKEY_LSHIFT; break;
		case GLFW_KEY_LEFT_CONTROL: tmp.keyValue = XKEY_LCTRL; break;
		case GLFW_KEY_LEFT_ALT: tmp.keyValue = XKEY_LALT; break;
		case GLFW_KEY_LEFT_SUPER: tmp.keyValue = XKEY_LSUPER; break;
		case GLFW_KEY_RIGHT_SHIFT: tmp.keyValue = XKEY_RSHIFT; break;
		case GLFW_KEY_RIGHT_CONTROL: tmp.keyValue = XKEY_RCTRL; break;
		case GLFW_KEY_RIGHT_ALT: tmp.keyValue = XKEY_RALT; break;
		case GLFW_KEY_RIGHT_SUPER: tmp.keyValue = XKEY_RSUPER; break;
		case GLFW_KEY_MENU: tmp.keyValue = XKEY_MENU; break;
		}

		//if (key == GLFW_KEY_ESCAPE && mods == 0)
		//	glfwSetWindowShouldClose(window, GLFW_TRUE);
		//if ((key == GLFW_KEY_ENTER && mods == GLFW_MOD_ALT) ||
		//	(key == GLFW_KEY_F11 && mods == GLFW_MOD_ALT))
		//{
		//	if (glfwGetWindowMonitor(window))
		//	{
		//		glfwSetWindowMonitor(window, NULL,
		//			windowed_xpos, windowed_ypos,
		//			windowed_width, windowed_height, 0);
		//	}
		//	else
		//	{
		//		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		//		if (monitor)
		//		{
		//			const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		//			glfwGetWindowPos(window, &windowed_xpos, &windowed_ypos);
		//			glfwGetWindowSize(window, &windowed_width, &windowed_height);
		//			glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
		//		}
		//	}
		//}
		pRef->m_events.push_back(tmp);
	}
	void XWindowGlfw::resize_cb(GLFWwindow* windowP_, int w, int h)
	{
		XWindowGlfw* pRef = (XWindowGlfw*)glfwGetWindowUserPointer(windowP_);
		if (pRef == nullptr) return;

		XInputEvent tmp;
		tmp.type = EVENT_RESIZE;
		tmp.mouseX = w;
		tmp.mouseY = h;
		pRef->m_events.push_back(tmp);
	}
	void XWindowGlfw::exit_cb(GLFWwindow* windowP_)
	{
		XWindowGlfw* pRef = (XWindowGlfw*)glfwGetWindowUserPointer(windowP_);
		if (pRef == nullptr) return;

		XInputEvent tmp;
		tmp.type = EVENT_EXIT;
		pRef->m_events.push_back(tmp);
	}
	void XWindowGlfw::scroll_cb(GLFWwindow* windowP_, double x, double y)
	{
		XWindowGlfw* pRef = (XWindowGlfw*)glfwGetWindowUserPointer(windowP_);
		if (pRef == nullptr) return;
		XInputEvent tmp;
		tmp.type = EVENT_MOUSE;
		if (y > 0) tmp.mouseState = MOUSE_WHEEL_DOWN_DOWN;
		else tmp.mouseState = MOUSE_WHEEL_UP_DOWN;
		tmp.mouseX = pRef->m_curMousePos.x;
		tmp.mouseY = pRef->m_curMousePos.y;
		pRef->m_events.push_back(tmp);
	}
	void XWindowGlfw::drop_cb(GLFWwindow* windowP_, int numFiles, const char** dropString)
	{
		//printf("entry_cb\n");
	}
	void XWindowGlfw::error_cb(int errorCode, const char* errorDescription)
	{
		//printf("entry_cb\n");
	}
}
#endif