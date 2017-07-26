#pragma once
#include <cocos2d.h>
#include <list>

USING_NS_CC;

#define MAX_RABBIT_NUM 50
#define MAX_TREE_NUM 30
#define MAX_SQUARAL_NUM 30
#define MAX_GUEST_NUM 10
#define MAX_ACORN_NUM 10


using namespace std;


class Player;
class Obj;
class ObjRabbit;
class ObjTree;
class ObjSquaral;
class ObjGuest;
class AcornAttack;

class ObjManager : public cocos2d::Node {	//._.
private:
	ObjRabbit* objRabbitList[MAX_RABBIT_NUM];
	ObjTree* objTreeList[MAX_TREE_NUM];
	ObjSquaral* objSquaralList[MAX_SQUARAL_NUM];
	ObjGuest* objGuestList[MAX_GUEST_NUM];
	AcornAttack* objAcornList[MAX_ACORN_NUM];

	cocos2d::Vector<Obj*> playerCollisionList;	//
	cocos2d::Vector<Obj*> acornCollisionList;

	cocos2d::Rect mapRect;	//쓸일이 있을까...?
	cocos2d::Rect mapBoundaryRect[4];

	list<Obj*> objAvailList;	//현재 작동중인 오브젝트들의 리스트

	ObjRabbit* getFreeObjRabbit();
	ObjTree* getFreeObjTree();
	ObjSquaral* getFreeObjSquaral();
	ObjGuest* getFreeGuest();
	AcornAttack* getFreeAcornAttack();

	Vec2 createColCheck(Vec2* pos, const cocos2d::Size* size);
	bool mapBoundaryCheck(cocos2d::Rect* exBox);
	

public:
	ObjManager() {
		CCLOG("objmanager init");
	}
	
	void ObjInit();
	void Objdeinit();

	void setMapRect(cocos2d::Rect mapBoundingBox);

	void addObjectAvailList(Obj *obj);
	void deleteObjectAvailList(Obj *obj);
	void addObjectAvailListFRONT(Obj *obj);

	void getObjRabbitFromPool(Node * parent, Vec2 initPos);
	void getObjTreeFromPool(Node * parent, Vec2 initPos);
	void getObjSquaralFromPool(Node * parent, Vec2 initPos);
	bool getObjGuestFromPool(Node * parent);
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