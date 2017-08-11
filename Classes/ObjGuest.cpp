#include "ObjGuest.h"
#include "StateGuest.h"
#include "GameScene.h"
#include "Raycasting.h"

USING_NS_CC;


ObjGuest::ObjGuest() : inUse(false), HP(5), detourSequence(-1) {
	typecode = TYPECODE_PEOPLE;
	objImg = Sprite::create("img/guest_down.png");
	addChild(objImg);
	eye[0] = new Raycasting(this, 50, 0);
	eye[1] = new Raycasting(this, 50, -45);
	eye[2] = new Raycasting(this, 50, 45);

	qnodeBound = Size(objImg->getContentSize());	//충돌 뒤에 검사하므로 시야 범위랑 상관 없은ㅁ

	normalTime = 7;

}


ObjGuest::~ObjGuest() {

}

bool ObjGuest::init(Vec2 initPos) {
	//member value init
	inUse = true;	//오브젝트를 사용 중인 것으로 변경

	HP = 5;

	//re set sprite position
	objImg->setPosition(initPos);
	objImg->setOpacity(255);	//opacity 초기화

	pausedTime = 0;
	normalTime = 7;

	qnodeIndexInit();

	//init action
	//첫 state는 normal
	state = dynamic_cast<StateGuest*> (StateGuest::guestNormal);
	state->initAction(this);

	this->scheduleUpdate();

	return true;
}

bool ObjGuest::deInit() {

	//CCLOG("deinit guest");
	//member value init

	inUse = false;	//오브젝트를 사용하지 않도록 변경
	this->removeFromParentAndCleanup(true);

	return true;
}


void ObjGuest::loseHPByPlayer() {
	deadByPlayer = true;
	HP--;
}
void ObjGuest::loseHPByOther(int damage) {
	deadByPlayer = false;
	HP -= damage;
}


void ObjGuest::update(float delta) {

	//각 state가 가지는 특수한 조건도 transition 내에서 확인
	//check state transition condition
	if (state->checkTransitionCond(this) && state == StateGuest::guestNormal) {
		normalTime -= delta;
	}

}