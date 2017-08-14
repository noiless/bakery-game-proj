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

	qnodeBound = Size(objImg->getContentSize());	//�浹 �ڿ� �˻��ϹǷ� �þ� ������ ��� ������

	maxNormalTime = objNode.child("NormalTime").text().as_float();
	initPos = Vec2(objNode.child("InitPosX").text().as_int(), objNode.child("InitPosY").text().as_int());

	state = dynamic_cast<StateGuest*> (StateGuest::guestNormal);
	state->initStates(guestNode.child("State"));

}


ObjGuest::~ObjGuest() {

}

bool ObjGuest::init() {
	//member value init
	inUse = true;	//������Ʈ�� ��� ���� ������ ����

	HP = MaxHP;

	//re set sprite position
	objImg->setPosition(initPos);
	objImg->setOpacity(255);	//opacity �ʱ�ȭ

	pausedTime = 0;
	normalTime = maxNormalTime;

	qnodeIndexInit();

	//init action
	//ù state�� normal
	state = dynamic_cast<StateGuest*> (StateGuest::guestNormal);
	state->initAction(this);

	this->scheduleUpdate();

	return true;
}

bool ObjGuest::deInit() {

	//CCLOG("deinit guest");
	//member value init

	inUse = false;	//������Ʈ�� ������� �ʵ��� ����
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

	//�� state�� ������ Ư���� ���ǵ� transition ������ Ȯ��
	//check state transition condition
	if (state->checkTransitionCond(this) && state == StateGuest::guestNormal) {
		normalTime -= delta;
	}

}