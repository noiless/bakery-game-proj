#pragma once
#include "cocos2d.h"

class StateEnemyNormal;
class StateEnemyDead;
class ObjEnemy;

enum { STATE_ENEMY_NORMAL, STATE_ENEMY_DEAD };

class StateEnemy {
public:
	virtual void initAction(ObjEnemy * obj) = 0;
	virtual bool checkTransitionCond(ObjEnemy * obj) = 0;
	void doTransition(ObjEnemy* obj, int source, int dest);

	int actionDuration;	

	static StateEnemyNormal* enemyNormal;
	static StateEnemyDead* enemyDead;

};

class StateEnemyNormal : public StateEnemy {
public:
	StateEnemyNormal() {
		
	};

	virtual void initAction(ObjEnemy * obj);
	virtual bool checkTransitionCond(ObjEnemy * obj);

};


////적이 죽으면 게임 오버됨
class StateEnemyDead : public StateEnemy {
public:
	StateEnemyDead() {

	};
	virtual void initAction(ObjEnemy * obj);
	virtual bool checkTransitionCond(ObjEnemy * obj);

};

