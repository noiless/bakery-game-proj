#pragma once
#include "cocos2d.h"
#include "AudioEngine.h" 
#include "pugixml\pugixml.hpp"


class ObjRabbit;
class StateRabbitNormal;
class StateRabbitRun;
class StateRabbitDead;

USING_NS_CC;

enum {STATE_RABBIT_NORMAL, STATE_RABBIT_RUN, STATE_RABBIT_DEAD};

class StateRabbit
{
public:
	StateRabbit() {
		rabbitDeadEffect.name = "rabbitDeadEffect";
		rabbitDeadEffect.maxInstances = 10;
	}
	virtual void initAction(ObjRabbit * obj) = 0;
	virtual bool checkTransitionCond(ObjRabbit * obj) = 0;
	void doTransition(ObjRabbit* obj, int source, int dest);
	void initStates(pugi::xml_node stateNode);

	int actionDuration;	//�� action�� ������ �ð�. �� state�� ������ action���� duration�� ��� ����...?
	int stateSpeed;

	experimental::AudioProfile rabbitDeadEffect;

	static StateRabbitNormal* rabbitNormal;
	static StateRabbitRun* rabbitRun;
	static StateRabbitDead* rabbitDead;
};

//�䳢 : ��� ���� Ŭ����
class StateRabbitNormal : public StateRabbit
{
private:
	MoveBy* makeRandDir(ObjRabbit* obj);
public:
	StateRabbitNormal() {
	};

	virtual void initAction(ObjRabbit * obj);
	virtual bool checkTransitionCond(ObjRabbit * obj);
	bool checkSight(ObjRabbit * obj);

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

	virtual void initAction(ObjRabbit * obj);
	virtual bool checkTransitionCond(ObjRabbit * obj);
};