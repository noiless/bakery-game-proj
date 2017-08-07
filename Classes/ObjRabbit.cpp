#include "ObjRabbit.h"
#include "StateRabbit.h"
#include "GameScene.h"

USING_NS_CC;

ObjRabbit::ObjRabbit() : inUse(false), HP(2) {
	typecode = TYPECODE_RABBIT;
	objImg = Sprite::create("img/rabbit_normal_down.png");

	rabbitSightTri = DrawNode::create();
	rabbitSightTri->setOpacity(64);	//opacity 0~255

	addChild(rabbitSightTri);
	addChild(objImg);

}

ObjRabbit::~ObjRabbit() {
	rabbitSightTri->removeFromParentAndCleanup(true);
}

bool ObjRabbit::init(Vec2 initPos)
{
	//member value init
	inUse = true;	//������Ʈ�� ��� ���� ������ ����

	HP = 2;
	
	pausedTime = 0;

	qnodeIndexInit();

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
	//CCLOG("deinit rabbit");
	//member value init
	
	inUse = false;	//������Ʈ�� ������� �ʵ��� ����
	this->removeFromParentAndCleanup(true);

	return true;

}


void ObjRabbit::loseHP() {
	HP--;
	CCLOG("%d HP %d",objIndex,HP);
}


void ObjRabbit::updateRabbitSight(){
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

	//���� �ð� �̻� �����־��� �� �׼� �ʱ�ȭ
	if (pausedTime > state->actionDuration) {
		objImg->getActionManager()->removeAllActionsFromTarget(objImg);
		objImg->getActionManager()->resumeTarget(objImg);
		pausedTime = 0;	//���� �ð� �ʱ�ȭ
		state->initAction(this);
	}

	//�� state�� ������ Ư���� ���ǵ� transition ������ Ȯ��
	//check state transition condition
	state->checkTransitionCond(this);

	
}
