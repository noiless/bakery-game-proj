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

	int actionDuration;	//�� action�� ������ �ð�. �� state�� ������ action���� duration�� ��� ����...?

	static StateRabbitNormal* rabbitNormal;
	static StateRabbitRun* rabbitRun;
	static StateRabbitDead* rabbitDead;
};

//�䳢 : ��� ���� Ŭ����
class StateRabbitNormal : public StateRabbit
{
public:
	StateRabbitNormal() {
	};

	virtual void initAction(ObjRabbit * obj);
	virtual bool checkTransitionCond(ObjRabbit * obj);

	MoveBy* makeRandDir(ObjRabbit* obj);

};

//�䳢 : ���� ���� Ŭ����
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