#pragma once
#include "cocos2d.h"
#include "pugixml\pugixml.hpp"


class ObjEnemy;

class StateEnemyNormal;
class StateEnemyDetour;
class StateEnemyAttack;
class StateEnemyEscape;
class StateEnemyDead;
class ObjEnemy;

class StateHPEnemyNormal;
class StateHPEnemyHurt;
class StateHPEnemyDeadly;


enum { STATE_ENEMY_NORMAL, STATE_ENEMY_DETOUR, STATE_ENEMY_ATTACK, STATE_ENEMY_ESCAPE, STATE_ENEMY_DEAD };

class StateEnemy {
public:

	float actionDuration;
	virtual void initAction(ObjEnemy * obj) = 0;
	virtual bool checkTransitionCond(ObjEnemy * obj) = 0;
	void doTransition(ObjEnemy* obj, int source, int dest);
	void initStates(pugi::xml_node stateNode);

	static StateEnemyNormal* enemyNormal;
	static StateEnemyDetour* enemyDetour;
	static StateEnemyAttack* enemyAttack;
	static StateEnemyEscape* enemyEscape;
	static StateEnemyDead* enemyDead;

};

class StateEnemyNormal : public StateEnemy {
private:
	cocos2d::MoveBy* makeRandDir(ObjEnemy* obj);

public:
	StateEnemyNormal() {
		
	};

	virtual void initAction(ObjEnemy * obj);
	virtual bool checkTransitionCond(ObjEnemy * obj);

};

class StateEnemyDetour : public StateEnemy {
public:
	StateEnemyDetour() {

	};

	virtual void initAction(ObjEnemy * obj);
	virtual bool checkTransitionCond(ObjEnemy * obj);

};


class StateEnemyAttack : public StateEnemy {
private:
	cocos2d::Vec2 targetPos;
public:
	StateEnemyAttack() {
		attackDuration = 0;
	};

	float attackDuration;
	virtual void initAction(ObjEnemy * obj);
	virtual bool checkTransitionCond(ObjEnemy * obj);
	void setMoveData(ObjEnemy* obj, cocos2d::Vec2 actionVector);

};


class StateEnemyEscape : public StateEnemy {
public:
	StateEnemyEscape() {

	};

	virtual void initAction(ObjEnemy * obj);
	virtual bool checkTransitionCond(ObjEnemy * obj);

};


////적이 죽으면 게임 오버됨
//피;ㄹ요 없을것같아.... HP 처리를 HPState에서 따로 해줘서
class StateEnemyDead : public StateEnemy {
public:
	StateEnemyDead() {

	};
	virtual void initAction(ObjEnemy * obj);
	virtual bool checkTransitionCond(ObjEnemy * obj);

};




/////////////////////////////////////
//StateHPEnemy : HP에 따른 sprite 변경 수행
////////////////////////////////////

class StateHPEnemy {
public:
	static StateHPEnemyNormal* HPEnemyNormal;
	static StateHPEnemyHurt* HPEnemyHurt;
	static StateHPEnemyDeadly* HPEnemyDeadly;

	StateHPEnemy() : HP(20) {

	}

	int HP;
	void changeHP(int newHP, ObjEnemy * obj);
	virtual void checkTransitionCond(ObjEnemy * obj) = 0;
	virtual void initAction(ObjEnemy * obj) = 0;
};


class StateHPEnemyNormal : public StateHPEnemy {
	virtual void checkTransitionCond(ObjEnemy * obj);
	virtual void initAction(ObjEnemy * obj);
};

class StateHPEnemyHurt : public StateHPEnemy {
	virtual void checkTransitionCond(ObjEnemy * obj);
	virtual void initAction(ObjEnemy * obj);

};

class StateHPEnemyDeadly : public StateHPEnemy {
	virtual void checkTransitionCond(ObjEnemy * obj);
	virtual void initAction(ObjEnemy * obj);
};


