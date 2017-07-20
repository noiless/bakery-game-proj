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
	//cocos2d::Vector<AcornAttack> acorn;	//�ٶ��㰡 ����ġ������ ��� �͵� �ƴϰ� ��� �ɰ� ����

public:
	ObjSquaral();
	int squaralSightRadius;
	DrawNode * squaralSightCircle;

	void drawSquaralSight();
	Obj * target;

	bool inUse;	//������̸� true, ��������� ������ false

	virtual void loseHP();
	int speed;

	virtual bool init(cocos2d::Vec2 initPos);
	virtual bool deInit();

	StateSquaral * state;
	Vec2 moveLen;
};