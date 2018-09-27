#ifndef _JIA_XWINDOWGLFW_
#define _JIA_XWINDOWGLFW_
#if CREATE_WINDOW_METHOD == 2
#include "XWindowCore.h"
#include "XStringFun.h"
#include <deque>
#include "XInputEventCore.h"
struct GLFWwindow;
namespace XE {
class XWindowGlfw :public XWindowCore
{
public:
	XWindowGlfw()
		:m_window(nullptr)
		, m_pixelScreenCoordScale(1.0f)
	{}
	virtual ~XWindowGlfw() {}
protected:
	XWindowGlfw(const XWindowGlfw&);
	XWindowGlfw &operator= (const XWindowGlfw&);

	GLFWwindow* m_window;
public:
	virtual bool createWindow(int width, int height, const char *windowTitle, bool isFullScreen, bool withFrame);
	virtual void setWindowTitle(const std::string& title);
	virtual void setCurcor(bool flag);
	virtual unsigned char *getWindowBuff();
	virtual bool getCurcor();
	virtual void release();
	virtual void update();
	virtual int mapKey(int key);
	//下面是一些私有的方法
private:
	float m_pixelScreenCoordScale;

	static void mouse_cb(GLFWwindow* windowP_, int button, int state, int mods);
	static void motion_cb(GLFWwindow* windowP_, double x, double y);
	static void entry_cb(GLFWwindow* windowP_, int entered);
	static void keyboard_cb(GLFWwindow* windowP_, int key, int scancode, int action, int mods);
	static void resize_cb(GLFWwindow* windowP_, int w, int h);
	static void exit_cb(GLFWwindow* windowP_);
	static void	scroll_cb(GLFWwindow* windowP_, double x, double y);
	static void drop_cb(GLFWwindow* windowP_, int numFiles, const char** dropString);
	static void	error_cb(int errorCode, const char* errorDescription);
public:
	XVec2 m_curMousePos;
	std::deque<XInputEvent> m_events;
	XVec2 getWindowPosition();
	void setWindowShape(const XVec2& size);
	void setWindowPosition(const XVec2& pos);
	void setVerticalSync(bool bVerticalSync);
};
}
#endif
#endif