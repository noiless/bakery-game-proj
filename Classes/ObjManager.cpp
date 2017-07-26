#include "ObjManager.h"
#include <Obj.h>
#include <ObjRabbit.h>
#include <ObjTree.h>
#include <ObjSquaral.h>
#include <ObjGuest.h>
#include <Attack.h>

USING_NS_CC;

void ObjManager::ObjInit() {
	//���� �� �ʱ�ȭ
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
		CCLOG("%d delete index",i->objIndex);
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
}

void ObjManager::addObjectAvailListFRONT(Obj *obj) {
	objAvailList.push_front(obj);
}

void ObjManager::deleteObjectAvailList(Obj *obj) {
	objAvailList.remove(obj);
	CCLOG("delete from avail list\n");
}



void ObjManager::getObjRabbitFromPool(Node * parent, Vec2 initPos) {

	ObjRabbit* newRabbit = getFreeObjRabbit();

	CCASSERT((newRabbit != nullptr), "NEED LARGER OBJECT POOL : Rabbit");

	newRabbit->init(createColCheck(&initPos, &(objRabbitList[0]->objImg->getContentSize())));	//�ʱ� ��ġ �̿��� �ʱ�ȭ

	parent->addChild(newRabbit);
}

void ObjManager::getObjTreeFromPool(Node * parent, Vec2 initPos) {
	ObjTree* newTree = getFreeObjTree();

	CCASSERT((newTree != nullptr), "NEED LARGER OBJECT POOL : Tree");

	newTree->init(createColCheck(&initPos, &(objTreeList[0]->objImg->getContentSize())));	//�ʱ� ��ġ �̿��� �ʱ�ȭ

	parent->addChild(newTree);
}

void ObjManager::getObjSquaralFromPool(Node * parent, Vec2 initPos) {
	ObjSquaral* newSquaral = getFreeObjSquaral();

	CCASSERT((newSquaral != nullptr), "NEED LARGER OBJECT POOL : Squaral");

	newSquaral->init(createColCheck(&initPos, &(objSquaralList[0]->objImg->getContentSize())));	//�ʱ� ��ġ �̿��� �ʱ�ȭ

	parent->addChild(newSquaral);
}

void ObjManager::getObjAcornFromPool(Node * parent, ObjSquaral* caller) {
	AcornAttack* newAcorn = getFreeAcornAttack();

	CCASSERT((newAcorn != nullptr), "NEED LARGER OBJECT POOL : Acorn");

	newAcorn->init(caller);	//�ʱ� ��ġ �̿��� �ʱ�ȭ

	parent->addChild(newAcorn);
}

//�׻� ������ ��ġ�� ������
bool ObjManager::getObjGuestFromPool(Node * parent) {
	ObjGuest* newGuest = getFreeGuest();

	if (newGuest == nullptr) {
		return false;	//������ �����ϸ� �� �Խ�Ʈ �ڸ��� ���� ������ delay�� �ְ� wait��
	}
	else {
		newGuest->init(Vec2(0, -1800));

		parent->addChild(newGuest);

		return true;
	}
}


ObjRabbit* ObjManager::getFreeObjRabbit() {

	for (int i = 0; i < MAX_RABBIT_NUM; i++) {
		//��������� ���� ������Ʈ�� ã�� ��ȯ
		if (!objRabbitList[i]->inUse) {
			addObjectAvailList(dynamic_cast<Obj*> (objRabbitList[i]));	//����� ������Ʈ�� availList�� �־���
			return objRabbitList[i];
		}
	}

	//��� ������Ʈ�� ������̸� nullptr ��ȯ
	return nullptr;	
}

ObjTree* ObjManager::getFreeObjTree() {
	for (int i = 0; i < MAX_TREE_NUM; i++) {
		//��������� ���� ������Ʈ�� ã�� ��ȯ
		if (!objTreeList[i]->inUse) {
			addObjectAvailList(dynamic_cast<Obj*> (objTreeList[i]));	//����� ������Ʈ�� availList�� �־���
			return objTreeList[i];
		}
	}

	//��� ������Ʈ�� ������̸� nullptr ��ȯ
	return nullptr;
}

ObjSquaral* ObjManager::getFreeObjSquaral() {
	for (int i = 0; i < MAX_SQUARAL_NUM; i++) {
		//��������� ���� ������Ʈ�� ã�� ��ȯ
		if (!objSquaralList[i]->inUse) {
			addObjectAvailList(dynamic_cast<Obj*> (objSquaralList[i]));	//����� ������Ʈ�� availList�� �־���
			return objSquaralList[i];
		}
	}

	//��� ������Ʈ�� ������̸� nullptr ��ȯ
	return nullptr;
}

ObjGuest* ObjManager::getFreeGuest() {
	for (int i = 0; i < MAX_GUEST_NUM; i++) {
		//��������� ���� ������Ʈ�� ã�� ��ȯ
		if (!objGuestList[i]->inUse) {
			addObjectAvailList(dynamic_cast<Obj*> (objGuestList[i]));	//����� ������Ʈ�� availList�� �־���
			return objGuestList[i];
		}
	}

	//��� ������Ʈ�� ������̸� nullptr ��ȯ
	return nullptr;
}

AcornAttack* ObjManager::getFreeAcornAttack() {
	for (int i = 0; i < MAX_ACORN_NUM; i++) {
		//��������� ���� ������Ʈ�� ã�� ��ȯ
		if (!objAcornList[i]->inUse) {
			//availList�� �߰����� ����			
			return objAcornList[i];
		}
	}

	//��� ������Ʈ�� ������̸� nullptr ��ȯ
	return nullptr;
}




void ObjManager::setMapRect(cocos2d::Rect mapBoundingBox) {
	mapRect.setRect(mapBoundingBox.origin.x, mapBoundingBox.origin.y, mapBoundingBox.size.width, mapBoundingBox.size.height);
	mapBoundaryRect[0].setRect(mapBoundingBox.origin.x - 100, mapBoundingBox.origin.y, 100, mapBoundingBox.size.height);	//��
	mapBoundaryRect[1].setRect(mapBoundingBox.origin.x + mapBoundingBox.size.width, mapBoundingBox.origin.y, 100, mapBoundingBox.size.height);//��
	mapBoundaryRect[2].setRect(mapBoundingBox.origin.x, mapBoundingBox.origin.y + mapBoundingBox.size.height, mapBoundingBox.size.width, 100);//��
	mapBoundaryRect[3].setRect(mapBoundingBox.origin.x, mapBoundingBox.origin.y - 100, mapBoundingBox.size.width, 100);//��

}

Vec2 ObjManager::createColCheck(Vec2* pos, const Size* size) {

	int randDir;
	Rect exBox;
	exBox.setRect(pos->x - size->width / 2, pos->y - size->height / 2, size->width, size->height);

	for each (Obj* i in objAvailList)
	{
		//�浹��
		if (exBox.intersectsRect(i->objImg->getBoundingBox())) {

			randDir = rand() % 4;

			//��
			if (randDir == DIR_LEFT) {
				pos->x = (i->objImg->getBoundingBox().getMaxX() + exBox.size.width / 2 + 1);
			}
			//��
			else if (randDir == DIR_RIGHT){
				pos->x = (i->objImg->getBoundingBox().getMinX() - exBox.size.width / 2 - 1);

			}
			//y������ �����̰� �־��� ��
			//��
			else if (randDir == DIR_UP) {
				pos->y =  (i->objImg->getBoundingBox().getMinY() - exBox.size.height / 2 - 1);

			}
			//��
			else if (randDir == DIR_DOWN) {
				pos->y = (i->objImg->getBoundingBox().getMaxY() + exBox.size.height / 2 + 1);
			}
		}
	}

	return *pos;
}

//�� �ܺη� ������ �ʰ�
bool ObjManager::mapBoundaryCheck(cocos2d::Rect* exBox) {

	if (exBox->intersectsRect(mapBoundaryRect[0])
		|| exBox->intersectsRect(mapBoundaryRect[1])
		|| exBox->intersectsRect(mapBoundaryRect[2])
		|| exBox->intersectsRect(mapBoundaryRect[3]))
		return true;
	else
		return false;
}


//ȣ���� ������Ʈ obj�� ���� �ٸ� ������Ʈ�� �浹�ߴ��� Ȯ���Ѵ�.
//�̵��� ���ؼ� -> �浹������ ������Ʈ�� ������ ��ġ�� �Ű���
//�� �� �浹�ÿ� �Ͼ�°ſ� ���ؼ���(��ɰ�����...) �����ε� �ؾߵɰ� ���٤�0��
//�̵��� : �׻� �ϳ��� �������θ� �̵��Ѵٰ� ���� Vec2�� ������ x, y �� �ϳ��� 0�� ���� ������.
//player������ ���� �������� �浹�� ������Ʈ�� �ʿ��ߴ��� ������Ʈ�� �ڿ������� ���̷��� �׷��ſ��µ� �̰� ���ϰ� ���� moveLen �ȹ޴°ɷ� �ص�
//����� �浹�� �����ϰ� ������ �ʴµ� ��¦ ���� �浹�ϴ°� �ִ٤�0��...
//�Ʒ��ɷ� �ص� �ɰ� ���� �ѵ� �� ����� ������.. ���� �÷��̾� ������ �ҰŸ� ���߿� �Լ� �̸� �ٲ��ٷ�
bool ObjManager::checkMoveCollision(Obj *obj, Rect* exBox, cocos2d::Vec2* moveLen) {

	CCASSERT(!(moveLen->x != 0 && moveLen->y != 0), "moveLen : x or y val should be 0");
	
	//�������� ���� ���� boundingbox�� �޾Ƽ� �װɷ� ������ �� �ִ��� �˻�


	//�� ������ �����°�?
	if (mapBoundaryCheck(exBox)) {
		if (obj->pausedTime == 0) {
			obj->getActionManager()->pauseTarget(obj->objImg);

			//x������ �����̰� �־��� ��
			//��
			if (moveLen->x < 0) {
				obj->objImg->setPositionX(mapBoundaryRect[0].getMaxX() + exBox->size.width / 2 + 1);

			}
			//��
			else if (moveLen->x > 0) {
				obj->objImg->setPositionX(mapBoundaryRect[1].getMinX() - exBox->size.width / 2 - 1);

			}
			//y������ �����̰� �־��� ��
			//��
			else if (moveLen->y>0) {
				obj->objImg->setPositionY(mapBoundaryRect[2].getMinY() - exBox->size.height / 2 - 1);

			}
			//��
			else if (moveLen->y<0) {

				obj->objImg->setPositionY(mapBoundaryRect[3].getMaxY() + exBox->size.height / 2 + 1);
			}

		}

		return false;
	}
	else {
		for each (Obj* i in objAvailList)
		{
			//�浹��
			if ((obj->objIndex != i->objIndex) && exBox->intersectsRect(i->objImg->getBoundingBox())) {

				//������Ʈ�� �̹� �����ִ� ��� pausedTime�� 0���� ŭ

				//ù �浹 üũ
				if (obj->pausedTime == 0) {
					obj->getActionManager()->pauseTarget(obj->objImg);

					//x������ �����̰� �־��� ��
					//��
					if (moveLen->x < 0) {
						obj->objImg->setPositionX(i->objImg->getBoundingBox().getMaxX() + exBox->size.width / 2 + 1);

					}
					//��
					else if (moveLen->x > 0) {
						obj->objImg->setPositionX(i->objImg->getBoundingBox().getMinX() - exBox->size.width / 2 - 1);

					}
					//y������ �����̰� �־��� ��
					//��
					else if (moveLen->y>0) {
						obj->objImg->setPositionY(i->objImg->getBoundingBox().getMinY() - exBox->size.height / 2 - 1);

					}
					//��
					else if (moveLen->y<0) {

						obj->objImg->setPositionY(i->objImg->getBoundingBox().getMaxY() + exBox->size.height / 2 + 1);
					}

				}

				return false;
			}
		}
	}



	//�浹���� �ʾ����� ��� ������
	obj->getActionManager()->resumeTarget(obj->objImg);
	obj->pausedTime = 0;

	return true;

}



bool ObjManager::checkAttackCollision(cocos2d::Rect* exBox) {
	bool hit = false;

	//�浹�� ��ü ����
	for each (Obj* i in objAvailList)
	{

		//�浹��
		if (exBox->intersectsRect(i->objImg->getBoundingBox())) {

			playerCollisionList.pushBack(i);
			hit = true;
			CCLOG("%d collision",i->objIndex);
			
		}
	}

	//�� �浹�� ��ü�� ���� ó��
	for each (Obj* i in playerCollisionList) {
		i->loseHP();	//<-�Ѳ����� ���߿� ó��
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


		//�ﰢ�� ���� �簢���� �������� ���ԵǴ��� Ȯ��
		for (int index = 0; index < 4; index++) {
			if (checkSightCond(obj->dir, slope1, b1, slope2, b2, &rectPoint[index])
				&& checkSightCond3(obj->dir, &tri[1], &rectPoint[index])) 
			{
				CCLOG("1 %d %d", index, i->objIndex);
				return true;
			}
				
		}

		//�簢�� ���� �ﰢ���� �������� ���ԵǴ��� Ȯ��
		for (int index = 1; index < 3; index++) {	//tri[0]�� �׻� object ���� �����Ƿ� ������ ����ģ��
			if (i->objImg->getBoundingBox().containsPoint(tri[index])) 
			{
				CCLOG("2 %d %d", index, i->objIndex);
				return true;
			}
				
		}

	}

	//��ġ�� �ʴ´ٸ� false ��ȯ
	return false;
}

//�䳢��
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

		//������Ʈ�� ���� �浹�ϴ��� Ȯ��
		if (!i->objImg->getBoundingBox().intersectsCircle(obj->objImg->getPosition(), obj->squaralSightRadius))
			continue;

		//������ ���� �ִ� ������ �������� Ȯ�� - ������ ������Ʈ�� �߽��� ���ԵǴ��� Ȯ��
		if (checkSightCond(obj->dir, b1, b2, &(i->objImg->getPosition()))) {
			return i;	//�浹�� ������Ʈ ��ȯ
		}
	}

	return nullptr;
}

//�ٶ���� �þ� üũ
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

//�ٶ���� ���� üũ
bool ObjManager::checkAttackCollision(int callerIndex, const cocos2d::Vec2* center, float radius) {
	bool hit = false;

	//�浹�� ��ü ����
	for each (Obj* i in objAvailList)
	{

		if (i->objIndex == callerIndex || i->typecode == TYPECODE_NONE) {	//�߻��ڴ� �浹 üũ���� ����
			continue;
		}

		//�浹��
		if (i->objImg->getBoundingBox().intersectsCircle(*center, radius)) {

			acornCollisionList.pushBack(i);
			hit = true;

		}
	}

	//�� �浹�� ��ü�� ���� ó��
	for each (Obj* i in acornCollisionList) {
		if (i->typecode == TYPECODE_PEOPLE)
			i->loseHP();	//<-�Ѳ����� ���߿� ó��
	}
	acornCollisionList.clear();

	return hit;
}