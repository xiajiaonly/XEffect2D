//********************************************************************************
//下面这个例子是用于测试刚性球的例子
//说明，由于重力引入的问题，造成目前进度只到这里，接下来考虑box2D的引入取代这一部分
//program by 贾胜华
//********************************************************************************

#include "XEffeEngine.h"
#include "XLiquidBall.h"

//下面是事件处理函数，例如：键盘事件或者鼠标事件
int SDL_event(SDL_Surface *screen);

_XLiquidBalls LiquidBalls;
int stepEnter = 0;

int main(int argc, char **argv)
{
	Uint32 interval;	//两次调用之间的时间差			
	SDL_Surface *screen = NULL;	//窗口绘图面
	int quit = 0;
	//建立窗体
	screen = initWindow(1024,768,"Xiajia");	
	if(screen == NULL)
	{
		printf("Window init error!\n");
		return 0;
	}

	//complex<double>  x1,x2,x3,x4;
	////quarticEquation(0,1,1,1,-3,x1,x2,x3,x4);
	//quarticEquation(1,0,0,0,1,x1,x2,x3,x4);
	////quarticEquation(1,0,1,0,0,x1,x2,x3,x4);
	////quarticEquation(1,1,1,1,-4,x1,x2,x3,x4);
	////quartic_equation(1,1,1,1,-4,x1,x2,x3,x4);

	//std::cout<<" x1="<< x1 <<endl;
	//std::cout<<" x2="<< x2 <<endl;
	//std::cout<<" x3="<< x3 <<endl;
	//std::cout<<" x4="<< x4 <<endl;
	//system("pause");

	_XSprite Sprite;
	Sprite.init("pic/GC.png");
	Sprite.setSize(0.5f,0.5f);
	LiquidBalls.init(128,128);
	LiquidBalls.setEnable(1);
	LiquidBalls.addOneLine(_XVector2(  50.0f,  50.0f),_XVector2(1000.0f,  50.0f));
	LiquidBalls.addOneLine(_XVector2(  50.0f, 700.0f),_XVector2(1000.0f, 700.0f));
	LiquidBalls.addOneLine(_XVector2(  50.0f,  50.0f),_XVector2(  50.0f, 700.0f));
	LiquidBalls.addOneLine(_XVector2(1000.0f,  50.0f),_XVector2(1000.0f, 700.0f));
	//LiquidBalls.addOneLine(_XVector2( 300.0f, 600.0f),_XVector2( 800.0f, 100.0f));
	LiquidBalls.addOneLine(_XVector2( 300.0f, 300.0f),_XVector2( 500.0f, 300.0f));
	LiquidBalls.addOneLine(_XVector2( 300.0f, 300.0f),_XVector2( 300.0f, 500.0f));
	LiquidBalls.addOneLine(_XVector2( 300.0f, 500.0f),_XVector2( 500.0f, 500.0f));
	LiquidBalls.addOneLine(_XVector2( 500.0f, 300.0f),_XVector2( 500.0f, 500.0f));
	//LiquidBalls.addOneLine(_XVector2( 300.0f, 600.0f),_XVector2( 800.0f, 100.0f));
	LiquidBalls.addOneLine(_XVector2( 600.0f, 400.0f),_XVector2( 700.0f, 300.0f));
	LiquidBalls.addOneLine(_XVector2( 600.0f, 400.0f),_XVector2( 700.0f, 500.0f));
	LiquidBalls.addOneLine(_XVector2( 700.0f, 300.0f),_XVector2( 800.0f, 400.0f));
	LiquidBalls.addOneLine(_XVector2( 700.0f, 500.0f),_XVector2( 800.0f, 400.0f));
	//测试7
	//LiquidBalls.addOneBall(_XVector2(500.0f + 34.0f,300.0f),32.0f,1000.0f,_XVector2(-500,2.0f),0.05f,0.0f,64.0f);
	//测试6
	//LiquidBalls.addOneBall(_XVector2(400.0f,100.0f),32.0f,1000.0f,_XVector2(0,0),0.05f,0.0f,64.0f);
	//测试5
	//LiquidBalls.addOneBall(_XVector2(300.0f,100.0f),32.0f,1000.0f,_XVector2(0,0),0.05f,0.0f,64.0f);
	//LiquidBalls.addOneBall(_XVector2(332.0f,100.0f),32.0f,1000.0f,_XVector2(0,0),0.05f,0.0f,64.0f);
	//测试2
	//LiquidBalls.addOneBall(_XVector2(300.0f,100.0f),32.0f,1000.0f,_XVector2(0,0),0.05f,0.0f,64.0f);
	//LiquidBalls.addOneBall(_XVector2(365.0f,100.0f),32.0f,1000.0f,_XVector2(0,0),0.05f,0.0f,64.0f);
	//LiquidBalls.addOneBall(_XVector2(430.0f,100.0f),32.0f,1000.0f,_XVector2(0,0),0.05f,0.0f,64.0f);
	//LiquidBalls.addOneBall(_XVector2(495.0f,100.0f),32.0f,1000.0f,_XVector2(0,0),0.05f,0.0f,64.0f);
	//LiquidBalls.addOneBall(_XVector2(560.0f,100.0f),32.0f,1000.0f,_XVector2(0,0),0.05f,0.0f,64.0f);
	//测试4
	//LiquidBalls.addOneBall(_XVector2(300.0f,100.0f),32.0f,1000.0f,_XVector2(0,0),0.05f,0.0f,64.0f);
	//LiquidBalls.addOneBall(_XVector2(300.0f,166.0f),32.0f,1000.0f,_XVector2(0,0),0.05f,0.0f,64.0f);
	//测试3
	//LiquidBalls.addOneBall(_XVector2(300.0f,100.0f),32.0f,1000.0f,_XVector2(0,0),0.05f,0.0f,64.0f);
	//下面是一个技术测试
	//_XVector2 L1S(100,500);
	//_XVector2 L1E(1000,500);
	////float tempXX = L1S.getAngle(L1E) / PI * 180.0f;
	//_XVector2 L2S(500,100);
	//_XVector2 L2E(500,700);
	//_XVector2 p1(0,0);
	//_XVector2 p2(0,0);
	//float t = getCrashTimerB2B(L1S,_XVector2(10.0f,0.0f),_XVector2(2.0f,0.0f),L2S,_XVector2(0.0f,10.0f),_XVector2(0.0f,2.0f),20,100);
	////t = getTwoLineTime(L1S,L1E,L2S,L2E,20,100);
	//if(t >= 0)
	//{
	//	p1.set(L1S.x + 10.0f * t + 0.5f * 2 * t * t,500);
	//	p2.set(500,L2S.y + 10.0f * t + 0.5f * 2 * t * t);
	////	p1.set(L1S.x + (L1E.x - L1S.x) * t / 100.0f,L1S.y + (L1E.y - L1S.y) * t / 100.0f);
	////	p2.set(L2S.x + (L2E.x - L2S.x) * t / 100.0f,L2S.y + (L2E.y - L2S.y) * t / 100.0f);
	//}

	while (!quit) 
	{
		quit = SDL_event(screen);
		interval = getFrameTiming();
		if(stepEnter == 1)
		{
			//stepEnter = 0;
			LiquidBalls.move(interval * 10);
		}

		//清除屏幕
		glClear(GL_COLOR_BUFFER_BIT);

		if(LiquidBalls.getIsEnable() != 0) 
		{
			for(int i = 0;i < LiquidBalls.m_liquidBallSum;++ i)
			{
				if(LiquidBalls.getBallIsEnable(i) != 0)
				{
					Sprite.setSize(LiquidBalls.m_pLiquidBall[i].m_radius / 32.0f,LiquidBalls.m_pLiquidBall[i].m_radius / 32.0f);
					Sprite.setPosition(LiquidBalls.getBallPosition(i).x - 32.0f,LiquidBalls.getBallPosition(i).y - 32.0f);
					Sprite.draw();
				}
			}
		}
		//在区域画上线
		if(LiquidBalls.getIsEnable() != 0) 
		{
			for(int i = 0;i < LiquidBalls.m_lineSum;++ i)
			{
				if(LiquidBalls.m_pLine[i].getIsEnable() != 0)
				{
					drawLine(LiquidBalls.m_pLine[i].getStartPosition().x,LiquidBalls.m_pLine[i].getStartPosition().y,
						LiquidBalls.m_pLine[i].getEndPosition().x,LiquidBalls.m_pLine[i].getEndPosition().y,1);
				}
			}
		}
		//drawLine(L1S.x,L1S.y,L1E.x,L1E.y,1);
		//drawLine(L2S.x,L2S.y,L2E.x,L2E.y,1);
		//drawLine(p1.x,p1.y,p2.x,p2.y,1,1,0,0,1);

		glFlush();
		SDL_GL_SwapBuffers();
		
		SDL_Delay(1);
	}
	SDL_Quit();
	return 0;	
}
   
int SDL_event(SDL_Surface *screen)
{
	int flag =0;
	SDL_Event event;		//SDL事件句柄
	float temp = 0;
	static int ballSum = 0;

	while(SDL_PollEvent(&event)) 
	{
		switch(event.type)
		{
		case SDL_QUIT:
			flag = 1;
			break;
		case SDL_KEYUP:
			break;
		case SDL_KEYDOWN:
			switch(event.key.keysym.sym )
			{
			case SDLK_s:
				ballSum ++;
				printf("ball Sum:%d\n",ballSum);
				temp = random(50) / 100.0f + 0.5;
				//LiquidBalls.addOneBall(_XVector2(100.0f,400.0f),32.0f,1000.0f,_XVector2(10.0f,-15.0f),0.05f,0.0f,64.0f);
				//LiquidBalls.addOneBall(_XVector2(100.0f,100.0f),32.0f * temp,1000.0f * temp * temp,_XVector2(30.0f * temp,30.0f * temp),0.05f,0.0f,64.0f);
				LiquidBalls.addOneBall(_XVector2(100.0f,100.0f),16.0f * temp,1000.0f * temp * temp,_XVector2(30.0f * temp,30.0f * temp),0.05f,0.0f,64.0f);
				//LiquidBalls.addOneBall(_XVector2(784.957458,522.088379),14.56,1000.0f * temp * temp,_XVector2(14.407974,92.833084),0.05f,0.0f,64.0f);
				//测试1
				//LiquidBalls.addOneBall(_XVector2(450.0f,150.0f),32.0f,1000.0f,_XVector2(10,10),0.05f,0.0f,64.0f);		//存在问题1(发生同时碰撞时会存在问题)	已经解决，是由于标记错误造成
				//LiquidBalls.addOneBall(_XVector2(200.0f,200.0f),32.0f,1000.0f,_XVector2(-10,-10),0.05f,0.0f,64.0f);	//存在问题1(发生同时碰撞时会存在问题)	已经解决，是由于标记错误造成
				//LiquidBalls.addOneBall(_XVector2(250.0f,500.0f),32.0f,1000.0f,_XVector2(-10,10),0.05f,0.0f,64.0f);	//存在问题1(发生同时碰撞时会存在问题)	已经解决，是由于标记错误造成
				//LiquidBalls.addOneBall(_XVector2(800.0f,250.0f),32.0f,1000.0f,_XVector2(10,-10),0.05f,0.0f,64.0f);	//存在问题1(发生同时碰撞时会存在问题)	已经解决，是由于标记错误造成
				//测试2
				//LiquidBalls.addOneBall(_XVector2(900.0f,100.0f),32.0f,1000.0f,_XVector2(-20,0),0.05f,0.0f,64.0f);	//存在问题2(多次碰撞之后的累计误差)
				//测试4
				//LiquidBalls.addOneBall(_XVector2(100.0f,133.0f),32.0f,1000.0f,_XVector2(30,0),0.05f,0.0f,64.0f);	//有时候会存在问题1
				//测试3
				//LiquidBalls.addOneBall(_XVector2(100.0f,100.0f),32.0f,1000.0f,_XVector2(10,0),0.05f,0.0f,64.0f);
				//LiquidBalls.addOneBall(_XVector2(500.0f,100.0f),32.0f,1000.0f,_XVector2(-10,0),0.05f,0.0f,64.0f);
				break;
			case SDLK_r:
				if(LiquidBalls.m_haveError != 0) LiquidBalls.m_haveError = 0;
				break;
			case SDLK_d:
				if(stepEnter == 0) stepEnter = 1;
				else stepEnter = 0;
				break;
			case SDLK_F1:
				SDL_WM_ToggleFullScreen(screen);
				break;
			}
			break;
		}
	}
	return flag;
}
/*
说明：
d1x = L1x1 - L1x2;	直线1的x增量
d1y = L1y1 - L1y2;	直线1的y增量

d2x = L2x1 - L2x2;	直线2的x增量
d2y = L2y1 - L2y2;	直线2的y增量

ptx1 = L1x1 + d1x * t/T;	t时刻的L1，x位置
pty1 = L1y1 + d1y * t/T;	t时刻的L1，y位置

ptx2 = L2x1 + d2x * t/T;	t时刻的L2，x位置
pty2 = L2y1 + d2y * t/T;	t时刻的L2，y位置

dt = sqrt((ptx1 - ptx2)^2 + (pty1 - pty2)^2);
求t = 什么的时候dt最小，或者是dt = K的时候的t的值

对于第二种问题的解

K^2 = (ptx1 - ptx2)^2 + (pty1 - pty2)^2
//带入
K^2 = (L1x1 + d1x * t/T - (L2x1 + d2x * t/T))^2 + (L1y1 + d1y * t/T - (L2y1 + d2y * t/T))^2
//化简
K^2 = (L1x1 - L2x1 + (d1x - d2x) * t/T)^2 + (L1y1 - L2y1 + (d1y - d2y) * t/T)^2
//再次带入
K^2 = (L1x1 - L2x1 + (L1x1 - L1x2 - (L2x1 - L2x2)) * t/T)^2 + (L1y1 - L2y1 + (L1y1 - L1y2 - (L2y1 - L2y2)) * t/T)^2
//再次化简
K^2 = (X1 + X2 * t)^2 + (Y1 + Y2 *t)^2;
K^2 = X1^2 + 2 * X1 * X2 * t + X2^2 * t^2 + Y1^2 + 2 * Y1 * Y2 * t + Y2^2 * t^2;
//再次化简
a*t^2 + b*t + c = 0;在0 - T的区间内是否有解，以及最小的解是什么
x=[-b ± (b^2 - 4ac) ^ (1/2)] / 2a

遍历所有的物体寻找第一次碰撞的物体并处理
判断是否已经时间执行完成，如果完成则退出
如果没有完成则继续迭代
*/