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
	static const int MAX_OBJ_NUM = 30; //�ִ� ������Ʈ ���� (�� Ŭ���� ��)	
	ObjRabbit* objRabbitList[MAX_OBJ_NUM];	//
	ObjTree* objTreeList[MAX_OBJ_NUM];	//
	ObjSquaral* ObjSquaralList[MAX_OBJ_NUM];
	AcornAttack* ObjAcornList[MAX_OBJ_NUM];

	cocos2d::Vector<Obj*> playerCollisionList;	//
	cocos2d::Vector<Obj*> acornCollisionList;

	cocos2d::Rect mapRect;	//������ ������...?
	cocos2d::Rect mapBoundaryRect[4];

	list<Obj*> objAvailList;	//���� �۵����� ������Ʈ���� ����Ʈ

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

	//�䳢 �þ�
	bool checkSightCollision(ObjRabbit * obj);
	bool checkSightCond(int dir, float slope1, float b1, float slope2, float b2, const cocos2d::Vec2* rectPoint);
	bool checkSightCond3(int dir, cocos2d::Vec2* triP1, cocos2d::Vec2* rectPoint);

	//�ٶ��� �þ�
	Obj* checkSightCollision(ObjSquaral * obj);
	bool checkSightCond(int dir, float b1, float b2, const cocos2d::Vec2* rectPos);

	//�ٶ��� ���� Ȯ��
	bool checkAttackCollision(int callerIndex, const cocos2d::Vec2* center, float radius);

	
	
	
};