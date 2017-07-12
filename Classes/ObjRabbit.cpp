#include "ObjRabbit.h"
#include "GameScene.h"

USING_NS_CC;

ObjRabbit::ObjRabbit() : inUse(false) {
	typecode = TYPECODE_RABBIT;

	//첫 state는 normal
	state = dynamic_cast<StateRabbit*> (StateRabbit::rabbitNormal);

	objImg = Sprite::create("rabbit_normal_down.png");

	rabbitSightTri = DrawNode::create();
	rabbitSightTri->setOpacity(64);	//opacity 0~255

	addChild(rabbitSightTri);
	addChild(objImg);

}

bool ObjRabbit::init(Vec2 initPos)
{
	//member value init
	inUse = true;	//오브젝트를 사용 중인 것으로 변경

	//re set sprite position
	objImg->setPosition(initPos);

	//init action
	state->initAction(this);

	this->scheduleUpdate();	//오브젝트 풀링으로 하면 이걸 오브젝트를 지정할 때 해준다.

	return true;

}


void ObjRabbit::updateRabbitSight(){
	//update Vec2 rabbitSight[3]

	rabbitSight[0] = objImg->getPosition();	// +objImg->getAnchorPointInPoints();

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

	//각 state가 가지는 특수한 조건도 transition 내에서 확인
	//check state transition condition
	state->checkTransitionCond(this);

	if (pausedTime > state->actionDuration) {
		objImg->stopActionByTag(0);
		pausedTime = 0;	//멈춘 시간 초기화
		state->initAction(this);
	}

	Rect newRect;

	newRect.setRect(objImg->getBoundingBox().origin.x + moveLen.x * delta, objImg->getBoundingBox().origin.y + moveLen.y * delta, objImg->getBoundingBox().size.width, objImg->getBoundingBox().size.height);

	//check collision
	if (!GameWorld::objManager->checkMoveCollision(this, newRect, moveLen * delta)) {
		//충돌 상태인 경우 pausedTime 증가
		pausedTime += delta;
	}

	
}
