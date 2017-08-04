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
	static cocos2d::Rect rootBound;	//�� ��ü:3...

	static QTree* root;

	void clear();	//clear elements
	void split();

	QTree(int parentLevel, cocos2d::Rect parentBound, int childIndex, int parentIndex);

public:
	QTree(cocos2d::Rect parentBound);
	~QTree();

	static void init(cocos2d::Rect mapBoundingBox);	//private ������ �ʱ�ȭ. �� �ѹ��� �Ҹ���...

	void removeObjFromAllNode(Obj* obj);

	int level;	//root level:0
	cocos2d::Rect bound;	//����
	list<Obj*> element;	//��忡 ���ԵǴ� ������Ʈ��
	bool isOutside;	//�ܰ��� �ִ� ����ΰ�?

	void insert(Obj* obj);
	void remove(Obj* obj);

	QTree* child[4];	//�ڽ� ��� - 0:���� 1:������ 2:�Ʒ��� 3:�Ʒ�����

	int nodeIndex;
	bool haveChild;

	QTree* searchNode(int nodeIndex);
	void renewObjNode(Obj* obj);
	void renewObjNodeWithSpec(Obj* obj, int* indexList);

	bool checkNodeBoundaryTouch(cocos2d::Rect nodeBound, cocos2d::Rect objBound);

	int* getExNodeIndexList(cocos2d::Rect* exbound);
	void exFindNode(QTree* nowNode, cocos2d::Rect* exbound, int* indexList);

	//�θ��� ���...?
	//����ε���...??

};
