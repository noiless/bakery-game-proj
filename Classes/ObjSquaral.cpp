#include "ObjSquaral.h"
#include "GameScene.h"

#define NUM_OF_SIGHT_POLY 5


ObjSquaral::ObjSquaral() : inUse(false), HP(5), squaralSightRadius(100), speed(150) {
	typecode = TYPECODE_SQUARAL;
	objImg = Sprite::create("squaral_down.png");

	squaralSightCircle = DrawNode::create();
	squaralSightCircle->setOpacity(64);	//opacity 0~255

	addChild(squaralSightCircle);
	addChild(objImg);
}


void ObjSquaral::loseHP() {
	HP--;
}

bool ObjSquaral::init(cocos2d::Vec2 initPos) {
	objImg->setPosition(initPos);
	inUse = true;
	state = dynamic_cast<StateSquaral*> (StateSquaral::squaralNormal);
	state->initAction(this);

	this->scheduleUpdate();

	return true;
}

bool ObjSquaral::deInit() {
	return true;
}

void ObjSquaral::drawSquaralSight() {
	int sectorDir;	//현재 dir에 따라 보정...
	if (dir == DIR_RIGHT) {
		sectorDir = 0;
	}
	else if (dir == DIR_UP) {
		sectorDir = 2;
	}
	else if (dir == DIR_LEFT) {
		sectorDir = 4;
	}
	else if (dir == DIR_DOWN) {
		sectorDir = 6;
	}

	const auto angle_step = 2 * M_PI * 45 / 360.f / NUM_OF_SIGHT_POLY;
	vector<Point> circle;

	circle.emplace_back(objImg->getPosition());
	for (int i = NUM_OF_SIGHT_POLY * (sectorDir - 1); i <= NUM_OF_SIGHT_POLY * (sectorDir + 1); i++)
	{
		auto rads = angle_step * i;
		auto x = objImg->getPositionX() + squaralSightRadius * cosf(rads);
		auto y = objImg->getPositionY() + squaralSightRadius * sinf(rads);
		circle.emplace_back(x, y);
	}

	squaralSightCircle->clear();
	squaralSightCircle->drawSolidPoly(circle.data(), circle.size(), Color4F::BLUE);
}

void ObjSquaral::update(float delta) {
	
	state->checkTransitionCond(this);

	if (pausedTime > state->actionDuration) {
		objImg->stopAllActions();
		pausedTime = 0;	//멈춘 시간 초기화
		state->initAction(this);
	}

	exBox.setRect(objImg->getBoundingBox().origin.x + moveLen.x * delta, objImg->getBoundingBox().origin.y + moveLen.y * delta, objImg->getBoundingBox().size.width, objImg->getBoundingBox().size.height);

	if (!GameWorld::objManager->checkMoveCollision(this, &exBox, &(moveLen * delta))) {
		//충돌 상태인 경우 pausedTime 증가
		pausedTime += delta;
	}
}