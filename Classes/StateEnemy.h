#pragma once

class ObjEnemy;

class StateEnemyNormal;
class StateEnemyDead;
class ObjEnemy;

class StateHPEnemyNormal;
class StateHPEnemyHurt;
class StateHPEnemyDeadly;


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


