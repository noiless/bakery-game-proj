#include "ObjTree.h"
#include "GameScene.h"

USING_NS_CC;

ObjTree::ObjTree() : inUse(false), HP(5) {
	typecode = TYPECODE_TREE;
	objImg = Sprite::create("img/tree_down.png");

	this->addChild(objImg);

	qnodeBound = Size(objImg->getContentSize());

}

bool ObjTree::init(cocos2d::Vec2 initPos) {

	inUse = true;
	HP = 5;
	objImg->setPosition(initPos);
	objImg->setOpacity(255);

	qnodeIndexInit();

	state = dynamic_cast<StateTree*> (StateTree::treeNormal);
	state->initAction(this);

	return true;
}

bool ObjTree::deInit() {
	//CCLOG("deinit tree");
	//member value init

	inUse = false;	//������Ʈ�� ������� �ʵ��� ����
	this->removeFromParentAndCleanup(true);

	return true;
}

void ObjTree::loseHPByPlayer() {
	HP--;
	if (HP <= 0) {
		deadByPlayer = true;
		//state ���� �� action �ʱ�ȭ
		state = dynamic_cast<StateTree*> (StateTree::treeDead);
		state->initAction(this);

	}
}

void ObjTree::loseHPByOther(int damage) {
	HP -= damage;
	if (HP <= 0) {
		deadByPlayer = false;
		//state ���� �� action �ʱ�ȭ
		state = dynamic_cast<StateTree*> (StateTree::treeDead);
		state->initAction(this);

	}
}