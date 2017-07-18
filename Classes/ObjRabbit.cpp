#include "ObjRabbit.h"
#include "GameScene.h"

USING_NS_CC;

ObjRabbit::ObjRabbit() : inUse(false), HP(5) {
	typecode = TYPECODE_RABBIT;
	objImg = Sprite::create("rabbit_normal_down.png");

	rabbitSightTri = DrawNode::create();
	rabbitSightTri->setOpacity(64);	//opacity 0~255

	addChild(rabbitSightTri);
	addChild(objImg);

}

bool ObjRabbit::init(Vec2 initPos)
{
	//member value init
	inUse = true;	//������Ʈ�� ��� ���� ������ ����

	HP = 5;

	//re set sprite position
	objImg->setPosition(initPos);
	objImg->setOpacity(255);	//opacity �ʱ�ȭ

	//init action
	//ù state�� normal
	state = dynamic_cast<StateRabbit*> (StateRabbit::rabbitNormal);
	state->initAction(this);
	
	this->scheduleUpdate();	//������Ʈ Ǯ������ �ϸ� �̰� ������Ʈ�� ������ �� ���ش�.

	return true;

}

//������Ʈ�� �Ҹ�
bool ObjRabbit::deInit()
{
	CCLOG("deinit rabbit");
	//member value init
	
	inUse = false;	//������Ʈ�� ������� �ʵ��� ����
	this->removeFromParent();

	return true;

}


void ObjRabbit::loseHP() {
	HP--;
	CCLOG("%d HP %d",objIndex,HP);
	if (HP <= 0) {
		//state ���� �� action �ʱ�ȭ
		state = dynamic_cast<StateRabbit*> (StateRabbit::rabbitDead);
		state->initAction(this);
	}
}


void ObjRabbit::updateRabbitSight(){
	//update Vec2 rabbitSight[3]

	rabbitSight[0] = objImg->getPosition();	

	if (dir == DIR_LEFT) {
		rabbitSight[1] = Vec2(rabbitSight[0].x - objImg->getBoundingBox().size.width * 1.5, rabbitSight[0].y + objImg->getBoundingBox().size.height * 1.5);
		rabbitSight[2] = Vec2(rabbitSight[0].x - objImg->getBoundingBox().size.width * 1.5, rabbitSight[0].y - objImg->getBoundingBox().size.height * 1.5);
	}
	else if (dir == DIR_RIGHT) {
		rabbitSight[1] = Vec2(rabbitSight[0].x + objImg->getBoundingBox().size.width * 1.5, rabbitSight[0].y + objImg->getBoundingBox().size.height * 1.5);
		rabbitSight[2] = Vec2(rabbitSight[0].x + objImg->getBoundingBox().size.width * 1.5, rabbitSight[0].y - objImg->getBoundingBox().size.height * 1.5);
	}
	else if (dir == DIR_UP) {
		rabbitSight[1] = Vec2(rabbitSight[0].x - objImg->getBoundingBox().size.width * 1.5, rabbitSight[0].y + objImg->getBoundingBox().size.height * 1.5);
		rabbitSight[2] = Vec2(rabbitSight[0].x + objImg->getBoundingBox().size.width * 1.5, rabbitSight[0].y + objImg->getBoundingBox().size.height * 1.5);
	}
	else if (dir == DIR_DOWN) {
		rabbitSight[1] = Vec2(rabbitSight[0].x - objImg->getBoundingBox().size.width * 1.5, rabbitSight[0].y - objImg->getBoundingBox().size.height * 1.5);
		rabbitSight[2] = Vec2(rabbitSight[0].x + objImg->getBoundingBox().size.width * 1.5, rabbitSight[0].y - objImg->getBoundingBox().size.height * 1.5);
	}

	rabbitSightTri->clear();
	rabbitSightTri->drawTriangle(rabbitSight[0], rabbitSight[1], rabbitSight[2], Color4F::YELLOW);

}

void ObjRabbit::update(float delta) {

	//�� state�� ������ Ư���� ���ǵ� transition ������ Ȯ��
	//check state transition condition
	state->checkTransitionCond(this);

	if (pausedTime > state->actionDuration) {
		objImg->stopActionByTag(0);
		pausedTime = 0;	//���� �ð� �ʱ�ȭ
		state->initAction(this);
	}

	Rect newRect;
	newRect.setRect(objImg->getBoundingBox().origin.x + moveLen.x * delta, objImg->getBoundingBox().origin.y + moveLen.y * delta, objImg->getBoundingBox().size.width, objImg->getBoundingBox().size.height);

	//check collision
	if (!GameWorld::objManager->checkMoveCollision(this, &newRect, &(moveLen * delta))) {
		//�浹 ������ ��� pausedTime ����
		pausedTime += delta;
	}

	
}
