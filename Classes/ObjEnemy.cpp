#include "ObjEnemy.h"
#include "StateEnemy.h"
#include "GameScene.h"
#include "Raycasting.h"
#include "Attack.h"

USING_NS_CC;


ObjEnemy::ObjEnemy() : HP(30) {
	typecode = TYPECODE_PEOPLE;	//�����

	GameWorld::objManager->addObjectAvailList(this);
	
	objImg = Sprite::create("img/enemy_normal_down.png");	//�ʱ� ��������Ʈ

	eye[0] = new Raycasting(this, 125, 0);
	eye[1] = new Raycasting(this, 125, 45);
	eye[2] = new Raycasting(this, 125, -45);
	this->addChild(eye[0]);
	this->addChild(eye[1]);
	this->addChild(eye[2]);

	this->addChild(objImg);
	
	//��ġ : �ӽ�
	init(Vec2(100,100));

	eye[0]->init();
	eye[1]->init();
	eye[2]->init();

	attack = new EnemyAttack(objImg);
}

ObjEnemy::~ObjEnemy() {

}

//�����ڿ��� ȣ��
bool ObjEnemy::init(cocos2d::Vec2 initPos) {
	//member value init

	HP = 20;
	colEyeIndex = -1;

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


void ObjEnemy::loseHPByPlayer() {
	HP--;
	deadByPlayer = true;
	//StateSprEnemy�� ������ HP�� �˸�
	stateHP->changeHP(HP, this);
}

void ObjEnemy::loseHPByOther(int damage) {
	HP -= damage;
	deadByPlayer = false;

	//StateSprEnemy�� ������ HP�� �˸�
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
			colEyeIndex = i;	//�浹�� �Ͼ�� ��� colEyeIndex�� ���ŵ�
			minDistance = colObj[i]->intersectDistance;
			target = colObj[i]->obj;
		}

	}

	//�浹���� �ʾ����� coleyeindex �ʱ�ȭ
	//���ص� �ɰŰ���
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
		//state�� escape�� ����
	}





}