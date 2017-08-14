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
	obj->getActionManager()->removeAllActionsFromTarget(obj->objImg);	//수행하던 action 제거

	if (dest == STATE_ENEMY_DETOUR) {

		obj->state = dynamic_cast<StateEnemy*> (StateEnemy::enemyDetour);
		obj->state->initAction(obj);

	}

	else if (dest == STATE_ENEMY_ESCAPE) {

		obj->state = dynamic_cast<StateEnemy*> (StateEnemy::enemyEscape);
		obj->state->initAction(obj);

	}

	else if (dest == STATE_ENEMY_ATTACK) {

		//시야 삭제
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

	//지정한 move들을 통한 sequence 지정
	Sequence* seq = Sequence::create(move, callback, nullptr);

	//sequence 실행 명령
	obj->objImg->runAction(seq);

}

MoveBy* StateEnemyNormal::makeRandDir(ObjEnemy* obj) {
	int randDir = rand() % 4;	//랜덤한 방향 설정

	if (randDir == DIR_LEFT) {
		obj->dir = DIR_LEFT;	//dir 갱신

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

	//eye에서 충돌이 발생했을 시
	if (obj->colEyeIndex >= 0) {

		if (obj->target->typecode == TYPECODE_PEOPLE || obj->target->typecode == TYPECODE_NONE) {
			doTransition(obj, STATE_ENEMY_NORMAL, STATE_ENEMY_DETOUR);
		}
		//공격했을때 게이지에 영향 가는 군이면 공격
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

	//처리 끝난 뒤 index 초기화해줌
	obj->colEyeIndex = -1;

	obj->objImg->runAction(move);

}

bool StateEnemyDetour::checkTransitionCond(ObjEnemy * obj) {

	//우회 도중에도 충돌 발생시 다시 우회
	if (obj->colEyeIndex >= 0) {

		if (obj->target->typecode == TYPECODE_PEOPLE || obj->target->typecode == TYPECODE_NONE) {
			doTransition(obj, STATE_ENEMY_DETOUR, STATE_ENEMY_DETOUR);
		}
		//공격했을때 게이지에 영향 가는 군이면 공격
		else {
			doTransition(obj, STATE_ENEMY_NORMAL, STATE_ENEMY_ATTACK);
		}
	}
	//액션이 끝났으면 normal 상태로 돌아가서 랜덤 이동
	else if (obj->objImg->getNumberOfRunningActions() == 0) {
		doTransition(obj, STATE_ENEMY_DETOUR, STATE_ENEMY_NORMAL);
	}

	return true;
}



//////StateEnemyAttack


void StateEnemyAttack::initAction(ObjEnemy * obj) {

	actionDuration = 0.5;

	targetPos = obj->target->objImg->getPosition();
	
	//action 설정

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

//dir, moveLen 설정
void StateEnemyAttack::setMoveData(ObjEnemy* obj, Vec2 actionVector) {

	//y축 이동
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
	//x축 이동
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

	//타겟이 죽으면 normal 상태로 돌아감
	if (!GameWorld::objManager->isObjColAvail(obj->target)) {
		doTransition(obj, STATE_ENEMY_ATTACK, STATE_ENEMY_NORMAL);
	}

	//escape 처리

	if (attackDuration > 0.5) {
		attackDuration = 0;

		//attack 띄우기
		obj->objImg->removeChild(obj->attack, false);
		obj->attack->init(obj);

		//타겟의 위치가 바뀌었을 경우 액션 갱신을 위해 state init
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
//StateHPEnemy : HP에 따른 sprite 변경 수행
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
		//상태 변경
		obj->stateHP = HPEnemyHurt;
		obj->stateHP->initAction(obj);
	}
}

void StateHPEnemyNormal::initAction(ObjEnemy * obj) {
	obj->objImg->setTexture(Director::getInstance()->getTextureCache()->addImage("img/enemy_normal_down.png"));	//sprite 이미지 재설정

}

////////////

void StateHPEnemyHurt::checkTransitionCond(ObjEnemy * obj) {
	if (HP <= 5) {
		//상태 변경
		obj->stateHP = HPEnemyDeadly;
		obj->stateHP->initAction(obj);
	}

}
void StateHPEnemyHurt::initAction(ObjEnemy * obj) {
	obj->objImg->setTexture(Director::getInstance()->getTextureCache()->addImage("img/enemy_hurt_down.png"));	//sprite 이미지 재설정

}

////////////

void StateHPEnemyDeadly::checkTransitionCond(ObjEnemy * obj) {
	if (HP <= 0) {
		//게임오버
		GameWorld::gameEnd();
	}

}

void StateHPEnemyDeadly::initAction(ObjEnemy * obj) {
	obj->objImg->setTexture(Director::getInstance()->getTextureCache()->addImage("img/enemy_deadly_down.png"));	//sprite 이미지 재설정

}