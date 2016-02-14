#include "GGameE.h"
using namespace XE;
bool GGame::init()
{
	XEE::setVSync(false);
	//TODO:初始化所有的东西，也可以开启逻辑线程
	tmpXSld.initWithoutSkin(XRect(0.0f,0.0f,256.0f,32.0f),1.0f,-1.0f);
	tmpXSld.setFont("X:%.6f");
	tmpXSld.setPosition(0.0f,16.0f);
	tmpYSld.initWithoutSkin(XRect(0.0f,0.0f,256.0f,32.0f),1.0f,-1.0f);
	tmpYSld.setFont("Y:%.6f");
	tmpYSld.setPosition(0.0f,56.0f);
	tmpZSld.initWithoutSkin(XRect(0.0f,0.0f,256.0f,32.0f),1.0f,-1.0f);
	tmpZSld.setFont("Z:%.6f");
	tmpZSld.setPosition(0.0f,96.0f);

	if(!tempSprite.init("ResourcePack/obj.png")) return 0;
	tempSprite.setPosition(100,100);
	if(!tempModel.load("ResourcePack/Girl2/peri.obj")) return 0;
	//建立数据
	v = XMem::createArrayMem<float>(150 * 150 * 4 * 3);
	n = XMem::createArrayMem<float>(150 * 150 * 4 * 3);
	t = XMem::createArrayMem<float>(150 * 150 * 4 * 2);
	index = XMem::createArrayMem<unsigned int>(150 * 150 * 4);
	//初始化数据
	for(int i = 0;i < 150;++ i)
	{
		for(int j = 0;j < 150;++ j)
		{
			n[(i * 150 + j) * 4 * 3 + 0] = 0.0f;
			n[(i * 150 + j) * 4 * 3 + 1] = 0.0f;
			n[(i * 150 + j) * 4 * 3 + 2] = 1.0f;

			t[(i * 150 + j) * 4 * 2 + 0] = 1.0f;
			t[(i * 150 + j) * 4 * 2 + 1] = 0.0f;

			v[(i * 150 + j) * 4 * 3 + 0] = i * 4 + 2.0f;
			v[(i * 150 + j) * 4 * 3 + 1] = j * 4 + -2.0f;
			v[(i * 150 + j) * 4 * 3 + 2] = 0.0f;

			n[(i * 150 + j) * 4 * 3 + 0 + 3] = 0.0f;
			n[(i * 150 + j) * 4 * 3 + 1 + 3] = 0.0f;
			n[(i * 150 + j) * 4 * 3 + 2 + 3] = 1.0f;

			t[(i * 150 + j) * 4 * 2 + 0 + 2] = 1.0f;
			t[(i * 150 + j) * 4 * 2 + 1 + 2] = 1.0f;

			v[(i * 150 + j) * 4 * 3 + 0 + 3] = i * 4 + 2.0f;
			v[(i * 150 + j) * 4 * 3 + 1 + 3] = j * 4 + 2.0f;
			v[(i * 150 + j) * 4 * 3 + 2 + 3] = 0.0f;

			n[(i * 150 + j) * 4 * 3 + 0 + 6] = 0.0f;
			n[(i * 150 + j) * 4 * 3 + 1 + 6] = 0.0f;
			n[(i * 150 + j) * 4 * 3 + 2 + 6] = 1.0f;

			t[(i * 150 + j) * 4 * 2 + 0 + 4] = 0.0f;
			t[(i * 150 + j) * 4 * 2 + 1 + 4] = 1.0f;

			v[(i * 150 + j) * 4 * 3 + 0 + 6] = i * 4 + -2.0f;
			v[(i * 150 + j) * 4 * 3 + 1 + 6] = j * 4 + 2.0f;
			v[(i * 150 + j) * 4 * 3 + 3 + 6] = 0.0f;

			n[(i * 150 + j) * 4 * 3 + 0 + 9] = 0.0f;
			n[(i * 150 + j) * 4 * 3 + 1 + 9] = 0.0f;
			n[(i * 150 + j) * 4 * 3 + 2 + 9] = 1.0f;

			t[(i * 150 + j) * 4 * 2 + 0 + 6] = 0.0f;
			t[(i * 150 + j) * 4 * 2 + 1 + 6] = 0.0f;

			v[(i * 150 + j) * 4 * 3 + 0 + 9] = i * 4 + -2.0f;
			v[(i * 150 + j) * 4 * 3 + 1 + 9] = j * 4 + -2.0f;
			v[(i * 150 + j) * 4 * 3 + 2 + 9] = 0.0f;
			index[(i * 150 + j) * 4 + 0] = (i * 150 + j) * 4;
			index[(i * 150 + j) * 4 + 1] = (i * 150 + j) * 4 + 1;
			index[(i * 150 + j) * 4 + 2] = (i * 150 + j) * 4 + 2;
			index[(i * 150 + j) * 4 + 3] = (i * 150 + j) * 4 + 3;
		}
	}
	//连接数据
	tmpVBO.init(150 * 150 * 4,v,t,n);
	return true;
}
void GGame::move(float stepTime)
{
	ctrlEventProc(tmpXSld.getControlID(),XSlider::SLD_VALUE_CHANGE);
	//遍历模型的所有点，找到离鼠标最近的点并标记出来
	float len = 100000000.0f;
	float tempLen;
	XVector3 tempV3;
	XVector2 tempV2;
	for(int i = 0;i < tempModel.getVectorSum();++ i)
	{
		tempV3 = X3DWld.worldToScreen(tempModel.getVector(i),XRect(0.0f,0.0f,1024.0f,768.0f));
		tempV2.set(tempV3.x,tempV3.y);
		tempLen = tempV2.getLengthSqure(getMousePos());
		if(tempLen < len)
		{
			len = tempLen;
			mouseVector = tempV2;
		}
	}
}
void GGame::draw()
{
	tempSprite.draw();
	XRender::drawBox(mouseVector.x,mouseVector.y,10,10);
	XRender::drawBox(getMousePos().x,getMousePos().y,10,10);
}
void GGame::ctrlEventProc(int id,int eventID)
{
	if(id != tmpXSld.getControlID()) return;
	if(eventID != XSlider::SLD_VALUE_CHANGE && eventID != XSlider::SLD_MOUSE_MOVE) return;
	float x,y;
	for(int i = 0;i < 150;++ i)
	{
		for(int j = 0;j < 150;++ j)
		{
			x = XRand::randomf(1.0f);
			y = XRand::randomf(1.0f);
			v[(i * 150 + j) * 4 * 3 + 0] = i * 4 + 2.0f + x * 100.0f;
			v[(i * 150 + j) * 4 * 3 + 1] = j * 4 + -2.0f + y * 100.0f;
			v[(i * 150 + j) * 4 * 3 + 2] = 0.0f;

			v[(i * 150 + j) * 4 * 3 + 0 + 3] = i * 4 + 2.0f + x * 100.0f;
			v[(i * 150 + j) * 4 * 3 + 1 + 3] = j * 4 + 2.0f + y * 100.0f;
			v[(i * 150 + j) * 4 * 3 + 2 + 3] = 0.0f;

			v[(i * 150 + j) * 4 * 3 + 0 + 6] = i * 4 + -2.0f + x * 100.0f;
			v[(i * 150 + j) * 4 * 3 + 1 + 6] = j * 4 + 2.0f + y * 100.0f;
			v[(i * 150 + j) * 4 * 3 + 3 + 6] = 0.0f;

			v[(i * 150 + j) * 4 * 3 + 0 + 9] = i * 4 + -2.0f + x * 100.0f;
			v[(i * 150 + j) * 4 * 3 + 1 + 9] = j * 4 + -2.0f + y * 100.0f;
			v[(i * 150 + j) * 4 * 3 + 2 + 9] = 0.0f;
		}
	}
	tmpVBO.updateDataV(150 * 150 * 4,v);
}
void GGame::draw3D(XBool withTex)
{
	glDisable(GL_TEXTURE_2D);
	XRender::drawOrigin();
//	tempModel.draw(withTex);
	angle = XMath::normalize(XVector3(tmpXSld.getCurValue(),tmpYSld.getCurValue(),tmpZSld.getCurValue()));
	XRender::drawBox(angle * 50.0f,XVector3(2.0f,2.0f,2.0f),XVector3::zero);
	angle = XMath::getAngleFromDirection(angle);
	XRender::drawCuboid(XVector3::zero,XVector3(100.0f,1.0f,1.0f),angle * RADIAN2DEGREE,XFColor::white);
	//for(int i = 0;i < 127;++ i)
	//{
	//	for(int j = 0;j < 127;++ j)
	//	{
	//		drawBox(_XVector3(i * 4,j * 4,0),_XVector3(2,2,2),_XVector3::zero,tempSprite.getTexture()->m_texture);
	//	}
	//}
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glDisable(GL_CULL_FACE);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	//glTranslatef(center.x,center.y,center.z);
	//glRotatef(angle.x,1,0,0);
	//glRotatef(angle.y,0,1,0);
	//glRotatef(angle.z,0,0,1);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,tempSprite.getTexture()->m_texture);
	glColor4fv(XFColor::white);

	tmpVBO.drawByIndex(GL_QUADS,150 * 150 * 4,GL_UNSIGNED_INT,index);
//	tmpVBO.use();
//	glDrawElements(GL_QUADS,150 * 150 * 4,GL_UNSIGNED_INT,index);	//m_index
//	tmpVBO.disuse();
	glDisable(GL_BLEND);
	glPopMatrix();
	glPopAttrib();
}
void GGame::input(const XInputEvent &event)
{
	if(event.isKeyBoardDown()
		&& event.keyValue == XKEY_S)
	{}
}
void GGame::release()
{
}