#pragma once
#include "cocos2d.h"
#include "Obj.h"
#include <list>

#define MAX_LEVEL 4
#define MAX_ELEMENT 10

using namespace std;

//quadtree for collision check

class QTree {
private:
	static cocos2d::Rect upOutside;
	static cocos2d::Rect downOutside;
	static cocos2d::Rect leftOutside;
	static cocos2d::Rect rightOutside;
	static cocos2d::Rect rootBound;	//맵 전체:3...

	static QTree* root;

	void clear();	//clear elements
	void split();

	QTree(int parentLevel, cocos2d::Rect parentBound, int childIndex, int parentIndex);

public:
	QTree(cocos2d::Rect parentBound);
	~QTree();

	static void init(cocos2d::Rect mapBoundingBox);	//private 변수들 초기화. 딱 한번만 불리게...

	void removeObjFromAllNode(Obj* obj);

	int level;	//root level:0
	cocos2d::Rect bound;	//범위
	list<Obj*> element;	//노드에 포함되는 오브젝트들
	bool isOutside;	//외곽에 있는 노드인가?

	void insert(Obj* obj);
	void remove(Obj* obj);

	QTree* child[4];	//자식 노드 - 0:위왼 1:위오른 2:아래왼 3:아래오른

	int nodeIndex;
	bool haveChild;

	QTree* searchNode(int nodeIndex);
	void renewObjNode(Obj* obj);
	void renewObjNodeWithSpec(Obj* obj, int* indexList);

	bool checkNodeBoundaryTouch(cocos2d::Rect nodeBound, cocos2d::Rect objBound);

	int* getExNodeIndexList(cocos2d::Rect* exbound);
	void exFindNode(QTree* nowNode, cocos2d::Rect* exbound, int* indexList);

	//부모노드 기억...?
	//노드인덱스...??

};
