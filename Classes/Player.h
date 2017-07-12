#pragma once
#include "cocos2d.h";
#include "Obj.h"

class Player : public Obj {
public:
	Player() {
		init();
	}

	virtual bool init();
	int speed = 500;
	bool isMoving[4];

	cocos2d::Vec2 moveLen;

	cocos2d::Camera* cam;

	bool setPlayerMoveLen(float actionDuration);

	void update(float) override;

};