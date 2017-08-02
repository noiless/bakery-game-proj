#pragma once
#include "cocos2d.h"

class ObjGuest;
class StateGuestNormal;
class StateGuestSelectShop;
class StateGuestBuyBread;
class StateGuestGoHome;
class StateGuestDead;

USING_NS_CC;

enum { STATE_GUEST_NORMAL, STATE_GUEST_SELECTSHOP, STATE_GUEST_BUYBREAD, STATE_GUEST_GOHOME, STATE_GUEST_DEAD };

class StateGuest {
public:
	StateGuest() {
	}
	virtual void initAction(ObjGuest * obj) = 0;
	virtual bool checkTransitionCond(ObjGuest * obj) = 0;
	void doTransition(ObjGuest* obj, int source, int dest);

	int actionDuration;	//각 action이 가지는 시간. 한 state가 가지는 action들의 duration을 모두 통일...?

	static StateGuestNormal* guestNormal;
	static StateGuestSelectShop* guestSelectShop;
	static StateGuestBuyBread* guestBuyBread;
	static StateGuestGoHome* guestGoHome;
	static StateGuestDead* guestDead;
};

class StateGuestNormal : public StateGuest {
public:
	StateGuestNormal() {
	};
	virtual void initAction(ObjGuest * obj);
	virtual bool checkTransitionCond(ObjGuest * obj);
};

class StateGuestSelectShop : public StateGuest {
public:
	StateGuestSelectShop() {
	};
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