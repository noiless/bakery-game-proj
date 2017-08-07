#pragma once

#include "cocos2d.h"
#include "GameScene.h"
#include "Obj.h"
#include "StateEnemy.h"

USING_NS_CC;


class ObjEnemy : public Obj {
private:
	void update(float) override;
	int HP;	//���� HP�� �翡 ���� ��������Ʈ ����

public:

	ObjEnemy();
	~ObjEnemy();

	virtual bool init(cocos2d::Vec2 initPos);
	virtual bool deInit();
	virtual void loseHP();

	StateEnemy* state;

	//StateHPEnemy* stateHP;	//enemy�� HP�� ���� Sprite�� ������

	//raycast eye

};