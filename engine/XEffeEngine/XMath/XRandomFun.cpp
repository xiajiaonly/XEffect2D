#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	¼ÖÊ¤»ª(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XRandomFun.h"
namespace XE{
namespace XRand
{
	void randomMomery(void *p,int size)
	{
		if(p == NULL ||
			size <= 1) return;
		unsigned char *tmp = (unsigned char *)p;
		for(int i = 0;i < size; ++ i)
		{
			tmp[i] = random(256);
		}
	}
}
}