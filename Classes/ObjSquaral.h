#pragma once
#include "cocos2d.h"
#include "StateSquaral.h"
#include "Obj.h"
#include "Attack.h"

class ObjSquaral : public Obj{
private:
	int HP;
	Rect exBox;
	void update(float) override;
	//cocos2d::Vector<AcornAttack> acorn;	//다람쥐가 경험치같은걸 얻는 것도 아니고 없어도 될거 같은

public:
	ObjSquaral();
	int squaralSightRadius;
	DrawNode * squaralSightCircle;

	void drawSquaralSight();
	Obj * target;

	bool inUse;	//사용중이면 true, 사용중이지 않으면 false

	virtual void loseHP();
	int speed;

	virtual bool init(cocos2d::Vec2 initPos);
	virtual bool deInit();

	StateSquaral * state;
	Vec2 moveLen;
};