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
	
	bool success;	//���� ���¿��� ���ݿ� �����ߴ��� (�ѹ� ������ �� �� ���� �������� ���� �� ����)
	void update(float) override;

public:
	Attack() {}
	Attack(Sprite* userSpr);
	virtual bool init(Obj* caller);

	int callerDir;
	bool showing;	//���� ���� �ִ���
	
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