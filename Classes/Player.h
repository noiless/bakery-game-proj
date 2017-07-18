#pragma once
#include "cocos2d.h";
#include "Obj.h"
#include "Attack.h"


class Player : public Obj {
private:
	bool isMoving[4];
	cocos2d::Vec2 moveLen;
	cocos2d::Camera* cam;
	Attack* attack;
	int HP;

	bool setPlayerMoveLen(float actionDuration);
	void update(float) override;
	virtual bool deInit();
	virtual bool init();

public:
	Player() {
		init();
	}
	virtual void loseHP();

	int dir;
	int speed = 500;
	
};