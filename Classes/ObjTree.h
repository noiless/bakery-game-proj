#pragma once
#include "cocos2d.h"
#include "Obj.h"
#include "StateTree.h"
#include "pugixml\pugixml.hpp"


class ObjTree : public Obj {
private:
	int HP;

public: 
	ObjTree(pugi::xml_node treeNode);

	bool inUse;	//������̸� true, ��������� ������ false
	
	virtual void loseHPByPlayer();
	virtual void loseHPByOther(int damage);

	virtual bool init(cocos2d::Vec2 initPos);
	virtual bool deInit();

	StateTree * state;
};