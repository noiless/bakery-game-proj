#pragma once
#include "cocos2d.h"

class ObjTree;
class StateTreeNormal;
class StateTreeDead;

class StateTree {
public:
	virtual void initAction(ObjTree * obj) = 0;

	int actionDuration = 10;	//각 action이 가지는 시간. 한 state가 가지는 action들의 duration을 모두 통일...?

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