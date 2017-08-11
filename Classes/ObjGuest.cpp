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

	qnodeBound = Size(objImg->getContentSize());	//�浹 �ڿ� �˻��ϹǷ� �þ� ������ ��� ������

	normalTime = 7;

}


ObjGuest::~ObjGuest() {

}

bool ObjGuest::init(Vec2 initPos) {
	//member value init
	inUse = true;	//������Ʈ�� ��� ���� ������ ����

	HP = 5;

	//re set sprite position
	objImg->setPosition(initPos);
	objImg->setOpacity(255);	//opacity �ʱ�ȭ

	pausedTime = 0;
	normalTime = 7;

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