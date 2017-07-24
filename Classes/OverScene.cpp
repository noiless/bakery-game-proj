#include "OverScene.h"

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

	this->addChild(label1);

	return true;

}