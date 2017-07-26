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
	
	//dead, gohome ���� üũ
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
		//���� ��µ� �����ߴٸ�
		if ((obj->dir == DIR_LEFT && GameWorld::ui->buyBread(false)) 
			|| (obj->dir == DIR_RIGHT && GameWorld::ui->buyBread(true))) {
			obj->objImg->setTexture(Director::getInstance()->getTextureCache()->addImage("img/guest_buybread_down.png"));	//sprite �̹��� �缳��
		}
		obj->state = StateGuest::guestBuyBread;
		obj->state->initAction(obj);
	}
}

////StateGuestNormal

void StateGuestNormal::initAction(ObjGuest * obj) {

	obj->objImg->setTexture(Director::getInstance()->getTextureCache()->addImage("img/guest_down.png"));	//sprite �̹��� �缳��
	obj->objImg->setOpacity(255);

	//state�� �´� speed, actionDuration ����
	
	actionDuration = 5;
	obj->speed = (abs(obj->getPositionY()) + 1800) / actionDuration;

	MoveTo *move1 = MoveTo::create(actionDuration, Vec2(0, 1600));
	obj->moveLen = obj->setMoveLen(DIR_UP, obj->speed);	//�̵� ����, �ӵ��� ���� moveLen �ʱ�ȭ
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

	//���ǵ�� moveLen�� �ȹٲ� �浹üũ �̻�ä���

	MoveBy *move1;

	//dir�� ���� ȸ��, �̵� ��ġ ����

	//��
	if (obj->dir == DIR_LEFT) {
		obj->objImg->setRotation(90);
		move1 = MoveBy::create(actionDuration, Vec2(-300,0));
	}
	//��
	else if (obj->dir == DIR_RIGHT) {
		obj->objImg->setRotation(270);
		move1 = MoveBy::create(actionDuration, Vec2(300,0));
	}
	//��
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
	//�̹��� �ٲ� �� 1�ʰ� ���
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
	//1�ʰ� fadeout �� ������Ʈ �Ҹ� ���� ó��
	actionDuration = 1;

	obj->unscheduleUpdate(); //������Ʈ ����

	GameWorld::objManager->deleteObjectAvailList(obj); //ObjManager���� avail list���� ��������

	//action ����
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