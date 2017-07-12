#pragma once
#include <cocos2d.h>
#include <list>
#include <Obj.h>
#include <ObjRabbit.h>

using namespace std;


class Player;

class ObjManager {
private:
	static const int MAX_OBJ_NUM = 20; //�ִ� ������Ʈ ���� (�� Ŭ���� ��)
	list<Obj*> objAvailList;	//���� �۵����� ������Ʈ���� ����Ʈ
	ObjRabbit* objRabbitList[MAX_OBJ_NUM];	//
public:
	ObjManager();
	void addObject(Obj *obj);
	void deleteObject(Obj *obj);

	void ObjInit();

	ObjRabbit* getFreeObjRabbit();

	bool checkMoveCollision(Obj *obj, cocos2d::Rect exBox, cocos2d::Vec2 moveLen);	//player��

	//�䳢 �þ�
	bool checkSightCollision(ObjRabbit * obj);
	bool checkSightCond(int dir, float slope1, float b1, float slope2, float b2, cocos2d::Vec2 rectPoint);
	bool checkSightCond3(int dir, cocos2d::Vec2 triP1, cocos2d::Vec2 rectPoint);
	
};