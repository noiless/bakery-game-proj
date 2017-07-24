#pragma once
#include "cocos2d.h"

class UI : public cocos2d::Node {
private:
	int myMoney;
	int otherMoney;
	int myBreadPoint;
	int otherBreadPoint;
	int adPoint;
	int myHP;

	cocos2d::Size visibleSize;

	//�� �� ������
	cocos2d::DrawNode * myBreadBox;
	cocos2d::DrawNode * myBreadBar;
	cocos2d::Label * myMoneyLabel;

	//��� �� ������
	cocos2d::DrawNode * otherBreadBox;
	cocos2d::DrawNode * otherBreadBar;
	cocos2d::Label * otherMoneyLabel;

	//ȫ�� ������
	cocos2d::DrawNode * adBox;
	cocos2d::DrawNode * adOtherBar;
	cocos2d::DrawNode * adMyBar;

	void allPointChange();
	void drawUI();

	float checkTime;
public:
	cocos2d::Camera* cam;

	UI();
	virtual bool init();

	void myBreadSold();
	void otherBreadSold();
	void myBreadPointGrow();
	void otherBreadPointDown();
	void myAdPointGrow();
	void loseMyHP();

	void update(float) override;
};