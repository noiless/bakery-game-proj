#include "cocos2d.h"
#include "StateEnemy.h"
#include "ObjEnemy.h"

#include "GameScene.h"

///////StateEnemy


void StateEnemy::doTransition(ObjEnemy* obj, int source, int dest) {

}

StateEnemyNormal* StateEnemy::enemyNormal = new StateEnemyNormal;
StateEnemyDead* StateEnemy::enemyDead = new StateEnemyDead;




//////////StateEnemyNormal

void StateEnemyNormal::initAction(ObjEnemy * obj) {


	MoveBy* move = MoveBy::create(3, Vec2(300, 300));

	obj->objImg->runAction(move);

}

bool StateEnemyNormal::checkTransitionCond(ObjEnemy * obj) {
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