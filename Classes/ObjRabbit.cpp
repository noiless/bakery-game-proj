#include "ObjRabbit.h"
#include "GameScene.h"

USING_NS_CC;

ObjRabbit::ObjRabbit() : inUse(false), HP(2), pausedTime(0) {
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
	inUse = true;	//오브젝트를 사용 중인 것으로 변경

	HP = 2;

	//re set sprite position
	objImg->setPosition(initPos);
	objImg->setOpacity(255);	//opacity 초기화

	//init action
	//첫 state는 normal
	state = dynamic_cast<StateRabbit*> (StateRabbit::rabbitNormal);
	state->initAction(this);
	
	this->scheduleUpdate();	//오브젝트 풀링으로 하면 이걸 오브젝트를 지정할 때 해준다.

	return true;

}

//오브젝트의 소멸
bool ObjRabbit::deInit()
{
	CCLOG("deinit rabbit");
	//member value init
	
	inUse = false;	//오브젝트를 사용하지 않도록 변경
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

	exBox.setRect(objImg->getBoundingBox().origin.x + moveLen.x * delta, objImg->getBoundingBox().origin.y + moveLen.y * delta, objImg->getBoundingBox().size.width, objImg->getBoundingBox().size.height);

	//check collision
	if (!GameWorld::objManager->checkMoveCollision(this, &exBox, &(moveLen * delta))) {
		//충돌 상태인 경우 pausedTime 증가
		pausedTime += delta;
	}

	if (pausedTime > state->actionDuration) {
		objImg->getActionManager()->removeAllActionsFromTarget(objImg);
		pausedTime = 0;	//멈춘 시간 초기화
		state->initAction(this);
	}

	//각 state가 가지는 특수한 조건도 transition 내에서 확인
	//check state transition condition
	state->checkTransitionCond(this);
	
}
