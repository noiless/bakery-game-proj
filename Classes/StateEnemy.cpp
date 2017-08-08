#include "cocos2d.h"
#include "StateEnemy.h"
#include "ObjEnemy.h"
#include "Raycasting.h"

#include "GameScene.h"

USING_NS_CC;

///////StateEnemy


void StateEnemy::doTransition(ObjEnemy* obj, int source, int dest) {

}

StateEnemyNormal* StateEnemy::enemyNormal = new StateEnemyNormal;
StateEnemyDetour* StateEnemy::enemyDetour = new StateEnemyDetour;
StateEnemyAttack* StateEnemy::enemyAttack = new StateEnemyAttack;
StateEnemyEscape* StateEnemy::enemyEscape = new StateEnemyEscape;
StateEnemyDead* StateEnemy::enemyDead = new StateEnemyDead;


//////////StateEnemyNormal

void StateEnemyNormal::initAction(ObjEnemy * obj) {

	obj->speed = 100;
	actionDuration = 5;

	MoveBy* move = makeRandDir(obj);
	obj->setMoveLen(obj->dir, obj->speed);

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
	return true;
}




//////StateEnemyDetour


void StateEnemyDetour::initAction(ObjEnemy * obj) {

}

bool StateEnemyDetour::checkTransitionCond(ObjEnemy * obj) {
	return true;
}



//////StateEnemyAttack


void StateEnemyAttack::initAction(ObjEnemy * obj) {

}

bool StateEnemyAttack::checkTransitionCond(ObjEnemy * obj) {
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
	if (HP <= 10) {
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