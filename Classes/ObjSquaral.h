#pragma once
#include "cocos2d.h"
#include "StateSquaral.h"
#include "Obj.h"
#include "Attack.h"

class ObjSquaral : public Obj{
private:
	void update(float) override;

public:
	ObjSquaral();
	~ObjSquaral();
	int squaralSightRadius;
	DrawNode * squaralSightCircle;

	void drawSquaralSight();
	Obj * target;

	bool inUse;	//사용중이면 true, 사용중이지 않으면 false

	float normalTime;	//생성자, transCondition에서 초기화

	virtual void loseHP();
	int speed;
	int HP;

	virtual bool init(cocos2d::Vec2 initPos);
	virtual bool deInit();

	StateSquaral * state;
};