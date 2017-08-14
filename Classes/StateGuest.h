#pragma once
#include "cocos2d.h"
#include "pugixml\pugixml.hpp"


class ObjGuest;
class StateGuestNormal;
class StateGuestSelectShop;
class StateGuestBuyBread;
class StateGuestGoHome;
class StateGuestDead;
class StateGuestDetourNormal;

USING_NS_CC;

enum { STATE_GUEST_NORMAL, STATE_GUEST_DETOURNORMAL,  STATE_GUEST_SELECTSHOP, STATE_GUEST_BUYBREAD, STATE_GUEST_GOHOME, STATE_GUEST_DEAD };

class StateGuest {
public:
	StateGuest() {
	}
	virtual void initAction(ObjGuest * obj) = 0;
	virtual bool checkTransitionCond(ObjGuest * obj) = 0;
	void doTransition(ObjGuest* obj, int source, int dest);
	void initStates(pugi::xml_node stateNode);

	float actionDuration;	//각 action이 가지는 시간. 한 state가 가지는 action들의 duration을 모두 통일...?

	static StateGuestNormal* guestNormal;
	static StateGuestSelectShop* guestSelectShop;
	static StateGuestBuyBread* guestBuyBread;
	static StateGuestGoHome* guestGoHome;
	static StateGuestDead* guestDead;
	static StateGuestDetourNormal* guestDetourNormal;
};

class StateGuestNormal : public StateGuest {
public:
	StateGuestNormal() {
		destPos = Vec2(0, 1400);
	};
	Vec2 destPos;
	virtual void initAction(ObjGuest * obj);
	virtual bool checkTransitionCond(ObjGuest * obj);
};

class StateGuestDetourNormal : public StateGuest {
private:
	float destX;	//목표 x좌표

public:
	StateGuestDetourNormal() {
		destX = 0;

	};
	virtual void initAction(ObjGuest * obj);
	virtual bool checkTransitionCond(ObjGuest * obj);
	void setNextActionSeq(ObjGuest* obj, int callerTag);

};

class StateGuestSelectShop : public StateGuest {
public:
	StateGuestSelectShop() {
		otherShopDest = Vec2(-750, 1400);
		myShopDest = Vec2(750, 1400);
		homeDest = Vec2(0, 1800);
	};
	Vec2 otherShopDest;
	Vec2 myShopDest;
	Vec2 homeDest;

	virtual void initAction(ObjGuest * obj);
	virtual bool checkTransitionCond(ObjGuest * obj);
};

class StateGuestBuyBread : public StateGuest {
public:
	StateGuestBuyBread() {
	};
	virtual void initAction(ObjGuest * obj);
	virtual bool checkTransitionCond(ObjGuest * obj);
};

class StateGuestGoHome : public StateGuest {
public:
	StateGuestGoHome() {

	};
	virtual void initAction(ObjGuest * obj);
	virtual bool checkTransitionCond(ObjGuest * obj);
};

class StateGuestDead : public StateGuest{
public:
	StateGuestDead() {

	};
	virtual void initAction(ObjGuest * obj);
	virtual bool checkTransitionCond(ObjGuest * obj);
};