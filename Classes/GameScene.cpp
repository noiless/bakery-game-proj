#include "GameScene.h"
#include "AudioEngine.h"
#include "OverScene.h"

#include "Obj.h"


#include<iostream>

USING_NS_CC;

ObjManager *GameWorld::objManager = new ObjManager;
Player *GameWorld::player;
ObjEnemy *GameWorld::enemy;	//적도 하나만 존재
UI * GameWorld::ui;
bool GameWorld::initiated = false;



Scene* GameWorld::createScene()
{
	
    return GameWorld::create();

}

void GameWorld::gameLoad() {
	//init pool's object
	objManager->ObjInit();

	StateSquaralAttack::tempSquaral = new Obj;
	StateSquaralAttack::tempSquaral->objImg = Sprite::create("img/squaral_down.png");
	StateSquaralAttack::tempSquaral->objImg->setScale(1.4);
	StateSquaralAttack::tempSquaral->addChild(StateSquaralAttack::tempSquaral->objImg);

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


	if (!initiated) {
		gameLoad();
		initiated = true;
	}
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	Sprite *map = Sprite::create("img/map.png");
	this->addChild(map, -1);	//가장 먼저 맵을 그린다.
	objManager->setMapRect(map->getBoundingBox());


	Obj* myShop = new Obj;
	myShop->objImg = Sprite::create("img/myShop.png");
	myShop->objImg->setPosition(map->getBoundingBox().origin.x + map->getBoundingBox().size.width / 4, map->getBoundingBox().origin.y + map->getBoundingBox().size.height / 8 * 7);
	myShop->addChild(myShop->objImg, 5);
	objManager->addObjectAvailList(myShop);

	CCLOG("dest %f", map->getBoundingBox().origin.x + map->getBoundingBox().size.width / 4 + myShop->objImg->getBoundingBox().size.width);

	Obj* otherShop = new Obj;
	otherShop->objImg = Sprite::create("img/otherShop.png");
	otherShop->objImg->setPosition(map->getBoundingBox().origin.x + map->getBoundingBox().size.width / 4 * 3, map->getBoundingBox().origin.y + map->getBoundingBox().size.height / 8 * 7);
	otherShop->addChild(otherShop->objImg, 5);
	objManager->addObjectAvailList(otherShop);

	CCLOG("dest %f", map->getBoundingBox().origin.x + map->getBoundingBox().size.width / 4 * 3 - otherShop->objImg->getBoundingBox().size.width);

	objManager->addChild(myShop, 5);
	objManager->addChild(otherShop, 5);




	//plant trees
	objManager->getObjTreeFromPool(this, Vec2(map->getBoundingBox().origin.x + map->getBoundingBox().size.width / 4, map->getBoundingBox().origin.y + map->getBoundingBox().size.height / 4 * 3));

	objManager->getObjTreeFromPool(this, Vec2(map->getBoundingBox().origin.x + map->getBoundingBox().size.width / 4 * 3, map->getBoundingBox().origin.y + map->getBoundingBox().size.height / 4 * 3));

	objManager->getObjTreeFromPool(this, Vec2(map->getBoundingBox().origin.x + map->getBoundingBox().size.width / 4, map->getBoundingBox().origin.y + map->getBoundingBox().size.height / 4));

	objManager->getObjTreeFromPool(this, Vec2(map->getBoundingBox().origin.x + map->getBoundingBox().size.width / 4 * 3, map->getBoundingBox().origin.y + map->getBoundingBox().size.height / 4));


	//플레이어 생성
	player = new Player;
	player->objImg->setPosition(cocos2d::Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(player, 3);

	enemy = new ObjEnemy;
	this->addChild(enemy);


	auto eventListener = EventListenerKeyboard::create();

	eventListener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event) {
		//다람쥐 생성
		if (keyCode == EventKeyboard::KeyCode::KEY_Z) {

			objManager->getObjSquaralFromPool(this, Vec2(0,0));
			
		}

		if (keyCode == EventKeyboard::KeyCode::KEY_C) {

			CCLOG("player position %f, %f", player->objImg->getPosition().x, player->objImg->getPosition().y);

		}

	};

	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, this);

	//objManager->getObjSquaralFromPool(this, Vec2(-100, -100));

	//////////////
	this->addChild(objManager, 10);
	objManager->scheduleUpdate(); //update 실행
	///////////////


	ui = new UI();
	ui->init();
	this->addChild(ui, 5);


	//////////// 5초마다 게스트 생성
	setGuestInitAction();


	/////////////

	//init random dir
	srand(time(NULL));	//random seed 설정
	

	CCLOG("gamescene init finish");

    return true;
}


void GameWorld::setGuestInitAction() {
	auto callback = CallFunc::create(
		[this]
	{
		objManager->getObjGuestFromPool(this);
		setGuestInitAction();

	});

	Sequence * seq = Sequence::create(DelayTime::create(5 + objManager->getNumBood()), callback, nullptr);

	this->runAction(seq);
}

void GameWorld::gameEnd() {
	
	ui->~UI();
	objManager->Objdeinit();
	auto gameOverScene = GameOver::createScene();
	Director::getInstance()->replaceScene(gameOverScene);
}