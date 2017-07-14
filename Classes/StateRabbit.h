#pragma once
#include "cocos2d.h"

class ObjRabbit;
class StateRabbitNormal;
class StateRabbitRun;
class StateRabbitDead;

USING_NS_CC;

class StateRabbit //: public State
{
public:
	virtual void initAction(ObjRabbit * obj) = 0;
	virtual bool checkTransitionCond(ObjRabbit * obj) = 0;

	bool checkSight(ObjRabbit * obj);

	int actionDuration;	//각 action이 가지는 시간. 한 state가 가지는 action들의 duration을 모두 통일...?

	static StateRabbitNormal* rabbitNormal;
	static StateRabbitRun* rabbitRun;
	static StateRabbitDead* rabbitDead;
};

//토끼 : 통상 상태 클래스
class StateRabbitNormal : public StateRabbit
{
public:
	StateRabbitNormal() {
	};

	virtual void initAction(ObjRabbit * obj);
	virtual bool checkTransitionCond(ObjRabbit * obj);

	MoveBy* makeRandDir(ObjRabbit* obj);

};

//토끼 : 도주 상태 클래스
class StateRabbitRun : public StateRabbit
{
public:
	StateRabbitRun() {

	};

	virtual void initAction(ObjRabbit * obj);
	virtual bool checkTransitionCond(ObjRabbit * obj);

};

class StateRabbitDead : public StateRabbit
{
public:
	StateRabbitDead() {

	}

	StateRabbitDead(ObjRabbit * obj) {

	}

	virtual void initAction(ObjRabbit * obj);
	virtual bool checkTransitionCond(ObjRabbit * obj);
};