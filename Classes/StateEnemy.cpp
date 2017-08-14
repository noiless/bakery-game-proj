#include "cocos2d.h"
#include "StateEnemy.h"
#include "ObjEnemy.h"
#include "Raycasting.h"
#include "GameScene.h"

USING_NS_CC;
using namespace pugi;

///////StateEnemy

void StateEnemy::initStates(xml_node stateNode) {

}


void StateEnemy::doTransition(ObjEnemy* obj, int source, int dest) {

	obj->pausedTime = 0;
	obj->getActionManager()->resumeTarget(obj->objImg);
	obj->getActionManager()->removeAllActionsFromTarget(obj->objImg);	//�����ϴ� action ����

	if (dest == STATE_ENEMY_DETOUR) {

		obj->state = dynamic_cast<StateEnemy*> (StateEnemy::enemyDetour);
		obj->state->initAction(obj);

	}

	else if (dest == STATE_ENEMY_ESCAPE) {

		obj->state = dynamic_cast<StateEnemy*> (StateEnemy::enemyEscape);
		obj->state->initAction(obj);

	}

	else if (dest == STATE_ENEMY_ATTACK) {

		//�þ� ����
		obj->eye[0]->lineSight->clear();
		obj->eye[1]->lineSight->clear();
		obj->eye[2]->lineSight->clear();

		obj->state = dynamic_cast<StateEnemy*> (StateEnemy::enemyAttack);
		obj->state->initAction(obj);

	}

	else if (dest == STATE_ENEMY_NORMAL) {

		obj->state = dynamic_cast<StateEnemy*> (StateEnemy::enemyNormal);
		obj->state->initAction(obj);

	}


}

StateEnemyNormal* StateEnemy::enemyNormal = new StateEnemyNormal;
StateEnemyDetour* StateEnemy::enemyDetour = new StateEnemyDetour;
StateEnemyAttack* StateEnemy::enemyAttack = new StateEnemyAttack;
StateEnemyEscape* StateEnemy::enemyEscape = new StateEnemyEscape;
StateEnemyDead* StateEnemy::enemyDead = new StateEnemyDead;


//////////StateEnemyNormal

void StateEnemyNormal::initAction(ObjEnemy * obj) {

	obj->speed = 100;
	actionDuration = 3;

	MoveBy* move = makeRandDir(obj);

	obj->moveLen = obj->setMoveLen(obj->dir, obj->speed);

	auto callback = CallFunc::create(
		[=]
	{
		initAction(obj);

	});

	//������ move���� ���� sequence ����
	Sequence* seq = Sequence::create(move, callback, nullptr);

	//sequence ���� ���
	obj->objImg->runAction(seq);

}

MoveBy* StateEnemyNormal::makeRandDir(ObjEnemy* obj) {
	int randDir = rand() % 4;	//������ ���� ����

	if (randDir == DIR_LEFT) {
		obj->dir = DIR_LEFT;	//dir ����

		obj->eye[0]->setDir(90);
		obj->eye[1]->setDir(90);
		obj->eye[2]->setDir(90);

		obj->objImg->setRotation(90);

		return MoveBy::create(actionDuration, Vec2(-actionDuration * obj->speed, 0));
	}
	else if (randDir == DIR_RIGHT) {
		obj->dir = DIR_RIGHT;
		obj->objImg->setRotation(-90);

		obj->eye[0]->setDir(270);
		obj->eye[1]->setDir(270);
		obj->eye[2]->setDir(270);

		return MoveBy::create(actionDuration, Vec2(actionDuration * obj->speed, 0));
	}
	else if (randDir == DIR_UP) {
		obj->dir = DIR_UP;
		obj->objImg->setRotation(180);

		obj->eye[0]->setDir(180);
		obj->eye[1]->setDir(180);
		obj->eye[2]->setDir(180);

		return MoveBy::create(actionDuration, Vec2(0, actionDuration * obj->speed));
	}
	else if (randDir == DIR_DOWN) {
		obj->dir = DIR_DOWN;
		obj->objImg->setRotation(0);

		obj->eye[0]->setDir(0);
		obj->eye[1]->setDir(0);
		obj->eye[2]->setDir(0);

		return MoveBy::create(actionDuration, Vec2(0, -actionDuration * obj->speed));
	}
	else {
		return nullptr;
	}
}

bool StateEnemyNormal::checkTransitionCond(ObjEnemy * obj) {

	//eye���� �浹�� �߻����� ��
	if (obj->colEyeIndex >= 0) {

		if (obj->target->typecode == TYPECODE_PEOPLE || obj->target->typecode == TYPECODE_NONE) {
			doTransition(obj, STATE_ENEMY_NORMAL, STATE_ENEMY_DETOUR);
		}
		//���������� �������� ���� ���� ���̸� ����
		else {
			doTransition(obj, STATE_ENEMY_NORMAL, STATE_ENEMY_ATTACK);
		}

	}

	return true;
}




//////StateEnemyDetour


void StateEnemyDetour::initAction(ObjEnemy * obj) {

	MoveBy* move;

	if (obj->colEyeIndex == 0) {
		obj->objImg->setRotation(obj->objImg->getRotation() + 180);
	}
	else if (obj->colEyeIndex == 1) {
		obj->objImg->setRotation(obj->objImg->getRotation() - 90);
	}
	else if (obj->colEyeIndex == 2) {
		obj->objImg->setRotation(obj->objImg->getRotation() + 90);
	}

	for (int i = 0; i < 3; i++) {
		obj->eye[i]->setDir(obj->objImg->getRotation());
	}

	int imgRot = (int)obj->objImg->getRotation();
	imgRot = (imgRot + 360) % 360;

	switch (imgRot) {
	case 0:
		//down
		obj->dir = DIR_DOWN;
		move = MoveBy::create(actionDuration, Vec2(0, -actionDuration * obj->speed));

		break;

	case 90:
		//left
		obj->dir = DIR_LEFT;
		move = MoveBy::create(actionDuration, Vec2(-actionDuration * obj->speed, 0));
		break;
	case 180:
		//up
		obj->dir = DIR_UP;
		move = MoveBy::create(actionDuration, Vec2(0, actionDuration * obj->speed));

		break;
	case 270:
		//right
		obj->dir = DIR_RIGHT;
		move = MoveBy::create(actionDuration, Vec2(actionDuration * obj->speed, 0));
		break;
	default:
		CCLOG("%d degree", (int)obj->objImg->getRotation());
		break;
	}

	obj->moveLen = obj->setMoveLen(obj->dir, obj->speed);

	//ó�� ���� �� index �ʱ�ȭ����
	obj->colEyeIndex = -1;

	obj->objImg->runAction(move);

}

bool StateEnemyDetour::checkTransitionCond(ObjEnemy * obj) {

	//��ȸ ���߿��� �浹 �߻��� �ٽ� ��ȸ
	if (obj->colEyeIndex >= 0) {

		if (obj->target->typecode == TYPECODE_PEOPLE || obj->target->typecode == TYPECODE_NONE) {
			doTransition(obj, STATE_ENEMY_DETOUR, STATE_ENEMY_DETOUR);
		}
		//���������� �������� ���� ���� ���̸� ����
		else {
			doTransition(obj, STATE_ENEMY_NORMAL, STATE_ENEMY_ATTACK);
		}
	}
	//�׼��� �������� normal ���·� ���ư��� ���� �̵�
	else if (obj->objImg->getNumberOfRunningActions() == 0) {
		doTransition(obj, STATE_ENEMY_DETOUR, STATE_ENEMY_NORMAL);
	}

	return true;
}



//////StateEnemyAttack


void StateEnemyAttack::initAction(ObjEnemy * obj) {

	actionDuration = 0.5;

	targetPos = obj->target->objImg->getPosition();
	
	//action ����

	if (targetPos.x == obj->objImg->getPositionX()) {

		MoveBy* move2 = MoveBy::create(actionDuration, Vec2(0, targetPos.y - obj->objImg->getPositionY()));
		auto callback2 = CallFunc::create(CC_CALLBACK_0(StateEnemyAttack::setMoveData, this, obj, Vec2(0, targetPos.y - obj->objImg->getPositionY())));

		obj->objImg->runAction(Sequence::create(callback2, move2, nullptr));

	}
	else {
		MoveBy* move1 = MoveBy::create(actionDuration, Vec2(targetPos.x - obj->objImg->getPositionX(), 0));
		auto callback1 = CallFunc::create(CC_CALLBACK_0(StateEnemyAttack::setMoveData, this, obj, Vec2(targetPos.x - obj->objImg->getPositionX(), 0)));

		MoveBy* move2 = MoveBy::create(actionDuration, Vec2(0, targetPos.y - obj->objImg->getPositionY()));
		auto callback2 = CallFunc::create(CC_CALLBACK_0(StateEnemyAttack::setMoveData, this, obj, Vec2(0, targetPos.y - obj->objImg->getPositionY())));

		obj->objImg->runAction(Sequence::create(callback1, move1, callback2, move2, nullptr));

	}
}

//dir, moveLen ����
void StateEnemyAttack::setMoveData(ObjEnemy* obj, Vec2 actionVector) {

	//y�� �̵�
	if (actionVector.x == 0) {
		
		//UP
		if (actionVector.y > 0) {
			obj->objImg->setRotation(180);
			obj->dir = DIR_UP;
		}
		//DOWN
		else {
			obj->objImg->setRotation(0);
			obj->dir = DIR_DOWN;

		}

		obj->speed = abs(actionVector.y) / actionDuration;

	}
	//x�� �̵�
	else if (actionVector.y == 0) {

		//RIGHT
		if (actionVector.x > 0) {
			obj->objImg->setRotation(270);
			obj->dir = DIR_RIGHT;

		}
		//LEFT
		else {
			obj->objImg->setRotation(90);
			obj->dir = DIR_LEFT;

		}

		obj->speed = abs(actionVector.x) / actionDuration;

	}

	obj->moveLen = obj->setMoveLen(obj->dir, obj->speed);

}

bool StateEnemyAttack::checkTransitionCond(ObjEnemy * obj) {

	//Ÿ���� ������ normal ���·� ���ư�
	if (!GameWorld::objManager->isObjColAvail(obj->target)) {
		doTransition(obj, STATE_ENEMY_ATTACK, STATE_ENEMY_NORMAL);
	}

	//escape ó��

	if (attackDuration > 0.5) {
		attackDuration = 0;

		//attack ����
		obj->objImg->removeChild(obj->attack, false);
		obj->attack->init(obj);

		//Ÿ���� ��ġ�� �ٲ���� ��� �׼� ������ ���� state init
		if (obj->target->objImg->getPosition() != targetPos) {
			doTransition(obj, STATE_ENEMY_ATTACK, STATE_ENEMY_ATTACK);
		}

	}

	return true;
}



//////StateEnemyEscape


void StateEnemyEscape::initAction(ObjEnemy * obj) {

}

bool StateEnemyEscape::checkTransitionCond(ObjEnemy * obj) {
	return true;
}





///////////StateEnemyDead

void StateEnemyDead::initAction(ObjEnemy * obj) {

}

bool StateEnemyDead::checkTransitionCond(ObjEnemy * obj) {
	return true;
}



/////////////////////////////////////
//StateHPEnemy : HP�� ���� sprite ���� ����
////////////////////////////////////

StateHPEnemyNormal* StateHPEnemy::HPEnemyNormal = new StateHPEnemyNormal;
StateHPEnemyHurt* StateHPEnemy::HPEnemyHurt = new StateHPEnemyHurt;
StateHPEnemyDeadly* StateHPEnemy::HPEnemyDeadly = new StateHPEnemyDeadly;

void StateHPEnemy::changeHP(int newHP, ObjEnemy * obj) {
	HP = newHP;
	checkTransitionCond(obj);
}

////////////

void StateHPEnemyNormal::checkTransitionCond(ObjEnemy * obj) {
	if (HP <= 15) {
		//���� ����
		obj->stateHP = HPEnemyHurt;
		obj->stateHP->initAction(obj);
	}
}

void StateHPEnemyNormal::initAction(ObjEnemy * obj) {
	obj->objImg->setTexture(Director::getInstance()->getTextureCache()->addImage("img/enemy_normal_down.png"));	//sprite �̹��� �缳��

}

////////////

void StateHPEnemyHurt::checkTransitionCond(ObjEnemy * obj) {
	if (HP <= 5) {
		//���� ����
		obj->stateHP = HPEnemyDeadly;
		obj->stateHP->initAction(obj);
	}

}
void StateHPEnemyHurt::initAction(ObjEnemy * obj) {
	obj->objImg->setTexture(Director::getInstance()->getTextureCache()->addImage("img/enemy_hurt_down.png"));	//sprite �̹��� �缳��

}

////////////

void StateHPEnemyDeadly::checkTransitionCond(ObjEnemy * obj) {
	if (HP <= 0) {
		//���ӿ���
		GameWorld::gameEnd();
	}

}

void StateHPEnemyDeadly::initAction(ObjEnemy * obj) {
	obj->objImg->setTexture(Director::getInstance()->getTextureCache()->addImage("img/enemy_deadly_down.png"));	//sprite �̹��� �缳��

}