#pragma once
#include "cocos2d.h"

class ObjRabbit;
class StateRabbitNormal;
class StateRabbitRun;

USING_NS_CC;

class StateRabbit //: public State
{
public:
	StateRabbit();
	~StateRabbit();

	virtual void initAction(ObjRabbit * obj) = 0;
	virtual bool checkTransitionCond(ObjRabbit * obj) = 0;

	bool checkSight(ObjRabbit * obj);

	int actionDuration;	//�� action�� ������ �ð�. �� state�� ������ action���� duration�� ��� ����...?

	static StateRabbitNormal* rabbitNormal;
	static StateRabbitRun* rabbitRun;
};

//�䳢 : ��� ���� Ŭ����
class StateRabbitNormal : public StateRabbit
{
public:
	StateRabbitNormal() {
	};

	StateRabbitNormal(ObjRabbit * obj) {
		//object pool �����ҰŸ� initAction�� �����ڰ� �ƴϰ� �ش� �ν��Ͻ��� ����� �� ���� ȣ������� �Ѵ�.
		CCLOG("�̰� �ȺҸ���?");
		initAction(obj);
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

	StateRabbitRun(ObjRabbit * obj) {
		initAction(obj);
	};

	virtual void initAction(ObjRabbit * obj);
	virtual bool checkTransitionCond(ObjRabbit * obj);

};