#include "StateRabbit.h"
#include "GameScene.h"
#include "ObjRabbit.h"
#include "cstdlib"

USING_NS_CC;

////StateRabbit func
StateRabbitNormal *StateRabbit::rabbitNormal = new StateRabbitNormal;
StateRabbitRun *StateRabbit::rabbitRun = new StateRabbitRun;
StateRabbitDead *StateRabbit::rabbitDead = new StateRabbitDead;

void StateRabbit::doTransition(ObjRabbit* obj, int source, int dest) {

	//source�� ��� ���� ��� ó��
	if (dest == STATE_RABBIT_DEAD) {

		obj->state = dynamic_cast<StateRabbit*> (StateRabbit::rabbitDead);
		obj->state->initAction(obj);
	}
	else if (source == STATE_RABBIT_NORMAL && dest == STATE_RABBIT_RUN) {
		// NORMAL->RUN ����

		experimental::AudioEngine::play2d("sound_rabbit_dead.mp3", false, 1.0f, &rabbitDeadEffect);
		obj->rabbitSightTri->clear();	//�þ� �ﰢ�� ����
		obj->objImg->getActionManager()->removeActionByTag(0, obj->objImg);	//�׼� ����
		obj->objImg->setTexture(Director::getInstance()->getTextureCache()->addImage("rabbit_run_down.png"));	//sprite �̹��� �缳��
		obj->state = StateRabbit::rabbitRun;	//rabbitRun������ ���� ����
		obj->state->initAction(obj);
		
	}
	else if (source == STATE_RABBIT_RUN && dest == STATE_RABBIT_NORMAL) {
		//RUN->NORMAL ����

		obj->objImg->setTexture(Director::getInstance()->getTextureCache()->addImage("rabbit_normal_down.png"));	//sprite image ����
		obj->state = StateRabbit::rabbitNormal;	//rabbitNormal������ ���� ����
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

	obj->objImg->setTexture(Director::getInstance()->getTextureCache()->addImage("rabbit_normal_down.png"));	//sprite �̹��� �缳��

	//state�� �´� speed, actionDuration ����
	obj->speed = 100;
	actionDuration = 2;

	//init random dir
	srand(time(NULL) + obj->objIndex);	//random seed ����

	//move1 : ������ �������� 2�ʰ� �̵�	
	MoveBy *move1 = StateRabbitNormal::makeRandDir(obj);
	obj->moveLen = obj->setMoveLen(obj->dir, obj->speed);	//�̵� ����, �ӵ��� ���� moveLen �ʱ�ȭ

	//move2 : 2�ʰ� �޽�
	DelayTime *move2 = DelayTime::create(2);

	//�������� ���Ḧ �˸��� ���� callback func
	auto callback = CallFunc::create(
		[=]
	{
		initAction(obj);

	});

	//������ move���� ���� sequence ����
	Sequence* seq = Sequence::create(move1, move2, callback, nullptr);
	seq->setTag(0);	//sequence ������ ����� ȣ���� �� ���� �� ����...

	//sequence ���� ���
	obj->objImg->runAction(seq);

}

MoveBy* StateRabbitNormal::makeRandDir(ObjRabbit* obj) {
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

bool StateRabbitNormal::checkTransitionCond(ObjRabbit * obj) {

	obj->updateRabbitSight();

	//Dead ���·��� ���� Ȯ��
	if (obj->HP <= 0) {
		doTransition(obj, STATE_RABBIT_NORMAL, STATE_RABBIT_DEAD);
		return true;
	}
	//Run ���·��� ���� ���� Ȯ��
	else if (StateRabbitNormal::checkSight(obj)) {
		//Run ���·� ����

		doTransition(obj, STATE_RABBIT_NORMAL, STATE_RABBIT_RUN);
		
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

	//�̵� ���� ���� -> ���� �̵� ������ �ݴ�������
	//move1 : ���� �̵� ������ �ݴ������� 2�ʰ� �̵�
	MoveBy *move1;

	//��->�� ��������
	if (obj->dir == DIR_LEFT) {
		obj->dir = DIR_RIGHT;
		move1 = MoveBy::create(actionDuration, Vec2(obj->speed * actionDuration, 0));
		obj->objImg->setRotation(-90);
	}
	//��->��
	else if (obj->dir == DIR_RIGHT) {
		obj->dir = DIR_LEFT;
		move1 = MoveBy::create(actionDuration, Vec2(-obj->speed * actionDuration, 0));
		obj->objImg->setRotation(90);
	}
	//��->��
	else if (obj->dir == DIR_UP) {
		obj->dir = DIR_DOWN;
		move1 = MoveBy::create(actionDuration, Vec2(0, -obj->speed * actionDuration));
		obj->objImg->setRotation(0);
	}
	//��->��
	else if (obj->dir == DIR_DOWN) {
		obj->dir = DIR_UP;
		move1 = MoveBy::create(actionDuration, Vec2(0, obj->speed * actionDuration));
		obj->objImg->setRotation(180);
	}
	obj->moveLen = obj->setMoveLen(obj->dir, obj->speed);	//�̵� ����, �ӵ��� ���� moveLen �ʱ�ȭ

															//������ move���� ���� sequence ����
	Sequence* seq = Sequence::create(move1, nullptr);
	seq->setTag(0);

	//sequence ���� ���
	obj->objImg->runAction(seq);
}

bool StateRabbitRun::checkTransitionCond(ObjRabbit * obj) {

	//Dead ���·��� ���� Ȯ��
	if (obj->HP <= 0) {
		doTransition(obj, STATE_RABBIT_RUN, STATE_RABBIT_DEAD);
		return true;
	}
	else if (obj->objImg->getNumberOfRunningActions() == 0) {	//action�� �Ϸ�Ǿ��� ��

		doTransition(obj, STATE_RABBIT_RUN, STATE_RABBIT_NORMAL);

		return true;
	}
	else {
		return false;
	}

}


////StateRabbitDead Func
void StateRabbitDead::initAction(ObjRabbit * obj) {
	obj->objImg->stopAllActions();	//�����ϴ� action ����
	obj->rabbitSightTri->clear();	//�þ� �ﰢ�� ����
	obj->objImg->setTexture(Director::getInstance()->getTextureCache()->addImage("rabbit_dead_down.png"));	//sprite image ����
	obj->speed = 0;
	actionDuration = 1;
	
	obj->unscheduleUpdate(); //������Ʈ ����

	GameWorld::objManager->deleteObjectAvailList(obj); //ObjManager���� avail list���� ��������

	//���� ��� ����Ʈ ���
	GameWorld::ui->myBreadPointGrow();

	//action ����
	auto fadeout = FadeOut::create(actionDuration);

	auto callback = CallFunc::create(
		[=]
	{
		obj->deInit();
	});

	obj->objImg->runAction(Sequence::create(fadeout, callback, nullptr));

}

bool StateRabbitDead::checkTransitionCond(ObjRabbit * obj) {
	//��� ���·� ���ٸ� ������ ���� ����
	return false;
}