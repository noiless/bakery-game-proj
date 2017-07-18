#include "UI.h"

#define LABEL_FONT_SIZE 20
#define BAR_HEIGHT 25
#define BAR_WEIHT 120
#define MY_BAR_ORIGIN (myMoneyLabel->getPosition() - Vec2(BAR_WEIHT, LABEL_FONT_SIZE + BAR_HEIGHT))
#define OTHER_BAR_ORIGIN (otherMoneyLabel->getPosition() + Vec2(0, -LABEL_FONT_SIZE - BAR_HEIGHT))
#define AD_BAR_ORIGIN ((MY_BAR_ORIGIN + OTHER_BAR_ORIGIN) / 2)
#define USER_CHANGE_UNIT 10
#define TIME_CHANGE_UNIT 3

USING_NS_CC;


//생성자 - 변수 초기화
UI::UI(){
	myMoney = 0;
	otherMoney = 0;
	myBreadPoint = 50;
	otherBreadPoint = 50;
	adPoint = 50;

	checkTime = 0;

	int labelFontSize = 20;

}

bool UI::init() {
	
	visibleSize = Director::getInstance()->getVisibleSize();

	//내 빵 게이지
	myBreadBox = DrawNode::create();
	myBreadBar = DrawNode::create();
	myBreadBox->setAnchorPoint(Vec2(1, 1));
	myBreadBar->setAnchorPoint(Vec2(1, 1));

	myMoneyLabel = Label::createWithTTF("0", "fonts/arial.ttf", LABEL_FONT_SIZE);
	myMoneyLabel->setAnchorPoint(Vec2(1, 1));

	//상대 빵 게이지
	otherBreadBox = DrawNode::create();
	otherBreadBar = DrawNode::create();
	otherBreadBox->setAnchorPoint(Vec2(0, 1));
	otherBreadBar->setAnchorPoint(Vec2(0, 1));

	otherMoneyLabel = Label::createWithTTF("500", "fonts/arial.ttf", LABEL_FONT_SIZE);
	otherMoneyLabel->setAnchorPoint(Vec2(0, 1));

	//홍보 게이지
	adBox = DrawNode::create();
	adOtherBar = DrawNode::create();
	adMyBar = DrawNode::create();

	this->addChild(myBreadBar);
	this->addChild(myBreadBox);	
	this->addChild(myMoneyLabel);

	this->addChild(otherBreadBar);
	this->addChild(otherBreadBox);
	this->addChild(otherMoneyLabel);

	this->addChild(adOtherBar);
	this->addChild(adMyBar);
	this->addChild(adBox);
	//UI : 매 프레임마다 새로 그려줘야 함

	this->scheduleUpdate();

	return true;
}


void UI::myBreadSold() {
	myMoney += 500;
}

void UI::otherBreadSold() {
	otherMoney += 500;
}

void UI::myBreadPointGrow() {
	if (myBreadPoint < (100 - USER_CHANGE_UNIT))
		myBreadPoint += USER_CHANGE_UNIT;
	else 
		myBreadPoint = 100;
}

void UI::otherBreadPointDown() {
	if (otherBreadPoint > USER_CHANGE_UNIT)
		otherBreadPoint -= USER_CHANGE_UNIT;
	else
		otherBreadPoint = 0;
}

void UI::myAdPointGrow() {
	if (adPoint < (100 - USER_CHANGE_UNIT))
		adPoint += USER_CHANGE_UNIT;
	else
		adPoint = 100;
}

//1초마다 포인트 관련한 처리
void UI::allPointChange() {
	if (adPoint > TIME_CHANGE_UNIT)
		adPoint -= TIME_CHANGE_UNIT;
	else
		adPoint = 0;

	if (myBreadPoint > TIME_CHANGE_UNIT)
		myBreadPoint -= TIME_CHANGE_UNIT;
	else
		myBreadPoint = 0;

	if (otherBreadPoint < (100- TIME_CHANGE_UNIT))
		otherBreadPoint += TIME_CHANGE_UNIT;
	else
		otherBreadPoint = 100;

	/////////temp
	myMoney += 500;
	otherMoney += 1000;
	/////////temp end

	//CCLOG("point changed");
}

void UI::drawUI() {
	cam = Camera::getDefaultCamera();
	
	//draw Label
	char text[256];

	sprintf(text, "%d", myMoney);
	myMoneyLabel->setString(text);

	sprintf(text, "%d", otherMoney);
	otherMoneyLabel->setString(text);

	myMoneyLabel->setPosition(cam->getPosition() + Vec2(visibleSize.width/2, visibleSize.height / 2));
	otherMoneyLabel->setPosition(cam->getPosition() + Vec2(- visibleSize.width / 2, visibleSize.height / 2));

	//draw box, bar
	myBreadBar->clear();
	myBreadBar->drawSolidRect(MY_BAR_ORIGIN + Vec2(1, 1), MY_BAR_ORIGIN + Vec2(myBreadPoint * 1.2, BAR_HEIGHT), Color4F::RED);

	myBreadBox->clear();
	myBreadBox->drawRect(MY_BAR_ORIGIN, MY_BAR_ORIGIN + Vec2(BAR_WEIHT, BAR_HEIGHT), Color4F::BLACK);

	otherBreadBar->clear();
	otherBreadBar->drawSolidRect(OTHER_BAR_ORIGIN + Vec2(1, 1), OTHER_BAR_ORIGIN + Vec2(otherBreadPoint * 1.2, BAR_HEIGHT), Color4F::BLUE);

	otherBreadBox->clear();
	otherBreadBox->drawRect(OTHER_BAR_ORIGIN, OTHER_BAR_ORIGIN + Vec2(BAR_WEIHT, BAR_HEIGHT), Color4F::BLACK);

	adOtherBar->clear();
	adOtherBar->drawSolidRect(AD_BAR_ORIGIN + Vec2(1, 1), AD_BAR_ORIGIN + Vec2((100 - adPoint) * 1.2, BAR_HEIGHT), Color4F::BLUE);

	adMyBar->clear();
	adMyBar->drawSolidRect(AD_BAR_ORIGIN + Vec2((100 - adPoint) * 1.2, 1), AD_BAR_ORIGIN + Vec2(BAR_WEIHT, BAR_HEIGHT), Color4F::RED);

	adBox->clear();
	adBox->drawRect(AD_BAR_ORIGIN, AD_BAR_ORIGIN + Vec2(BAR_WEIHT, BAR_HEIGHT), Color4F::BLACK);
	
}

void UI::update(float delta) {

	//2초마다 포인트 변동 - 지난 시간만큼 게이지 하락/상승
	if (checkTime > 2) {
		checkTime = 0;
		allPointChange();
	}
	else {
		checkTime += delta;
	}

	//draw point
	drawUI();

}