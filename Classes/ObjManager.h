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

	bool checkCollision(Obj *obj, cocos2d::Rect exBox, cocos2d::Vec2 moveLen);	//player용

	bool checkCollision(Obj *obj, cocos2d::Rect exBox);	//그외 오브젝트
	
};