#pragma once
#include "cocos2d.h"

class Obj;
struct ColObj;

USING_NS_CC;

class Raycasting : public Node {

public:
	Raycasting() {
		d = 100; //�Ÿ� ����

		lineSight = DrawNode::create();
		this->addChild(lineSight);

		rot = 0;
	};

	~Raycasting();

	Raycasting(Obj* caller, int d, int rot);

	bool init();

	Vec2 startPoint;
	
	int rot;

	float d;	//distance

	Vec2 dir;	//rot, callerRot�� ���� ���� ����ȭ�� ���⺤��

	ColObj* doRaycast();

	Obj* caller;	//tq

	DrawNode* lineSight;

	Vec2 anchorDiff;

	void setDir(int callerRot);
};
