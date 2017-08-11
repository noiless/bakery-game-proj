#pragma once
#include "cocos2d.h"
#include "Obj.h"
#include "Attack.h"
#include "AudioEngine.h"  

USING_NS_CC;

class Player : public Obj {
private:
	bool isMoving[4];
	Vec2 moveLen;
	Camera* cam;
	Attack* attack;
	int HP;
	experimental::AudioProfile playerAttactEffect;
	Rect exBox;

	bool setPlayerMoveLen(float actionDuration);
	void update(float) override;
	virtual bool deInit();
	virtual bool init();

public:
	Player() {
		init();
		qnodeBound = Size(objImg->getContentSize());
	}

	virtual void loseHPByPlayer();
	virtual void loseHPByOther(int damage);

	int dir;
	int speed = 500;
	
	
};