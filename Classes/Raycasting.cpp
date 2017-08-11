#include "Raycasting.h"
#include "GameScene.h"

USING_NS_CC;

Raycasting::~Raycasting() {
	lineSight->clear();
	lineSight->removeFromParentAndCleanup(true);
}

//���ڸ� ���� �þ߰Ÿ�, ȸ���� �ʱ�ȭ
Raycasting::Raycasting(Obj* caller, int d, int rot) {
	this->caller = caller;
	this->d = d; //�Ÿ� ����
	this->rot = rot;

	//lingSight ����
	lineSight = DrawNode::create();
	this->addChild(lineSight);

}

bool Raycasting::init() {

	anchorDiff = caller->objImg->getPosition() - getPosition();
	setPosition(caller->objImg->getPosition());

	return true;
}


//StateEnemy ��� Enemy�� ������ �ٲ� ������ ȣ������� ��
void Raycasting::setDir(int callerRot) {

	dir = Vec2(sin(CC_DEGREES_TO_RADIANS(callerRot + rot + 180)), cos(CC_DEGREES_TO_RADIANS(callerRot + rot + 180)));

	if (dir.x < 0.05f && dir.x > -0.05f) {
		dir.x = 0;
	}
	if (dir.y < 0.05f && dir.y > -0.05f) {
		dir.y = 0;
	}

	dir.normalize();

}

//�浹�� �⵿�� ������Ʈ ��ȯ
ColObj* Raycasting::doRaycast() {

	return GameWorld::objManager->doRaycast(caller->objIndex, startPoint, dir, d);
}