#include "cocos2d.h"
#include "StateEnemy.h"
#include "ObjEnemy.h"

///////StateEnemy


void StateEnemy::doTransition(ObjEnemy* obj, int source, int dest) {

}

StateEnemyNormal* StateEnemy::enemyNormal = new StateEnemyNormal;
StateEnemyDead* StateEnemy::enemyDead = new StateEnemyDead;




//////////StateEnemyNormal

void StateEnemyNormal::initAction(ObjEnemy * obj) {

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