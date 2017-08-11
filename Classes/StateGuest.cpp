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
	obj->getActionManager()->removeAllActionsFromTarget(obj->objImg);	//�����ϴ� action ����
	
	//dead, gohome ���� üũ
	if (dest == STATE_GUEST_DEAD) {
		obj->objImg->setTexture(Director::getInstance()->getTextureCache()->addImage("img/guest_dead_down.png"));	//sprite �̹��� �缳��
		obj->state = StateGuest::guestDead;
		obj->state->initAction(obj);
	}
	else if (dest == STATE_GUEST_GOHOME) {
		obj->state = StateGuest::guestGoHome;
		obj->state->initAction(obj);
	}

	/////detour ����
	if (dest == STATE_GUEST_DETOURNORMAL) {
		obj->state = StateGuest::guestDetourNormal;
		obj->state->initAction(obj);
	}

	if (dest == STATE_GUEST_NORMAL) {
		obj->detourSequence = -1;	//�ʱ�ȭ

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

	obj->dir = DIR_UP;

	obj->objImg->setTexture(Director::getInstance()->getTextureCache()->addImage("img/guest_down.png"));	//sprite �̹��� �缳��
	obj->objImg->setOpacity(255);

	//state�� �´� speed, actionDuration ����
	
	actionDuration = obj->normalTime;
	obj->speed = (destPos.y - obj->getPositionY()) / actionDuration;

	MoveTo *move1 = MoveTo::create(actionDuration, destPos);	//���� ���� ��ġ���� �̵�
	obj->moveLen = obj->setMoveLen(DIR_UP, obj->speed);	//�̵� ����, �ӵ��� ���� moveLen �ʱ�ȭ
	obj->objImg->setRotation(180);

	obj->objImg->runAction(move1);
}

bool StateGuestNormal::checkTransitionCond(ObjGuest * obj) {

	if (obj->HP <= 0) {
		doTransition(obj, STATE_GUEST_NORMAL, STATE_GUEST_DEAD);
	}
	///pausedTime > 0�϶� detour ���·� ����
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

	obj->pausedTime = 0;	//pauseTime �ʱ�ȭ

	actionDuration = 1;

	//��ȸ�ؾ� �� ������Ʈ Ȯ��, �� boundbox size ����

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


	///////////////////////////////obj->detourSequence�� ���� ���� �ʱ�ȭ �ٸ���

	Sequence* seq;

	if (obj->detourSequence < 0 || obj->detourSequence == 2) {

		//init to seq1
		obj->detourSequence = 1;

		//seq->setTag(1);
		MoveBy * move;

		//���� ��ȸ
		if (obj->objIndex % 2 == 0) {

			obj->objImg->setRotation(90);

			//������ ����
			move = MoveBy::create(actionDuration, Vec2(-obj->detourSize.width, 0));

			obj->dir = DIR_LEFT;
			
		}
		//���� ��ȸ
		else {

			obj->objImg->setRotation(270);

			//������ ����
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

		//���� ��ȸ -> ���������� ���ƿ�
		if (obj->objIndex % 2 == 0) {
			obj->dir = DIR_RIGHT;
			obj->objImg->setRotation(270);
		}
		//���� ��ȸ -> �������� ���ƿ�
		else {
			obj->dir = DIR_LEFT;
			obj->objImg->setRotation(90);
		}

		move = MoveBy::create(actionDuration, Vec2(destX - obj->objImg->getPositionX(), 0));

		//objindex�� ���� moveLen, dir ����

	}
	else if (callerTag == 3) {
		return;	//������ ���������� �ٸ� ���� �������� ����
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
	//��ȸ ���߿� ������ �������̴� �׼� �±׿� ���� �ʱ�ȭ... �굵 ���� �ð� �̻� �����
	else if (obj->pausedTime > 0.5) {
		doTransition(obj, STATE_GUEST_DETOURNORMAL, STATE_GUEST_DETOURNORMAL);
	}

	//���� ��ȸ
	if (obj->objIndex % 2 == 0) {

		//���� ��ġ�� ���ƿ����� normal�� ����
		if (obj->objImg->getNumberOfRunningActions() == 0 && obj->getPositionX() >= destX) {
			doTransition(obj, STATE_GUEST_DETOURNORMAL, STATE_GUEST_NORMAL);
		}

	}
	//���� ��ȸ
	else {

		//���� ��ġ�� ���ƿ����� normal�� ����
		if (obj->objImg->getNumberOfRunningActions() == 0 && obj->getPositionX() <= destX) {
			doTransition(obj, STATE_GUEST_DETOURNORMAL, STATE_GUEST_NORMAL);
		}

	}

	return true;

}


////StateGuestSelectShop

//�ӵ��� ��ġ �ϵ��ڵ�����
void StateGuestSelectShop::initAction(ObjGuest * obj) {

	actionDuration = 3;

	obj->speed = 100;

		
	MoveTo *move1;

	//dir�� ���� ȸ��, �̵� ��ġ ����

	//��
	if (obj->dir == DIR_LEFT) {
		obj->objImg->setRotation(90);
		obj->speed = abs(otherShopDest.x - obj->getPositionX()) / actionDuration;
		move1 = MoveTo::create(actionDuration, otherShopDest);
	}
	//��
	else if (obj->dir == DIR_RIGHT) {
		obj->objImg->setRotation(270);
		obj->speed = abs(myShopDest.x - obj->getPositionX()) / actionDuration;
		move1 = MoveTo::create(actionDuration, myShopDest);
	}
	//��
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
	//�̹��� �ٲ� �� 1�ʰ� ���
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
	obj->unscheduleUpdate(); //������Ʈ ����

	actionDuration = 1;

	GameWorld::objManager->deleteObjectAvailList(obj); //ObjManager���� avail list���� ��������

	//������ ���� ����
	GameWorld::objManager->addBlood(obj->getParent(), obj->objImg->getPosition());


	//action ����
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