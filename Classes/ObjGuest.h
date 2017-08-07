#pragma once
#include "cocos2d.h"
#include "GameScene.h"
#include "Obj.h"


USING_NS_CC;

class StateGuest;


class ObjGuest : public Obj {
private:
	void update(float) override;

public:
	ObjGuest();
	~ObjGuest();

	virtual bool init(cocos2d::Vec2 initPos);
	virtual bool deInit();
	virtual void loseHP();

	int speed;
	int HP;

	bool inUse;	//������̸� true, ��������� ������ false

	Size detourSize;
	int detourSequence = -1;

	StateGuest* state;

	//Vec2 moveLen;
};
