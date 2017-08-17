#pragma once
#include "cocos2d.h"
#include "StateSquaral.h"
#include "Obj.h"
#include "Attack.h"
#include "pugixml\pugixml.hpp"


class ObjSquaral : public Obj{
private:
	void update(float) override;

public:
	ObjSquaral(pugi::xml_node squaralNode);
	~ObjSquaral();
	int squaralSightRadius;
	DrawNode * squaralSightCircle;

	void drawSquaralSight();
	Obj * target;
	Obj* tempSquaral;

	bool inUse;	//������̸� true, ��������� ������ false

	float normalTime;	//������, transCondition���� �ʱ�ȭ

	virtual void loseHPByPlayer();
	virtual void loseHPByOther(int damage);
	int speed;
	int HP;

	virtual bool init(cocos2d::Vec2 initPos);
	virtual bool deInit();

	StateSquaral * state;
};