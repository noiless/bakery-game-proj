#pragma once
#include <cocos2d.h>
#include <State.h>

class Obj : public cocos2d::Node {
private:
	static int totalObjNum;
public:
	Obj(Obj *obj);
	~Obj();

	int objIndex;	//각 Obj의 인스턴스들이 가지는 인스턴스의 고유 번호
	int speed;
	cocos2d::Sprite *objImg;
};