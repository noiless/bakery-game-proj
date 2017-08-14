#include "ObjGuest.h"
#include "StateGuest.h"
#include "GameScene.h"
#include "Raycasting.h"


USING_NS_CC;
using namespace pugi;

ObjGuest::ObjGuest(xml_node guestNode) : inUse(false), detourSequence(-1) {

	xml_node objNode = guestNode.child("Obj");

	typecode = TYPECODE_PEOPLE;
	objImg = Sprite::create("img/guest_down.png");
	MaxHP = objNode.child("HP").text().as_int();

	addChild(objImg);
	eye[0] = new Raycasting(this, objNode.child("Sight").text().as_int(), 0);
	eye[1] = new Raycasting(this, objNode.child("Sight").text().as_int(), -45);
	eye[2] = new Raycasting(this, objNode.child("Sight").text().as_int(), 45);

	qnodeBound = Size(objImg->getContentSize());	//충돌 뒤에 검사하므로 시야 범위랑 상관 없은ㅁ

	maxNormalTime = objNode.child("NormalTime").text().as_float();
	initPos = Vec2(objNode.child("InitPosX").text().as_int(), objNode.child("InitPosY").text().as_int());

	state = dynamic_cast<StateGuest*> (StateGuest::guestNormal);
	state->initStates(guestNode.child("State"));

}


ObjGuest::~ObjGuest() {

}

bool ObjGuest::init() {
	//member value init
	inUse = true;	//오브젝트를 사용 중인 것으로 변경

	HP = MaxHP;

	//re set sprite position
	objImg->setPosition(initPos);
	objImg->setOpacity(255);	//opacity 초기화

	pausedTime = 0;
	normalTime = maxNormalTime;

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