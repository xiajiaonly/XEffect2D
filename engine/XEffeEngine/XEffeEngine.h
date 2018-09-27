#ifndef _JIA_XEFFEENGINE_
#define _JIA_XEFFEENGINE_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------
//++++++++++++++++++++++++++++++++
/*说明:
1、引擎可以通过example09的程序对资源进行自动优化，但是注意粒子系统的资源不能通过这个程序进行优化，
否则将会造成粒子系统的贴图错误，原因在于粒子系统需要较高的贴图处理速度，所以图片资源不能进行优化，
否则将会损失效率，综合考虑之后取消对粒子系统的资源进行优化拼合。
2、需要注意的是，资源优化的资源都是在loading完成之后才能被调用，所以如果在loading之前显示的资源，
或者在loading之时使用的资源最好不要经过资源优化，否则loading将会变得没有什么意义。
3、引擎所有的资源除了music和movie都可以通过XResourcePack程序对资源进行打包和加密，之后引擎可以直
接从资源包中提取资源文件。
4、当然引擎也支持资源不优化或者支援不打包都可以直接使用。
5、目前资源只能一次loading载入，然后在使用，没有测试过动态载入，之后需要测试。
6、
*/
//--------------------------------

#include <stdio.h>
#include <stdlib.h>

#include "XBasicOpenGL.h"

#include "XSprite.h"
#include "XMaskTex.h"
#include "XFrame.h"
#include "XFont/XFont.h"
#include "XFont/XFontUnicode.h"

#include "XOSDefine.h"

#include "XFont/XNumber.h"
#include "XFont/XNumberEx.h"
#include "XMedia/XSound.h"
#include "XMedia/XMusic.h"
#include "XNodeLine.h"
#include "XMath/XBezierSpline.h"
#include "XMath/XBezierInterpolation.h"
#include "XLogBook.h"
#include "XString.h"
#include "XParticle/XParticles.h"
#include "XControl/XControls.h"
#include "XPhysics/XPhysicsBasic.h"
#include "XGameElement/XGameElement.h"
#include "XXml.h"
#include "XConfigManager.h"
#include "XTimer.h"
#include "XErrorReporter.h"
#include "XThread.h"
#include "XPixelCommon.h"
#include "XWindowCommon.h"
#include "XMath/XPressDataMode.h"

#include "XInputEventCommon.h"
#include "XCommonDefine.h"
#include "XMedia/XMovieFfmpeg.h"

#include "XBmp.h"
#include "XDataBasic.h"
#include "XWindowTitle.h"

#include "XTextureInformation.h"
#include "XFrameEx.h"
#include "XMath/X2DPacker.h"
#include "XResourcePack.h"

#include "XGameBasic.h"
#include "XException.h"
#include "XErrorReporter.h"
#ifdef _DEBUG
#pragma comment(lib, "../../engine/bin/XEffectEngineD.lib")
#else 
#pragma comment(lib, "../../engine/bin/XEffectEngine.lib")
#endif

#if WITH_OBJECT_MANAGER
#include "XObjectManager.h"
#endif
#include "XControl/XControlManager.h"

#include "XFrameWork.h"
#endif
