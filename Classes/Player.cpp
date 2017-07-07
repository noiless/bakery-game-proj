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
	//��
	if (isMoving[LEFT_ARROW]) {
		moveLen = Vec2(-speed * delta, 0);
	}
	//��
	else if (isMoving[RIGHT_ARROW]) {
		moveLen = Vec2(speed * delta, 0);
	}
	//��
	else if (isMoving[UP_ARROW]) {
		moveLen = Vec2(0, speed * delta);
	}
	//��
	else if (isMoving[DOWN_ARROW]) {
		moveLen = Vec2(0, -speed * delta);
	}
	else {
		moveLen = Vec2(0, 0);
		return false;	//�������� �ʰ� ���� ��쿡�� false ��ȯ
	}

	return true;
}

void Player::update(float delta) {

	//���� �����̴� ������ Ȯ��
	moveLenUpdate(delta);

	Rect exBox; //�ش� �������� �������� ���� ���� �ٿ���ڽ�

	cam = Camera::getDefaultCamera();

	exBox.setRect(objImg->getBoundingBox().origin.x + moveLen.x, objImg->getBoundingBox().origin.y + moveLen.y, objImg->getBoundingBox().size.width, objImg->getBoundingBox().size.height);


	//plyer only collision start
		
		//�浹 üũ
		if (GameWorld::objManager->checkCollision(this, exBox, moveLen)) {
			//�浹���� ������ �̵�
			objImg->setPosition(objImg->getPositionX() + moveLen.x, objImg->getPositionY() + moveLen.y);
		}

		//�÷��̾� �̵��� ���� ī�޶� �̵�
		cam->setPosition(objImg->getPosition());

	//plyer only collision end


		//if (GameWorld::objManager->checkCollision(this, exBox)) {
		//	//�浹���� ������ �̵�
		//	objImg->setPosition(objImg->getPositionX() + moveLen.x, objImg->getPositionY() + moveLen.y);
		//	cam->setPosition(objImg->getPosition());
		//}

	
	
}
