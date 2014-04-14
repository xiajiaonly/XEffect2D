//++++++++++++++++++++++++++++++++
//Author:	¼ÖÊ¤»ª(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XRandomFun.h"
#include "XBasicFun.h"
void randomMomery(char *p,int size)
{
	if(p == NULL) return;
	if(size <= 1) return;
	int order;
	char temp;
	for(int i = 0;i < size; ++ i)
	{
		order = random(size);
		if(order >= 0 && order < size && order != i)
		{
			temp = p[i];
			p[i] = p[order];
			p[order] = temp;
		}
	}
}