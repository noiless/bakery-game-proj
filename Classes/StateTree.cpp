#include "ObjTree.h"
#include "StateTree.h"
#include "GameScene.h"

USING_NS_CC;

//class StateTree

StateTreeNormal* StateTree::treeNormal = new StateTreeNormal;
StateTreeDead* StateTree::treeDead = new StateTreeDead;


//class StateTreeNormal

StateTreeNormal::StateTreeNormal() {

}

void StateTreeNormal::initAction(ObjTree * obj) {

	//5초간 대기
	//delay
	DelayTime * delay1 = DelayTime::create(actionDuration);

	//토끼 생성
	auto createRabbit = CallFunc::create(
		[=]
	{
		ObjRabbit* newRabbit = GameWorld::objManager->getFreeObjRabbit();

		CCASSERT((newRabbit != nullptr), "NEED LARGER OBJECT POOL");

		newRabbit->init(obj->objImg->getPosition() + Vec2(0, obj->objImg->getContentSize().height / 2));

		obj->getParent()->addChild(newRabbit);

		

	});

	Sequence* seq = Sequence::create(delay1, createRabbit, nullptr);


	obj->objImg->runAction(RepeatForever::create(seq));
	
}



//class StateTreeDead

StateTreeDead::StateTreeDead() {

}

void StateTreeDead::initAction(ObjTree * obj) {

	GameWorld::objManager->deleteObjectAvailList(obj); //ObjManager에서 avail list에서 제거해줌
	GameWorld::ui->myAdPointGrow();

	obj->objImg->stopAllActions();	//기존의 액션 중지

	//action 설정
	auto fadeout = FadeOut::create(2);

	auto callback = CallFunc::create(
		[=]
	{
		obj->deInit();
	});

	obj->objImg->runAction(Sequence::create(fadeout, callback, nullptr));

}