#include "GameScene.h"
#include "SimpleAudioEngine.h"

#include "ObjRabbit.h"


#include<iostream>

USING_NS_CC;

ObjManager *GameWorld::objManager = new ObjManager;
Player *GameWorld::player;


Scene* GameWorld::createScene()
{
    return GameWorld::create();

}


// on "init" you need to initialize your instance
bool GameWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();



    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

    // add "HelloWorld" splash screen"
    sprite1 = Sprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    sprite1->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));



	ObjRabbit* m1;
	//ObjRabbit* m2;

	m1 = new ObjRabbit;
	//m1->objImg = Sprite::create("test2.png");
	//m1->objImg->setPosition(100, 100);

	//m1->objImg->runAction(RepeatForever::create(m1->state->initActionTemp(m1)));
	

	this->addChild(m1);
	this->addChild(m1->objImg);

	

	//m2 = new ObjRabbit;
	//m2->objImg = Sprite::create("test3.png");
	//m2->speed = 100;
	//m2->objImg->setPosition(600, 100);
	//m2->moveLen = Vec2(-(m2->speed), 0);
	//auto s1 = Sequence::create(MoveBy::create(2, Vec2(m2->speed * -2, 0)), nullptr);
	//m2->objImg->runAction(s1);

	//this->addChild(m2);
	//this->addChild(m2->objImg);

	//가장 마지막에 플레이어 생성
	player = new Player;
	this->addChild(player);

    return true;
}


