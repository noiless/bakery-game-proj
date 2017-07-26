#include "ObjGuest.h"
#include "GameScene.h"

USING_NS_CC;


ObjGuest::ObjGuest() : inUse(false), HP(5), pausedTime(0) {
	typecode = TYPECODE_PEOPLE;
	objImg = Sprite::create("img/guest_down.png");
	addChild(objImg);
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

	//init action
	//ù state�� normal
	state = dynamic_cast<StateGuest*> (StateGuest::guestNormal);
	state->initAction(this);

	this->scheduleUpdate();

	return true;
}

bool ObjGuest::deInit() {

	CCLOG("deinit guest");
	//member value init

	inUse = false;	//������Ʈ�� ������� �ʵ��� ����
	this->removeFromParentAndCleanup(true);

	return true;
}

void ObjGuest::loseHP() {
	HP--;
}

void ObjGuest::update(float delta) {
	exBox.setRect(objImg->getBoundingBox().origin.x + moveLen.x * delta, objImg->getBoundingBox().origin.y + moveLen.y * delta, objImg->getBoundingBox().size.width, objImg->getBoundingBox().size.height);

	//check collision
	if (!GameWorld::objManager->checkMoveCollision(this, &exBox, &(moveLen * delta))) {
		//�浹 ������ ��� pausedTime ����
		pausedTime += delta;
	}

	//if (pausedTime > state->actionDuration) {
	//	objImg->getActionManager()->removeAllActionsFromTarget(objImg);	//��� ������ �����̶� �̰� ���� ���ŷ� �ؾ���
	//	pausedTime = 0;	//���� �ð� �ʱ�ȭ
	//	state->initAction(this);
	//}

	//�� state�� ������ Ư���� ���ǵ� transition ������ Ȯ��
	//check state transition condition
	state->checkTransitionCond(this);
}