#pragma once
#include "cocos2d.h"
#include "GameScene.h"
#include "Obj.h"
#include "StateGuest.h"

USING_NS_CC;


class ObjGuest : public Obj {
private:
	void update(float) override;
	Rect exBox;

public:
	ObjGuest();
	~ObjGuest();

	virtual bool init(cocos2d::Vec2 initPos);
	virtual bool deInit();
	virtual void loseHP();

	int speed;
	int HP;

	bool inUse;	//������̸� true, ��������� ������ false
	float pausedTime;

	StateGuest* state;

	Vec2 moveLen;
};
