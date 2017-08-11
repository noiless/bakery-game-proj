#pragma once
#include <cocos2d.h>
#include <Obj.h>

class ObjSquaral;
class ObjEnemy;

USING_NS_CC;

class Attack : public Node {
protected:
	Sprite * attackImg;
	Vec2 anchorDiff;
	Obj* caller;
	
	bool success;	//켜진 상태에서 공격에 성공했는지 (한번 켜졌을 때 한 번의 데미지만 입힐 수 있음)
	void update(float) override;

public:
	Attack() {}
	Attack(Sprite* userSpr);
	virtual bool init(Obj* caller);

	int callerDir;
	bool showing;	//현재 켜져 있는지
	
};

class AcornAttack : public Attack {
private:
	void update(float) override;
public:
	AcornAttack();
	~AcornAttack();
	int callerIndex;
	int inUse;
	virtual bool init(ObjSquaral* caller);
};

class EnemyAttack : public Attack {
private:
	void update(float) override;
	int callerDir;
public:
	EnemyAttack(Sprite* enemySpr);
	~EnemyAttack();
	virtual bool init(ObjEnemy* caller);

};