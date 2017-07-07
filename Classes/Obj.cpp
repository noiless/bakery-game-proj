#include "Obj.h"
#include "GameScene.h"

USING_NS_CC;

int Obj::totalObjNum = 0;

//생성자
Obj::Obj(Obj *Obj){
	GameWorld::objManager->addObject(Obj);
	objIndex = totalObjNum;
	totalObjNum++;

}


//소멸자
Obj::~Obj() {
	//HelloWorld::objManager->deleteObject(this);
	//소멸자 말고 delete를 호출하는데(인스턴스가 해제되는곳)에서 deleteObject를 따로 호출해줘야 할거같다
	//으;
}
