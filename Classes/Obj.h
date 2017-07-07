#pragma once
#include <cocos2d.h>
#include <State.h>

class Obj : public cocos2d::Node {
private:
	static int totalObjNum;
public:
	Obj(Obj *obj);
	~Obj();

	int objIndex;	//�� Obj�� �ν��Ͻ����� ������ �ν��Ͻ��� ���� ��ȣ
	int speed;
	cocos2d::Sprite *objImg;
};