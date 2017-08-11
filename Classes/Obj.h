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

protected:
	void qnodeIndexInit();


public:
	Obj();
	~Obj();

	bool deadByPlayer;

	int objIndex;	//�� Obj�� �ν��Ͻ����� ������ �ν��Ͻ��� ���� ��ȣ
	int speed;
	int typecode = TYPECODE_NONE;	//�� Ŭ������ ������ Ÿ��
	int HP = 5;	//�� ������Ʈ�� ü��
	int dir;	//������ ����
	int qnodeIndex[4];	//�Ҽӵ� qtree node index
	float pausedTime;

	cocos2d::Sprite *objImg;
	cocos2d::Vec2 moveLen;	//speed * delta��
	cocos2d::Vec2 setMoveLen(int dir, float speed);

	cocos2d::Size qnodeBound;

	virtual void loseHPByPlayer() {};
	virtual void loseHPByOther(int damage) {};
	virtual bool deInit() { return true; };
};