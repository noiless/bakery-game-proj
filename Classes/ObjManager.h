#pragma once
#include <cocos2d.h>
#include <list>
#include <Obj.h>
#include <ObjRabbit.h>
#include <ObjTree.h>
#include <ObjSquaral.h>
#include <Attack.h>

using namespace std;


class Player;

class ObjManager : public cocos2d::Node {	//._.
private:
	static const int MAX_OBJ_NUM = 30; //최대 오브젝트 갯수 (각 클래스 별)	
	ObjRabbit* objRabbitList[MAX_OBJ_NUM];	//
	ObjTree* objTreeList[MAX_OBJ_NUM];	//
	ObjSquaral* ObjSquaralList[MAX_OBJ_NUM];
	AcornAttack* ObjAcornList[MAX_OBJ_NUM];

	cocos2d::Vector<Obj*> playerCollisionList;	//
	cocos2d::Vector<Obj*> acornCollisionList;

	cocos2d::Rect mapRect;	//쓸일이 있을까...?
	cocos2d::Rect mapBoundaryRect[4];

	list<Obj*> objAvailList;	//현재 작동중인 오브젝트들의 리스트

	ObjRabbit* getFreeObjRabbit();
	ObjTree* getFreeObjTree();
	ObjSquaral* getFreeObjSquaral();
	AcornAttack* getFreeAcornAttack();

	Vec2 createColCheck(Vec2* pos, const cocos2d::Size* size);
	bool mapBoundaryCheck(cocos2d::Rect* exBox);
	

public:
	ObjManager() {

	}
	
	void ObjInit();

	void setMapRect(cocos2d::Rect mapBoundingBox);

	void addObjectAvailList(Obj *obj);
	void deleteObjectAvailList(Obj *obj);

	void getObjRabbitFromPool(Node * parent, Vec2 initPos);
	void getObjTreeFromPool(Node * parent, Vec2 initPos);
	void getObjSquaralFromPool(Node * parent, Vec2 initPos);
	void getObjAcornFromPool(Node * parent, ObjSquaral* caller);

	bool checkMoveCollision(Obj *obj, cocos2d::Rect* exBox, cocos2d::Vec2* moveLen);
	bool checkAttackCollision(cocos2d::Rect* exBox);

	//토끼 시야
	bool checkSightCollision(ObjRabbit * obj);
	bool checkSightCond(int dir, float slope1, float b1, float slope2, float b2, const cocos2d::Vec2* rectPoint);
	bool checkSightCond3(int dir, cocos2d::Vec2* triP1, cocos2d::Vec2* rectPoint);

	//다람쥐 시야
	Obj* checkSightCollision(ObjSquaral * obj);
	bool checkSightCond(int dir, float b1, float b2, const cocos2d::Vec2* rectPos);

	//다람쥐 공격 확인
	bool checkAttackCollision(int callerIndex, const cocos2d::Vec2* center, float radius);

	
	
	
};