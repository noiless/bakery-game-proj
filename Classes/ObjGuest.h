#pragma once
#include "cocos2d.h"
#include "GameScene.h"
#include "Obj.h"


USING_NS_CC;

class StateGuest;
class Raycasting;


class ObjGuest : public Obj {
private:
	void update(float) override;

public:
	ObjGuest();
	~ObjGuest();

	virtual bool init(cocos2d::Vec2 initPos);
	virtual bool deInit();
	virtual void loseHPByPlayer();
	virtual void loseHPByOther(int damage);

	int speed;
	int HP;

	bool inUse;	//사용중이면 true, 사용중이지 않으면 false

	Size detourSize;
	int detourSequence = -1;
	
	Raycasting* eye;

	StateGuest* state;

	//Vec2 moveLen;
};
