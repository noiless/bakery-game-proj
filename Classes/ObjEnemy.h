#pragma once

#include "cocos2d.h"
#include "GameScene.h"
#include "Obj.h"
#include "StateEnemy.h"

USING_NS_CC;


class ObjEnemy : public Obj {
private:
	void update(float) override;
	int HP;	//남은 HP의 양에 따라 스프라이트 변경

public:

	ObjEnemy();
	~ObjEnemy();

	virtual bool init(cocos2d::Vec2 initPos);
	virtual bool deInit();
	virtual void loseHP();

	StateEnemy* state;

	//StateHPEnemy* stateHP;	//enemy의 HP에 따른 Sprite만 변경함

	//raycast eye

};