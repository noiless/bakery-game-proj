#include "ObjManager.h"
#include <Obj.h>
#include <ObjRabbit.h>
#include <ObjTree.h>
#include <ObjSquaral.h>
#include <ObjGuest.h>
#include <Attack.h>
#include <QTree.h>

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

	unscheduleUpdate();

	for each (Obj * i in objAvailList) {
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

	QTree::removeNodes(qtree);

	objAvailList.clear();
	objUpdateList.clear();
}


void ObjManager::addObjectAvailList(Obj *obj) {
	objAvailList.push_back(obj);
	//qtree->insert(obj);	//��ġ �ʱ�ȭ ���Ŀ� ���� ���������� ��

	//���� �浹üũ ������ ������Ʈ���� ��� updateList�� �߰�����
	if (obj->typecode == TYPECODE_RABBIT || obj->typecode == TYPECODE_SQUARAL || obj->typecode == TYPECODE_PEOPLE) {
		objUpdateList.push_back(obj);
	}

}

void ObjManager::addObjectAvailListFRONT(Obj *obj) {
	objAvailList.push_front(obj);
	qtree->insert(obj);
}

void ObjManager::deleteObjectAvailList(Obj *obj) {
	objAvailList.remove(obj);
	objUpdateList.remove(obj);

	//QTree������ ����
	qtree->removeObjFromAllNode(obj);
}

void ObjManager::addUpdateList(Obj* obj) {
	objUpdateList.push_back(obj);
}

void ObjManager::deleteUpdateList(Obj *obj) {
	objUpdateList.remove(obj);
}



void ObjManager::getObjRabbitFromPool(Node * parent, Vec2 initPos) {

	ObjRabbit* newRabbit = getFreeObjRabbit();

	CCASSERT((newRabbit != nullptr), "NEED LARGER OBJECT POOL : Rabbit");

	newRabbit->init(createColCheck(&initPos, &(objRabbitList[0]->objImg->getContentSize())));	//�ʱ� ��ġ �̿��� �ʱ�ȭ

	parent->addChild(newRabbit);

	qtree->insert(newRabbit);
}

void ObjManager::getObjTreeFromPool(Node * parent, Vec2 initPos) {
	ObjTree* newTree = getFreeObjTree();

	CCASSERT((newTree != nullptr), "NEED LARGER OBJECT POOL : Tree");

	newTree->init(createColCheck(&initPos, &(objTreeList[0]->objImg->getContentSize())));	//�ʱ� ��ġ �̿��� �ʱ�ȭ

	parent->addChild(newTree);

	qtree->insert(newTree);
}

void ObjManager::getObjSquaralFromPool(Node * parent, Vec2 initPos) {
	ObjSquaral* newSquaral = getFreeObjSquaral();

	CCASSERT((newSquaral != nullptr), "NEED LARGER OBJECT POOL : Squaral");

	newSquaral->init(createColCheck(&initPos, &(objSquaralList[0]->objImg->getContentSize())));	//�ʱ� ��ġ �̿��� �ʱ�ȭ

	parent->addChild(newSquaral);

	qtree->insert(newSquaral);
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
		newGuest->init(Vec2(0, -1800));	//>>�ʱ� �մ� ���� ��ġ<<

		parent->addChild(newGuest);

		qtree->insert(newGuest);

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


bool ObjManager::isObjColAvail(Obj * obj) {
	for each (Obj* i in objAvailList)
	{
		if (i->objIndex == obj->objIndex)
			return true;
	}

	return false;
}

void ObjManager::addBlood(Node* parent, const Vec2 initPos) {
	//blood �߰� �� ��ġ, �׼� ����
	Sprite* newBlood = Sprite::create("img/guest_blood.png");
	newBlood->setPosition(initPos);

	bloodNum++;

	//�׼�
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
	mapBoundaryRect[0].setRect(mapBoundingBox.origin.x - 100, mapBoundingBox.origin.y, 100, mapBoundingBox.size.height);	//��
	mapBoundaryRect[1].setRect(mapBoundingBox.origin.x + mapBoundingBox.size.width, mapBoundingBox.origin.y, 100, mapBoundingBox.size.height);//��
	mapBoundaryRect[2].setRect(mapBoundingBox.origin.x, mapBoundingBox.origin.y + mapBoundingBox.size.height, mapBoundingBox.size.width, 100);//��
	mapBoundaryRect[3].setRect(mapBoundingBox.origin.x, mapBoundingBox.origin.y - 100, mapBoundingBox.size.width, 100);//��

	QTree::init(mapBoundingBox);
	qtree = new QTree(mapBoundingBox);	//init qtree
}

//�̰� ��¥�� ���浹�Ҷ����� �����°� �߰��ؾߵ�;
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
			else if (randDir == DIR_RIGHT) {
				pos->x = (i->objImg->getBoundingBox().getMinX() - exBox.size.width / 2 - 1);

			}
			//y������ �����̰� �־��� ��
			//��
			else if (randDir == DIR_UP) {
				pos->y = (i->objImg->getBoundingBox().getMinY() - exBox.size.height / 2 - 1);

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

//�÷��̾ ���
bool ObjManager::checkMoveCollision(Obj *obj, Rect* exBox, cocos2d::Vec2* moveLen) {
	int* exNodeIndexList;

	CCASSERT(!(moveLen->x != 0 && moveLen->y != 0), "moveLen : x or y val should be 0");

	//�������� ���� ���� boundingbox�� �޾Ƽ� �װɷ� ������ �� �ִ��� �˻�
	exNodeIndexList = qtree->getExNodeIndexList(exBox);

	QTree * tempPtr;

	//�� ��� ���� Ž��
	for (int i = 0; i < 4; i++) {

		tempPtr = qtree->searchNode(exNodeIndexList[i]);

		if (tempPtr != nullptr) {

			//�� ������ �����°�?
			if (tempPtr->isOutside && mapBoundaryCheck(exBox)) {
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
				else if (moveLen->y > 0) {
					obj->objImg->setPositionY(mapBoundaryRect[2].getMinY() - exBox->size.height / 2 - 1);
				}
				//��
				else if (moveLen->y < 0) {
					obj->objImg->setPositionY(mapBoundaryRect[3].getMaxY() + exBox->size.height / 2 + 1);
				}

				qtree->renewObjNode(obj);

				return false;
			}

			for each (Obj* element in tempPtr->element) {

				//�浹��
				if ((obj->objIndex != element->objIndex) && exBox->intersectsRect(element->objImg->getBoundingBox())) {

					//x������ �����̰� �־��� ��
					//��
					if (moveLen->x < 0) {
						obj->objImg->setPositionX(element->objImg->getBoundingBox().getMaxX() + exBox->size.width / 2 + 1);
					}
					//��
					else if (moveLen->x > 0) {
						obj->objImg->setPositionX(element->objImg->getBoundingBox().getMinX() - exBox->size.width / 2 - 1);
					}
					//y������ �����̰� �־��� ��
					//��
					else if (moveLen->y > 0) {
						obj->objImg->setPositionY(element->objImg->getBoundingBox().getMinY() - exBox->size.height / 2 - 1);
					}
					//��
					else if (moveLen->y < 0) {
						obj->objImg->setPositionY(element->objImg->getBoundingBox().getMaxY() + exBox->size.height / 2 + 1);
					}

					qtree->renewObjNode(obj);

					return false;
				}
			}

		}

	}

	//�浹���� �ʾ��� ��

	qtree->renewObjNodeWithSpec(obj, exNodeIndexList);

	return true;
}


void ObjManager::update(float delta) {

	bool doCntn = false;
	QTree* qtreePtr;
	int* exNodeIndexList;

	//�� ������Ʈ���� exBox
	Rect exBox;

	//�� ������Ʈ���� �浹 üũ
	for each (Obj* obj in objUpdateList) {

		doCntn = false;

		exBox.setRect(obj->objImg->getBoundingBox().origin.x + obj->moveLen.x * delta, obj->objImg->getBoundingBox().origin.y + obj->moveLen.y * delta, obj->objImg->getBoundingBox().size.width, obj->objImg->getBoundingBox().size.height);
		exNodeIndexList = qtree->getExNodeIndexList(&exBox);

		////////////
		//�� ��庰�� �浹 Ȯ��

		for (int i = 0; i < 4; i++) {

			if (exNodeIndexList[i] >= 0) {

				qtreePtr = qtree->searchNode(exNodeIndexList[i]);

				//�ܰ� ����̸� �� ������ �������� Ȯ��
				if (qtreePtr->isOutside) {

					if (mapBoundaryCheck(&exBox)) {
				
						//ù �浹 üũ
						if (obj->pausedTime == 0) {

							obj->getActionManager()->pauseTarget(obj->objImg);
				
							//x������ �����̰� �־��� ��
							//��
							if (obj->moveLen.x < 0) {
								obj->objImg->setPositionX(mapBoundaryRect[0].getMaxX() + exBox.size.width / 2 + 1);
				
							}
							//��
							else if (obj->moveLen.x > 0) {
								obj->objImg->setPositionX(mapBoundaryRect[1].getMinX() - exBox.size.width / 2 - 1);
				
							}
							//y������ �����̰� �־��� ��
							//��
							else if (obj->moveLen.y > 0) {
								obj->objImg->setPositionY(mapBoundaryRect[2].getMinY() - exBox.size.height / 2 - 1);
				
							}
							//��
							else if (obj->moveLen.y < 0) {
				
								obj->objImg->setPositionY(mapBoundaryRect[3].getMaxY() + exBox.size.height / 2 + 1);
							}
				
							//�浹 �� ���߹Ƿ� ù �浹 üũ���� renew����
							qtree->renewObjNode(obj);
				
						}
				
						obj->pausedTime += delta;
						doCntn = true;
						break;	//continuing outer loop

					}
				} //�� Ȯ�� end


				for each (Obj* colEle in qtreePtr->element) {

					//�浹
					if ((obj->objIndex != colEle->objIndex) && exBox.intersectsRect(colEle->objImg->getBoundingBox())) {

						//������Ʈ�� �̹� �����ִ� ��� pausedTime�� 0���� ŭ

						//ù �浹 üũ
						if (obj->pausedTime == 0) {

							obj->getActionManager()->pauseTarget(obj->objImg); //������Ʈ ����

							//x������ �����̰� �־��� ��
							//��
							if (obj->moveLen.x < 0) {
								obj->objImg->setPositionX(colEle->objImg->getBoundingBox().getMaxX() + exBox.size.width / 2 + 1);

							}
							//��
							else if (obj->moveLen.x > 0) {
								obj->objImg->setPositionX(colEle->objImg->getBoundingBox().getMinX() - exBox.size.width / 2 - 1);

							}
							//y������ �����̰� �־��� ��
							//��
							else if (obj->moveLen.y > 0) {
								obj->objImg->setPositionY(colEle->objImg->getBoundingBox().getMinY() - exBox.size.height / 2 - 1);

							}
							//��
							else if (obj->moveLen.y < 0) {
								obj->objImg->setPositionY(colEle->objImg->getBoundingBox().getMaxY() + exBox.size.height / 2 + 1);
							}

							//�浹 �� ���߹Ƿ� ù �浹 üũ���� renew����
							qtree->renewObjNode(obj);

						}

						//�浹 ���¸� doCntn�� true�� ����
						obj->pausedTime += delta;
						doCntn = true;
						break;	//�浹������ ���̻� ������Ʈ obj�� ���� �浹üũ �� �ʿ� ����

					}
					/////end collision if

				}
			}

			if (doCntn) {
				break;	//node loop break
			}

		}
		////node loop end

		if (doCntn) {
			continue;	//�浹�ÿ� update loop coninue
		}

		//�浹���� �ʾ��� �� �Ʊ� ã�� exbox�� node�� ������Ʈ�� ���� ����
		qtree->renewObjNodeWithSpec(obj, exNodeIndexList);
		obj->getActionManager()->resumeTarget(obj->objImg);

	}
	////update loop end
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
				return true;
			}

		}

		//�簢�� ���� �ﰢ���� �������� ���ԵǴ��� Ȯ��
		for (int index = 1; index < 3; index++) {	//tri[0]�� �׻� object ���� �����Ƿ� ������ ����ģ��
			if (i->objImg->getBoundingBox().containsPoint(tri[index]))
			{
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

Obj* ObjManager::checkSightCollision(ObjSquaral * obj) {

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



//raycasting
//�굵 �θ��ֶ� �浹�ϸ� �ȵż� �ᱹ ������Ʈ �ʿ���������������������������������������������������
Obj* ObjManager::doRaycast(Vec2 startPoint, Vec2 dir, float d) {

	float t1x;
	float t1y;
	float t2x;
	float t2y;

	float Tnear;
	float Tfar;

	Vec2 invDir = Vec2(1.0f / dir.x, 1.0f / dir.y);


	for each (Obj* element in objAvailList) {

		//�̷��� �ȵȴ�
		if (element->objIndex == 294) {
			continue;
		}

		t1x = (element->objImg->getBoundingBox().getMinX() - startPoint.x) * invDir.x;
		t2x = (element->objImg->getBoundingBox().getMaxX() - startPoint.x) * invDir.x;

		Tnear = min(t1x, t2x);
		Tfar = max(t1x, t2x);

		t1y = (element->objImg->getBoundingBox().getMinY() - startPoint.y) * invDir.y;
		t2y = (element->objImg->getBoundingBox().getMaxY() - startPoint.y) * invDir.y;

		Tnear = max(Tnear, min(t1y, t2y));
		Tfar = min(Tfar, max(t1y, t2y));


		if (Tfar < 0 || Tfar < Tnear) {
			continue;
		}

		//������� ��Ƴ������� ������ ������		
		//���������� �Ÿ��� d���� Ŀ�� �ȵ�
		if ((Tnear * dir).x*(Tnear * dir).x + (Tnear * dir).y*(Tnear * dir).y > d*d) {
			continue;
		}

		////������ �����ϴ� �ٸ� ������Ʈ���� ������ ���� �� ����� ���� ����
		////�ȸ��������


		//����
		DrawNode* dddd = DrawNode::create();
		dddd->drawPoint((startPoint + Tnear * dir), 5, Color4F::BLACK);
		this->addChild(dddd);


	}

	//���� continue ���� ��� ���� ���� - ���ư�
	return nullptr;



}