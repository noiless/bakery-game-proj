#include "Obj.h"
#include "GameScene.h"

USING_NS_CC;

int Obj::totalObjNum = 0;

//������
Obj::Obj(Obj *Obj){
	GameWorld::objManager->addObject(Obj);
	objIndex = totalObjNum;
	totalObjNum++;

}


//�Ҹ���
Obj::~Obj() {
	//HelloWorld::objManager->deleteObject(this);
	//�Ҹ��� ���� delete�� ȣ���ϴµ�(�ν��Ͻ��� �����Ǵ°�)���� deleteObject�� ���� ȣ������� �ҰŰ���
	//��;
}
