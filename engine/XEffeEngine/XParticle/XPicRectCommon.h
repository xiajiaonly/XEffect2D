#ifndef _JIA_XPICRECTCOMMON_
#define _JIA_XPICRECTCOMMON_
namespace XE {
enum XPicState
{
	PIC_STA_NORMAL,
	PIC_STA_UP_TURN_ENTER,
	PIC_STA_UP_TURN_IN,
	PIC_STA_DOWN_TURN_ENTER,
	PIC_STA_DOWN_TURN_IN,
	PIC_STA_LEFT_TURN_ENTER,
	PIC_STA_LEFT_TURN_IN,
	PIC_STA_RIGHT_TURN_ENTER,
	PIC_STA_RIGHT_TURN_IN,
	PIC_STA_NULL,
};
enum
{
	TEXTURE_UP = 0,
	TEXTURE_DOWN,
	TEXTURE_LEFT,
	TEXTURE_RIGHT
};
}
#endif