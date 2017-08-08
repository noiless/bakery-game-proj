#pragma once
#include "cocos2d.h"

class Obj;
class ObjEnemy;

USING_NS_CC;

class Raycasting : public Node {

public:
	Raycasting() {
		d = 100; //�Ÿ� ����

		lineSight = DrawNode::create();
		this->addChild(lineSight);

		rot = 0;

		scheduleUpdate();

	};

	~Raycasting();

	Raycasting(ObjEnemy* caller, int d, int rot);

	bool init();

	Vec2 startPoint;
	
	int rot;

	float d;	//distance

	Vec2 dir;	//rot, callerRot�� ���� ���� ����ȭ�� ���⺤��

	Obj* doRaycast();

	ObjEnemy* caller;	//tq

	DrawNode* lineSight;

	Vec2 anchorDiff;

	void setDir(int callerRot);
};