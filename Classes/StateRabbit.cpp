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

	//state�� �´� speed, actionDuration ����
	obj->speed = 100;
	actionDuration = 2;

	//init random dir
	srand(time(NULL) + obj->objIndex);	//random seed ����

	//move1 : ������ �������� 2�ʰ� �̵�	
	MoveBy *move1 = StateRabbitNormal::makeRandDir(obj);
	obj->moveLen = obj->setMoveLen(obj->dir, obj->speed);	//�̵� ����, �ӵ��� ���� moveLen �ʱ�ȭ

															//move2 : 2�ʰ� �޽�
	MoveBy *move2 = MoveBy::create(actionDuration, Vec2(0, 0));

	//�������� ���Ḧ �˸��� ���� callback func
	auto callback = CallFunc::create(
		[=]
	{
		initAction(obj);

	});

	////������ move���� ���� sequence ����
	Sequence* seq = Sequence::create(move1, move2, callback, nullptr);
	seq->setTag(0);	//sequence ������ ����� ȣ���� �� ���� �� ����...

					////sequence ���� ���
	obj->objImg->runAction(seq);

}

MoveBy* StateRabbitNormal::makeRandDir(ObjRabbit* obj) {
	int randDir = rand() % 4;	//������ ���� ����

	if (randDir == DIR_LEFT) {
		obj->dir = DIR_LEFT;	//dir ����
		obj->objImg->setTexture(Director::getInstance()->getTextureCache()->addImage("rabbit_normal_left.png"));	//sprite image ����
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

	//Run ���·��� ���� ���� Ȯ��
	if (StateRabbit::checkSight(obj)) {
		obj->rabbitSightTri->clear();	//�þ� �ﰢ�� ����
		obj->getActionManager()->removeActionByTag(0, obj->objImg);
		obj->state = StateRabbit::rabbitRun;	//rabbitRun������ ���� ����
		obj->state->initAction(obj);

		return true;
	}
	else {
		return false;
	}
}

////StateRabbitRun Func
void StateRabbitRun::initAction(ObjRabbit * obj) {

	//state�� �´� �ӵ� ����
	obj->speed = 300;
	actionDuration = 2;

	//onjimg ����
	obj->objImg->setTexture(CCTextureCache::sharedTextureCache()->addImage("test3.png"));

	//�̵� ���� ���� -> ���� �̵� ������ �ݴ�������
	//move1 : ���� �̵� ������ �ݴ������� 2�ʰ� �̵�
	MoveBy *move1;

	//��->�� ��������
	if (obj->dir == DIR_LEFT) {
		obj->dir = DIR_RIGHT;
		move1 = MoveBy::create(actionDuration, Vec2(obj->speed * actionDuration, 0));
		obj->objImg->setTexture(Director::getInstance()->getTextureCache()->addImage("rabbit_run_right.png"));	//sprite image ����
	}
	//��->��
	else if (obj->dir == DIR_RIGHT) {
		obj->dir = DIR_LEFT;
		move1 = MoveBy::create(actionDuration, Vec2(-obj->speed * actionDuration, 0));
		obj->objImg->setTexture(Director::getInstance()->getTextureCache()->addImage("rabbit_run_left.png"));	//sprite image ����
	}
	//��->��
	else if (obj->dir == DIR_UP) {
		obj->dir = DIR_DOWN;
		move1 = MoveBy::create(actionDuration, Vec2(0, -obj->speed * actionDuration));
		obj->objImg->setTexture(Director::getInstance()->getTextureCache()->addImage("rabbit_run_down.png"));	//sprite image ����
	}
	//��->��
	else if (obj->dir == DIR_DOWN) {
		obj->dir = DIR_UP;
		move1 = MoveBy::create(actionDuration, Vec2(0, obj->speed * actionDuration));
		obj->objImg->setTexture(Director::getInstance()->getTextureCache()->addImage("rabbit_run_up.png"));	//sprite image ����
	}
	obj->moveLen = obj->setMoveLen(obj->dir, obj->speed);	//�̵� ����, �ӵ��� ���� moveLen �ʱ�ȭ

															//������ move���� ���� sequence ����
	Sequence* seq = Sequence::create(move1, nullptr);
	seq->setTag(0);

	//sequence ���� ���
	obj->objImg->runAction(seq);
}

bool StateRabbitRun::checkTransitionCond(ObjRabbit * obj) {

	if (obj->objImg->getNumberOfRunningActions() == 0) {	//action�� �Ϸ�Ǿ��� ��

		obj->state = StateRabbit::rabbitNormal;	//rabbitNormal������ ���� ����
		obj->state->initAction(obj);
		return true;
	}
	else {
		return false;
	}

}
