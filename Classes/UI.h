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

	//내 빵 게이지
	cocos2d::DrawNode * myBreadBox;
	cocos2d::DrawNode * myBreadBar;
	cocos2d::Label * myMoneyLabel;

	//상대 빵 게이지
	cocos2d::DrawNode * otherBreadBox;
	cocos2d::DrawNode * otherBreadBar;
	cocos2d::Label * otherMoneyLabel;

	//홍보 게이지
	cocos2d::DrawNode * adBox;
	cocos2d::DrawNode * adOtherBar;
	cocos2d::DrawNode * adMyBar;

	cocos2d::Label * HPLable;

	void allPointChange();
	void drawUI();

	float checkTime;
public:
	cocos2d::Camera* cam;

	UI();
	~UI();
	virtual bool init();

	void myBreadSold();
	void otherBreadSold();
	void myBreadPointGrow();
	void myBreadPointDown();
	void otherBreadPointGrow();
	void otherBreadPointDown();
	void myAdPointGrow();
	void myAdPointDown();
	void loseMyHP(int damage);
	int selectShop();
	bool buyBread(bool myStore);

	void update(float) override;
};