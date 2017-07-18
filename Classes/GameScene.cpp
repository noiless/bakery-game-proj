#include "GameScene.h"
#include "SimpleAudioEngine.h"

#include "ObjRabbit.h"
#include "ObjTree.h"

#include<iostream>

USING_NS_CC;

ObjManager *GameWorld::objManager = new ObjManager;
Player *GameWorld::player;
UI * GameWorld::ui;


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

	Sprite *map = Sprite::create("map.png");
	this->addChild(map, -1);	//���� ���� ���� �׸���.
    
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

	//init pool's object
	objManager->ObjInit();

	//plant trees
	for (int i = 0; i < 3; i++) {
		//CREATE NEW TREE OBJECT
		
		ObjTree* newTree = objManager->getFreeObjTree();

		CCASSERT((newTree != nullptr), "NEED LARGER OBJECT POOL");

		newTree->init(Vec2(i * 700 - 700, 0));	//�ʱ� ��ġ �̿��� �ʱ�ȭ

		this->addChild(newTree);

		//CREATE NEW TREE OBJECT END

	}

	


	//�÷��̾� ����
	player = new Player;
	player->objImg->setPosition(cocos2d::Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(player, 3);

	auto eventListener = EventListenerKeyboard::create();

	eventListener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event) {
		//�䳢 ����
		if (keyCode == EventKeyboard::KeyCode::KEY_Z) {

			//CREATE NEW RABBIT OBJECT
			
			ObjRabbit* newRabbit = objManager->getFreeObjRabbit();

			CCASSERT((newRabbit != nullptr), "NEED LARGER OBJECT POOL");

			newRabbit->init(Vec2(0, 0));	//�ʱ� ��ġ �̿��� �ʱ�ȭ

			this->addChild(newRabbit);

			//CREATE NEW RABBIT OBJECT END
			
		}

	};

	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, this);

	this->addChild(objManager);

	ui = new UI();
	this->addChild(ui, 5);
	ui->init();

    return true;
}


