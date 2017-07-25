#include "OverScene.h"
#include "GameScene.h"

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

	cocos2d::Label * label1 = Label::createWithTTF("GAME OVER", "fonts/arial.ttf", 30);
	label1->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));

	cocos2d::Label * label2 = Label::createWithTTF("Press z to restart", "fonts/arial.ttf", 20);
	label2->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 3));

	this->addChild(label1);
	this->addChild(label2);

	auto eventListener = EventListenerKeyboard::create();

	eventListener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event) {
		//°ÔÀÓ ¾ÀÀ¸·Î ¾À º¯°æ
		if (keyCode == EventKeyboard::KeyCode::KEY_Z) {

			auto gamScene = GameWorld::createScene();
			Director::getInstance()->replaceScene(gamScene);

		}

	};

	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, this);

	return true;

}