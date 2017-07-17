#include "ObjManager.h"


USING_NS_CC;


ObjManager::ObjManager() {
	

}

void ObjManager::ObjInit() {
	//시작 후 초기화...
	for (int i = 0; i < MAX_OBJ_NUM; i++) {
		objRabbitList[i] = new ObjRabbit;
	}
}


void ObjManager::addObjectAvailList(Obj *obj) {
	objAvailList.push_back(obj);
}

//테스팅 안해봄
void ObjManager::deleteObjectAvailList(Obj *obj) {
	objAvailList.remove(obj);
	CCLOG("delete from avail list\n");
}

ObjRabbit* ObjManager::getFreeObjRabbit() {

	for (int i = 0; i < MAX_OBJ_NUM; i++) {
		//사용중이지 않은 오브젝트를 찾아 반환
		if (!objRabbitList[i]->inUse) {
			addObjectAvailList(dynamic_cast<Obj*> (objRabbitList[i]));	//사용할 오브젝트를 availList에 넣어줌
			return objRabbitList[i];
		}
	}

	//모든 오브젝트가 사용중이면 nullptr 반환
	return nullptr;	
}


//호출한 오브젝트 obj에 대해 다른 오브젝트와 충돌했는지 확인한다.
//이동에 대해서 -> 충돌했을시 오브젝트를 인접한 위치로 옮겨줌
//그 외 충돌시에 일어나는거에 대해서는(사냥같은거...) 오버로드 해야될거 같다ㅇ0ㅇ
//이동시 : 항상 하나의 방향으로만 이동한다고 가정 Vec2의 값에서 x, y 중 하나는 0의 값을 가진다.
//player용으로 따로 나눠놓고 충돌된 오브젝트가 필요했던게 오브젝트들 자연스럽게 붙이려고 그런거였는데 이걸 안하고 밑의 moveLen 안받는걸로 해도
//제대로 충돌도 동작하고 붙지도 않는데 살짝 떠서 충돌하는게 있다ㅇ0ㅇ...
//아래걸로 해도 될거 같긴 한데 좀 어색한 느낌임.. 완전 플레이어 용으로 할거면 나중에 함수 이름 바꿔줄래
bool ObjManager::checkMoveCollision(Obj *obj, Rect* exBox, cocos2d::Vec2* moveLen) {

	CCASSERT(!(moveLen->x != 0 && moveLen->y != 0), "moveLen : x or y val should be 0");
	
	//움직였을 시의 예상 boundingbox를 받아서 그걸로 움직일 수 있는지 검사

	for each (Obj* i in objAvailList)
	{
		//충돌시
		if ((obj->objIndex != i->objIndex) && exBox->intersectsRect(i->objImg->getBoundingBox())) {

			//오브젝트가 이미 멈춰있는 경우 pausedTime이 0보다 큼

			//첫 충돌 체크
			if (obj->pausedTime == 0) {
				obj->getActionManager()->pauseTarget(obj->objImg);

				//x축으로 움직이고 있었을 때
				//좌
				if (moveLen->x < 0) {
					obj->objImg->setPositionX(i->objImg->getBoundingBox().getMaxX() + exBox->size.width / 2 + 1);
				}
				//우
				else if (moveLen->x > 0) {
					obj->objImg->setPositionX(i->objImg->getBoundingBox().getMinX() - exBox->size.width / 2 - 1);

				}
				//y축으로 움직이고 있었을 때
				//상
				else if (moveLen->y>0) {
					obj->objImg->setPositionY(i->objImg->getBoundingBox().getMinY() - exBox->size.height / 2 - 1);

				}
				//하
				else if (moveLen->y<0) {

					obj->objImg->setPositionY(i->objImg->getBoundingBox().getMaxY() + exBox->size.height / 2 + 1);
				}
				else {
					CCLOG("error");
				}

			}

			return false;
		}
	}

	//충돌하지 않았으면 계속 움직임
	obj->getActionManager()->resumeTarget(obj->objImg);
	obj->pausedTime = 0;

	return true;

}

bool ObjManager::checkAttackCollision(cocos2d::Rect* exBox) {
	bool hit = false;

	//충돌한 물체 검출
	for each (Obj* i in objAvailList)
	{

		if (i->typecode != TYPECODE_RABBIT) {	//임시... 나중엔 객체 타입별로 분류할 것?
			continue;
		}

		//충돌시
		if (exBox->intersectsRect(i->objImg->getBoundingBox())) {

			playerCollisionList.pushBack(i);
			hit = true;
			CCLOG("%d collision",i->objIndex);
			
		}
	}

	//각 충돌한 물체에 대한 처리
	for each (Obj* i in playerCollisionList) {
		i->loseHP();	//<-한꺼번에 나중에 처리...?
	}
	playerCollisionList.clear();

	return hit;
}


bool ObjManager::checkSightCollision(ObjRabbit * obj) {

	Vec2* tri = obj->rabbitSight;

	//1
	float slope1 = (tri[0].y - tri[1].y) / (tri[0].x - tri[1].x);
	float b1 = tri[0].y - slope1 * tri[0].x;

	//2
	float slope2 = (tri[0].y - tri[2].y) / (tri[0].x - tri[2].x);
	float b2 = tri[0].y - slope2 * tri[0].x;

	for each (Obj* i in objAvailList)
	{
		if (i->typecode != TYPECODE_PEOPLE) {
			continue;
		}

		Vec2 rectPoint[4];
		rectPoint[0] = Vec2(i->objImg->getBoundingBox().getMaxX(), i->objImg->getBoundingBox().getMaxY());
		rectPoint[1] = Vec2(i->objImg->getBoundingBox().getMinX(), i->objImg->getBoundingBox().getMaxY());
		rectPoint[2] = Vec2(i->objImg->getBoundingBox().getMaxX(), i->objImg->getBoundingBox().getMinY());
		rectPoint[3] = Vec2(i->objImg->getBoundingBox().getMinX(), i->objImg->getBoundingBox().getMinY());


		//삼각형 내에 사각형의 꼭짓점이 포함되는지 확인
		for (int index = 0; index < 4; index++) {
			if (checkSightCond(obj->dir, slope1, b1, slope2, b2, &rectPoint[index])
				&& checkSightCond3(obj->dir, &tri[1], &rectPoint[index])) 
			{
				CCLOG("1 %d %d", index, i->objIndex);
				return true;
			}
				
		}

		//사각형 내에 삼각형의 꼭짓점이 포함되는지 확인
		for (int index = 1; index < 3; index++) {	//tri[0]은 항상 object 내에 있으므로 어차피 못겹친다
			if (i->objImg->getBoundingBox().containsPoint(tri[index])) 
			{
				CCLOG("2 %d %d", index, i->objIndex);
				return true;
			}
				
		}

	}

	//겹치지 않는다면 false 반환
	return false;
}

bool ObjManager::checkSightCond(int dir, float slope1, float b1, float slope2, float b2, Vec2* rectPoint) {
	if (dir == DIR_LEFT) {
		return ((slope1 * rectPoint->x - rectPoint->y + b1 > 0)
			&& (slope2 * rectPoint->x - rectPoint->y + b2 < 0));
	}
	else if (dir == DIR_RIGHT) {
		return ((slope1 * rectPoint->x - rectPoint->y + b1 > 0)
			&& (slope2 * rectPoint->x - rectPoint->y + b2 < 0));
	}
	else if (dir == DIR_UP) {
		return ((slope1 * rectPoint->x - rectPoint->y + b1 < 0)
			&& (slope2 * rectPoint->x - rectPoint->y + b2 < 0));
	}
	else if (dir == DIR_DOWN) {
		return ((slope1 * rectPoint->x - rectPoint->y + b1 > 0)
			&& (slope2 * rectPoint->x - rectPoint->y + b2 > 0));
	}
	else {
		return false;
	}
	
}

bool ObjManager::checkSightCond3(int dir, Vec2* triP1, Vec2* rectPoint) {
	if (dir == DIR_LEFT) {
		if (triP1->x < rectPoint->x)
			return true;
	}
	else if (dir == DIR_RIGHT) {
		if (triP1->x > rectPoint->x)
			return true;
	}
	else if (dir == DIR_UP) {
		if (triP1->y > rectPoint->y)
			return true;
	}
	else if (dir == DIR_DOWN) {
		if (triP1->y < rectPoint->y)
			return true;
	}
	return false;
}