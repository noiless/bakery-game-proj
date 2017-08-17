#include "ObjTree.h"
#include "StateTree.h"
#include "GameScene.h"

USING_NS_CC;
using namespace pugi;

//class StateTree

StateTreeNormal* StateTree::treeNormal = new StateTreeNormal;
StateTreeDead* StateTree::treeDead = new StateTreeDead;

void StateTree::initStates(xml_node stateNode) {
	xml_node tempNode = stateNode.child("Normal");

	treeNormal->actionDuration = tempNode.child("ActionDuration").text().as_int();

	tempNode = stateNode.child("Dead");
	treeDead->actionDuration = tempNode.child("ActionDuration").text().as_int();

}


//class StateTreeNormal

StateTreeNormal::StateTreeNormal() {

}

void StateTreeNormal::initAction(ObjTree * obj) {

	//delay
	DelayTime * delay1 = DelayTime::create(actionDuration);

	//토끼 생성
	auto createRabbit = CallFunc::create(
		[=]
	{
		GameWorld::objManager->getObjRabbitFromPool(obj->getParent() ,obj->objImg->getPosition() - Vec2(0, obj->objImg->getContentSize().height - 1));

	});

	Sequence* seq = Sequence::create(delay1, createRabbit, nullptr);


	obj->objImg->runAction(RepeatForever::create(seq));
	
}



//class StateTreeDead

StateTreeDead::StateTreeDead() {
	experimental::AudioEngine::play2d("sound/sound_tree_dead.mp3", false, 0, &treeDeadEffect);
}

void StateTreeDead::initAction(ObjTree * obj) {

	GameWorld::objManager->deleteObjectAvailList(obj); //ObjManager에서 avail list에서 제거해줌

	if (obj->deadByPlayer) {
		GameWorld::ui->myAdPointGrow();
	}
	else {
		GameWorld::ui->myAdPointDown();
	}

	obj->objImg->stopAllActions();	//기존의 액션 중지

	//사운드 재생
	if (obj->objImg->getPositionX() > Camera::getDefaultCamera()->getPositionX() - 480 && obj->objImg->getPositionX() < Camera::getDefaultCamera()->getPositionX() + 480
		&& obj->objImg->getPositionY() > Camera::getDefaultCamera()->getPositionY() - 320 && obj->objImg->getPositionY() < Camera::getDefaultCamera()->getPositionY() + 320) {
		experimental::AudioEngine::play2d("sound/sound_tree_dead.mp3", false, 0.3, &treeDeadEffect);
	}

	//action 설정
	auto fadeout = FadeOut::create(actionDuration);

	auto callback = CallFunc::create(
		[=]
	{
		obj->deInit();
	});

	obj->objImg->runAction(Sequence::create(fadeout, callback, nullptr));

}