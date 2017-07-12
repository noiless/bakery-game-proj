#pragma once
#include "cocos2d.h"
#include "Obj.h"
#include "StateRabbit.h"

class StateRabbit;

class ObjRabbit : public Obj {

public:

	ObjRabbit();

	//ObjRabbit(cocos2d::Vec2 initPos) : Obj(this) {
	//	init(initPos);
	//};

	bool inUse;	//������̸� true, ��������� ������ false

	void update(float) override;

	virtual bool init(cocos2d::Vec2 initPos);

	cocos2d::Vec2 rabbitSight[3];

	cocos2d::DrawNode* rabbitSightTri;

	void updateRabbitSight();

	StateRabbit* state;

};
