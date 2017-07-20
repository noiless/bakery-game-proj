#include "StateSquaral.h"
#include "GameScene.h"

USING_NS_CC;

//StateSquaral

StateSquaralNormal* StateSquaral::squaralNormal = new StateSquaralNormal;
StateSquaralAttack* StateSquaral::squaralAttack = new StateSquaralAttack;
StateSquaralSeed* StateSquaral::squaralSeed = new StateSquaralSeed;
StateSquaralDead* StateSquaral::squaralDead = new StateSquaralDead;

void StateSquaral::doTransition(ObjSquaral* obj, int source, int dest) {
	
	//dead 처리 가장 먼저

	if (source == STATE_SQUARAL_NORMAL && dest == STATE_SQUARAL_ATTACK) {
		
		obj->squaralSightCircle->clear();	//시야 제거
		obj->squaralSightCircle->drawSolidCircle(obj->objImg->getPosition(), obj->squaralSightRadius * 2, 30, 12, Color4F::RED);
		obj->getActionManager()->removeAllActionsFromTarget(obj->objImg);
		obj->objImg->setTexture(Director::getInstance()->getTextureCache()->addImage("squaral_attack_down.png"));	//sprite 이미지 재설정
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


//다람쥐 : 통상 상태 클래스

MoveBy* StateSquaralNormal::makeRandDir(ObjSquaral* obj) {
	int randDir = rand() % 4;	//랜덤한 방향 설정

	if (randDir == DIR_LEFT) {
		obj->dir = DIR_LEFT;	//dir 갱신
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
	
	obj->objImg->setTexture(Director::getInstance()->getTextureCache()->addImage("squaral_down.png"));	//sprite 이미지 재설정

	//state에 맞는 speed, actionDuration 설정
	obj->speed = 150;
	actionDuration = 1;

	//init random dir
	srand(time(NULL) + obj->objIndex);	//random seed 설정

	//move1 : 랜덤한 방향으로 2초간 이동	
	MoveBy *move1 = makeRandDir(obj);
	obj->moveLen = obj->setMoveLen(obj->dir, obj->speed);	//이동 방향, 속도에 따른 moveLen 초기화

	//시퀸스의 종료를 알리기 위한 callback func
	//방향이 다시 설정되어야 해서 init을 계속 불러줘야 함...
	auto callback = CallFunc::create(
		[=]
	{
		initAction(obj);

	});

	//지정한 move들을 통한 sequence 지정
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


//다람쥐 : 공격 상태 클래스

void StateSquaralAttack::initAction(ObjSquaral * obj) {
	auto callback = CallFunc::create(
		[=]
	{
		AcornAttack *newAcorn = new AcornAttack;	//나중에 오브젝트 매니저에서 풀로 관리해줄것...?
		newAcorn->init(obj);

	});

	//지정한 move들을 통한 sequence 지정
	Sequence* seq = Sequence::create(DelayTime::create(0.5) ,callback, nullptr);

	obj->objImg->runAction(RepeatForever::create(seq));

}

bool StateSquaralAttack::checkTransitionCond(ObjSquaral * obj) {

	float radVal = atan2f((obj->target->objImg->getPositionY() - obj->objImg->getPositionY()) , (obj->target->objImg->getPositionX() - obj->objImg->getPositionX()));
	obj->objImg->setRotation( - CC_RADIANS_TO_DEGREES(radVal) - 90);


	//더이상 충돌하지 않을 때 노말 상태로 전이
	if (!obj->target->objImg->getBoundingBox().intersectsCircle(obj->objImg->getPosition(), obj->squaralSightRadius * 2)) {
		doTransition(obj, STATE_SQUARAL_ATTACK, STATE_SQUARAL_NORMAL);
	}

	return false;
}


//다람쥐 : 나무 심기 클래스

void StateSquaralSeed::initAction(ObjSquaral * obj) {

}

bool StateSquaralSeed::checkTransitionCond(ObjSquaral * obj) {
	return false;
}


//다람쥐 : 사망 클래스
void StateSquaralDead::initAction(ObjSquaral * obj) {

}

bool StateSquaralDead::checkTransitionCond(ObjSquaral * obj) {
	return false;
}
