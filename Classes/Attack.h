#pragma once
#include <cocos2d.h>
#include "ObjSquaral.h"
#include <Obj.h>

USING_NS_CC;

class Attack : public Node {
protected:
	Sprite * attackImg;
	Vec2 anchorDiff;
	
	bool success;	//���� ���¿��� ���ݿ� �����ߴ��� (�ѹ� ������ �� �� ���� �������� ���� �� ����)

	void update(float) override;

public:
	Attack() {}
	Attack(Sprite* userSpr);
	virtual bool init(Node* caller);

	int callerDir;
	bool showing;	//���� ���� �ִ���
	
};

class AcornAttack : public Attack {
private:
	void update(float) override;
public:
	AcornAttack();
	int callerIndex;
	int inUse;
	virtual bool init(ObjSquaral* caller);
};