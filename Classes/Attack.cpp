#include "Attack.h"
#include "GameScene.h"

Attack::Attack(Sprite* userSpr) {
	showing = false;	//기본
	success = false;
	attackImg = Sprite::create("attack.png");
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
		this->unscheduleUpdate();
		this->removeFromParent();

	});

	this->runAction(Sequence::create(DelayTime::create(0.15), callback, nullptr));

	return true;
}

void Attack::update(float) {

	if (!success) {
		//newOrigin
		Vec2 p = attackImg->convertToWorldSpace(Vec2(attackImg->getBoundingBox().origin.x, attackImg->getBoundingBox().origin.y) - anchorDiff);


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