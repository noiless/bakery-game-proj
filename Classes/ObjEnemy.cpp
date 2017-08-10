#include "ObjEnemy.h"
#include "StateEnemy.h"
#include "GameScene.h"
#include "Raycasting.h"
#include "Attack.h"

USING_NS_CC;


ObjEnemy::ObjEnemy() : HP(30) {
	typecode = TYPECODE_PEOPLE;	//사람군

	GameWorld::objManager->addObjectAvailList(this);
	
	objImg = Sprite::create("img/enemy_normal_down.png");	//초기 스프라이트

	eye[0] = new Raycasting(this, 125, 0);
	eye[1] = new Raycasting(this, 125, 45);
	eye[2] = new Raycasting(this, 125, -45);
	this->addChild(eye[0]);
	this->addChild(eye[1]);
	this->addChild(eye[2]);

	this->addChild(objImg);
	
	//위치 : 임시
	init(Vec2(100,100));

	eye[0]->init();
	eye[1]->init();
	eye[2]->init();

	attack = new EnemyAttack(objImg);
}

ObjEnemy::~ObjEnemy() {

}

//생성자에서 호출
bool ObjEnemy::init(cocos2d::Vec2 initPos) {
	//member value init

	HP = 20;
	colEyeIndex = -1;

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


void ObjEnemy::loseHPByPlayer() {
	HP--;
	deadByPlayer = true;
	//StateSprEnemy에 현재의 HP를 알림
	stateHP->changeHP(HP, this);
}

void ObjEnemy::loseHPByOther(int damage) {
	HP -= damage;
	deadByPlayer = false;

	//StateSprEnemy에 현재의 HP를 알림
	stateHP->changeHP(HP, this);
}


void ObjEnemy::updateEye() {
	ColObj* colObj[3];
	float minDistance = eye[0]->d * 2;

	for (int i = 0; i < 3; i++) {
		eye[i]->startPoint = objImg->getPosition();

		eye[i]->lineSight->clear();
		eye[i]->lineSight->drawLine(eye[i]->startPoint - eye[i]->anchorDiff, eye[i]->startPoint + eye[i]->d * eye[i]->dir - eye[i]->anchorDiff, Color4F::GRAY);

		colObj[i] = eye[i]->doRaycast();

		if (colObj[i] != nullptr && colObj[i]->intersectDistance < minDistance) {
			colEyeIndex = i;	//충돌이 일어났을 경우 colEyeIndex가 갱신됨
			minDistance = colObj[i]->intersectDistance;
			target = colObj[i]->obj;
		}

	}

	//충돌하지 않았으면 coleyeindex 초기화
	//안해도 될거같음
	if (minDistance == eye[0]->d * 2) {
		colEyeIndex = -1;
	}
}


void ObjEnemy::update(float delta) {


	if (state != StateEnemy::enemyAttack && state != StateEnemy::enemyEscape) {
		updateEye();
	}

	if (state == StateEnemy::enemyAttack) {
		dynamic_cast<StateEnemyAttack*>(state)->attackDuration += delta;
	}

	state->checkTransitionCond(this);

	if (pausedTime > 5) {
		//state를 escape로 변경
	}





}