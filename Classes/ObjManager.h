#pragma once
#include <cocos2d.h>
#include <list>

USING_NS_CC;

#define MAX_RABBIT_NUM 200
#define MAX_TREE_NUM 50
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
class ObjEnemy;
class AcornAttack;
class QTree;
struct ColObj;

class ObjManager : public cocos2d::Node {	//._.
private:
	ObjRabbit* objRabbitList[MAX_RABBIT_NUM];
	ObjTree* objTreeList[MAX_TREE_NUM];
	ObjSquaral* objSquaralList[MAX_SQUARAL_NUM];
	ObjGuest* objGuestList[MAX_GUEST_NUM];
	AcornAttack* objAcornList[MAX_ACORN_NUM];

	cocos2d::Vector<Obj*> playerCollisionList;	//
	cocos2d::Vector<Obj*> acornCollisionList;

	cocos2d::Rect mapRect;	//������ ������...?
	cocos2d::Rect mapBoundaryRect[4];

	list<Obj*> objAvailList;	//���� �۵����� �浹 ������ ������Ʈ���� ����Ʈ
	list<Obj*> objUpdateList;	//���� �۵����� �̵��ϴ�(�浹üũ�� �����ϴ�) ������Ʈ���� ����Ʈ (�䳢/�ٶ���/�մ�)

	ObjRabbit* getFreeObjRabbit();
	ObjTree* getFreeObjTree();
	ObjSquaral* getFreeObjSquaral();
	ObjGuest* getFreeGuest();
	AcornAttack* getFreeAcornAttack();
	ColObj* intersectedObj;

	Vec2 createColCheck(Vec2* pos, const cocos2d::Size* size);
	bool mapBoundaryCheck(cocos2d::Rect* exBox);

	int bloodNum;

	void update(float) override;

	QTree* qtree;

public:
	ObjManager();
	void ObjInit();
	void Objdeinit();

	void setMapRect(cocos2d::Rect mapBoundingBox);

	void addObjectAvailList(Obj *obj);
	void deleteObjectAvailList(Obj *obj);
	void addObjectAvailListFRONT(Obj *obj);

	void addUpdateList(Obj* obj);
	void deleteUpdateList(Obj *obj);


	void addBlood(Node* parent, const Vec2 initPos);
	int getNumBood();

	void getObjRabbitFromPool(Node * parent, Vec2 initPos);
	void getObjTreeFromPool(Node * parent, Vec2 initPos);
	void getObjSquaralFromPool(Node * parent, Vec2 initPos);
	bool getObjGuestFromPool(Node * parent);
	void getObjAcornFromPool(Node * parent, ObjSquaral* caller);

	bool isObjColAvail(Obj * obj);

	bool checkMoveCollision(Obj *obj, cocos2d::Rect* exBox, cocos2d::Vec2* moveLen);
	bool checkAttackCollision(Obj* caller, cocos2d::Rect* exBox, bool isPlayer);


	//�䳢 �þ�
	bool checkSightCollision(ObjRabbit * obj);
	bool checkSightCond(int dir, float slope1, float b1, float slope2, float b2, const cocos2d::Vec2* rectPoint);
	bool checkSightCond3(int dir, cocos2d::Vec2* triP1, cocos2d::Vec2* rectPoint);

	//�ٶ��� �þ�
	Obj* checkSightCollision(ObjSquaral * obj);
	bool checkSightCond(int dir, float b1, float b2, const cocos2d::Vec2* rectPos);

	//�ٶ��� ���� Ȯ��
	bool checkAttackCollision(Obj* obj, const cocos2d::Vec2* center, float radius);

	//raycast Ȯ��
	ColObj* doRaycast(Obj* caller, cocos2d::Vec2 startPoint, cocos2d::Vec2 dir, float d);


};
