#include "Attack.h"
#include "GameScene.h"

Attack::Attack(Sprite* userSpr) {
	showing = false;	//기본
	success = false;
	attackImg = Sprite::create("img/attack.png");
	attackImg->setAnchorPoint(Vec2(0.5, 1));
	attackImg->setPosition(userSpr->getContentSize() / 2);	//원점
	attackImg->setPositionY(attackImg->getPosition().y - userSpr->getContentSize().height / 2 - 1);
	this->addChild(attackImg);

	anchorDiff = attackImg->getBoundingBox().origin;
	
}

bool Attack::init(Node* caller){
	showing = true;
	success = false;
	this->scheduleUpdate();

	caller->addChild(this);
	
	auto callback = CallFunc::create(
		[=]()
	{
		showing = false;	//보이지 않게 변경
		success = false;
		this->removeFromParent();
		this->unscheduleUpdate();		

	});

	this->runAction(Sequence::create(DelayTime::create(0.15), callback, nullptr));

	return true;
}

void Attack::update(float) {

	if (!success) {
		//newOrigin
		Vec2 p = attackImg->convertToWorldSpace(attackImg->getBoundingBox().origin - anchorDiff);

		switch (callerDir) {
		case DIR_LEFT:
			p.y = p.y - attackImg->getBoundingBox().size.width;
			break;
		case DIR_RIGHT:
			p.x = p.x - attackImg->getBoundingBox().size.height;
			break;
		case DIR_DOWN:
			break;
		case DIR_UP:
			p.x = p.x - attackImg->getBoundingBox().size.width;
			p.y = p.y - attackImg->getBoundingBox().size.height;
			break;
		}

		if ((callerDir == DIR_LEFT) || (callerDir == DIR_RIGHT)) {
			success = GameWorld::objManager->checkAttackCollision(&Rect(p.x, p.y, attackImg->getBoundingBox().size.height, attackImg->getBoundingBox().size.width));
		}
		//dir == up || down
		else {
			success = GameWorld::objManager->checkAttackCollision(&Rect(p.x, p.y, attackImg->getBoundingBox().size.width, attackImg->getBoundingBox().size.height));
		}
		
	}
	
}

/////class AcornAttack

AcornAttack::AcornAttack()  : Attack() {
	attackImg = Sprite::create("img/acorn.png");
	inUse = false;
	this->addChild(attackImg);
}


AcornAttack::~AcornAttack() {
	attackImg->removeFromParentAndCleanup(true);
	this->removeFromParentAndCleanup(true);
}

bool AcornAttack::init(ObjSquaral* caller) {
	attackImg->setPosition(caller->objImg->getPosition());
	attackImg->setRotation(caller->objImg->getRotation());
	//position, rotation 설정

	inUse = true;

	this->scheduleUpdate();

	callerIndex = caller->objIndex;

	MoveBy* move1 = MoveBy::create(0.5, Vec2(caller->squaralSightRadius * 2 * cos(CC_DEGREES_TO_RADIANS(attackImg->getRotation() + 90)), caller->squaralSightRadius * 2 * sin(- CC_DEGREES_TO_RADIANS(attackImg->getRotation() + 90))));

	auto callback = CallFunc::create(
		[=]()
	{
		inUse = false;
		this->removeFromParent();
		this->unscheduleUpdate();
		
	});

	this->attackImg->runAction(Sequence::create(move1, callback, nullptr));

	return true;
}


void AcornAttack::update(float delta) {

	//다른 오브젝트와 충돌확인

	if (GameWorld::objManager->checkAttackCollision(callerIndex, &attackImg->getPosition(), attackImg->getContentSize().width / 2)) {
		attackImg->getActionManager()->removeAllActionsFromTarget(attackImg);
		inUse = false;
		this->removeFromParent();
		this->unscheduleUpdate();
	}
}