#pragma once
#include "cocos2d.h";
#include "Obj.h"

class Player : public Obj {
public:
	Player() : Obj(this) {
		init();
	}

	virtual bool init();
	int speed = 500;
	bool isMoving[4];

	cocos2d::Vec2 moveLen;

	bool moveLenUpdate(float delta);
	
	cocos2d::Camera* cam;

	void update(float) override;
};