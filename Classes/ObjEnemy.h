#pragma once

#include "cocos2d.h"
#include "GameScene.h"
#include "Obj.h"
#include "pugixml\pugixml.hpp"


class StateEnemy;
class StateHPEnemy;
class Raycasting;
class EnemyAttack;

USING_NS_CC;


class ObjEnemy : public Obj {
private:
	void update(float) override;
	int HP;	//���� HP�� �翡 ���� ��������Ʈ ����

public:

	ObjEnemy(pugi::xml_node enemyNode);
	~ObjEnemy();

	virtual bool init(cocos2d::Vec2 initPos);
	virtual bool deInit();
	virtual void loseHPByPlayer();
	virtual void loseHPByOther(int damage);

	StateEnemy* state;

	StateHPEnemy* stateHP;	//enemy�� HP�� ���� Sprite�� ������

	Raycasting* eye[3];
	int colEyeIndex;

	void updateEye();

	Obj* target;

	EnemyAttack * attack;

};

struct ColObj {
	Obj* obj;
	Vec2 intersectPoint;
	float intersectDistance;
};