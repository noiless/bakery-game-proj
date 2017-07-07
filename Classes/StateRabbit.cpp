#include "StateRabbit.h"
#include "cstdlib"

USING_NS_CC;

////StateRabbit func
StateRabbit::StateRabbit()
{
}


StateRabbit::~StateRabbit()
{
}

StateRabbitNormal StateRabbit::rabbitNormal;
StateRabbitRun StateRabbit::rabbitRun;



////StateRabbitNormal Func
void StateRabbitNormal::doAction() {

}


void StateRabbitNormal::initAction(Obj* obj) {

	//init random dir
	srand(8);	//seed 설정
	int randDir = rand() % 4;	//랜덤한 방향 설정 <- 근데 이게 맨 처음에만 설정되고 지속적으로 불리는게 아니라 랜덤이 아님
	//RepearForever이 아니고 sequence 끝날 때마다 새로 함수 호출해서 랜덤하게 넣어주고 해야될 것 같은데ㅋㅋㅋㅋ?ㅁ?

	//랜덤한 방향으로 2초간 이동
	MoveBy *move1;
	
	if (randDir == 0) {
		move1 = MoveBy::create(2, Vec2(-2 *  obj->speed,0));
	}
	else if (randDir == 1) {
		move1 = MoveBy::create(2, Vec2(2 * obj->speed, 0));
	}
	else if (randDir == 2) {
		move1 = MoveBy::create(2, Vec2(0, 2 * obj->speed));
	}
	else if (randDir == 3) {
		move1 = MoveBy::create(2, Vec2(0, -2 * obj->speed));
	}
	
	//2초간 휴식
	MoveBy *move2 = MoveBy::create(2, Vec2(0, 0));

	Sequence* seq = Sequence::create(move1, move2, nullptr);

	obj->objImg->runAction(RepeatForever::create(seq));
	
}

////StateRabbitRun Func
void StateRabbitRun::doAction() {

}

void StateRabbitRun::initAction(Obj * obj) {

}