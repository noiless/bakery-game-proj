#pragma once
#include <cocos2d.h>
#include <Obj.h>

USING_NS_CC;

class Attack : public Node {
public:
	Attack(Sprite* userSpr);
	Sprite * attackImg;
	void update(float) override;
	virtual bool init(Node* caller);
	int callerDir;

	bool showing;	//���� ���� �ִ���
	bool success;	//���� ���¿��� ���ݿ� �����ߴ��� (�ѹ� ������ �� �� ���� �������� ���� �� ����)

	Vec2 anchorDiff;
};