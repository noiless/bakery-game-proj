#include "Raycasting.h"
#include "GameScene.h"

USING_NS_CC;

Raycasting::~Raycasting() {
	lineSight->clear();
	lineSight->removeFromParentAndCleanup(true);
}

//인자를 통해 시야거리, 회전각 초기화
Raycasting::Raycasting(Obj* caller, int d, int rot) {
	this->caller = caller;
	this->d = d; //거리 설정
	this->rot = rot;

	//lingSight 설정
	lineSight = DrawNode::create();
	this->addChild(lineSight);

}

bool Raycasting::init() {

	anchorDiff = caller->objImg->getPosition() - getPosition();
	setPosition(caller->objImg->getPosition());

	return true;
}


//StateEnemy 등에서 Enemy의 방향이 바뀔 때마다 호출해줘야 함
void Raycasting::setDir(int callerRot) {

	dir = Vec2(sin(CC_DEGREES_TO_RADIANS(callerRot + rot + 180)), cos(CC_DEGREES_TO_RADIANS(callerRot + rot + 180)));

	if (dir.x < 0.05f && dir.x > -0.05f) {
		dir.x = 0;
	}
	if (dir.y < 0.05f && dir.y > -0.05f) {
		dir.y = 0;
	}

	dir.normalize();

}

//충돌시 출동한 오브젝트 반환
ColObj* Raycasting::doRaycast() {

	return GameWorld::objManager->doRaycast(caller->objIndex, startPoint, dir, d);
}