#ifndef _XODDS_DLL_H_
#define _XODDS_DLL_H_

#if defined DLL_EXPORT
#define DECLDIR __declspec(dllexport)
#else
#define DECLDIR __declspec(dllimport)
#endif

class DECLDIR _XPoint
{
public:
	int x;
	int y;
public:
	int getLength();
};

#endif