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

//�䳢 : ��� ���� Ŭ����
class StateRabbitNormal : public StateRabbit
{
public:
	StateRabbitNormal() {

	};

	StateRabbitNormal(Obj * obj) {
		//object pool �����ҰŸ� initAction�� �����ڰ� �ƴϰ� �ش� �ν��Ͻ��� ����� �� ���� ȣ������� �Ѵ�.
		initAction(obj);
	};
	virtual void doAction();
	virtual void initAction(Obj * obj);

};

//�䳢 : ���� ���� Ŭ����
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