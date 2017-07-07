#pragma once
#include "ObjRabbit.h"
#include "cocos2d.h"

class StateRabbitNormal;
class StateRabbitRun;

class StateRabbit : public State
{
public:
	StateRabbit();
	~StateRabbit();
	virtual void doAction() = 0;
	virtual void initAction(Obj * obj) = 0;

	static StateRabbitNormal rabbitNormal;
	static StateRabbitRun rabbitRun;
};

//토끼 : 통상 상태 클래스
class StateRabbitNormal : public StateRabbit
{
public:
	StateRabbitNormal() {

	};

	StateRabbitNormal(Obj * obj) {
		//object pool 적용할거면 initAction을 생성자가 아니고 해당 인스턴스가 재배정 될 때에 호출해줘야 한다.
		initAction(obj);
	};
	virtual void doAction();
	virtual void initAction(Obj * obj);

};

//토끼 : 도주 상태 클래스
class StateRabbitRun : public StateRabbit
{
public:
	StateRabbitRun() {

	};

	StateRabbitRun(Obj * obj) {
		initAction(obj);
	};
	virtual void doAction();
	virtual void initAction(Obj * obj);

};