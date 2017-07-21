#pragma once
#include "cocos2d.h"
#include "AudioEngine.h"

class ObjSquaral;
class StateSquaralNormal;
class StateSquaralAttack;
class StateSquaralSeed;
class StateSquaralDead;

USING_NS_CC;

enum {STATE_SQUARAL_NORMAL, STATE_SQUARAL_ATTACK, STATE_SQUARAL_SEED, STATE_SQUARAL_DEAD};

class StateSquaral
{
public:
	StateSquaral() {
		//rabbitDeadEffect.name = "rabbitDeadEffect";
		//rabbitDeadEffect.maxInstances = 10;
	}
	virtual void initAction(ObjSquaral * obj) = 0;
	virtual bool checkTransitionCond(ObjSquaral * obj) = 0;
	void doTransition(ObjSquaral* obj, int source, int dest);

	int actionDuration = 1;	//각 action이 가지는 시간. 한 state가 가지는 action들의 duration을 모두 통일...?

	//experimental::AudioProfile rabbitDeadEffect;	///효과음

	static StateSquaralNormal* squaralNormal;
	static StateSquaralAttack* squaralAttack;
	static StateSquaralSeed* squaralSeed;
	static StateSquaralDead* squaralDead;
};

//다람쥐 : 통상 상태 클래스
class StateSquaralNormal : public StateSquaral
{
private:
	MoveBy* makeRandDir(ObjSquaral* obj);
public:
	StateSquaralNormal() {
	};

	virtual void initAction(ObjSquaral * obj);
	virtual bool checkTransitionCond(ObjSquaral * obj);
	bool checkSight(ObjSquaral * obj);

};

//다람쥐 : 공격 상태 클래스
class StateSquaralAttack : public StateSquaral
{
private:
	float imgDir;
	int looking;
public:
	StateSquaralAttack() {
	};

	virtual void initAction(ObjSquaral * obj);
	virtual bool checkTransitionCond(ObjSquaral * obj);
	void shotAcorn(ObjSquaral * obj);

};

//다람쥐 : 나무 심기 클래스
class StateSquaralSeed : public StateSquaral
{
public:
	StateSquaralSeed() {
	}

	virtual void initAction(ObjSquaral * obj);
	virtual bool checkTransitionCond(ObjSquaral * obj);
};

//다람쥐 : 사망 클래스
class StateSquaralDead : public StateSquaral
{
public:
	StateSquaralDead() {
	}

	virtual void initAction(ObjSquaral * obj);
	virtual bool checkTransitionCond(ObjSquaral * obj);
};