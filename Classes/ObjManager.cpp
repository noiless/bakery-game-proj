#include "ObjManager.h"
#include <Obj.h>
#include <ObjRabbit.h>
#include <ObjTree.h>
#include <ObjSquaral.h>
#include <ObjGuest.h>
#include <Attack.h>

USING_NS_CC;

void ObjManager::ObjInit() {
	//시작 후 초기화
	for (int i = 0; i < MAX_RABBIT_NUM; i++) {
		objRabbitList[i] = new ObjRabbit;
	}

	for (int i = 0; i < MAX_TREE_NUM; i++) {
		objTreeList[i] = new ObjTree;
	}

	for (int i = 0; i < MAX_SQUARAL_NUM; i++) {
		objSquaralList[i] = new ObjSquaral;
	}

	for (int i = 0; i < MAX_ACORN_NUM; i++) {
		objAcornList[i] = new AcornAttack;
	}

	for (int i = 0; i < MAX_GUEST_NUM; i++) {
		objGuestList[i] = new ObjGuest;
	}
}

void ObjManager::Objdeinit() {

	for each (Obj * i in objAvailList) {
		//CCLOG("%d delete index",i->objIndex);
		i->objImg->getActionManager()->removeAllActions();
		i->unscheduleUpdate();
		i->deInit();
	}

	objAvailList.clear();
	
	//for (int i = 0; i < MAX_OBJ_NUM; i++) {
	//	delete objRabbitList[i];
	//	delete objTreeList[i];
	//	delete ObjSquaralList[i];
	//	delete ObjAcornList[i];
	//}
}


void ObjManager::addObjectAvailList(Obj *obj) {
	objAvailList.push_back(obj);

	//충돌체크 가능한 오브젝트군일 경우 updateList에 추가해줌
	if (obj->typecode == TYPECODE_RABBIT || obj->typecode == TYPECODE_SQUARAL || obj->typecode == TYPECODE_PEOPLE) {
		objUpdateList.push_back(obj);
	}

}

void ObjManager::addObjectAvailListFRONT(Obj *obj) {
	objAvailList.push_front(obj);
}

void ObjManager::deleteObjectAvailList(Obj *obj) {
	objAvailList.remove(obj);
	objUpdateList.remove(obj);
	//CCLOG("delete from avail list\n");
}

void ObjManager::addUpdateList(Obj* obj) {
	objUpdateList.push_back(obj);
}


void ObjManager::deleteUpdateList(Obj *obj) {
	objUpdateList.remove(obj);
}



void ObjManager::getObjRabbitFromPool(Node * parent, Vec2 initPos) {

	ObjRabbit* newRabbit = getFreeObjRabbit();

	//CCASSERT((newRabbit != nullptr), "NEED LARGER OBJECT POOL : Rabbit");

	if (newRabbit != nullptr) {
		newRabbit->init(createColCheck(&initPos, &(objRabbitList[0]->objImg->getContentSize())));	//초기 위치 이용해 초기화

		parent->addChild(newRabbit);
	}

	
}

void ObjManager::getObjTreeFromPool(Node * parent, Vec2 initPos) {
	ObjTree* newTree = getFreeObjTree();

	CCASSERT((newTree != nullptr), "NEED LARGER OBJECT POOL : Tree");

	newTree->init(createColCheck(&initPos, &(objTreeList[0]->objImg->getContentSize())));	//초기 위치 이용해 초기화

	parent->addChild(newTree);
}

void ObjManager::getObjSquaralFromPool(Node * parent, Vec2 initPos) {
	ObjSquaral* newSquaral = getFreeObjSquaral();

	CCASSERT((newSquaral != nullptr), "NEED LARGER OBJECT POOL : Squaral");

	newSquaral->init(createColCheck(&initPos, &(objSquaralList[0]->objImg->getContentSize())));	//초기 위치 이용해 초기화

	parent->addChild(newSquaral);
}

void ObjManager::getObjAcornFromPool(Node * parent, ObjSquaral* caller) {
	AcornAttack* newAcorn = getFreeAcornAttack();

	CCASSERT((newAcorn != nullptr), "NEED LARGER OBJECT POOL : Acorn");

	newAcorn->init(caller);	//초기 위치 이용해 초기화

	parent->addChild(newAcorn);
}

//항상 정해진 위치에 리젠함
bool ObjManager::getObjGuestFromPool(Node * parent) {
	ObjGuest* newGuest = getFreeGuest();

	if (newGuest == nullptr) {
		return false;	//생성에 실패하면 빈 게스트 자리가 생길 때까지 delay를 주고 wait함
	}
	else {
		newGuest->init(Vec2(0, -1800));	//>>초기 손님 리젠 위치<<

		parent->addChild(newGuest);

		return true;
	}
}


ObjRabbit* ObjManager::getFreeObjRabbit() {

	for (int i = 0; i < MAX_RABBIT_NUM; i++) {
		//사용중이지 않은 오브젝트를 찾아 반환
		if (!objRabbitList[i]->inUse) {
			addObjectAvailList(dynamic_cast<Obj*> (objRabbitList[i]));	//사용할 오브젝트를 availList에 넣어줌
			return objRabbitList[i];
		}
	}

	//모든 오브젝트가 사용중이면 nullptr 반환
	return nullptr;	
}

ObjTree* ObjManager::getFreeObjTree() {
	for (int i = 0; i < MAX_TREE_NUM; i++) {
		//사용중이지 않은 오브젝트를 찾아 반환
		if (!objTreeList[i]->inUse) {
			addObjectAvailList(dynamic_cast<Obj*> (objTreeList[i]));	//사용할 오브젝트를 availList에 넣어줌
			return objTreeList[i];
		}
	}

	//모든 오브젝트가 사용중이면 nullptr 반환
	return nullptr;
}

ObjSquaral* ObjManager::getFreeObjSquaral() {
	for (int i = 0; i < MAX_SQUARAL_NUM; i++) {
		//사용중이지 않은 오브젝트를 찾아 반환
		if (!objSquaralList[i]->inUse) {
			addObjectAvailList(dynamic_cast<Obj*> (objSquaralList[i]));	//사용할 오브젝트를 availList에 넣어줌
			return objSquaralList[i];
		}
	}

	//모든 오브젝트가 사용중이면 nullptr 반환
	return nullptr;
}

ObjGuest* ObjManager::getFreeGuest() {
	for (int i = 0; i < MAX_GUEST_NUM; i++) {
		//사용중이지 않은 오브젝트를 찾아 반환
		if (!objGuestList[i]->inUse) {
			addObjectAvailList(dynamic_cast<Obj*> (objGuestList[i]));	//사용할 오브젝트를 availList에 넣어줌
			return objGuestList[i];
		}
	}

	//모든 오브젝트가 사용중이면 nullptr 반환
	return nullptr;
}

AcornAttack* ObjManager::getFreeAcornAttack() {
	for (int i = 0; i < MAX_ACORN_NUM; i++) {
		//사용중이지 않은 오브젝트를 찾아 반환
		if (!objAcornList[i]->inUse) {
			//availList에 추가하지 않음			
			return objAcornList[i];
		}
	}

	//모든 오브젝트가 사용중이면 nullptr 반환
	return nullptr;
}


bool ObjManager::isObjColAvail(Obj * obj) {
	for each (Obj* i in objAvailList)
	{
		if (i->objIndex == obj->objIndex)
			return true;
	}

	return false;
}

void ObjManager::addBlood(Node* parent, const Vec2 initPos) {
	//blood 추가 및 위치, 액션 설정
	Sprite* newBlood = Sprite::create("img/guest_blood.png");
	newBlood->setPosition(initPos);

	bloodNum++;

	//newBlood->removeFromParent

	//액션
	CallFunc *action1 = CallFunc::create([=] {
		newBlood->removeFromParent();
		bloodNum--;
	});
	Sequence* seq = Sequence::create(DelayTime::create(30), action1, nullptr);
	newBlood->runAction(seq);

	parent->addChild(newBlood, -1);

}

int ObjManager::getNumBood() {
	return bloodNum;
}



void ObjManager::setMapRect(cocos2d::Rect mapBoundingBox) {
	mapRect.setRect(mapBoundingBox.origin.x, mapBoundingBox.origin.y, mapBoundingBox.size.width, mapBoundingBox.size.height);
	mapBoundaryRect[0].setRect(mapBoundingBox.origin.x - 100, mapBoundingBox.origin.y, 100, mapBoundingBox.size.height);	//좌
	mapBoundaryRect[1].setRect(mapBoundingBox.origin.x + mapBoundingBox.size.width, mapBoundingBox.origin.y, 100, mapBoundingBox.size.height);//우
	mapBoundaryRect[2].setRect(mapBoundingBox.origin.x, mapBoundingBox.origin.y + mapBoundingBox.size.height, mapBoundingBox.size.width, 100);//상
	mapBoundaryRect[3].setRect(mapBoundingBox.origin.x, mapBoundingBox.origin.y - 100, mapBoundingBox.size.width, 100);//하

}

Vec2 ObjManager::createColCheck(Vec2* pos, const Size* size) {

	int randDir;
	Rect exBox;
	exBox.setRect(pos->x - size->width / 2, pos->y - size->height / 2, size->width, size->height);

	for each (Obj* i in objAvailList)
	{
		//충돌시
		if (exBox.intersectsRect(i->objImg->getBoundingBox())) {

			randDir = rand() % 4;

			//좌
			if (randDir == DIR_LEFT) {
				pos->x = (i->objImg->getBoundingBox().getMaxX() + exBox.size.width / 2 + 1);
			}
			//우
			else if (randDir == DIR_RIGHT){
				pos->x = (i->objImg->getBoundingBox().getMinX() - exBox.size.width / 2 - 1);

			}
			//y축으로 움직이고 있었을 때
			//상
			else if (randDir == DIR_UP) {
				pos->y =  (i->objImg->getBoundingBox().getMinY() - exBox.size.height / 2 - 1);

			}
			//하
			else if (randDir == DIR_DOWN) {
				pos->y = (i->objImg->getBoundingBox().getMaxY() + exBox.size.height / 2 + 1);
			}
		}
	}

	return *pos;
}

//맵 외부로 나가지 않게
bool ObjManager::mapBoundaryCheck(cocos2d::Rect* exBox) {

	if (exBox->intersectsRect(mapBoundaryRect[0])
		|| exBox->intersectsRect(mapBoundaryRect[1])
		|| exBox->intersectsRect(mapBoundaryRect[2])
		|| exBox->intersectsRect(mapBoundaryRect[3]))
		return true;
	else
		return false;
}



//플레이어만 사용
bool ObjManager::checkMoveCollision(Obj *obj, Rect* exBox, cocos2d::Vec2* moveLen) {

	CCASSERT(!(moveLen->x != 0 && moveLen->y != 0), "moveLen : x or y val should be 0");
	
	//움직였을 시의 예상 boundingbox를 받아서 그걸로 움직일 수 있는지 검사

	//맵 밖으로 나가는가?
	if (mapBoundaryCheck(exBox)) {

		//x축으로 움직이고 있었을 때
		//좌
		if (moveLen->x < 0) {
			obj->objImg->setPositionX(mapBoundaryRect[0].getMaxX() + exBox->size.width / 2 + 1);
		}
		//우
		else if (moveLen->x > 0) {
			obj->objImg->setPositionX(mapBoundaryRect[1].getMinX() - exBox->size.width / 2 - 1);
		}
		//y축으로 움직이고 있었을 때
		//상
		else if (moveLen->y>0) {
			obj->objImg->setPositionY(mapBoundaryRect[2].getMinY() - exBox->size.height / 2 - 1);
		}
		//하
		else if (moveLen->y<0) {
			obj->objImg->setPositionY(mapBoundaryRect[3].getMaxY() + exBox->size.height / 2 + 1);
		}

		return false;
	}
	else {

		for each (Obj* i in objAvailList){

			//충돌시
			if ((obj->objIndex != i->objIndex) && exBox->intersectsRect(i->objImg->getBoundingBox())) {

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

				return false;
			}
		}
	}

	return true;
}


void ObjManager::update(float delta) {

	bool doCntn = false;

	//각 오브젝트마다 exBox
	Rect exBox;

	//각 오브젝트마다 충돌 체크
	 for each (Obj* obj in objUpdateList) {

		doCntn = false;

		exBox.setRect(obj->objImg->getBoundingBox().origin.x + obj->moveLen.x * delta, obj->objImg->getBoundingBox().origin.y + obj->moveLen.y * delta, obj->objImg->getBoundingBox().size.width, obj->objImg->getBoundingBox().size.height);

		//움직였을 시의 예상 boundingbox를 통해 이동 가능한지 검사

		//맵 밖으로 나가는가?
		if (mapBoundaryCheck(&exBox)) {

			//첫 충돌 체크
			if (obj->pausedTime == 0) {
				obj->getActionManager()->pauseTarget(obj->objImg);

				//x축으로 움직이고 있었을 때
				//좌
				if (obj->moveLen.x < 0) {
					obj->objImg->setPositionX(mapBoundaryRect[0].getMaxX() + exBox.size.width / 2 + 1);

				}
				//우
				else if (obj->moveLen.x > 0) {
					obj->objImg->setPositionX(mapBoundaryRect[1].getMinX() - exBox.size.width / 2 - 1);

				}
				//y축으로 움직이고 있었을 때
				//상
				else if (obj->moveLen.y>0) {
					obj->objImg->setPositionY(mapBoundaryRect[2].getMinY() - exBox.size.height / 2 - 1);

				}
				//하
				else if (obj->moveLen.y<0) {

					obj->objImg->setPositionY(mapBoundaryRect[3].getMaxY() + exBox.size.height / 2 + 1);
				}

			}

			////
			obj->pausedTime += delta;

			continue;	//continuing outer loop
		}
		else {
			for each (Obj* i in objAvailList)
			{
				//충돌시
				if ((obj->objIndex != i->objIndex) && exBox.intersectsRect(i->objImg->getBoundingBox())) {

					//오브젝트가 이미 멈춰있는 경우 pausedTime이 0보다 큼

					//첫 충돌 체크
					if (obj->pausedTime == 0) {

						obj->getActionManager()->pauseTarget(obj->objImg); //오브젝트 정지

						//x축으로 움직이고 있었을 때
						//좌
						if (obj->moveLen.x < 0) {
							obj->objImg->setPositionX(i->objImg->getBoundingBox().getMaxX() + exBox.size.width / 2 + 1);

						}
						//우
						else if (obj->moveLen.x > 0) {
							obj->objImg->setPositionX(i->objImg->getBoundingBox().getMinX() - exBox.size.width / 2 - 1);

						}
						//y축으로 움직이고 있었을 때
						//상
						else if (obj->moveLen.y>0) {
							obj->objImg->setPositionY(i->objImg->getBoundingBox().getMinY() - exBox.size.height / 2 - 1);

						}
						//하
						else if (obj->moveLen.y<0) {
							obj->objImg->setPositionY(i->objImg->getBoundingBox().getMaxY() + exBox.size.height / 2 + 1);
						}

					}

					////
					obj->pausedTime += delta;
					doCntn = true;
				}
			}
		}

		if (doCntn) {
			continue;
		}
		
		//충돌하지 않았으면 계속 움직임
		obj->getActionManager()->resumeTarget(obj->objImg);
		obj->pausedTime = 0;

	}

}



bool ObjManager::checkAttackCollision(cocos2d::Rect* exBox) {
	bool hit = false;

	//충돌한 물체 검출
	for each (Obj* i in objAvailList)
	{

		//충돌시
		if (exBox->intersectsRect(i->objImg->getBoundingBox())) {

			playerCollisionList.pushBack(i);
			hit = true;
			//CCLOG("%d collision",i->objIndex);
			
		}
	}

	//각 충돌한 물체에 대한 처리
	for each (Obj* i in playerCollisionList) {
		i->loseHP();	//<-한꺼번에 나중에 처리
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
				return true;
			}
				
		}

		//사각형 내에 삼각형의 꼭짓점이 포함되는지 확인
		for (int index = 1; index < 3; index++) {	//tri[0]은 항상 object 내에 있으므로 어차피 못겹친다
			if (i->objImg->getBoundingBox().containsPoint(tri[index])) 
			{
				return true;
			}
				
		}

	}

	//겹치지 않는다면 false 반환
	return false;
}

//토끼용
bool ObjManager::checkSightCond(int dir, float slope1, float b1, float slope2, float b2, const Vec2* rectPoint) {
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

Obj* ObjManager::checkSightCollision(ObjSquaral * obj){

	//1
	float b1 = obj->objImg->getPositionY() - obj->objImg->getPositionX();

	//2
	float b2 = obj->objImg->getPositionY() + obj->objImg->getPositionX();

	for each (Obj* i in objAvailList)
	{
		if (i->typecode != TYPECODE_PEOPLE) {
			continue;
		}

		//오브젝트가 원과 충돌하는지 확인
		if (!i->objImg->getBoundingBox().intersectsCircle(obj->objImg->getPosition(), obj->squaralSightRadius))
			continue;

		//범위가 보고 있는 방향의 범위인지 확인 - 범위에 오브젝트의 중심이 포함되는지 확인
		if (checkSightCond(obj->dir, b1, b2, &(i->objImg->getPosition()))) {
			return i;	//충돌한 오브젝트 반환
		}
	}

	return nullptr;
}

//다람쥐용 시야 체크
bool ObjManager::checkSightCond(int dir, float b1, float b2, const cocos2d::Vec2* rectPos) {
	if (dir == DIR_LEFT) {
		return ((rectPos->x - rectPos->y + b1 < 0)
			&& (-rectPos->x - rectPos->y + b2 > 0));
	}
	else if (dir == DIR_RIGHT) {
		return ((rectPos->x - rectPos->y + b1 > 0)
			&& (-rectPos->x - rectPos->y + b2 < 0));
	}
	else if (dir == DIR_UP) {
		return ((rectPos->x - rectPos->y + b1 < 0)
			&& (-rectPos->x - rectPos->y + b2 < 0));
	}
	else if (dir == DIR_DOWN) {
		return ((rectPos->x - rectPos->y + b1 > 0)
			&& (-rectPos->x - rectPos->y + b2 > 0));
	}
	else {
		return false;
	}
}

//다람쥐용 공격 체크
bool ObjManager::checkAttackCollision(int callerIndex, const cocos2d::Vec2* center, float radius) {
	bool hit = false;

	//충돌한 물체 검출
	for each (Obj* i in objAvailList)
	{

		if (i->objIndex == callerIndex || i->typecode == TYPECODE_NONE) {	//발사자는 충돌 체크에서 제외
			continue;
		}

		//충돌시
		if (i->objImg->getBoundingBox().intersectsCircle(*center, radius)) {

			acornCollisionList.pushBack(i);
			hit = true;

		}
	}

	//각 충돌한 물체에 대한 처리
	for each (Obj* i in acornCollisionList) {
		if (i->typecode == TYPECODE_PEOPLE)
			i->loseHP();	//<-한꺼번에 나중에 처리
	}
	acornCollisionList.clear();

	return hit;
}