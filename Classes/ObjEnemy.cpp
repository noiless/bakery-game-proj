#include "ObjEnemy.h"
#include "GameScene.h"


ObjEnemy::ObjEnemy() : HP(20) {
	typecode = TYPECODE_PEOPLE;	//사람군

	GameWorld::objManager->addObjectAvailList(this);

	objImg = Sprite::create("img/enemy_normal_down.png");	//초기 스프라이트
	this->addChild(objImg);

	init(Vec2::ZERO);


}

ObjEnemy::~ObjEnemy() {

}

//생성자에서 호출
bool ObjEnemy::init(cocos2d::Vec2 initPos) {
	//member value init

	HP = 20;

	//re set sprite position
	objImg->setPosition(initPos);
	objImg->setOpacity(255);	//opacity 초기화

	pausedTime = 0;

	//init action
	//첫 state는 normal
	state = dynamic_cast<StateEnemy*> (StateEnemy::enemyNormal);
	state->initAction(this);

	qnodeIndexInit();
	CCLOG("%d %d %d %d 11111~~",qnodeIndex[0], qnodeIndex[1], qnodeIndex[2], qnodeIndex[3]);

	//this->scheduleUpdate();

	return true;

}

bool ObjEnemy::deInit() {
	return true;
}

void ObjEnemy::loseHP() {

	HP--;
	//StateSprEnemy에 현재의 HP를 알림

	CCLOG("hp %d",HP);

}


void ObjEnemy::update(float delta) {

}