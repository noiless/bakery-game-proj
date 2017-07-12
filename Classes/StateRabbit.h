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

	int actionDuration;	//각 action이 가지는 시간. 한 state가 가지는 action들의 duration을 모두 통일...?

	static StateRabbitNormal* rabbitNormal;
	static StateRabbitRun* rabbitRun;
};

//토끼 : 통상 상태 클래스
class StateRabbitNormal : public StateRabbit
{
public:
	StateRabbitNormal() {
	};

	StateRabbitNormal(ObjRabbit * obj) {
		//object pool 적용할거면 initAction을 생성자가 아니고 해당 인스턴스가 재배정 될 때에 호출해줘야 한다.
		CCLOG("이거 안불리죠?");
		initAction(obj);
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

	StateRabbitRun(ObjRabbit * obj) {
		initAction(obj);
	};

	virtual void initAction(ObjRabbit * obj);
	virtual bool checkTransitionCond(ObjRabbit * obj);

};