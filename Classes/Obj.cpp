#include "Obj.h"
#include "GameScene.h"

USING_NS_CC;

int Obj::totalObjNum = 0;

//생성자
Obj::Obj(){
	objIndex = totalObjNum;
	totalObjNum++;

	CCLOG("%d", objIndex);

}


//소멸자
Obj::~Obj() {
	//HelloWorld::objManager->deleteObject(this);
	//소멸자 말고 delete를 호출하는데(인스턴스가 해제되는곳)에서 deleteObject를 따로 호출해줘야 할거같다
	//totalObjNum은 줄어들면 objIndex가 딴거랑 중복되게 되니까 수정해줄 필요 없음
}

Vec2 Obj::setMoveLen(int dir, float speed) {
	//좌
	if (dir == DIR_LEFT) {
		return Vec2(-speed, 0);
	}
	//우
	else if (dir == DIR_RIGHT) {
		return Vec2(speed, 0);
	}
	//상
	else if (dir == DIR_UP) {
		return Vec2(0, speed);
	}
	//하
	else if (dir == DIR_DOWN) {
		return Vec2(0, -speed);
	}
	else {
		//움직이지 않을 때 false 반환
		CCLOG("...");
		return Vec2(0, 0);
	}
}