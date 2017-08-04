#pragma once
#include <cocos2d.h>


#define DIR_NONE -1
#define DIR_LEFT 0
#define DIR_RIGHT 1
#define DIR_UP 2
#define DIR_DOWN  3

#define TYPECODE_NONE -1
#define TYPECODE_PEOPLE 0
#define TYPECODE_RABBIT 1
#define TYPECODE_TREE 2
#define TYPECODE_SQUARAL 3

class Obj : public cocos2d::Node {

private:
	static int totalObjNum;

public:
	Obj();
	~Obj();

	int objIndex;	//각 Obj의 인스턴스들이 가지는 인스턴스의 고유 번호
	int speed;
	int typecode = TYPECODE_NONE;	//각 클래스가 가지는 타입
	int HP = 5;	//각 오브젝트의 체력
	int dir;	//현재의 방향
	int qnodeIndex[4];	//소속된 qtree node index
	float pausedTime = 0;

	cocos2d::Sprite *objImg;
	cocos2d::Vec2 moveLen;	//speed * delta값
	cocos2d::Vec2 setMoveLen(int dir, float speed);

	virtual void loseHP() {};
	virtual bool deInit() { return true; };
};
