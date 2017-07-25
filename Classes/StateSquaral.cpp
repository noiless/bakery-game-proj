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
	obj->getActionManager()->removeAllActionsFromTarget(obj->objImg);	//�����ϴ� action ����
	
	//dead ó�� ���� ����
	if (dest == STATE_SQUARAL_DEAD) {
		if (source == STATE_SQUARAL_ATTACK) {
			GameWorld::objManager->deleteObjectAvailList(StateSquaralAttack::tempSquaral);
		}

		obj->state = StateSquaral::squaralDead;
		obj->state->initAction(obj);
	}
	

	if (source == STATE_SQUARAL_NORMAL) {
		
		if (dest == STATE_SQUARAL_ATTACK) {
			obj->squaralSightCircle->clear();	//�þ� ����
			obj->squaralSightCircle->drawSolidCircle(obj->objImg->getPosition(), obj->squaralSightRadius * 2, 30, 12, Color4F::RED);
			obj->objImg->setTexture(Director::getInstance()->getTextureCache()->addImage("squaral_attack_down.png"));	//sprite �̹��� �缳��
			obj->state = StateSquaral::squaralAttack;
			obj->state->initAction(obj);

			//tempSquaral
			StateSquaralAttack::tempSquaral->objImg->setPosition(obj->objImg->getPosition());
			GameWorld::objManager->addObjectAvailListFRONT(StateSquaralAttack::tempSquaral);

		}

		else if (dest == STATE_SQUARAL_SEED) {

			obj->squaralSightCircle->clear();	//�þ� ����
			obj->objImg->setTexture(Director::getInstance()->getTextureCache()->addImage("squaral_seed_down.png"));	//sprite �̹��� �缳��
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
	obj->objImg->setOpacity(255);

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

	//10��
	if (obj->normalTime > 10) {
		doTransition(obj, STATE_SQUARAL_NORMAL, STATE_SQUARAL_SEED);
	}

	return true;
}



//�ٶ��� : ���� ���� Ŭ����

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

	//������ move���� ���� sequence ����
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

	//Ÿ���� �þ� �������� ������� �븻 ���·� ����
	if (!obj->target->objImg->getBoundingBox().intersectsCircle(obj->objImg->getPosition(), obj->squaralSightRadius * 2)) {
		doTransition(obj, STATE_SQUARAL_ATTACK, STATE_SQUARAL_NORMAL);
	}

	return false;
}


//�ٶ��� : ���� �ɱ� Ŭ����

void StateSquaralSeed::initAction(ObjSquaral * obj) {

	auto callback = CallFunc::create(
		[=]
	{
		//���� ����
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

	//������ move���� ���� sequence ����
	Sequence* seq = Sequence::create(DelayTime::create(3), callback, nullptr);

	obj->objImg->runAction(seq);

}

bool StateSquaralSeed::checkTransitionCond(ObjSquaral * obj) {

	//Dead ���·��� ���� Ȯ��
	if (obj->HP <= 0) {
		doTransition(obj, STATE_SQUARAL_SEED, STATE_SQUARAL_DEAD);
		return true;
	}
	else if (obj->objImg->getNumberOfRunningActions() == 0) {	//action�� �Ϸ�Ǿ��� ��

		doTransition(obj, STATE_SQUARAL_SEED, STATE_SQUARAL_NORMAL);

		return true;
	}
	else {
		return false;
	}

}


//�ٶ��� : ��� Ŭ����
void StateSquaralDead::initAction(ObjSquaral * obj) {
	obj->squaralSightCircle->clear();	//�þ� �ﰢ�� ����
	obj->objImg->setTexture(Director::getInstance()->getTextureCache()->addImage("squaral_dead_down.png"));	//sprite image ����
	obj->speed = 0;
	actionDuration = 1;

	obj->unscheduleUpdate(); //������Ʈ ����

	GameWorld::objManager->deleteObjectAvailList(obj); //ObjManager���� avail list���� ��������

	//��� ��� ����Ʈ �ϰ�
	GameWorld::ui->otherBreadPointDown();

	//action ����
	auto fadeout = FadeOut::create(actionDuration);

	auto callback = CallFunc::create(
		[=]
	{
		obj->deInit();
	});

	obj->objImg->runAction(Sequence::create(fadeout, callback, nullptr));
}

bool StateSquaralDead::checkTransitionCond(ObjSquaral * obj) {
	//��� ���·� ���ٸ� �ൿ�� ���� ����
	return false;
}
