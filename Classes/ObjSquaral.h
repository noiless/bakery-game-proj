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

	bool inUse;	//������̸� true, ��������� ������ false

	float normalTime;	//������, transCondition���� �ʱ�ȭ

	virtual void loseHP();
	int speed;
	int HP;

	virtual bool init(cocos2d::Vec2 initPos);
	virtual bool deInit();

	StateSquaral * state;
};