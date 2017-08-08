#include "ObjEnemy.h"
#include "StateEnemy.h"
#include "GameScene.h"
#include "Raycasting.h"

USING_NS_CC;


ObjEnemy::ObjEnemy() : HP(20) {
	typecode = TYPECODE_PEOPLE;	//�����

	GameWorld::objManager->addObjectAvailList(this);
	
	objImg = Sprite::create("img/enemy_normal_down.png");	//�ʱ� ��������Ʈ

	eye[0] = new Raycasting(this, 100, 0);
	eye[1] = new Raycasting(this, 100, 45);
	eye[2] = new Raycasting(this, 100, -45);
	this->addChild(eye[0]);
	this->addChild(eye[1]);
	this->addChild(eye[2]);

	this->addChild(objImg);
	
	//��ġ : �ӽ�
	init(Vec2(100,100));

	eye[0]->init();
	eye[1]->init();
	eye[2]->init();
}

ObjEnemy::~ObjEnemy() {

}

//�����ڿ��� ȣ��
bool ObjEnemy::init(cocos2d::Vec2 initPos) {
	//member value init

	HP = 20;

	//re set sprite position
	objImg->setPosition(initPos);
	objImg->setOpacity(255);	//opacity �ʱ�ȭ

	pausedTime = 0;

	//init action
	//ù state�� normal
	state = dynamic_cast<StateEnemy*> (StateEnemy::enemyNormal);
	state->initAction(this);


	stateHP = dynamic_cast<StateHPEnemy*> (StateHPEnemy::HPEnemyNormal);
	stateHP->initAction(this);

	qnodeIndexInit();

	this->scheduleUpdate();

	//�ӽ�

	return true;

}

bool ObjEnemy::deInit() {
	return true;
}

void ObjEnemy::loseHP() {

	HP--;

	//StateSprEnemy�� ������ HP�� �˸�
	stateHP->changeHP(HP, this);

}

void ObjEnemy::updateEye() {
	
	for (int i = 0; i < 3; i++) {
		eye[i]->startPoint = objImg->getPosition();

		eye[i]->lineSight->clear();
		eye[i]->lineSight->drawLine(eye[i]->startPoint - eye[i]->anchorDiff, eye[i]->startPoint + eye[i]->d * eye[i]->dir - eye[i]->anchorDiff, Color4F::GRAY);

		eye[i]->doRaycast();
	}

	//�̷��� �Ǹ� �׳� ����ĳ��Ʈ�� ����;�� �϶�� ��Ű�°� �����Ű�����
}


void ObjEnemy::update(float delta) {

	updateEye();



}