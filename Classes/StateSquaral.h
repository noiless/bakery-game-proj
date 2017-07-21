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

	int actionDuration = 1;	//�� action�� ������ �ð�. �� state�� ������ action���� duration�� ��� ����...?

	//experimental::AudioProfile rabbitDeadEffect;	///ȿ����

	static StateSquaralNormal* squaralNormal;
	static StateSquaralAttack* squaralAttack;
	static StateSquaralSeed* squaralSeed;
	static StateSquaralDead* squaralDead;
};

//�ٶ��� : ��� ���� Ŭ����
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

//�ٶ��� : ���� ���� Ŭ����
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

//�ٶ��� : ���� �ɱ� Ŭ����
class StateSquaralSeed : public StateSquaral
{
public:
	StateSquaralSeed() {
	}

	virtual void initAction(ObjSquaral * obj);
	virtual bool checkTransitionCond(ObjSquaral * obj);
};

//�ٶ��� : ��� Ŭ����
class StateSquaralDead : public StateSquaral
{
public:
	StateSquaralDead() {
	}

	virtual void initAction(ObjSquaral * obj);
	virtual bool checkTransitionCond(ObjSquaral * obj);
};