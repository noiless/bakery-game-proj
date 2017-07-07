#include "Player.h"
#include "GameScene.h"

#define LEFT_ARROW 0
#define RIGHT_ARROW 1
#define UP_ARROW 2
#define DOWN_ARROW 3

USING_NS_CC;

bool Player::init() {

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();

	isMoving[0] = false; isMoving[1] = false; isMoving[2] = false; isMoving[3] = false;

	moveLen = Vec2(0, 0);

	objImg = Sprite::create("test.png");
	objImg->setPosition(cocos2d::Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

	auto eventListener = EventListenerKeyboard::create();

	eventListener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event) {

		switch (keyCode) {
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
			isMoving[LEFT_ARROW] = true;
			break;
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
			isMoving[RIGHT_ARROW] = true;
			break;
		case EventKeyboard::KeyCode::KEY_UP_ARROW:
			isMoving[UP_ARROW] = true;
			break;
		case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
			isMoving[DOWN_ARROW] = true;
			break;
		}
	};

	eventListener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event) {

		switch (keyCode) {
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
			isMoving[LEFT_ARROW] = false;
			break;
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
			isMoving[RIGHT_ARROW] = false;
			break;
		case EventKeyboard::KeyCode::KEY_UP_ARROW:
			isMoving[UP_ARROW] = false;
			break;
		case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
			isMoving[DOWN_ARROW] = false;
			break;
		}

	};

	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, objImg);
	this->addChild(objImg);
	
	this->scheduleUpdate();



	

	return true;
}


bool Player::moveLenUpdate(float delta) {
	//좌
	if (isMoving[LEFT_ARROW]) {
		moveLen = Vec2(-speed * delta, 0);
	}
	//우
	else if (isMoving[RIGHT_ARROW]) {
		moveLen = Vec2(speed * delta, 0);
	}
	//상
	else if (isMoving[UP_ARROW]) {
		moveLen = Vec2(0, speed * delta);
	}
	//하
	else if (isMoving[DOWN_ARROW]) {
		moveLen = Vec2(0, -speed * delta);
	}
	else {
		moveLen = Vec2(0, 0);
		return false;	//움직이지 않고 있을 경우에만 false 반환
	}

	return true;
}

void Player::update(float delta) {

	//현재 움직이는 중인지 확인
	moveLenUpdate(delta);

	Rect exBox; //해당 방향으로 움직였을 때의 예상 바운딩박스

	cam = Camera::getDefaultCamera();

	exBox.setRect(objImg->getBoundingBox().origin.x + moveLen.x, objImg->getBoundingBox().origin.y + moveLen.y, objImg->getBoundingBox().size.width, objImg->getBoundingBox().size.height);


	//plyer only collision start
		
		//충돌 체크
		if (GameWorld::objManager->checkCollision(this, exBox, moveLen)) {
			//충돌하지 않으면 이동
			objImg->setPosition(objImg->getPositionX() + moveLen.x, objImg->getPositionY() + moveLen.y);
		}

		//플레이어 이동에 따라 카메라 이동
		cam->setPosition(objImg->getPosition());

	//plyer only collision end


		//if (GameWorld::objManager->checkCollision(this, exBox)) {
		//	//충돌하지 않으면 이동
		//	objImg->setPosition(objImg->getPositionX() + moveLen.x, objImg->getPositionY() + moveLen.y);
		//	cam->setPosition(objImg->getPosition());
		//}

	
	
}
