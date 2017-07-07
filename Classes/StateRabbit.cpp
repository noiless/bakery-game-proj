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
	srand(8);	//seed ����
	int randDir = rand() % 4;	//������ ���� ���� <- �ٵ� �̰� �� ó������ �����ǰ� ���������� �Ҹ��°� �ƴ϶� ������ �ƴ�
	//RepearForever�� �ƴϰ� sequence ���� ������ ���� �Լ� ȣ���ؼ� �����ϰ� �־��ְ� �ؾߵ� �� ��������������?��?

	//������ �������� 2�ʰ� �̵�
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
	
	//2�ʰ� �޽�
	MoveBy *move2 = MoveBy::create(2, Vec2(0, 0));

	Sequence* seq = Sequence::create(move1, move2, nullptr);

	obj->objImg->runAction(RepeatForever::create(seq));
	
}

////StateRabbitRun Func
void StateRabbitRun::doAction() {

}

void StateRabbitRun::initAction(Obj * obj) {

}