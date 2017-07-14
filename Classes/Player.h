#pragma once
#include "cocos2d.h";
#include "Obj.h"
#include "Attack.h"


class Player : public Obj {
public:
	Player() {
		init();
	}

	virtual bool init();
	int HP;
	int dir;
	int speed = 500;
	bool isMoving[4];

	cocos2d::Vec2 moveLen;

	cocos2d::Camera* cam;

	Attack* attack;

	bool setPlayerMoveLen(float actionDuration);

	void update(float) override;
	virtual void loseHP();
};