#pragma once
#include "cocos2d.h"
#include "GameScene.h"
#include "Obj.h"
#include "pugixml\pugixml.hpp"



USING_NS_CC;

class StateGuest;
class Raycasting;


class ObjGuest : public Obj {
private:
	void update(float) override;
	cocos2d::Vec2 initPos;	//static�̾�� �ϴ� ���� �ƴѰ�

public:
	ObjGuest(pugi::xml_node guestNode);
	~ObjGuest();

	virtual bool init();
	virtual bool deInit();
	virtual void loseHPByPlayer();
	virtual void loseHPByOther(int damage);

	int speed;
	int HP;

	bool inUse;	//������̸� true, ��������� ������ false

	Size detourSize;
	int detourSequence = -1;
	
	Raycasting* eye[3];

	StateGuest* state;

	float normalTime;
	float maxNormalTime;


};
