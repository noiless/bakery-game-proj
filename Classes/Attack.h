#pragma once
#include <cocos2d.h>
#include <Obj.h>

USING_NS_CC;

class Attack : public Node {
private:
	Sprite * attackImg;
	Vec2 anchorDiff;
	
	bool success;	//���� ���¿��� ���ݿ� �����ߴ��� (�ѹ� ������ �� �� ���� �������� ���� �� ����)

	void update(float) override;

public:
	Attack(Sprite* userSpr);
	virtual bool init(Node* caller);

	int callerDir;
	bool showing;	//���� ���� �ִ���
	
};