#pragma once
#include "cocos2d.h"

class Obj;
class ObjEnemy;

USING_NS_CC;

class Raycasting : public Node {

public:
	Raycasting() {
		d = 100; //거리 설정

		lineSight = DrawNode::create();
		this->addChild(lineSight);

		rot = -45;

		scheduleUpdate();

	};

	~Raycasting();

	Raycasting(int d, int rot);

	Vec2 startPoint;
	//Vec2 dir;	//dir vector, //초기 위치에서의 보고있는 방향으로의 방향벡터
	int callerRot;

	int rot;

	float d;	//distance

	//DrawNode* linesight;

	Obj* doRaycast();

	ObjEnemy* caller;	//tq

	DrawNode* lineSight = DrawNode::create();


	//얘도 임시...
	void update(float) override;
};