#pragma once
#include "cocos2d.h"

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
public:
	StateEnemyAttack() {

	};

	virtual void initAction(ObjEnemy * obj);
	virtual bool checkTransitionCond(ObjEnemy * obj);

};


class StateEnemyEscape : public StateEnemy {
public:
	StateEnemyEscape() {

	};

	virtual void initAction(ObjEnemy * obj);
	virtual bool checkTransitionCond(ObjEnemy * obj);

};


////���� ������ ���� ������
//��;���� �����Ͱ���.... HP ó���� HPState���� ���� ���༭
class StateEnemyDead : public StateEnemy {
public:
	StateEnemyDead() {

	};
	virtual void initAction(ObjEnemy * obj);
	virtual bool checkTransitionCond(ObjEnemy * obj);

};




/////////////////////////////////////
//StateHPEnemy : HP�� ���� sprite ���� ����
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


