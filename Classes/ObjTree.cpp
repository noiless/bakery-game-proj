#include "ObjTree.h"
#include "GameScene.h"

USING_NS_CC;

ObjTree::ObjTree() : inUse(false), HP(5) {
	typecode = TYPECODE_TREE;
	objImg = Sprite::create("tree_down.png");

	this->addChild(objImg);
}

bool ObjTree::init(cocos2d::Vec2 initPos) {

	inUse = true;
	HP = 5;
	objImg->setPosition(initPos);
	objImg->setOpacity(255);

	state = dynamic_cast<StateTree*> (StateTree::treeNormal);
	state->initAction(this);

	return true;
}

bool ObjTree::deInit() {
	CCLOG("deinit tree");
	//member value init

	inUse = false;	//������Ʈ�� ������� �ʵ��� ����
	this->removeFromParentAndCleanup(true);

	return true;
}

void ObjTree::loseHP() {
	HP--;
	if (HP <= 0) {
		//state ���� �� action �ʱ�ȭ
		state = dynamic_cast<StateTree*> (StateTree::treeDead);
		state->initAction(this);

	}
}