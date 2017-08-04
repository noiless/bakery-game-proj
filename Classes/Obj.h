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

	int objIndex;	//�� Obj�� �ν��Ͻ����� ������ �ν��Ͻ��� ���� ��ȣ
	int speed;
	int typecode = TYPECODE_NONE;	//�� Ŭ������ ������ Ÿ��
	int HP = 5;	//�� ������Ʈ�� ü��
	int dir;	//������ ����
	int qnodeIndex[4];	//�Ҽӵ� qtree node index
	float pausedTime = 0;

	cocos2d::Sprite *objImg;
	cocos2d::Vec2 moveLen;	//speed * delta��
	cocos2d::Vec2 setMoveLen(int dir, float speed);

	virtual void loseHP() {};
	virtual bool deInit() { return true; };
};
