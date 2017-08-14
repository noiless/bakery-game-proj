#pragma once
#include "cocos2d.h"
#include "AudioEngine.h"
#include "pugixml\pugixml.hpp"


class ObjTree;
class StateTreeNormal;
class StateTreeDead;

USING_NS_CC;

class StateTree {
public:
	StateTree() {
		treeDeadEffect.name = "treeDeadEffect";
		treeDeadEffect.maxInstances = 10;
	}
	virtual void initAction(ObjTree * obj) = 0;
	void initStates(pugi::xml_node stateNode);

	int stateSpeed;
	int actionDuration;	//각 action이 가지는 시간. 한 state가 가지는 action들의 duration을 모두 통일...?
	experimental::AudioProfile treeDeadEffect;
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