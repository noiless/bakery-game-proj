#include "ObjEnemy.h"
#include "StateEnemy.h"
#include "GameScene.h"
#include "Raycasting.h"

USING_NS_CC;


ObjEnemy::ObjEnemy() : HP(20) {
	typecode = TYPECODE_PEOPLE;	//사람군

	GameWorld::objManager->addObjectAvailList(this);
	
	objImg = Sprite::create("img/enemy_normal_down.png");	//초기 스프라이트

	eye[0] = new Raycasting(this, 100, 0);
	eye[1] = new Raycasting(this, 100, 45);
	eye[2] = new Raycasting(this, 100, -45);
	this->addChild(eye[0]);
	this->addChild(eye[1]);
	this->addChild(eye[2]);

	this->addChild(objImg);
	
	//위치 : 임시
	init(Vec2(100,100));

	eye[0]->init();
	eye[1]->init();
	eye[2]->init();
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


	stateHP = dynamic_cast<StateHPEnemy*> (StateHPEnemy::HPEnemyNormal);
	stateHP->initAction(this);

	qnodeIndexInit();

	this->scheduleUpdate();

	//임시

	return true;

}

bool ObjEnemy::deInit() {
	return true;
}

void ObjEnemy::loseHP() {

	HP--;

	//StateSprEnemy에 현재의 HP를 알림
	stateHP->changeHP(HP, this);

}

void ObjEnemy::updateEye() {
	
	for (int i = 0; i < 3; i++) {
		eye[i]->startPoint = objImg->getPosition();

		eye[i]->lineSight->clear();
		eye[i]->lineSight->drawLine(eye[i]->startPoint - eye[i]->anchorDiff, eye[i]->startPoint + eye[i]->d * eye[i]->dir - eye[i]->anchorDiff, Color4F::GRAY);

		eye[i]->doRaycast();
	}

	//이렇게 되면 그냥 레이캐스트도 여기;서 하라고 시키는게 나을거같은데
}


void ObjEnemy::update(float delta) {

	updateEye();



}