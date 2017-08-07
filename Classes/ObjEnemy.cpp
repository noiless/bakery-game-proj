#include "ObjEnemy.h"
#include "GameScene.h"


ObjEnemy::ObjEnemy() : HP(20) {
	typecode = TYPECODE_PEOPLE;	//�����

	GameWorld::objManager->addObjectAvailList(this);

	objImg = Sprite::create("img/enemy_normal_down.png");	//�ʱ� ��������Ʈ
	this->addChild(objImg);

	init(Vec2::ZERO);


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
	//StateSprEnemy�� ������ HP�� �˸�

	CCLOG("hp %d",HP);

}


void ObjEnemy::update(float delta) {

}