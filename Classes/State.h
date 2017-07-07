#pragma once
#include "cocos2d.h"

class Obj;

class State {
public:
	int nextStateNum;

	State *nextState;

	virtual void doAction() = 0;	//

	//make Action, Sequence, Spawn, ... and this->runAction();
	virtual void initAction(Obj* obj) = 0;

};