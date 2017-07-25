#include "StateSquaral.h"
#include "GameScene.h"

USING_NS_CC;

//StateSquaral

StateSquaralNormal* StateSquaral::squaralNormal = new StateSquaralNormal;
StateSquaralAttack* StateSquaral::squaralAttack = new StateSquaralAttack;
StateSquaralSeed* StateSquaral::squaralSeed = new StateSquaralSeed;
StateSquaralDead* StateSquaral::squaralDead = new StateSquaralDead;

void StateSquaral::doTransition(ObjSquaral* obj, int source, int dest) {

	obj->pausedTime = 0;
	obj->getActionManager()->resumeTarget(obj->objImg);
	obj->getActionManager()->removeAllActionsFromTarget(obj->objImg);	//수행하던 action 제거
	
	//dead 처리 가장 먼저
	if (dest == STATE_SQUARAL_DEAD) {
		if (source == STATE_SQUARAL_ATTACK) {
			GameWorld::objManager->deleteObjectAvailList(StateSquaralAttack::tempSquaral);
		}

		obj->state = StateSquaral::squaralDead;
		obj->state->initAction(obj);
	}
	

	if (source == STATE_SQUARAL_NORMAL) {
		
		if (dest == STATE_SQUARAL_ATTACK) {
			obj->squaralSightCircle->clear();	//시야 제거
			obj->squaralSightCircle->drawSolidCircle(obj->objImg->getPosition(), obj->squaralSightRadius * 2, 30, 12, Color4F::RED);
			obj->objImg->setTexture(Director::getInstance()->getTextureCache()->addImage("squaral_attack_down.png"));	//sprite 이미지 재설정
			obj->state = StateSquaral::squaralAttack;
			obj->state->initAction(obj);

			//tempSquaral
			StateSquaralAttack::tempSquaral->objImg->setPosition(obj->objImg->getPosition());
			GameWorld::objManager->addObjectAvailListFRONT(StateSquaralAttack::tempSquaral);

		}

		else if (dest == STATE_SQUARAL_SEED) {

			obj->squaralSightCircle->clear();	//시야 제거
			obj->objImg->setTexture(Director::getInstance()->getTextureCache()->addImage("squaral_seed_down.png"));	//sprite 이미지 재설정
			obj->state = StateSquaral::squaralSeed;
			obj->state->initAction(obj);
		}
		
	}
	else if (source == STATE_SQUARAL_ATTACK && dest == STATE_SQUARAL_NORMAL) {
		GameWorld::objManager->deleteObjectAvailList(StateSquaralAttack::tempSquaral);

		obj->squaralSightCircle->clear();
		obj->normalTime = 0;
		obj->state = StateSquaral::squaralNormal;
		obj->state->initAction(obj);

	}
	else if (source == STATE_SQUARAL_SEED && dest == STATE_SQUARAL_NORMAL) {
		obj->normalTime = 0;
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
	obj->objImg->setOpacity(255);

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

	if (obj->HP <= 0) {
		doTransition(obj, STATE_SQUARAL_NORMAL, STATE_SQUARAL_DEAD);
		return true;
	}

	obj->drawSquaralSight();

	obj->target = GameWorld::objManager->checkSightCollision(obj);	//check sight

	if (obj->target != nullptr) {
		doTransition(obj, STATE_SQUARAL_NORMAL, STATE_SQUARAL_ATTACK);
		CCLOG("index %d", obj->target->objIndex);
	}

	//10초
	if (obj->normalTime > 10) {
		doTransition(obj, STATE_SQUARAL_NORMAL, STATE_SQUARAL_SEED);
	}

	return true;
}



//다람쥐 : 공격 상태 클래스

Obj * StateSquaralAttack::tempSquaral;

StateSquaralAttack::StateSquaralAttack() {
	squaralAttackEffect.name = "squaralAttackEffect";
	squaralAttackEffect.maxInstances = 10;
};

void StateSquaralAttack::initAction(ObjSquaral * obj) {
	//obj->moveLen = Vec2::ZERO;

	auto callback = CallFunc::create(
		[=]
	{
		experimental::AudioEngine::play2d("acorn_shot.mp3", false, 1.0f, &squaralAttackEffect);
		GameWorld::objManager->getObjAcornFromPool(obj->getParent(), obj);

	});

	//지정한 move들을 통한 sequence 지정
	Sequence* seq = Sequence::create(DelayTime::create(0.5) ,callback, nullptr);

	obj->objImg->runAction(RepeatForever::create(seq));

}

bool StateSquaralAttack::checkTransitionCond(ObjSquaral * obj) {

	if (obj->HP <= 0) {
		doTransition(obj, STATE_SQUARAL_ATTACK, STATE_SQUARAL_DEAD);
		return true;
	}

	float radVal = atan2f((obj->target->objImg->getPositionY() - obj->objImg->getPositionY()) , (obj->target->objImg->getPositionX() - obj->objImg->getPositionX()));
	obj->objImg->setRotation( - CC_RADIANS_TO_DEGREES(radVal) - 90);

	obj->moveLen = Vec2::ZERO;

	//타겟이 시야 범위에서 벗어났을때 노말 상태로 전이
	if (!obj->target->objImg->getBoundingBox().intersectsCircle(obj->objImg->getPosition(), obj->squaralSightRadius * 2)) {
		doTransition(obj, STATE_SQUARAL_ATTACK, STATE_SQUARAL_NORMAL);
	}

	return false;
}


//다람쥐 : 나무 심기 클래스

void StateSquaralSeed::initAction(ObjSquaral * obj) {

	auto callback = CallFunc::create(
		[=]
	{
		//나무 심음
		if (obj->dir == DIR_LEFT) {
			GameWorld::objManager->getObjTreeFromPool(obj->getParent(), obj->objImg->getPosition() - Vec2(obj->objImg->getContentSize().width * 2, 0));
		}
		else if (obj->dir == DIR_RIGHT) {
			GameWorld::objManager->getObjTreeFromPool(obj->getParent(), obj->objImg->getPosition() + Vec2(obj->objImg->getContentSize().width * 2, 0));
		}
		else if (obj->dir == DIR_UP) {
			GameWorld::objManager->getObjTreeFromPool(obj->getParent(), obj->objImg->getPosition() + Vec2(0, obj->objImg->getContentSize().height * 2));
		}
		else if (obj->dir == DIR_DOWN) {
			GameWorld::objManager->getObjTreeFromPool(obj->getParent(), obj->objImg->getPosition() - Vec2(0, obj->objImg->getContentSize().height * 2));
		}
		

	});

	//지정한 move들을 통한 sequence 지정
	Sequence* seq = Sequence::create(DelayTime::create(3), callback, nullptr);

	obj->objImg->runAction(seq);

}

bool StateSquaralSeed::checkTransitionCond(ObjSquaral * obj) {

	//Dead 상태로의 조건 확인
	if (obj->HP <= 0) {
		doTransition(obj, STATE_SQUARAL_SEED, STATE_SQUARAL_DEAD);
		return true;
	}
	else if (obj->objImg->getNumberOfRunningActions() == 0) {	//action이 완료되었을 때

		doTransition(obj, STATE_SQUARAL_SEED, STATE_SQUARAL_NORMAL);

		return true;
	}
	else {
		return false;
	}

}


//다람쥐 : 사망 클래스
void StateSquaralDead::initAction(ObjSquaral * obj) {
	obj->squaralSightCircle->clear();	//시야 삼각형 제거
	obj->objImg->setTexture(Director::getInstance()->getTextureCache()->addImage("squaral_dead_down.png"));	//sprite image 변경
	obj->speed = 0;
	actionDuration = 1;

	obj->unscheduleUpdate(); //업데이트 중지

	GameWorld::objManager->deleteObjectAvailList(obj); //ObjManager에서 avail list에서 제거해줌

	//상대 재료 포인트 하강
	GameWorld::ui->otherBreadPointDown();

	//action 설정
	auto fadeout = FadeOut::create(actionDuration);

	auto callback = CallFunc::create(
		[=]
	{
		obj->deInit();
	});

	obj->objImg->runAction(Sequence::create(fadeout, callback, nullptr));
}

bool StateSquaralDead::checkTransitionCond(ObjSquaral * obj) {
	//사망 상태로 별다른 행동을 하지 않음
	return false;
}
