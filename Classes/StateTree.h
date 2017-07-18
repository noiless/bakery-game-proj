#pragma once
#include "cocos2d.h"

class ObjTree;
class StateTreeNormal;
class StateTreeDead;

class StateTree {
public:
	virtual void initAction(ObjTree * obj) = 0;

	int actionDuration = 10;	//�� action�� ������ �ð�. �� state�� ������ action���� duration�� ��� ����...?

	static StateTreeNormal* treeNormal;
	static StateTreeDead* treeDead;
};

class StateTreeNormal : public StateTree  {
public:
	StateTreeNormal();
	virtual void initAction(ObjTree * obj);
};

class StateTreeDead : public StateTree {
public:
	StateTreeDead();
	virtual void initAction(ObjTree * obj);
};