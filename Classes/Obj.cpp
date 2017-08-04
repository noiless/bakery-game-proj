#include "Obj.h"
#include "GameScene.h"

USING_NS_CC;

int Obj::totalObjNum = 0;

//������
Obj::Obj() {
	typecode = TYPECODE_NONE;
	pausedTime = 0;
	objIndex = totalObjNum;
	totalObjNum++;

	for (int i = 0; i < 4; i++) {
		qnodeIndex[i] = -1;	//-1�� �ʱ�ȭ
	}
}


//�Ҹ���
Obj::~Obj() {
	//HelloWorld::objManager->deleteObject(this);
	//�Ҹ��� ���� delete�� ȣ���ϴµ�(�ν��Ͻ��� �����Ǵ°�)���� deleteObject�� ���� ȣ������� �ҰŰ���
	//totalObjNum�� �پ��� objIndex�� ���Ŷ� �ߺ��ǰ� �Ǵϱ� �������� �ʿ� ����
	objImg->removeFromParentAndCleanup(true);
	this->removeFromParentAndCleanup(true);
}

Vec2 Obj::setMoveLen(int dir, float speed) {
	//��
	if (dir == DIR_LEFT) {
		return Vec2(-speed, 0);
	}
	//��
	else if (dir == DIR_RIGHT) {
		return Vec2(speed, 0);
	}
	//��
	else if (dir == DIR_UP) {
		return Vec2(0, speed);
	}
	//��
	else if (dir == DIR_DOWN) {
		return Vec2(0, -speed);
	}
	else {
		//�������� ���� �� false ��ȯ
		CCLOG("...");
		return Vec2(0, 0);
	}
}
