#include "Player.h"
#include "GameScene.h"
#include "ObjRabbit.h"



USING_NS_CC;

bool Player::deInit() {
	return true;
}

bool Player::init() {

	GameWorld::objManager->addObjectAvailList(this);	//availList에 추가

	typecode = TYPECODE_PEOPLE;
	HP = 10;

	isMoving[0] = false; isMoving[1] = false; isMoving[2] = false; isMoving[3] = false;

	moveLen = Vec2(0, 0);

	objImg = Sprite::create("player_down.png");

	attack = new Attack(objImg);

	playerAttactEffect.name = "playerAttactEffect";
	playerAttactEffect.maxInstances = 10;

	auto eventListener = EventListenerKeyboard::create();

	eventListener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event) {

		switch (keyCode) {
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
			isMoving[DIR_LEFT] = true; dir = DIR_LEFT;
			break;

		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
			isMoving[DIR_RIGHT] = true; dir = DIR_RIGHT;
			break;

		case EventKeyboard::KeyCode::KEY_UP_ARROW:
			isMoving[DIR_UP] = true; dir = DIR_UP;
			break;

		case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
			isMoving[DIR_DOWN] = true; dir = DIR_DOWN;
			break;
		}

		//화살표와 별개로 X입력 받음
		if (keyCode == EventKeyboard::KeyCode::KEY_X) {
			if (!attack->showing) {
				experimental::AudioEngine::play2d("sound_player_attack.mp3", false, 1.0f, &playerAttactEffect);
				attack->init(objImg);
			}
		}

		attack->callerDir = dir;

	};

	eventListener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event) {

		switch (keyCode) {
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
			isMoving[DIR_LEFT] = false;
			break;
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
			isMoving[DIR_RIGHT] = false;
			break;
		case EventKeyboard::KeyCode::KEY_UP_ARROW:
			isMoving[DIR_UP] = false;
			break;
		case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
			isMoving[DIR_DOWN] = false;
			break;
		}

	};

	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, objImg);
	this->addChild(objImg);
	
	this->scheduleUpdate();

	return true;
}

void Player::loseHP() {
	HP--;
}

bool Player::setPlayerMoveLen(float actionDuration) {
	//좌
	if (isMoving[DIR_LEFT]) {
		moveLen = Vec2(-speed * actionDuration, 0);
		objImg->setRotation(90);
	}
	//우
	else if (isMoving[DIR_RIGHT]) {
		moveLen = Vec2(speed * actionDuration, 0);
		objImg->setRotation(-90);
	}
	//상
	else if (isMoving[DIR_UP]) {
		moveLen = Vec2(0, speed * actionDuration);
		objImg->setRotation(180);
	}
	//하
	else if (isMoving[DIR_DOWN]) {
		moveLen = Vec2(0, -speed * actionDuration);
		objImg->setRotation(0);
	}
	else {
		//움직이지 않을 때 false 반환
		return false;
	}

	return true;
}

void Player::update(float delta) {

	//현재 움직이는 중인지 확인
	if (setPlayerMoveLen(delta)) {

		cam = Camera::getDefaultCamera();

		//moveLen = speed * delta
		exBox.setRect(objImg->getBoundingBox().origin.x + moveLen.x, objImg->getBoundingBox().origin.y + moveLen.y, objImg->getBoundingBox().size.width, objImg->getBoundingBox().size.height);

		//plyer only collision start

			//충돌 체크
		if (GameWorld::objManager->checkMoveCollision(this, &exBox, &moveLen)) {
			//충돌하지 않으면 이동
			objImg->setPosition(objImg->getPositionX() + moveLen.x, objImg->getPositionY() + moveLen.y);
		}

		//플레이어 이동에 따라 카메라 이동
		cam->setPosition(objImg->getPosition());

		//plyer only collision end


	}
	
	
}
