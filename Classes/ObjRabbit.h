#pragma once
#include "cocos2d.h"
#include "Obj.h"
#include "StateRabbit.h"

class StateRabbit;

class ObjRabbit : public Obj {
private:
	
	void update(float) override;
	
public:
	ObjRabbit();
	~ObjRabbit();

	virtual bool init(cocos2d::Vec2 initPos);
	virtual bool deInit();
	
	virtual void loseHP();
	void updateRabbitSight();
	int speed;
	int HP;

	bool inUse;	//사용중이면 true, 사용중이지 않으면 false

	cocos2d::Vec2 rabbitSight[3];
	cocos2d::DrawNode* rabbitSightTri;

	StateRabbit* state;

};
