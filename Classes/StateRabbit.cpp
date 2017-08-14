#include "StateRabbit.h"
#include "GameScene.h"
#include "ObjRabbit.h"
#include "cstdlib"

USING_NS_CC;
using namespace pugi;

////StateRabbit func
StateRabbitNormal *StateRabbit::rabbitNormal = new StateRabbitNormal;
StateRabbitRun *StateRabbit::rabbitRun = new StateRabbitRun;
StateRabbitDead *StateRabbit::rabbitDead = new StateRabbitDead;

void StateRabbit::initStates(pugi::xml_node stateNode) {
	xml_node tempNode = stateNode.child("Normal");

	rabbitNormal->actionDuration = tempNode.child("ActionDuration").text().as_int();
	rabbitNormal->stateSpeed = tempNode.child("Speed").text().as_int();

	tempNode = stateNode.child("Run");
	rabbitRun->actionDuration = tempNode.child("ActionDuration").text().as_int();
	rabbitRun->stateSpeed = tempNode.child("Speed").text().as_int();

	tempNode = stateNode.child("Dead");
	rabbitDead->actionDuration = tempNode.child("ActionDuration").text().as_int();

}

void StateRabbit::doTransition(ObjRabbit* obj, int source, int dest) {

	obj->pausedTime = 0;
	obj->getActionManager()->resumeTarget(obj->objImg);
	obj->getActionManager()->removeAllActionsFromTarget(obj->objImg);	//수행하던 action 제거

	//source에 상관 없이 사망 처리
	if (dest == STATE_RABBIT_DEAD) {
		obj->state = dynamic_cast<StateRabbit*> (StateRabbit::rabbitDead);
		obj->state->initAction(obj);
	}
	else if (source == STATE_RABBIT_NORMAL && dest == STATE_RABBIT_RUN) {
		// NORMAL->RUN 전이

		experimental::AudioEngine::play2d("sound/sound_rabbit_dead.mp3", false, 1.0f, &rabbitDeadEffect);
		obj->rabbitSightTri->clear();	//시야 삼각형 제거
		//obj->objImg->getActionManager()->removeActionByTag(0, obj->objImg);	//액션 제거
		obj->objImg->setTexture(Director::getInstance()->getTextureCache()->addImage("img/rabbit_run_down.png"));	//sprite 이미지 재설정
		obj->state = StateRabbit::rabbitRun;	//rabbitRun으로의 상태 전이
		obj->state->initAction(obj);
		
	}
	else if (source == STATE_RABBIT_RUN && dest == STATE_RABBIT_NORMAL) {
		//RUN->NORMAL 전이

		obj->objImg->setTexture(Director::getInstance()->getTextureCache()->addImage("img/rabbit_normal_down.png"));	//sprite image 변경
		obj->state = StateRabbit::rabbitNormal;	//rabbitNormal으로의 상태 전이
		obj->state->initAction(obj);

	}

}

////StateRabbitNormal Func

bool StateRabbitNormal::checkSight(ObjRabbit * obj) {
	if (GameWorld::objManager->checkSightCollision(obj)) {
		return true;
	}
	else {
		return false;
	}
}

//state init
void StateRabbitNormal::initAction(ObjRabbit* obj) {

	obj->objImg->setTexture(Director::getInstance()->getTextureCache()->addImage("img/rabbit_normal_down.png"));	//sprite 이미지 재설정

	//state에 맞는 speed, actionDuration 설정
	obj->speed = stateSpeed;

	//move1 : 랜덤한 방향으로 2초간 이동	
	MoveBy *move1 = StateRabbitNormal::makeRandDir(obj);
	obj->moveLen = obj->setMoveLen(obj->dir, obj->speed);	//이동 방향, 속도에 따른 moveLen 초기화

	//move2 : 2초간 휴식
	DelayTime *move2 = DelayTime::create(actionDuration);

	//시퀸스의 종료를 알리기 위한 callback func
	auto callback = CallFunc::create(
		[=]
	{
		initAction(obj);

	});

	//지정한 move들을 통한 sequence 지정
	Sequence* seq = Sequence::create(move1, move2, callback, nullptr);
	seq->setTag(0);	//sequence 내부의 명령은 호출할 수 없는 것 같다...

	//sequence 실행 명령
	obj->objImg->runAction(seq);

}

MoveBy* StateRabbitNormal::makeRandDir(ObjRabbit* obj) {
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

bool StateRabbitNormal::checkTransitionCond(ObjRabbit * obj) {

	obj->updateRabbitSight();

	//Dead 상태로의 조건 확인
	if (obj->HP <= 0) {
		doTransition(obj, STATE_RABBIT_NORMAL, STATE_RABBIT_DEAD);
		return true;
	}
	//Run 상태로의 전이 조건 확인
	else if (StateRabbitNormal::checkSight(obj)) {
		//Run 상태로 전이

		doTransition(obj, STATE_RABBIT_NORMAL, STATE_RABBIT_RUN);
		
		return true;
	}
	else {
		return false;
	}
}

////StateRabbitRun Func
void StateRabbitRun::initAction(ObjRabbit * obj) {

	//state에 맞는 속도 설정
	obj->speed = stateSpeed;

	//이동 방향 설정 -> 기존 이동 방향의 반대편으로
	//move1 : 기존 이동 방향의 반대편으로 2초간 이동
	MoveBy *move1;

	//좌->우 방향으로
	if (obj->dir == DIR_LEFT) {
		obj->dir = DIR_RIGHT;
		move1 = MoveBy::create(actionDuration, Vec2(obj->speed * actionDuration, 0));
		obj->objImg->setRotation(-90);
	}
	//우->좌
	else if (obj->dir == DIR_RIGHT) {
		obj->dir = DIR_LEFT;
		move1 = MoveBy::create(actionDuration, Vec2(-obj->speed * actionDuration, 0));
		obj->objImg->setRotation(90);
	}
	//상->하
	else if (obj->dir == DIR_UP) {
		obj->dir = DIR_DOWN;
		move1 = MoveBy::create(actionDuration, Vec2(0, -obj->speed * actionDuration));
		obj->objImg->setRotation(0);
	}
	//하->상
	else if (obj->dir == DIR_DOWN) {
		obj->dir = DIR_UP;
		move1 = MoveBy::create(actionDuration, Vec2(0, obj->speed * actionDuration));
		obj->objImg->setRotation(180);
	}
	obj->moveLen = obj->setMoveLen(obj->dir, obj->speed);	//이동 방향, 속도에 따른 moveLen 초기화

															//지정한 move들을 통한 sequence 지정
	Sequence* seq = Sequence::create(move1, nullptr);
	seq->setTag(0);

	//sequence 실행 명령
	obj->objImg->runAction(seq);
}

bool StateRabbitRun::checkTransitionCond(ObjRabbit * obj) {

	//Dead 상태로의 조건 확인
	if (obj->HP <= 0) {
		doTransition(obj, STATE_RABBIT_RUN, STATE_RABBIT_DEAD);
		return true;
	}
	else if (obj->objImg->getNumberOfRunningActions() == 0) {	//action이 완료되었을 때

		doTransition(obj, STATE_RABBIT_RUN, STATE_RABBIT_NORMAL);

		return true;
	}
	else {
		return false;
	}

}


////StateRabbitDead Func
void StateRabbitDead::initAction(ObjRabbit * obj) {
	obj->unscheduleUpdate(); //업데이트 중지
	
	obj->rabbitSightTri->clear();	//시야 삼각형 제거
	obj->objImg->setTexture(Director::getInstance()->getTextureCache()->addImage("img/rabbit_dead_down.png"));	//sprite image 변경
	actionDuration = 1;
	
	GameWorld::objManager->deleteObjectAvailList(obj); //ObjManager에서 avail list에서 제거해줌

	if (obj->deadByPlayer) {
		GameWorld::ui->myBreadPointGrow();
	}
	else{
		GameWorld::ui->myBreadPointDown();
	}
	//action 설정
	auto fadeout = FadeOut::create(actionDuration);

	auto callback = CallFunc::create(
		[=]
	{
		obj->deInit();
	});

	obj->objImg->runAction(Sequence::create(fadeout, callback, nullptr));
	
}

bool StateRabbitDead::checkTransitionCond(ObjRabbit * obj) {
	//사망 상태로 별다른 동작을 하지 않음
	return false;
}