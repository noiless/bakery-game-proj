#include "StateGuest.h"
#include "ObjGuest.h"
#include "GameScene.h"

////StateGuest


StateGuestNormal* StateGuest::guestNormal = new StateGuestNormal;
StateGuestSelectShop* StateGuest::guestSelectShop = new StateGuestSelectShop;
StateGuestBuyBread* StateGuest::guestBuyBread = new StateGuestBuyBread;
StateGuestGoHome* StateGuest::guestGoHome = new StateGuestGoHome;
StateGuestDead* StateGuest::guestDead = new StateGuestDead;

void StateGuest::doTransition(ObjGuest* obj, int source, int dest) {
	
	//dead, gohome 먼저 체크
	if (dest == STATE_GUEST_GOHOME) {
		obj->state = StateGuest::guestGoHome;
		obj->state->initAction(obj);
	}

	if (source == STATE_GUEST_NORMAL && dest == STATE_GUEST_SELECTSHOP) {
		obj->state = StateGuest::guestSelectShop;	
		obj->dir = GameWorld::ui->selectShop();
		obj->moveLen =  obj->setMoveLen(obj->dir, obj->speed);

		obj->state->initAction(obj);
	}
	else if (source == STATE_GUEST_SELECTSHOP && dest == STATE_GUEST_BUYBREAD) {
		//빵을 사는데 성공했다면
		if ((obj->dir == DIR_LEFT && GameWorld::ui->buyBread(false)) 
			|| (obj->dir == DIR_RIGHT && GameWorld::ui->buyBread(true))) {
			obj->objImg->setTexture(Director::getInstance()->getTextureCache()->addImage("img/guest_buybread_down.png"));	//sprite 이미지 재설정
		}
		obj->state = StateGuest::guestBuyBread;
		obj->state->initAction(obj);
	}
}

////StateGuestNormal

void StateGuestNormal::initAction(ObjGuest * obj) {

	obj->objImg->setTexture(Director::getInstance()->getTextureCache()->addImage("img/guest_down.png"));	//sprite 이미지 재설정
	obj->objImg->setOpacity(255);

	//state에 맞는 speed, actionDuration 설정
	
	actionDuration = 5;
	obj->speed = (abs(obj->getPositionY()) + 1800) / actionDuration;

	MoveTo *move1 = MoveTo::create(actionDuration, Vec2(0, 1600));
	obj->moveLen = obj->setMoveLen(DIR_UP, obj->speed);	//이동 방향, 속도에 따른 moveLen 초기화
	obj->objImg->setRotation(180);

	obj->objImg->runAction(move1);
}

bool StateGuestNormal::checkTransitionCond(ObjGuest * obj) {
	if (obj->objImg->getNumberOfRunningActions() == 0) {
		doTransition(obj, STATE_GUEST_NORMAL, STATE_GUEST_SELECTSHOP);
	}

	return true;
}


////StateGuestSelectShop

void StateGuestSelectShop::initAction(ObjGuest * obj) {

	actionDuration = 3;

	//스피드랑 moveLen이 안바뀌어서 충돌체크 이사ㅓㅇ함

	MoveBy *move1;

	//dir에 따른 회전, 이동 위치 결정

	//좌
	if (obj->dir == DIR_LEFT) {
		obj->objImg->setRotation(90);
		move1 = MoveBy::create(actionDuration, Vec2(-300,0));
	}
	//우
	else if (obj->dir == DIR_RIGHT) {
		obj->objImg->setRotation(270);
		move1 = MoveBy::create(actionDuration, Vec2(300,0));
	}
	//상
	else if (obj->dir == DIR_UP) {
		obj->objImg->setRotation(180);
		move1 = MoveBy::create(actionDuration, Vec2(0,150));
	}

	obj->objImg->runAction(move1);
}

bool StateGuestSelectShop::checkTransitionCond(ObjGuest * obj) {
	if (obj->objImg->getNumberOfRunningActions() == 0) {
		if (obj->dir == DIR_UP) {
			doTransition(obj, STATE_GUEST_SELECTSHOP, STATE_GUEST_GOHOME);
		}
		else {
			doTransition(obj, STATE_GUEST_SELECTSHOP, STATE_GUEST_BUYBREAD);
		}
		
	}

	return true;
}


////StateGuestBuyBread

void StateGuestBuyBread::initAction(ObjGuest * obj) {
	//이미지 바뀐 후 1초간 대기
	actionDuration = 1;
	obj->objImg->runAction(DelayTime::create(actionDuration));
}

bool StateGuestBuyBread::checkTransitionCond(ObjGuest * obj) {
	if (obj->objImg->getNumberOfRunningActions() == 0) {
		doTransition(obj, STATE_GUEST_BUYBREAD, STATE_GUEST_GOHOME);
	}

	return true;
}


////StateGuestGoHome

void StateGuestGoHome::initAction(ObjGuest * obj) {
	//1초간 fadeout 후 오브젝트 소멸 관련 처리
	actionDuration = 1;

	obj->unscheduleUpdate(); //업데이트 중지

	GameWorld::objManager->deleteObjectAvailList(obj); //ObjManager에서 avail list에서 제거해줌

	//action 설정
	auto fadeout = FadeOut::create(actionDuration);

	auto callback = CallFunc::create(
		[=]
	{
		obj->deInit();
	});

	obj->objImg->runAction(Sequence::create(fadeout, callback, nullptr));
}

bool StateGuestGoHome::checkTransitionCond(ObjGuest * obj) {
	return true;
}



////StateGuestDead

void StateGuestDead::initAction(ObjGuest * obj) {

}

bool StateGuestDead::checkTransitionCond(ObjGuest * obj) {
	return true;
}