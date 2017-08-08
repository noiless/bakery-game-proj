#include "ObjEnemy.h"
#include "StateEnemy.h"
#include "GameScene.h"
#include "Raycasting.h"

ObjEnemy::ObjEnemy() : HP(20) {
	typecode = TYPECODE_PEOPLE;	//�����

	//////////////////
	//Updatelist�� �ȳ־�����!!! ���߿� �׼� ����� moveLen���� �ٿ��� ������ ����
	GameWorld::objManager->addObjectAvailListFRONT(this);
	///////////////

	objImg = Sprite::create("img/enemy_normal_down.png");	//�ʱ� ��������Ʈ
	this->addChild(objImg);

	//��ġ : �ӽ�
	init(Vec2(100,100));

	eye = new Raycasting;
	eye->caller = this;
	eye->setPosition(objImg->getPosition());

	eye->startPoint = objImg->getPosition();

	objImg->addChild(eye);
	CCLOG("eye %f %f", eye->startPoint.x, eye->startPoint.y);

	//DrawNode* ddd = DrawNode::create();
	//ddd->drawPoint(eye->convertToWorldSpace(eye->getPosition()), 10, Color4F::BLACK);
	//this->addChild(ddd);


	//�ӽ�
	dir = DIR_UP;

	if (dir == DIR_LEFT) {
		objImg->setRotation(90);
		eye->callerRot = 90;


	}
	else if (dir == DIR_RIGHT) {
		objImg->setRotation(270);
		eye->callerRot = 270;

	}
	else if (dir == DIR_UP) {
		objImg->setRotation(180);
		eye->callerRot = 180;

	}
	else if (dir == DIR_DOWN) {
		eye->callerRot = 0;

	}

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


void ObjEnemy::update(float delta) {


}