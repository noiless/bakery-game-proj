#include "ObjSquaral.h"
#include "GameScene.h"

#define NUM_OF_SIGHT_POLY 5


ObjSquaral::ObjSquaral() : inUse(false), HP(5), squaralSightRadius(100), speed(150) {
	typecode = TYPECODE_SQUARAL;
	normalTime = 0;
	objImg = Sprite::create("img/squaral_down.png");

	squaralSightCircle = DrawNode::create();
	squaralSightCircle->setOpacity(64);	//opacity 0~255

	addChild(squaralSightCircle);
	addChild(objImg);
}

ObjSquaral::~ObjSquaral() {
	squaralSightCircle->removeFromParentAndCleanup(true);
}

void ObjSquaral::loseHPByPlayer() {
	HP--;
	deadByPlayer = true;
}

void ObjSquaral::loseHPByOther(int damage) {
	HP -= damage;
	deadByPlayer = false;
}


bool ObjSquaral::init(cocos2d::Vec2 initPos) {
	HP = 5;
	objImg->setPosition(initPos);
	inUse = true;
	normalTime = 0;
	pausedTime = 0;

	qnodeIndexInit();

	state = dynamic_cast<StateSquaral*> (StateSquaral::squaralNormal);
	state->initAction(this);


	this->scheduleUpdate();

	return true;
}

bool ObjSquaral::deInit() {
	//CCLOG("deinit squaral..");
	//member value init

	inUse = false;	//오브젝트를 사용하지 않도록 변경
	this->removeFromParentAndCleanup(true);

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

	normalTime += delta;

	state->checkTransitionCond(this);

	if (pausedTime > state->actionDuration) {
		objImg->getActionManager()->removeAllActionsFromTarget(objImg);
		objImg->getActionManager()->resumeTarget(objImg);
		pausedTime = 0;	//멈춘 시간 초기화
		state->initAction(this);
	}
}