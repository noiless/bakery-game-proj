#pragma once
#include <cocos2d.h>
#include <list>
#include <Obj.h>
#include <ObjRabbit.h>
#include <ObjTree.h>

using namespace std;


class Player;

class ObjManager : public cocos2d::Node {	//._.
private:
	static const int MAX_OBJ_NUM = 20; //�ִ� ������Ʈ ���� (�� Ŭ���� ��)	
	ObjRabbit* objRabbitList[MAX_OBJ_NUM];	//
	ObjTree* objTreeList[MAX_OBJ_NUM];	//

	cocos2d::Vector<Obj*> playerCollisionList;	//

	list<Obj*> objAvailList;	//���� �۵����� ������Ʈ���� ����Ʈ
	

public:
	ObjManager();
	
	void ObjInit();

	void addObjectAvailList(Obj *obj);
	void deleteObjectAvailList(Obj *obj);

	ObjRabbit* getFreeObjRabbit();
	ObjTree* getFreeObjTree();

	bool checkMoveCollision(Obj *obj, cocos2d::Rect* exBox, cocos2d::Vec2* moveLen);
	bool checkAttackCollision(cocos2d::Rect* exBox);

	//�䳢 �þ�
	bool checkSightCollision(ObjRabbit * obj);
	bool checkSightCond(int dir, float slope1, float b1, float slope2, float b2, cocos2d::Vec2* rectPoint);
	bool checkSightCond3(int dir, cocos2d::Vec2* triP1, cocos2d::Vec2* rectPoint);
	
};