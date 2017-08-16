#include "QTree.h"
#include "GameScene.h"

cocos2d::Rect QTree::upOutside;
cocos2d::Rect QTree::downOutside;
cocos2d::Rect QTree::leftOutside;
cocos2d::Rect QTree::rightOutside;
QTree* QTree::root;
QTree* QTree::qtreeMap[MAX_LEVEL*MAX_LEVEL*MAX_LEVEL*MAX_LEVEL];

//objManager - setMapRect에서 호출
void QTree::init(cocos2d::Rect mapBoundingBox) {
	upOutside.setRect(mapBoundingBox.origin.x, mapBoundingBox.origin.y + mapBoundingBox.size.height - 50, mapBoundingBox.size.width, 50);
	downOutside.setRect(mapBoundingBox.origin.x, mapBoundingBox.origin.y, mapBoundingBox.size.width, 50);
	leftOutside.setRect(mapBoundingBox.origin.x, mapBoundingBox.origin.y, 50, mapBoundingBox.size.height);
	rightOutside.setRect(mapBoundingBox.origin.x + mapBoundingBox.size.width - 50, mapBoundingBox.origin.y, 50, mapBoundingBox.size.height);

}

//set root node
QTree::QTree(cocos2d::Rect parentBound) {
	level = 0;
	bound = parentBound;
	isOutside = true;
	nodeIndex = 0;
	root = this;	//되나...?
	qtreeMap[0] = this;

	haveChild = false;
}


QTree::QTree(int parentLevel, cocos2d::Rect parentBound, int childIndex, int parentIndex) {
	level = parentLevel + 1;

	if (childIndex == 0) {
		bound.setRect(parentBound.origin.x, parentBound.origin.y + parentBound.size.height / 2, parentBound.size.width / 2, parentBound.size.height / 2);
	}
	else if (childIndex == 1) {
		bound.setRect(parentBound.origin.x + parentBound.size.width / 2, parentBound.origin.y + parentBound.size.height / 2, parentBound.size.width / 2, parentBound.size.height / 2);
	}
	else if (childIndex == 2) {
		bound.setRect(parentBound.origin.x, parentBound.origin.y, parentBound.size.width / 2, parentBound.size.height / 2);
	}
	else if (childIndex == 3) {
		bound.setRect(parentBound.origin.x + parentBound.size.width / 2, parentBound.origin.y, parentBound.size.width / 2, parentBound.size.height / 2);
	}

	//외곽 노드인지 확인
	if (bound.intersectsRect(upOutside) || bound.intersectsRect(downOutside) || bound.intersectsRect(leftOutside) || bound.intersectsRect(rightOutside)) {
		isOutside = true;
	}
	else {
		isOutside = false;
	}

	nodeIndex = parentIndex * 4 + childIndex + 1;
	qtreeMap[nodeIndex] = this;
	haveChild = false;

}

QTree::~QTree() {
	clear();
}

void QTree::clear() {
	element.clear();
}


//MAX LEVEL에 도달했는지는 split을 호출하기 전에 판단
void QTree::split() {
	//child 생성 후 각각 초기화, 현재 이 노드가 가지고 있는 오브젝트를 자식들에 재배치

	haveChild = true;

	//자식노드 생성
	child[0] = new QTree(level, bound, 0, nodeIndex);
	child[1] = new QTree(level, bound, 1, nodeIndex);
	child[2] = new QTree(level, bound, 2, nodeIndex);
	child[3] = new QTree(level, bound, 3, nodeIndex);

	//가지고 있던 오브젝트 재배치
	for each (Obj* obj in element) {
		insert(obj, false);
	}

	//현재 원소 리스트 비움
	clear();

	/////temp
	DrawNode* d1 = DrawNode::create();
	d1->drawLine(bound.origin + Vec2(bound.size.width / 2, 0), bound.origin + Vec2(bound.size.width / 2, bound.size.height), Color4F::RED);

	DrawNode* d2 = DrawNode::create();
	d2->drawLine(bound.origin + Vec2(0, bound.size.height / 2), bound.origin + Vec2(bound.size.width, bound.size.height / 2), Color4F::BLUE);

	GameWorld::objManager->addChild(d1);
	GameWorld::objManager->addChild(d2);
	//////end temp

}

void QTree::insert(Obj* obj, bool front) {

	//현재 노드가 leaf
	if (!haveChild) {

		//element에 삽입 - insert 호출 전 범위에 포함되는지 먼저 확인했음
		if (front) {
			element.push_front(obj);
		}
		else {
			element.push_back(obj);
		}

		//qnodeindex 변경

		for (int i = 0; i < 4; i++) {
			if (obj->qnodeIndex[i] < 0) {
				obj->qnodeIndex[i] = nodeIndex;
				break;
			}
		}

		//마지막 레벨이 아니고 max element 갯수를 넘었다면 자식노드 만들어줌
		if (level < MAX_LEVEL && element.size() > MAX_ELEMENT) {

			//현재 노드에 속한 원소들의 qnodeIndex에서 이 노드의 인덱스를 -1로 초기화해줌
			for each (Obj* i in element) {
				for (int index = 0; index < 4; index++) {
					if (i->qnodeIndex[index] == nodeIndex) {
						i->qnodeIndex[index] = -1;
						break;
					}
				}
			}

			split();
		}

	}
	//자식 노드가 존재
	else {
		//자식 노드의 범위 확인해서 여기서 pruning 해줌. 범위 확인시 자식 노드에서 insert 호출

		Rect exBoundBox;
		exBoundBox.setRect(obj->objImg->getPositionX() - obj->qnodeBound.width / 2, obj->objImg->getPositionY() - obj->qnodeBound.height / 2, obj->qnodeBound.width, obj->qnodeBound.height);

		if (exBoundBox.intersectsRect(child[0]->bound)) {
			child[0]->insert(obj, front);
		}
		if (exBoundBox.intersectsRect(child[1]->bound)) {
			child[1]->insert(obj, front);
		}
		if (exBoundBox.intersectsRect(child[2]->bound)) {
			child[2]->insert(obj, front);
		}
		if (exBoundBox.intersectsRect(child[3]->bound)) {
			child[3]->insert(obj, front);
		}

	}
}

//nodeIndex로 노드 탐색 후 해당 노드 반환
//이젠 안씀...
QTree* QTree::searchNode(int nodeIndex) {
	if (nodeIndex < 0) {
		return nullptr;
	}
	else if (nodeIndex == 0) {
		return root;
	}
	else {
		int childSearchRoute[MAX_LEVEL];
		int newNodeIdx = nodeIndex;
		int loopCount = 0;
		int q = -1;	//몫
		int r;	//나머지

		while (q != 0) {
			q = (newNodeIdx - 1) / 4;
			r = (newNodeIdx - 1) - q * 4;
			newNodeIdx = q;
			childSearchRoute[loopCount] = r;
			loopCount++;
		}

		QTree* tempNode = root;

		//찾은 child index를 통해서 노드 찾음
		for (int i = loopCount - 1; i >= 0; i--) {
			tempNode = tempNode->child[childSearchRoute[i]];
		}

		return tempNode;
	}
}

void QTree::remove(Obj* obj) {

	//CCLOG("nodeindex %d",nodeIndex);
	
	element.remove(obj);
	
	for (int i = 0; i < 4; i++) {
		//remove 후 오브젝트의 qnodeindex 초기화
		if (obj->qnodeIndex[i] == nodeIndex) {
			obj->qnodeIndex[i] = -1;
			break;
		}
	}

}

void QTree::removeObjFromAllNode(Obj* obj) {
	for (int i = 0; i < 4; i++) {
		if (qtreeMap[obj->qnodeIndex[i]] != nullptr)
			qtreeMap[obj->qnodeIndex[i]]->remove(obj);
	}


}

//속한 모든 노드에서 현재 오브젝를 제거 후 오브젝트를 다시 insert해 오브젝트가 속한 노드를 갱신
//충돌시에만 사용
void QTree::renewObjNode(Obj* obj) {

	removeObjFromAllNode(obj);
	insert(obj, false);	//root로 호출해서 root에다 넣음

}

void QTree::renewObjNodeWithSpec(Obj* obj, int* indexList) {
	//속해있는 노드들에서 오브젝트 제거
	removeObjFromAllNode(obj);

	for (int i = 0; i < 4; i++) {
		//각 노드에 이 오브젝트 추가
		obj->qnodeIndex[i] = indexList[i];
		if (indexList[i] >= 0) {
			qtreeMap[indexList[i]]->insert(obj, false);
		}
	}
}

//안씀
bool QTree::checkNodeBoundaryTouch(cocos2d::Rect nodeBound, cocos2d::Rect objBound) {

	if ((nodeBound.origin.x > objBound.origin.x && nodeBound.origin.x < objBound.origin.x + objBound.size.width)
		|| (nodeBound.origin.x + nodeBound.size.width > objBound.origin.x && nodeBound.origin.x + nodeBound.size.width < objBound.origin.x + objBound.size.width)
		|| (nodeBound.origin.y > objBound.origin.y && nodeBound.origin.y < objBound.origin.y + objBound.size.height)
		|| (nodeBound.origin.y + nodeBound.size.height > objBound.origin.y && nodeBound.origin.y + nodeBound.size.height < objBound.origin.y + objBound.size.height)) {
		return true;
	}
	else {
		return false;
	}


}

//exBox로 포함될 예상 노드 구하기
int* QTree::getExNodeIndexList(Rect* exbound, int* exNodeList) {
	exNodeList[0] = -1; exNodeList[1] = -1; exNodeList[2] = -1; exNodeList[3] = -1;

	exFindNode(root, exbound, exNodeList);

	return exNodeList;
}

void QTree::exFindNode(QTree* nowNode, cocos2d::Rect* exbound, int* indexList) {

	if (nowNode->haveChild) {
		//자식의 범위와 맞는지 비교하고 맞을 경우 자식 노드에 대해 동일 함수를 재귀적으로 호출

		if (nowNode->child[0]->bound.intersectsRect(*exbound)) {
			exFindNode(nowNode->child[0], exbound, indexList);
		}
		if (nowNode->child[1]->bound.intersectsRect(*exbound)) {
			exFindNode(nowNode->child[1], exbound, indexList);
		}
		if (nowNode->child[2]->bound.intersectsRect(*exbound)) {
			exFindNode(nowNode->child[2], exbound, indexList);
		}
		if (nowNode->child[3]->bound.intersectsRect(*exbound)) {
			exFindNode(nowNode->child[3], exbound, indexList);
		}
	}
	else {
		//포함될 경우 indexList에 이 노드의 인덱스를 포함시켜준다
		for (int i = 0; i < 4; i++) {
			if (indexList[i] < 0) {
				indexList[i] = nowNode->nodeIndex;
				break;
			}
		}
	}

}


void QTree::removeNodes(QTree* nowNode) {

	//자식이 있을때
	if (nowNode->haveChild) {

		//자식 노드 모두 삭제 후 현재 노드의 haveChild값 변경
		for (int i = 0; i < 4; i++) {
			removeNodes(nowNode->child[i]);
		}

		nowNode->haveChild = false;
		nowNode->~QTree();

	}
	//leaf node일때
	else {
		nowNode->~QTree();	//해제
	}

}

QTree* QTree::getNode(int nodeIndex) {
	return qtreeMap[nodeIndex];
}