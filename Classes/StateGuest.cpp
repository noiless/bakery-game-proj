#include "StateGuest.h"
#include "ObjGuest.h"
#include "Raycasting.h"
#include "GameScene.h"


USING_NS_CC;


////StateGuest


StateGuestNormal* StateGuest::guestNormal = new StateGuestNormal;
StateGuestSelectShop* StateGuest::guestSelectShop = new StateGuestSelectShop;
StateGuestBuyBread* StateGuest::guestBuyBread = new StateGuestBuyBread;
StateGuestGoHome* StateGuest::guestGoHome = new StateGuestGoHome;
StateGuestDead* StateGuest::guestDead = new StateGuestDead;
StateGuestDetourNormal* StateGuest::guestDetourNormal = new StateGuestDetourNormal;

void StateGuest::doTransition(ObjGuest* obj, int source, int dest) {

	obj->pausedTime = 0;
	obj->getActionManager()->resumeTarget(obj->objImg);
	obj->getActionManager()->removeAllActionsFromTarget(obj->objImg);	//수행하던 action 제거
	
	//dead, gohome 먼저 체크
	if (dest == STATE_GUEST_DEAD) {
		obj->objImg->setTexture(Director::getInstance()->getTextureCache()->addImage("img/guest_dead_down.png"));	//sprite 이미지 재설정
		obj->state = StateGuest::guestDead;
		obj->state->initAction(obj);
	}
	else if (dest == STATE_GUEST_GOHOME) {
		obj->state = StateGuest::guestGoHome;
		obj->state->initAction(obj);
	}

	/////detour 전이
	if (dest == STATE_GUEST_DETOURNORMAL) {
		obj->state = StateGuest::guestDetourNormal;
		obj->state->initAction(obj);
	}

	if (dest == STATE_GUEST_NORMAL) {
		obj->detourSequence = -1;	//초기화

		obj->state = StateGuest::guestNormal;
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

	obj->dir = DIR_UP;

	obj->objImg->setTexture(Director::getInstance()->getTextureCache()->addImage("img/guest_down.png"));	//sprite 이미지 재설정
	obj->objImg->setOpacity(255);

	//state에 맞는 speed, actionDuration 설정
	
	actionDuration = obj->normalTime;
	obj->speed = (destPos.y - obj->getPositionY()) / actionDuration;

	MoveTo *move1 = MoveTo::create(actionDuration, destPos);	//가게 선택 위치까지 이동
	obj->moveLen = obj->setMoveLen(DIR_UP, obj->speed);	//이동 방향, 속도에 따른 moveLen 초기화
	obj->objImg->setRotation(180);

	obj->objImg->runAction(move1);
}

bool StateGuestNormal::checkTransitionCond(ObjGuest * obj) {

	if (obj->HP <= 0) {
		doTransition(obj, STATE_GUEST_NORMAL, STATE_GUEST_DEAD);
	}
	///pausedTime > 0일때 detour 상태로 전이
	else if (obj->pausedTime > 0) {
		CCLOG("1");
		doTransition(obj, STATE_GUEST_NORMAL, STATE_GUEST_DETOURNORMAL);
	}

	else if (obj->objImg->getNumberOfRunningActions() == 0) {
		doTransition(obj, STATE_GUEST_NORMAL, STATE_GUEST_SELECTSHOP);
	}

	return true;
}



//////StateGuestDetourNormal

void StateGuestDetourNormal::initAction(ObjGuest * obj) {

	obj->pausedTime = 0;	//pauseTime 초기화

	actionDuration = 1;

	//우회해야 할 오브젝트 확인, 그 boundbox size 저장

	for (int i = 0; i < 3; i++) {
		obj->eye[i]->startPoint = obj->objImg->getPosition();
		obj->eye[i]->setDir(obj->objImg->getRotation());

		ColObj* tempColObj = obj->eye[i]->doRaycast();

		if (tempColObj != nullptr) {
			obj->detourSize = tempColObj->obj->objImg->getContentSize() + obj->objImg->getContentSize() + Size(10, 10);
			break;
		}

	}

	obj->speed = 100;


	///////////////////////////////obj->detourSequence에 따라 상태 초기화 다르게

	Sequence* seq;

	if (obj->detourSequence < 0 || obj->detourSequence == 2) {

		//init to seq1
		obj->detourSequence = 1;

		//seq->setTag(1);
		MoveBy * move;

		//좌측 우회
		if (obj->objIndex % 2 == 0) {

			obj->objImg->setRotation(90);

			//시퀸스 설정
			move = MoveBy::create(actionDuration, Vec2(-obj->detourSize.width, 0));

			obj->dir = DIR_LEFT;
			
		}
		//우측 우회
		else {

			obj->objImg->setRotation(270);

			//시퀸스 설정
			move = MoveBy::create(actionDuration, Vec2(obj->detourSize.width, 0));

			obj->dir = DIR_RIGHT;

		}

		auto callback = CallFunc::create(CC_CALLBACK_0(StateGuestDetourNormal::setNextActionSeq, this, obj, 1));
		seq = Sequence::create(move, callback, nullptr);
		seq->setTag(1);

	}
	else if (obj->detourSequence == 1 || obj->detourSequence == 3) {
		//init to seq2
		obj->detourSequence = 2;

		//seq->setTag(2);
		MoveBy * move = MoveBy::create(actionDuration, Vec2(0, obj->detourSize.height));
		auto callback = CallFunc::create(CC_CALLBACK_0(StateGuestDetourNormal::setNextActionSeq, this, obj, 2));

		seq = Sequence::create(move, callback, nullptr);
		seq->setTag(2);

		obj->dir = DIR_UP;
		obj->objImg->setRotation(180);

	}
	else {
		CCASSERT(false, "GUEST : detourSequence error - detourSequence value");
	}

	obj->moveLen = obj->setMoveLen(obj->dir, obj->speed);

	obj->objImg->runAction(seq);

}

void StateGuestDetourNormal::setNextActionSeq(ObjGuest* obj, int callerTag) {


	MoveBy * move;
	
	if (callerTag == 1) {
		//init seq2
		obj->detourSequence = 2;
		move = MoveBy::create(actionDuration, Vec2(0, obj->detourSize.height));

		obj->dir = DIR_UP;
		obj->objImg->setRotation(180);

	}
	else if (callerTag == 2) {
		//init seq3
		obj->detourSequence = 3;

		//좌측 우회 -> 오른쪽으로 돌아옴
		if (obj->objIndex % 2 == 0) {
			obj->dir = DIR_RIGHT;
			obj->objImg->setRotation(270);
		}
		//우측 우회 -> 왼쪽으로 돌아옴
		else {
			obj->dir = DIR_LEFT;
			obj->objImg->setRotation(90);
		}

		move = MoveBy::create(actionDuration, Vec2(destX - obj->objImg->getPositionX(), 0));

		//objindex에 따른 moveLen, dir 설정

	}
	else if (callerTag == 3) {
		return;	//마지막 시퀸스에선 다른 동작 지정하지 않음
	}

	CallFunc* callback = CallFunc::create(CC_CALLBACK_0(StateGuestDetourNormal::setNextActionSeq, this, obj, callerTag + 1));

	Sequence* seq = Sequence::create(move, callback, nullptr);
	seq->setTag(callerTag + 1);

	obj->moveLen = obj->setMoveLen(obj->dir, obj->speed);
	obj->objImg->runAction(seq);
}


bool StateGuestDetourNormal::checkTransitionCond(ObjGuest * obj) {

	if (obj->HP <= 0) {
		doTransition(obj, STATE_GUEST_DETOURNORMAL, STATE_GUEST_DEAD);
	}
	//우회 도중에 정지시 진행중이던 액션 태그에 따라 초기화... 얘도 일정 시간 이상 멈출시
	else if (obj->pausedTime > 0.5) {
		doTransition(obj, STATE_GUEST_DETOURNORMAL, STATE_GUEST_DETOURNORMAL);
	}

	//좌측 우회
	if (obj->objIndex % 2 == 0) {

		//기존 위치로 돌아왔을시 normal로 전이
		if (obj->objImg->getNumberOfRunningActions() == 0 && obj->getPositionX() >= destX) {
			doTransition(obj, STATE_GUEST_DETOURNORMAL, STATE_GUEST_NORMAL);
		}

	}
	//우측 우회
	else {

		//기존 위치로 돌아왔을시 normal로 전이
		if (obj->objImg->getNumberOfRunningActions() == 0 && obj->getPositionX() <= destX) {
			doTransition(obj, STATE_GUEST_DETOURNORMAL, STATE_GUEST_NORMAL);
		}

	}

	return true;

}


////StateGuestSelectShop

//속도랑 위치 하드코딩했음
void StateGuestSelectShop::initAction(ObjGuest * obj) {

	actionDuration = 3;

	obj->speed = 100;

		
	MoveTo *move1;

	//dir에 따른 회전, 이동 위치 결정

	//좌
	if (obj->dir == DIR_LEFT) {
		obj->objImg->setRotation(90);
		obj->speed = abs(otherShopDest.x - obj->getPositionX()) / actionDuration;
		move1 = MoveTo::create(actionDuration, otherShopDest);
	}
	//우
	else if (obj->dir == DIR_RIGHT) {
		obj->objImg->setRotation(270);
		obj->speed = abs(myShopDest.x - obj->getPositionX()) / actionDuration;
		move1 = MoveTo::create(actionDuration, myShopDest);
	}
	//상
	else if (obj->dir == DIR_UP) {
		obj->objImg->setRotation(180);
		obj->speed = abs(homeDest.y - obj->getPositionY()) / actionDuration;
		move1 = MoveTo::create(actionDuration, homeDest);
	}
	obj->moveLen = obj->setMoveLen(obj->dir, obj->speed);
	obj->objImg->runAction(move1);
}

bool StateGuestSelectShop::checkTransitionCond(ObjGuest * obj) {

	if (obj->HP <= 0) {
		doTransition(obj, STATE_GUEST_SELECTSHOP, STATE_GUEST_DEAD);
	}
	else if (obj->objImg->getNumberOfRunningActions() == 0) {
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
	if (obj->HP <= 0) {
		doTransition(obj, STATE_GUEST_BUYBREAD, STATE_GUEST_DEAD);
	}
	else if (obj->objImg->getNumberOfRunningActions() == 0) {
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
	obj->unscheduleUpdate(); //업데이트 중지

	actionDuration = 1;

	GameWorld::objManager->deleteObjectAvailList(obj); //ObjManager에서 avail list에서 제거해줌

	//리젠율 관련 설정
	GameWorld::objManager->addBlood(obj->getParent(), obj->objImg->getPosition());


	//action 설정
	auto fadeout = FadeOut::create(actionDuration);

	auto callback = CallFunc::create(
		[=]
	{
		obj->deInit();
	});

	obj->objImg->runAction(Sequence::create(fadeout, callback, nullptr));
}

bool StateGuestDead::checkTransitionCond(ObjGuest * obj) {
	return true;
}