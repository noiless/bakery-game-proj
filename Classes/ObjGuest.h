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
	cocos2d::Vec2 initPos;	//static이어야 하는 것이 아닌가

public:
	ObjGuest(pugi::xml_node guestNode);
	~ObjGuest();

	virtual bool init();
	virtual bool deInit();
	virtual void loseHPByPlayer();
	virtual void loseHPByOther(int damage);

	int speed;
	int HP;

	bool inUse;	//사용중이면 true, 사용중이지 않으면 false

	Size detourSize;
	int detourSequence = -1;
	
	Raycasting* eye[3];

	StateGuest* state;

	float normalTime;
	float maxNormalTime;


};
