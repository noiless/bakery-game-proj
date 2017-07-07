#pragma once
#include "cocos2d.h"
#include "Obj.h"
#include "StateRabbit.h"

class StateRabbit;

class ObjRabbit : public Obj {

public:

	ObjRabbit() : Obj(this) {
		init();
	};

	void update(float) override;

	virtual bool init();

	cocos2d::Vec2 moveLen;

	StateRabbit* state;

};
