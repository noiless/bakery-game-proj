#include "QTree.h"
#include "GameScene.h"

cocos2d::Rect QTree::upOutside;
cocos2d::Rect QTree::downOutside;
cocos2d::Rect QTree::leftOutside;
cocos2d::Rect QTree::rightOutside;
QTree* QTree::root;

//objManager�� setMapRect���� ȣ���
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
	root = this;	//�ǳ�...?

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

	//�ܰ� ������� Ȯ��
	if (bound.intersectsRect(upOutside) || bound.intersectsRect(downOutside) || bound.intersectsRect(leftOutside) || bound.intersectsRect(rightOutside)) {
		CCLOG("outside!");
		isOutside = true;
	}
	else {
		isOutside = false;
	}

	nodeIndex = parentIndex * 4 + childIndex + 1;
	haveChild = false;

	CCLOG("element size %d",element.size());

}

QTree::~QTree() {
	delete []child;
}

void QTree::clear() {
	element.clear();
}


//MAX LEVEL�� �����ߴ����� split�� ȣ���ϱ� ���� �Ǵ�
void QTree::split() {
	//child ���� �� ���� �ʱ�ȭ, ���� �� ��尡 ������ �ִ� ������Ʈ�� �ڽĵ鿡 ���ġ

	haveChild = true;

	//�ڽĳ�� ����
	child[0] = new QTree(level, bound, 0, nodeIndex);
	child[1] = new QTree(level, bound, 1, nodeIndex);
	child[2] = new QTree(level, bound, 2, nodeIndex);
	child[3] = new QTree(level, bound, 3, nodeIndex);

	//������ �ִ� ������Ʈ ���ġ
	for each (Obj* obj in element) {
		insert(obj);
	}

	//���� ���� ����Ʈ ���
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

void QTree::insert(Obj* obj) {

	//���� ��尡 leaf
	if (!haveChild) {

		CCLOG("inserting %d in node %d ", obj->objIndex, nodeIndex);

		//element�� ���� - insert ȣ�� �� ������ ���ԵǴ��� ���� Ȯ������
		element.push_back(obj);

		//qnodeindex ����
		for (int i = 0; i < 4; i++) {
			if (obj->qnodeIndex[i] < 0) {
				obj->qnodeIndex[i] = nodeIndex;	//���԰� �Բ� qnodeIndex�� ���� ����� ����ε����� ����
				break;
			}
		}

		//������ ������ �ƴϰ� max element ������ �Ѿ��ٸ� �ڽĳ�� �������
		if (level != MAX_LEVEL && element.size() > MAX_ELEMENT) {

			//���� ��忡 ���� ���ҵ��� qnodeIndex���� �� ����� �ε����� -1�� �ʱ�ȭ����
			for each (Obj* i in element) {
				for (int index = 0; index < 4; index++) {
					if (i->qnodeIndex[index] == nodeIndex) {
						i->qnodeIndex[index] = -1;
						break;
					}						
				}
			}

			CCLOG("%d level node  %d split",level,nodeIndex);
			split();
		}

	}
	//�ڽ� ��尡 ����
	else {
		//�ڽ� ����� ���� Ȯ���ؼ� ���⼭ pruning ����. ���� Ȯ�ν� �ڽ� ��忡�� insert ȣ��
		if (obj->objImg->getBoundingBox().intersectsRect(child[0]->bound)) {
			child[0]->insert(obj);
		}
		if (obj->objImg->getBoundingBox().intersectsRect(child[1]->bound)) {
			child[1]->insert(obj);
		}
		if (obj->objImg->getBoundingBox().intersectsRect(child[2]->bound)) {
			child[2]->insert(obj);
		}
		if (obj->objImg->getBoundingBox().intersectsRect(child[3]->bound)) {
			child[3]->insert(obj);
		}

	}
}

//nodeIndex�� ��� Ž�� �� �ش� ��� ��ȯ
QTree* QTree::searchNode(int nodeIndex){
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
		int q = -1;	//��
		int r;	//������

		while (q != 0) {
			q = (newNodeIdx - 1) / 4;
			r = (newNodeIdx - 1) - q * 4;
			newNodeIdx = q;
			childSearchRoute[loopCount] = r;
			loopCount++;
		}

		QTree* tempNode = root;

		//ã�� child index�� ���ؼ� ��� ã��
		for (int i = loopCount - 1; i >= 0; i--) {
			tempNode = tempNode->child[childSearchRoute[i]];
		}

		return tempNode;
	}
}

void QTree::remove(Obj* obj) {
	CCLOG("nodeindex %d before remove %d",nodeIndex, element.size());

	element.remove(obj);
	for (int i = 0; i < 4; i++) {
		//remove �� ������Ʈ�� qnodeindex �ʱ�ȭ
		if (obj->qnodeIndex[i] == nodeIndex) {
			obj->qnodeIndex[i] = -1;
			break;
		}
	}

	CCLOG("after remove %d", element.size());

	CCLOG("remove %d in node %d", obj->objIndex, nodeIndex);
	


	if (element.size() > 15) {
		for each (Obj* i in element) {
			CCLOG("???index %d",i->objIndex);
		}
	}

}

void QTree::removeObjFromAllNode(Obj* obj) {
	QTree* tempPtr;
	CCLOG("removeall");
	for (int i = 0; i < 4; i++) {
		tempPtr = searchNode(obj->qnodeIndex[i]);
		if (tempPtr != nullptr)
			tempPtr->remove(obj);
	}

}

//���� ��� ��忡�� ���� �������� ���� �� ������Ʈ�� �ٽ� insert�� ������Ʈ�� ���� ��带 ����
void QTree::renewObjNode(Obj* obj) {

	for (int i = 0; i < 4; i++) {
		//������ ��� �������� ���

		if((obj->qnodeIndex[i] >= 0) && checkNodeBoundaryTouch(searchNode(obj->qnodeIndex[i])->bound, obj->objImg->getBoundingBox())){

			removeObjFromAllNode(obj);
			insert(obj);	//root�� ȣ���ؼ� root���� ����
			CCLOG("renewed %d",obj->objIndex);
			break;
		}
	}

	//removeObjFromAllNode(obj);
	//insert(obj);	//root�� ȣ���ؼ� root���� ����
	
}

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