#pragma once
#include <cocos2d.h>
#include <list>
#include <Obj.h>
#include <ObjRabbit.h>

using namespace std;


class Player;

class ObjManager : public cocos2d::Node {	//._.
private:
	static const int MAX_OBJ_NUM = 20; //최대 오브젝트 갯수 (각 클래스 별)
	list<Obj*> objAvailList;	//현재 작동중인 오브젝트들의 리스트
	ObjRabbit* objRabbitList[MAX_OBJ_NUM];	//
	cocos2d::Vector<Obj*> playerCollisionList;	//

public:
	ObjManager();
	void addObjectAvailList(Obj *obj);
	void deleteObjectAvailList(Obj *obj);

	void ObjInit();

	ObjRabbit* getFreeObjRabbit();

	bool checkMoveCollision(Obj *obj, cocos2d::Rect* exBox, cocos2d::Vec2* moveLen);
	bool checkAttackCollision(cocos2d::Rect* exBox);

	//토끼 시야
	bool checkSightCollision(ObjRabbit * obj);
	bool checkSightCond(int dir, float slope1, float b1, float slope2, float b2, cocos2d::Vec2* rectPoint);
	bool checkSightCond3(int dir, cocos2d::Vec2* triP1, cocos2d::Vec2* rectPoint);
	
};