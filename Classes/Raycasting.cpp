#include "Raycasting.h"
#include "GameScene.h"

USING_NS_CC;

Raycasting::~Raycasting() {
	lineSight->clear();
	lineSight->removeFromParentAndCleanup(true);
}

//인자를 통해 시야거리, 회전각 초기화
Raycasting::Raycasting(int d, int rot) {
	this->d = d; //거리 설정
	this->rot = rot;

	lineSight = DrawNode::create();
	this->addChild(lineSight);

	scheduleUpdate();
}


//충돌시 출동한 오브젝트 반환
Obj* Raycasting::doRaycast() {

	Vec2 newDir = Vec2(sin(CC_DEGREES_TO_RADIANS(callerRot + rot + 180)), cos(CC_DEGREES_TO_RADIANS(callerRot + rot + 180)));

	if (newDir.x < 0.0001f && newDir.x > -0.0001f) {
		newDir.x = 0;
	}
	if (newDir.y < 0.0001f && newDir.y > -0.0001f) {
		newDir.y = 0;
	}

	newDir.normalize();
	
	GameWorld::objManager->doRaycast(startPoint, newDir, d);

	CCLOG("%f %f", startPoint.x, startPoint.y);

	//시선 그리기
	lineSight->clear();
	lineSight->drawLine(startPoint, startPoint + d * newDir, Color4F::BLACK);

	return nullptr;
}

void Raycasting::update(float delta) {

	startPoint = caller->objImg->getPosition();	//대체 왜...

	doRaycast();

}