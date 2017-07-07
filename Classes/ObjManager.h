#pragma once
#include <cocos2d.h>
#include <list>
#include <Obj.h>

using namespace std;

class ObjManager {
private:
	list<Obj*> objList;
public:
	void addObject(Obj *obj);
	void deleteObject(Obj *obj);

	bool checkCollision(Obj *obj, cocos2d::Rect exBox, cocos2d::Vec2 moveLen);	//player��

	bool checkCollision(Obj *obj, cocos2d::Rect exBox);	//�׿� ������Ʈ
	
};