#include "ObjManager.h"

USING_NS_CC;

void ObjManager::addObject(Obj *obj) {
	objList.push_back(obj);
	CCLOG("initiated\n");
}

void ObjManager::deleteObject(Obj *obj) {
	//objList.remove(*obj);
}



//호출한 오브젝트 obj에 대해 다른 오브젝트와 충돌했는지 확인한다.
//이동에 대해서 -> 충돌했을시 오브젝트를 인접한 위치로 옮겨줌
//그 외 충돌시에 일어나는거에 대해서는(사냥같은거...) 오버로드 해야될거 같다ㅇ0ㅇ
//이동시 : 항상 하나의 방향으로만 이동한다고 가정 Vec2의 값에서 x, y 중 하나는 0의 값을 가진다.

//player용으로 따로 나눠놓고 충돌된 오브젝트가 필요했던게 오브젝트들 자연스럽게 붙이려고 그런거였는데 이걸 안하고 밑의 moveLen 안받는걸로 해도
//제대로 충돌도 동작하고 붙지도 않는데 살짝 떠서 충돌하는게 있다ㅇ0ㅇ...
//아래걸로 해도 될거 같긴 한데 좀 어색한 느낌임.. 완전 플레이어 용으로 할거면 나중에 함수 이름 바꿔줄래
bool ObjManager::checkCollision(Obj *obj, Rect exBox, cocos2d::Vec2 moveLen) {
	
	//움직였을 시의 예상 boundingbox를 받아서 그걸로 움직일 수 있는지 검사

	for each (Obj* i in objList)
	{
		//충돌시
		if ((obj->objIndex != i->objIndex) && exBox.intersectsRect(i->objImg->getBoundingBox())) {

			//x축으로 움직이고 있었을 때
			//좌
			if (moveLen.x < 0) {
				obj->objImg->setPositionX(i->objImg->getBoundingBox().getMaxX() + exBox.size.width / 2 + 1);
			}
			//우
			else if (moveLen.x > 0) {
				obj->objImg->setPositionX(i->objImg->getBoundingBox().getMinX() - exBox.size.width / 2 - 1);
			}
			//y축으로 움직이고 있었을 때
			//상
			else if (moveLen.y>0) {
				obj->objImg->setPositionY(i->objImg->getBoundingBox().getMinY() - exBox.size.height / 2 - 1);
			}
			//하
			else if (moveLen.y<0) {
				obj->objImg->setPositionY(i->objImg->getBoundingBox().getMaxY() + exBox.size.height / 2 + 1);
			}

			return false;
		}
	}

	return true;

}

//일반 오브젝트용 충돌 체크
//한박자 늦게 체크되어서 겹치는 문제때문에 결국 예상 바운딩박스를 받아야한다..
//혹시 플레이어 이동도 이걸로 통일되게 된다면 pause랑 resume을 object update쪽으로 옮겨줘야 함
//아 플레이어 이동은 이걸로 하면 같은 방향으로 가는 오브젝트 흔들리게 보이는 현상 나타남 걍 위엣걸로 하자
bool ObjManager::checkCollision(Obj *obj, Rect exBox) {

	for each (Obj* i in objList)
	{
		//충돌시
		if ((obj->objIndex != i->objIndex) && exBox.intersectsRect(i->objImg->getBoundingBox())) {
			//정지
			obj->getActionManager()->pauseTarget(obj->objImg);
			return false;
		}
		//충돌하지 않았으면 액션을 계속 한다.
		obj->getActionManager()->resumeTarget(obj->objImg);
	}

	return true;

}