#pragma once
#include "cocos2d.h"
#include "Obj.h"
#include "StateTree.h"

class ObjTree : public Obj {
private:
	int HP;

public: 
	ObjTree();

	bool inUse;	//������̸� true, ��������� ������ false
	
	virtual void loseHP();

	virtual bool init(cocos2d::Vec2 initPos);
	virtual bool deInit();

	StateTree * state;
};