#include "OverScene.h"
#include "GameScene.h"
#include "Score.h"

USING_NS_CC;

Scene* GameOver::createScene()
{
	return GameOver::create();

}

// on "init" you need to initialize your instance
bool GameOver::init()
{

	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	cocos2d::Label * label1 = Label::createWithTTF("GAME OVER", "fonts/arial.ttf", 40);
	label1->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));

	cocos2d::Label * label2 = Label::createWithTTF("Press z to restart", "fonts/arial.ttf", 20);
	label2->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 4));

	char text[256];
	sprintf(text, "Final Score : %d", Score::finalScore);

	cocos2d::Label * label3 = Label::createWithTTF("final score", "fonts/arial.ttf", 30);
	label3->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 3));
	label3->setString(text);

	this->addChild(label1);
	this->addChild(label2);
	this->addChild(label3);

	auto eventListener = EventListenerKeyboard::create();

	eventListener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event) {
		//°ÔÀÓ ¾ÀÀ¸·Î ¾À º¯°æ
		if (keyCode == EventKeyboard::KeyCode::KEY_Z) {

			auto gameScene = GameWorld::createScene();
			Director::getInstance()->replaceScene(gameScene);
			//

		}

	};

	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, this);

	return true;

}