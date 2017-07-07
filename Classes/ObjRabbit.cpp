#include "ObjRabbit.h"
#include "GameScene.h"

USING_NS_CC;

bool ObjRabbit::init()
{
	//member value init
	speed = 200;
	
	//objImg = new Sprite;

	//objImg = Sprite::create("test2.png");
	/*objImg->setPosition(100, 100);	
	auto s = Sequence::create(MoveBy::create(10, Vec2(speed * 10, 0)), nullptr);
	objImg->runAction(s);*/
	

	//this->addChild(objImg);

	//sprite init
	objImg = Sprite::create("test2.png");
	objImg->setPosition(100, 100);

	moveLen = Vec2(speed,0);

	state = dynamic_cast<StateRabbit*> (&StateRabbit::rabbitNormal);

	state->initAction(this);

	this->scheduleUpdate();

	return true;
}

void ObjRabbit::update(float delta) {
	Rect newRect;
	Vec2 u_moveLen = moveLen * delta;

	newRect.setRect(objImg->getBoundingBox().origin.x+ u_moveLen.x, objImg->getBoundingBox().origin.y + u_moveLen.y, objImg->getBoundingBox().size.width, objImg->getBoundingBox().size.height);

	GameWorld::objManager->checkCollision(this, newRect);

}
