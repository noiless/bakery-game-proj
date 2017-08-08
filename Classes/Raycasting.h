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

		rot = -45;

		scheduleUpdate();

	};

	~Raycasting();

	Raycasting(int d, int rot);

	Vec2 startPoint;
	//Vec2 dir;	//dir vector, //�ʱ� ��ġ������ �����ִ� ���������� ���⺤��
	int callerRot;

	int rot;

	float d;	//distance

	//DrawNode* linesight;

	Obj* doRaycast();

	ObjEnemy* caller;	//tq

	DrawNode* lineSight = DrawNode::create();


	//�굵 �ӽ�...
	void update(float) override;
};