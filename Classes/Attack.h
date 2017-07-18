#pragma once
#include <cocos2d.h>
#include <Obj.h>

USING_NS_CC;

class Attack : public Node {
private:
	Sprite * attackImg;
	Vec2 anchorDiff;
	
	bool success;	//켜진 상태에서 공격에 성공했는지 (한번 켜졌을 때 한 번의 데미지만 입힐 수 있음)

	void update(float) override;

public:
	Attack(Sprite* userSpr);
	virtual bool init(Node* caller);

	int callerDir;
	bool showing;	//현재 켜져 있는지
	
};