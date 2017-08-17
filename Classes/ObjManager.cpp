#include "ObjManager.h"
#include <Obj.h>
#include <ObjRabbit.h>
#include <ObjTree.h>
#include <ObjSquaral.h>
#include <ObjGuest.h>
#include <Attack.h>
#include <QTree.h>

#define cos45 cos(CC_DEGREES_TO_RADIANS(45))

USING_NS_CC;
using namespace pugi;

ObjManager::ObjManager() {
	CCLOG("objmanager init");
	intersectedObj = new ColObj;
	bloodNum = 0;
	exNodeIndexList = new int[4];
}

void ObjManager::ObjInit(xml_node headnode) {
	xml_node tempNode;

	//시작 후 초기화
	for (int i = 0; i < MAX_RABBIT_NUM; i++) {
		objRabbitList[i] = new ObjRabbit(headnode.child("Rabbit"));
	}

	for (int i = 0; i < MAX_TREE_NUM; i++) {
		objTreeList[i] = new ObjTree(headnode.child("Tree"));
	}

	for (int i = 0; i < MAX_SQUARAL_NUM; i++) {
		objSquaralList[i] = new ObjSquaral(headnode.child("Squaral"));

		objSquaralList[i]->tempSquaral = new Obj;
		objSquaralList[i]->tempSquaral->objImg = Sprite::create("img/squaral_down.png");
		objSquaralList[i]->tempSquaral->objImg->setScale(1.5);
		objSquaralList[i]->tempSquaral->addChild(objSquaralList[i]->tempSquaral->objImg);
	}

	

	for (int i = 0; i < MAX_ACORN_NUM; i++) {
		objAcornList[i] = new AcornAttack;
	}

	for (int i = 0; i < MAX_GUEST_NUM; i++) {
		objGuestList[i] = new ObjGuest(headnode.child("Guest"));
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
	//qtree->insert(obj);	//위치 초기화 이후에 들어가야 정상적으로 들어감

	//직접 충돌체크 가능한 오브젝트군일 경우 updateList에 추가해줌
	if (obj->typecode == TYPECODE_RABBIT || obj->typecode == TYPECODE_SQUARAL || obj->typecode == TYPECODE_PEOPLE) {
		objUpdateList.push_back(obj);
	}

}

void ObjManager::addObjectAvailListFRONT(Obj *obj) {
	objAvailList.push_front(obj);
	qtree->insert(obj, true);
}

void ObjManager::deleteObjectAvailList(Obj *obj) {
	objAvailList.remove(obj);
	objUpdateList.remove(obj);

	//QTree에서도 삭제
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

	if (newRabbit != nullptr) {

		newRabbit->init(createColCheck(&initPos, &(objRabbitList[0]->objImg->getContentSize())));	//초기 위치 이용해 초기화

		parent->addChild(newRabbit);

		qtree->insert(newRabbit, false);
	}
	else {
		CCLOG("NEED LARGER OBJECT POOL : Rabbit. now obj # %d", objAvailList.size());
	}

}

void ObjManager::getObjTreeFromPool(Node * parent, Vec2 initPos) {
	ObjTree* newTree = getFreeObjTree();

	if (newTree != nullptr) {
		newTree->init(createColCheck(&initPos, &(objTreeList[0]->objImg->getContentSize())));	//초기 위치 이용해 초기화

		parent->addChild(newTree);

		qtree->insert(newTree, false);

	}
	else {
		CCLOG("NEED LARGER OBJECT POOL : Tree. now obj # %d", objAvailList.size());

	}

}

void ObjManager::getObjSquaralFromPool(Node * parent, Vec2 initPos) {
	ObjSquaral* newSquaral = getFreeObjSquaral();

	CCASSERT((newSquaral != nullptr), "NEED LARGER OBJECT POOL : Squaral");

	newSquaral->init(createColCheck(&initPos, &(objSquaralList[0]->objImg->getContentSize())));	//초기 위치 이용해 초기화

	parent->addChild(newSquaral);

	qtree->insert(newSquaral, false);
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
		newGuest->init();	//초기 손님 리젠 위치는 ObjGuest 내에 있음

		parent->addChild(newGuest);

		qtree->insert(newGuest, false);

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

	QTree::init(mapBoundingBox);
	qtree = new QTree(mapBoundingBox);	//init qtree
}

//이거 진짜로 안충돌할때까지 돌리는거 추가해야됨;
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
			else if (randDir == DIR_RIGHT) {
				pos->x = (i->objImg->getBoundingBox().getMinX() - exBox.size.width / 2 - 1);

			}
			//y축으로 움직이고 있었을 때
			//상
			else if (randDir == DIR_UP) {
				pos->y = (i->objImg->getBoundingBox().getMinY() - exBox.size.height / 2 - 1);

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
	int* exNodeIndexList = new int[4];

	CCASSERT(!(moveLen->x != 0 && moveLen->y != 0), "moveLen : x or y val should be 0");

	//움직였을 시의 예상 boundingbox를 받아서 그걸로 움직일 수 있는지 검사
	exNodeIndexList = qtree->getExNodeIndexList(exBox, exNodeIndexList);

	QTree * tempPtr;

	//각 노드 별로 탐색
	for (int i = 0; i < 4; i++) {

		//tempPtr = qtree->searchNode(exNodeIndexList[i]);
		tempPtr = qtree->getNode(exNodeIndexList[i]);

		if (tempPtr != nullptr) {

			//맵 밖으로 나가는가?
			if (tempPtr->isOutside && mapBoundaryCheck(exBox)) {
				//x축으로 움직이고 있었을 때
				//좌
				if (moveLen->x < 0) {
					obj->objImg->setPositionX(mapBoundaryRect[0].getMaxX() + exBox->size.width / 2 + 5);
				}
				//우
				else if (moveLen->x > 0) {
					obj->objImg->setPositionX(mapBoundaryRect[1].getMinX() - exBox->size.width / 2 - 5);
				}
				//y축으로 움직이고 있었을 때
				//상
				else if (moveLen->y > 0) {
					obj->objImg->setPositionY(mapBoundaryRect[2].getMinY() - exBox->size.height / 2 - 5);
				}
				//하
				else if (moveLen->y < 0) {
					obj->objImg->setPositionY(mapBoundaryRect[3].getMaxY() + exBox->size.height / 2 + 5);
				}

				qtree->renewObjNode(obj);
				delete[] exNodeIndexList;

				return false;
			}

			for each (Obj* element in tempPtr->element) {

				//충돌시
				if ((obj->objIndex != element->objIndex) && exBox->intersectsRect(element->objImg->getBoundingBox())) {


					//x축으로 움직이고 있었을 때
					//좌
					if (moveLen->x < 0) {
						obj->objImg->setPositionX(element->objImg->getBoundingBox().getMaxX() + exBox->size.width / 2 + 1);
					}
					//우
					else if (moveLen->x > 0) {
						obj->objImg->setPositionX(element->objImg->getBoundingBox().getMinX() - exBox->size.width / 2 - 1);
					}
					//y축으로 움직이고 있었을 때
					//상
					else if (moveLen->y > 0) {
						obj->objImg->setPositionY(element->objImg->getBoundingBox().getMinY() - exBox->size.height / 2 - 1);
					}
					//하
					else if (moveLen->y < 0) {
						obj->objImg->setPositionY(element->objImg->getBoundingBox().getMaxY() + exBox->size.height / 2 + 1);
					}

					qtree->renewObjNode(obj);
					delete[] exNodeIndexList;

					return false;
				}
			}

		}

	}

	//충돌하지 않았을 때

	if (exNodeIndexList[0] != obj->qnodeIndex[0] || exNodeIndexList[1] != obj->qnodeIndex[1] || exNodeIndexList[2] != obj->qnodeIndex[2] || exNodeIndexList[3] != obj->qnodeIndex[3]) {
		qtree->renewObjNodeWithSpec(obj, exNodeIndexList);
	}

	delete[] exNodeIndexList;

	return true;
}


void ObjManager::update(float delta) {

	bool doCntn = false;
	QTree* qtreePtr;

	//각 오브젝트마다 exBox
	Rect exBoundBox;
	Rect exImgBox;

	//각 오브젝트마다 충돌 체크
	for each (Obj* obj in objUpdateList) {

		doCntn = false;

		//exBoundBox : 예상 노드 구할때 사용
		exBoundBox.setRect(obj->objImg->getPositionX() - obj->qnodeBound.width / 2 + obj->moveLen.x * delta, obj->objImg->getPositionY() - obj->qnodeBound.height / 2 + obj->moveLen.y * delta, obj->qnodeBound.width, obj->qnodeBound.height);
		exNodeIndexList = qtree->getExNodeIndexList(&exBoundBox, exNodeIndexList);

		//exImgBox : 다음 프레임에 현재 오브젝트(obj)의 sprite의 바운딩 박스의 위치
		exImgBox.setRect(obj->objImg->getBoundingBox().origin.x + obj->moveLen.x * delta, obj->objImg->getBoundingBox().origin.y + obj->moveLen.y * delta, obj->objImg->getBoundingBox().size.width, obj->objImg->getBoundingBox().size.height);

		////////////
		//각 노드별로 충돌 확인

		for (int i = 0; i < 4; i++) {

			if (exNodeIndexList[i] >= 0) {

				//qtreePtr = qtree->searchNode(exNodeIndexList[i]);
				qtreePtr = qtree->getNode(exNodeIndexList[i]);

				//외곽 노드이면 맵 밖으로 나가는지 확인
				if (qtreePtr->isOutside) {

					if (mapBoundaryCheck(&exImgBox)) {

						//첫 충돌 체크
						if (obj->pausedTime == 0) {

							obj->getActionManager()->pauseTarget(obj->objImg);

							//x축으로 움직이고 있었을 때
							//좌
							if (obj->moveLen.x < 0) {
								obj->objImg->setPositionX(mapBoundaryRect[0].getMaxX() + exImgBox.size.width / 2 + 1);
							}
							//우
							else if (obj->moveLen.x > 0) {
								obj->objImg->setPositionX(mapBoundaryRect[1].getMinX() - exImgBox.size.width / 2 - 1);
							}
							//y축으로 움직이고 있었을 때
							//상
							else if (obj->moveLen.y > 0) {
								obj->objImg->setPositionY(mapBoundaryRect[2].getMinY() - exImgBox.size.height / 2 - 1);
							}
							//하
							else if (obj->moveLen.y < 0) {
								obj->objImg->setPositionY(mapBoundaryRect[3].getMaxY() + exImgBox.size.height / 2 + 1);
							}

							//충돌 후 멈추므로 첫 충돌 체크에서 renew해줌
							qtree->renewObjNode(obj);

						}

						obj->pausedTime += delta;
						doCntn = true;
						break;	//continuing outer loop

					}
				} //맵 확인 end


				for each (Obj* colEle in qtreePtr->element) {

					//충돌
					if ((obj->objIndex != colEle->objIndex) && exImgBox.intersectsRect(colEle->objImg->getBoundingBox())) {

						//오브젝트가 이미 멈춰있는 경우 pausedTime이 0보다 큼

						//첫 충돌 체크
						if (obj->pausedTime == 0) {

							obj->getActionManager()->pauseTarget(obj->objImg); //오브젝트 정지

																			   //x축으로 움직이고 있었을 때
																			   //좌
							if (obj->moveLen.x < 0) {
								obj->objImg->setPositionX(colEle->objImg->getBoundingBox().getMaxX() + exImgBox.size.width / 2 + 1);

							}
							//우
							else if (obj->moveLen.x > 0) {
								obj->objImg->setPositionX(colEle->objImg->getBoundingBox().getMinX() - exImgBox.size.width / 2 - 1);

							}
							//y축으로 움직이고 있었을 때
							//상
							else if (obj->moveLen.y > 0) {
								obj->objImg->setPositionY(colEle->objImg->getBoundingBox().getMinY() - exImgBox.size.height / 2 - 1);

							}
							//하
							else if (obj->moveLen.y < 0) {
								obj->objImg->setPositionY(colEle->objImg->getBoundingBox().getMaxY() + exImgBox.size.height / 2 + 1);
							}

							//충돌 후 멈추므로 첫 충돌 체크에서 renew해줌
							qtree->renewObjNode(obj);

						}

						//충돌 상태면 doCntn을 true로 해줌
						obj->pausedTime += delta;
						doCntn = true;
						break;	//충돌했으면 더이상 오브젝트 obj에 대해 충돌체크 할 필요 없음

					}
					/////end collision if

				}
			}
			else {
				break;
			}

			if (doCntn) {
				break;	//node loop break
			}

		}
		////node loop end

		if (doCntn) {
			continue;	//충돌시에 update loop coninue
		}

		//충돌하지 않았을 때 아까 찾은 exbox의 node를 오브젝트의 노드로 대입

		if (exNodeIndexList[0] != obj->qnodeIndex[0] || exNodeIndexList[1] != obj->qnodeIndex[1] || exNodeIndexList[2] != obj->qnodeIndex[2] || exNodeIndexList[3] != obj->qnodeIndex[3]) {
			qtree->renewObjNodeWithSpec(obj, exNodeIndexList);
		}

		obj->getActionManager()->resumeTarget(obj->objImg);
		obj->pausedTime = 0;	//pauseTime 초기화

	}
	////update loop end
}


//isPlayer가 true일때 플레이어 어택이 호출
bool ObjManager::checkAttackCollision(Obj* caller, cocos2d::Rect* exBox, bool isPlayer) {

	bool hit = false;

	//각 노드별로 exBox - 노드 내의 element와 충돌하는지 확인
	for (int i = 0; i < 4; i++) {
		if (caller->qnodeIndex[i] >= 0) {

			//충돌한 물체 검출
			for each (Obj* element in qtree->getNode(caller->qnodeIndex[i])->element)
			{
				//충돌시
				if (exBox->intersectsRect(element->objImg->getBoundingBox())) {

					playerCollisionList.pushBack(element);
					hit = true;

				}
			}
		}
	}

	//각 충돌한 물체에 대한 처리
	for each (Obj* element in playerCollisionList) {
		//<-한꺼번에 나중에 처리
		if (isPlayer) {
			element->loseHPByPlayer();
		}
		else {
			element->loseHPByOther(1);
		}
	}
	playerCollisionList.clear();

	return hit;
}


bool ObjManager::checkSightCollision(ObjRabbit * obj) {

	Rect sightBox;
	sightBox.setRect(obj->objImg->getPositionX() - obj->qnodeBound.width / 2, obj->objImg->getPositionY() - obj->qnodeBound.height/2, obj->qnodeBound.width, obj->qnodeBound.height);
	
	//각 노드별로 exBox - 노드 내의 element와 충돌하는지 확인
	for (int i = 0; i < 4; i++) {

		if (obj->qnodeIndex[i] >= 0) {

			for each (Obj* element in objAvailList)
			{
				if (element->typecode != TYPECODE_PEOPLE) {
					continue;
				}

				if (!sightBox.intersectsRect(element->objImg->getBoundingBox())) {
					continue;
				}

				Vec2 dirVec = element->objImg->getPosition() - obj->objImg->getPosition();

				if (((dirVec.x * obj->moveLen.x + dirVec.y * obj->moveLen.y) / (dirVec.length() * obj->moveLen.length())) > cos45) {
					return true;
				}


			}

		}
	}

	//겹치지 않는다면 false 반환
	return false;
}


//다람쥐용 시야체크
Obj* ObjManager::checkSightCollision(ObjSquaral * obj) {

	//1
	float b1 = obj->objImg->getPositionY() - obj->objImg->getPositionX();

	//2
	float b2 = obj->objImg->getPositionY() + obj->objImg->getPositionX();

	//각 노드별로 exBox - 노드 내의 element와 충돌하는지 확인
	for (int i = 0; i < 4; i++) {
		if (obj->qnodeIndex[i] >= 0) {

			for each (Obj* element in objAvailList)
			{
				if (element->typecode != TYPECODE_PEOPLE) {
					continue;
				}

				//오브젝트가 원과 충돌하는지 확인
				if (!element->objImg->getBoundingBox().intersectsCircle(obj->objImg->getPosition(), obj->squaralSightRadius)) {
					continue;
				}

				Vec2 dirVec = element->objImg->getPosition() - obj->objImg->getPosition();

				if (((dirVec.x*obj->moveLen.x + dirVec.y*obj->moveLen.y)/ (dirVec.length() * obj->moveLen.length())) > cos45) {
					return element;
				}

			}

		}
	}



	return nullptr;
}

//다람쥐용 공격 체크
bool ObjManager::checkAttackCollision(Obj* obj, const cocos2d::Vec2* center, float radius) {
	bool hit = false;

	//충돌한 물체 검출
	for each (Obj* element in objAvailList)
	{

		if (element->objIndex == obj->objIndex || element->typecode == TYPECODE_NONE) {	//발사자는 충돌 체크에서 제외
			continue;
		}

		//충돌시
		if (element->objImg->getBoundingBox().intersectsCircle(*center, radius)) {

			acornCollisionList.pushBack(element);
			hit = true;

		}
	}

	//각 충돌한 물체에 대한 처리
	for each (Obj* element in acornCollisionList) {
		if (element->typecode == TYPECODE_PEOPLE)
			element->loseHPByOther(1);	//<-한꺼번에 나중에 처리
	}
	acornCollisionList.clear();

	return hit;
}



//raycasting
ColObj* ObjManager::doRaycast(Obj* caller, Vec2 startPoint, Vec2 dir, float d) {

	bool intersectionUpdate = false;

	float t1x;
	float t1y;
	float t2x;
	float t2y;

	float Tnear = mapRect.getMinX();
	float Tfar = mapRect.getMaxX();

	Vec2 invDir = Vec2(1.0f / dir.x, 1.0f / dir.y);

	intersectedObj->intersectDistance = d * 2;	//d보다 크도록 초기화

												//각 노드별로 exBox - 노드 내의 element와 충돌하는지 확인
	for (int i = 0; i < 4; i++) {
		if (caller->qnodeIndex[i] >= 0) {

			//충돌한 물체 검출
			for each (Obj* element in qtree->getNode(caller->qnodeIndex[i])->element)
			{

				for each (Obj* element in objAvailList) {

					if (element->objIndex == caller->objIndex) {
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

					//여기까지 살아남았으면 교점이 존재함		
					//교점까지의 거리가 d보다 커도 안됨
					if ((startPoint + Tnear * dir).distance(startPoint) > d) {
						continue;
					}

					////기존에 존재하던 다른 오브젝트와의 교점과 비교해 더 가까운 것을 선택
					if (intersectedObj->intersectDistance > (startPoint + Tnear * dir).distance(startPoint)) {
						intersectionUpdate = true;

						intersectedObj->obj = element;
						intersectedObj->intersectPoint = startPoint + Tnear * dir;
						intersectedObj->intersectDistance = intersectedObj->intersectPoint.distance(startPoint);

					}


				}


			}
		}
	}




	if (intersectionUpdate) {
		return intersectedObj;
	}
	else {
		//전부 continue 됐을 경우 교점 없음 - 돌아감
		return nullptr;
	}


}
