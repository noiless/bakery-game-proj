#include "UI.h"
#include "Score.h"

#define LABEL_FONT_SIZE 20
#define BAR_HEIGHT 25
#define BAR_WEIHT 120
#define MY_BAR_ORIGIN (myMoneyLabel->getPosition() - Vec2(BAR_WEIHT, LABEL_FONT_SIZE + BAR_HEIGHT))
#define OTHER_BAR_ORIGIN (otherMoneyLabel->getPosition() + Vec2(0, -LABEL_FONT_SIZE - BAR_HEIGHT))
#define AD_BAR_ORIGIN ((MY_BAR_ORIGIN + OTHER_BAR_ORIGIN) / 2)
#define USER_CHANGE_UNIT 10
#define TIME_CHANGE_UNIT 3

USING_NS_CC;


//������ - ���� �ʱ�ȭ
UI::UI(int playerMaxHP) : myMoney(0), otherMoney(0), myBreadPoint(50), otherBreadPoint(50), adPoint(50), checkTime(0), myHP(playerMaxHP) {

}

UI::~UI() {
	//�� �� ������
	Score::finalScore = myMoney;

	this->removeAllChildren();
	this->unscheduleUpdate();
	this->removeFromParentAndCleanup(true);

	//delete myBreadBox;
	//delete myBreadBar;
	//delete myMoneyLabel;

	////��� �� ������
	//delete otherBreadBox;
	//delete otherBreadBar;
	//delete otherMoneyLabel;

	////ȫ�� ������
	//delete adBox;
	//delete adOtherBar;
	//delete adMyBar;

	//delete HPLable;
}

bool UI::init() {
	
	visibleSize = Director::getInstance()->getVisibleSize();

	//�� �� ������
	myBreadBox = DrawNode::create();
	myBreadBar = DrawNode::create();
	myBreadBox->setAnchorPoint(Vec2(1, 1));
	myBreadBar->setAnchorPoint(Vec2(1, 1));

	myMoneyLabel = Label::createWithTTF("0", "fonts/arial.ttf", LABEL_FONT_SIZE);
	myMoneyLabel->setAnchorPoint(Vec2(1, 1));

	//��� �� ������
	otherBreadBox = DrawNode::create();
	otherBreadBar = DrawNode::create();
	otherBreadBox->setAnchorPoint(Vec2(0, 1));
	otherBreadBar->setAnchorPoint(Vec2(0, 1));

	otherMoneyLabel = Label::createWithTTF("500", "fonts/arial.ttf", LABEL_FONT_SIZE);
	otherMoneyLabel->setAnchorPoint(Vec2(0, 1));

	//ȫ�� ������
	adBox = DrawNode::create();
	adOtherBar = DrawNode::create();
	adMyBar = DrawNode::create();

	HPLable = Label::createWithTTF("Player HP : 20", "fonts/arial.ttf", LABEL_FONT_SIZE);
	HPLable->setAnchorPoint(Vec2(1, 0));

	this->addChild(myBreadBar);
	this->addChild(myBreadBox);	
	this->addChild(myMoneyLabel);

	this->addChild(otherBreadBar);
	this->addChild(otherBreadBox);
	this->addChild(otherMoneyLabel);

	this->addChild(adOtherBar);
	this->addChild(adMyBar);
	this->addChild(adBox);

	this->addChild(HPLable);
	//UI : �� �����Ӹ��� ���� �׷���� ��

	this->scheduleUpdate();

	return true;
}

//�մ��� �� ���� / ��� ���� / Ȥ�� �ƹ� ���Կ��� ���� �ʴ��� Ȯ�� �� dir�� ����
int UI::selectShop() {
	int ranVal = rand() % 100;
	
	//�� ����
	if (ranVal <= adPoint) {
		return 1;	//DIR_RIGHT
	}

	ranVal = rand() % 100;	//������ ����
	
	if (ranVal <= (100 - adPoint)) {
		return 0;	//DIR_LEFT
	}
	else {
		return 2;	//DIR_UP - �ƹ� ���Կ��� ���� ����
	}
}

bool UI::buyBread(bool myStore) {
	int ranVal = rand() % 100;
	//mystore : true - �� ���Կ� ���� �� ����Ʈ Ȯ��
	if (myStore) {
		if (ranVal <= myBreadPoint) {
			myMoney += 500;
			return true;	//�� ��µ� ����
		}
		else {
			return false;	//�� ��µ��� ����
		}
	}
	//mystore : false - ��� ���Կ� ���� �� ����Ʈ Ȯ��
	else {
		if (ranVal <= otherBreadPoint) {
			otherMoney += 500;
			return true;	//�� ��µ� ����
		}
		else {
			return false;	//�� ��µ��� ����
		}
	}
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

void UI::myBreadPointDown() {
	if (myBreadPoint > USER_CHANGE_UNIT)
		myBreadPoint -= USER_CHANGE_UNIT;
	else
		myBreadPoint = 0;
}

void UI::otherBreadPointGrow() {
	if (otherBreadPoint < (100 - USER_CHANGE_UNIT))
		otherBreadPoint += USER_CHANGE_UNIT;
	else
		otherBreadPoint = 100;

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

void UI::myAdPointDown() {
	if (adPoint > USER_CHANGE_UNIT)
		adPoint -= USER_CHANGE_UNIT;
	else
		adPoint = 0;
}

//Player::loseHP()���� ȣ��
void UI::loseMyHP(int damage) {
	myHP -= damage;

	char text[256];
	sprintf(text, "Player HP : %d", myHP);

	HPLable->setString(text);

	HPLable->setPosition(cam->getPosition() + Vec2(visibleSize.width / 2, - visibleSize.height / 2));

}

//1�ʸ��� ����Ʈ ������ ó��
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

	HPLable->setPosition(cam->getPosition() + Vec2(visibleSize.width / 2, -visibleSize.height / 2));	//position�� �ٲ���

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

	HPLable->setPosition(cam->getPosition() + Vec2(visibleSize.width / 2, - visibleSize.height / 2));
	
}

void UI::update(float delta) {

	//3�ʸ��� ����Ʈ ���� - ���� �ð���ŭ ������ �϶�/���
	if (checkTime > 3) {
		checkTime = 0;
		allPointChange();
	}
	else {
		checkTime += delta;
	}

	//draw point
	drawUI();

}