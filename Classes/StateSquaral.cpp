#include "StateSquaral.h"
#include "GameScene.h"

USING_NS_CC;

//StateSquaral

StateSquaralNormal* StateSquaral::squaralNormal = new StateSquaralNormal;
StateSquaralAttack* StateSquaral::squaralAttack = new StateSquaralAttack;
StateSquaralSeed* StateSquaral::squaralSeed = new StateSquaralSeed;
StateSquaralDead* StateSquaral::squaralDead = new StateSquaralDead;

void StateSquaral::doTransition(ObjSquaral* obj, int source, int dest) {
	
	//dead ó�� ���� ����

	if (source == STATE_SQUARAL_NORMAL && dest == STATE_SQUARAL_ATTACK) {
		
		obj->squaralSightCircle->clear();	//�þ� ����
		obj->squaralSightCircle->drawSolidCircle(obj->objImg->getPosition(), obj->squaralSightRadius * 2, 30, 12, Color4F::RED);
		obj->getActionManager()->removeAllActionsFromTarget(obj->objImg);
		obj->objImg->setTexture(Director::getInstance()->getTextureCache()->addImage("squaral_attack_down.png"));	//sprite �̹��� �缳��
		obj->state = StateSquaral::squaralAttack;
		obj->state->initAction(obj);
	}
	else if (source == STATE_SQUARAL_ATTACK && dest == STATE_SQUARAL_NORMAL) {
		obj->squaralSightCircle->clear();
		obj->getActionManager()->removeAllActionsFromTarget(obj->objImg);
		obj->state = StateSquaral::squaralNormal;
		obj->state->initAction(obj);
	}


}


//�ٶ��� : ��� ���� Ŭ����

MoveBy* StateSquaralNormal::makeRandDir(ObjSquaral* obj) {
	int randDir = rand() % 4;	//������ ���� ����

	if (randDir == DIR_LEFT) {
		obj->dir = DIR_LEFT;	//dir ����
		obj->objImg->setRotation(90);

		return MoveBy::create(actionDuration, Vec2(-actionDuration * obj->speed, 0));
	}
	else if (randDir == DIR_RIGHT) {
		obj->dir = DIR_RIGHT;
		obj->objImg->setRotation(-90);
		return MoveBy::create(actionDuration, Vec2(actionDuration * obj->speed, 0));
	}
	else if (randDir == DIR_UP) {
		obj->dir = DIR_UP;
		obj->objImg->setRotation(180);
		return MoveBy::create(actionDuration, Vec2(0, actionDuration * obj->speed));
	}
	else if (randDir == DIR_DOWN) {
		obj->dir = DIR_DOWN;
		obj->objImg->setRotation(0);
		return MoveBy::create(actionDuration, Vec2(0, -actionDuration * obj->speed));
	}
	else {
		return nullptr;
	}
}

void StateSquaralNormal::initAction(ObjSquaral * obj) {
	
	obj->objImg->setTexture(Director::getInstance()->getTextureCache()->addImage("squaral_down.png"));	//sprite �̹��� �缳��

	//state�� �´� speed, actionDuration ����
	obj->speed = 150;
	actionDuration = 1;

	//init random dir
	srand(time(NULL) + obj->objIndex);	//random seed ����

	//move1 : ������ �������� 2�ʰ� �̵�	
	MoveBy *move1 = makeRandDir(obj);
	obj->moveLen = obj->setMoveLen(obj->dir, obj->speed);	//�̵� ����, �ӵ��� ���� moveLen �ʱ�ȭ

	//�������� ���Ḧ �˸��� ���� callback func
	//������ �ٽ� �����Ǿ�� �ؼ� init�� ��� �ҷ���� ��...
	auto callback = CallFunc::create(
		[=]
	{
		initAction(obj);

	});

	//������ move���� ���� sequence ����
	Sequence* seq = Sequence::create(move1, callback, nullptr);

	obj->objImg->runAction(seq);
}

bool StateSquaralNormal::checkTransitionCond(ObjSquaral * obj) {

	obj->drawSquaralSight();

	obj->target = GameWorld::objManager->checkSightCollision(obj);

	if (obj->target != nullptr) {
		doTransition(obj, STATE_SQUARAL_NORMAL, STATE_SQUARAL_ATTACK);
		CCLOG("index %d", obj->target->objIndex);
	}

	return true;
}

bool StateSquaralNormal::checkSight(ObjSquaral * obj) {
	return false;

}


//�ٶ��� : ���� ���� Ŭ����

void StateSquaralAttack::initAction(ObjSquaral * obj) {
	auto callback = CallFunc::create(
		[=]
	{
		AcornAttack *newAcorn = new AcornAttack;	//���߿� ������Ʈ �Ŵ������� Ǯ�� �������ٰ�...?
		newAcorn->init(obj);

	});

	//������ move���� ���� sequence ����
	Sequence* seq = Sequence::create(DelayTime::create(0.5) ,callback, nullptr);

	obj->objImg->runAction(RepeatForever::create(seq));

}

bool StateSquaralAttack::checkTransitionCond(ObjSquaral * obj) {

	float radVal = atan2f((obj->target->objImg->getPositionY() - obj->objImg->getPositionY()) , (obj->target->objImg->getPositionX() - obj->objImg->getPositionX()));
	obj->objImg->setRotation( - CC_RADIANS_TO_DEGREES(radVal) - 90);


	//���̻� �浹���� ���� �� �븻 ���·� ����
	if (!obj->target->objImg->getBoundingBox().intersectsCircle(obj->objImg->getPosition(), obj->squaralSightRadius * 2)) {
		doTransition(obj, STATE_SQUARAL_ATTACK, STATE_SQUARAL_NORMAL);
	}

	return false;
}


//�ٶ��� : ���� �ɱ� Ŭ����

void StateSquaralSeed::initAction(ObjSquaral * obj) {

}

bool StateSquaralSeed::checkTransitionCond(ObjSquaral * obj) {
	return false;
}


//�ٶ��� : ��� Ŭ����
void StateSquaralDead::initAction(ObjSquaral * obj) {

}

bool StateSquaralDead::checkTransitionCond(ObjSquaral * obj) {
	return false;
}
