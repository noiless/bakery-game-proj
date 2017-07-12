#include "StateRabbit.h"
#include "GameScene.h"
#include "ObjRabbit.h"
#include "cstdlib"

USING_NS_CC;

////StateRabbit func
StateRabbit::StateRabbit()
{
}


StateRabbit::~StateRabbit()
{
}

bool StateRabbit::checkSight(ObjRabbit * obj) {
	if (GameWorld::objManager->checkSightCollision(obj)) {
		return true;
	}
	else {
		return false;
	}
}

StateRabbitNormal *StateRabbit::rabbitNormal = new StateRabbitNormal;
StateRabbitRun *StateRabbit::rabbitRun = new StateRabbitRun;



////StateRabbitNormal Func

//state init
void StateRabbitNormal::initAction(ObjRabbit* obj) {

	//state에 맞는 speed, actionDuration 설정
	obj->speed = 100;
	actionDuration = 2;

	//init random dir
	srand(time(NULL) + obj->objIndex);	//random seed 설정

	//move1 : 랜덤한 방향으로 2초간 이동	
	MoveBy *move1 = StateRabbitNormal::makeRandDir(obj);
	obj->moveLen = obj->setMoveLen(obj->dir, obj->speed);	//이동 방향, 속도에 따른 moveLen 초기화

															//move2 : 2초간 휴식
	MoveBy *move2 = MoveBy::create(actionDuration, Vec2(0, 0));

	//시퀸스의 종료를 알리기 위한 callback func
	auto callback = CallFunc::create(
		[=]
	{
		initAction(obj);

	});

	////지정한 move들을 통한 sequence 지정
	Sequence* seq = Sequence::create(move1, move2, callback, nullptr);
	seq->setTag(0);	//sequence 내부의 명령은 호출할 수 없는 것 같다...

					////sequence 실행 명령
	obj->objImg->runAction(seq);

}

MoveBy* StateRabbitNormal::makeRandDir(ObjRabbit* obj) {
	int randDir = rand() % 4;	//랜덤한 방향 설정

	if (randDir == DIR_LEFT) {
		obj->dir = DIR_LEFT;	//dir 갱신
		obj->objImg->setTexture(Director::getInstance()->getTextureCache()->addImage("rabbit_normal_left.png"));	//sprite image 변경
		return MoveBy::create(actionDuration, Vec2(-actionDuration * obj->speed, 0));
	}
	else if (randDir == DIR_RIGHT) {
		obj->dir = DIR_RIGHT;
		obj->objImg->setTexture(Director::getInstance()->getTextureCache()->addImage("rabbit_normal_right.png"));
		return MoveBy::create(actionDuration, Vec2(actionDuration * obj->speed, 0));
	}
	else if (randDir == DIR_UP) {
		obj->dir = DIR_UP;
		obj->objImg->setTexture(Director::getInstance()->getTextureCache()->addImage("rabbit_normal_up.png"));
		return MoveBy::create(actionDuration, Vec2(0, actionDuration * obj->speed));
	}
	else if (randDir == DIR_DOWN) {
		obj->dir = DIR_DOWN;
		obj->objImg->setTexture(Director::getInstance()->getTextureCache()->addImage("rabbit_normal_down.png"));
		return MoveBy::create(actionDuration, Vec2(0, -actionDuration * obj->speed));
	}
	else {
		return nullptr;
	}

}

bool StateRabbitNormal::checkTransitionCond(ObjRabbit * obj) {

	obj->updateRabbitSight();

	//Run 상태로의 전이 조건 확인
	if (StateRabbit::checkSight(obj)) {
		obj->rabbitSightTri->clear();	//시야 삼각형 제거
		obj->getActionManager()->removeActionByTag(0, obj->objImg);
		obj->state = StateRabbit::rabbitRun;	//rabbitRun으로의 상태 전이
		obj->state->initAction(obj);

		return true;
	}
	else {
		return false;
	}
}

////StateRabbitRun Func
void StateRabbitRun::initAction(ObjRabbit * obj) {

	//state에 맞는 속도 설정
	obj->speed = 300;
	actionDuration = 2;

	//onjimg 설정
	obj->objImg->setTexture(CCTextureCache::sharedTextureCache()->addImage("test3.png"));

	//이동 방향 설정 -> 기존 이동 방향의 반대편으로
	//move1 : 기존 이동 방향의 반대편으로 2초간 이동
	MoveBy *move1;

	//좌->우 방향으로
	if (obj->dir == DIR_LEFT) {
		obj->dir = DIR_RIGHT;
		move1 = MoveBy::create(actionDuration, Vec2(obj->speed * actionDuration, 0));
		obj->objImg->setTexture(Director::getInstance()->getTextureCache()->addImage("rabbit_run_right.png"));	//sprite image 변경
	}
	//우->좌
	else if (obj->dir == DIR_RIGHT) {
		obj->dir = DIR_LEFT;
		move1 = MoveBy::create(actionDuration, Vec2(-obj->speed * actionDuration, 0));
		obj->objImg->setTexture(Director::getInstance()->getTextureCache()->addImage("rabbit_run_left.png"));	//sprite image 변경
	}
	//상->하
	else if (obj->dir == DIR_UP) {
		obj->dir = DIR_DOWN;
		move1 = MoveBy::create(actionDuration, Vec2(0, -obj->speed * actionDuration));
		obj->objImg->setTexture(Director::getInstance()->getTextureCache()->addImage("rabbit_run_down.png"));	//sprite image 변경
	}
	//하->상
	else if (obj->dir == DIR_DOWN) {
		obj->dir = DIR_UP;
		move1 = MoveBy::create(actionDuration, Vec2(0, obj->speed * actionDuration));
		obj->objImg->setTexture(Director::getInstance()->getTextureCache()->addImage("rabbit_run_up.png"));	//sprite image 변경
	}
	obj->moveLen = obj->setMoveLen(obj->dir, obj->speed);	//이동 방향, 속도에 따른 moveLen 초기화

															//지정한 move들을 통한 sequence 지정
	Sequence* seq = Sequence::create(move1, nullptr);
	seq->setTag(0);

	//sequence 실행 명령
	obj->objImg->runAction(seq);
}

bool StateRabbitRun::checkTransitionCond(ObjRabbit * obj) {

	if (obj->objImg->getNumberOfRunningActions() == 0) {	//action이 완료되었을 때

		obj->state = StateRabbit::rabbitNormal;	//rabbitNormal으로의 상태 전이
		obj->state->initAction(obj);
		return true;
	}
	else {
		return false;
	}

}
