#include "ObjManager.h"


USING_NS_CC;


ObjManager::ObjManager() {
	

}

void ObjManager::ObjInit() {
	//���� �� �ʱ�ȭ...
	for (int i = 0; i < MAX_OBJ_NUM; i++) {
		objRabbitList[i] = new ObjRabbit;
	}
}


void ObjManager::addObjectAvailList(Obj *obj) {
	objAvailList.push_back(obj);
}

//�׽��� ���غ�
void ObjManager::deleteObjectAvailList(Obj *obj) {
	objAvailList.remove(obj);
	CCLOG("delete from avail list\n");
}

ObjRabbit* ObjManager::getFreeObjRabbit() {

	for (int i = 0; i < MAX_OBJ_NUM; i++) {
		//��������� ���� ������Ʈ�� ã�� ��ȯ
		if (!objRabbitList[i]->inUse) {
			addObjectAvailList(dynamic_cast<Obj*> (objRabbitList[i]));	//����� ������Ʈ�� availList�� �־���
			return objRabbitList[i];
		}
	}

	//��� ������Ʈ�� ������̸� nullptr ��ȯ
	return nullptr;	
}


//ȣ���� ������Ʈ obj�� ���� �ٸ� ������Ʈ�� �浹�ߴ��� Ȯ���Ѵ�.
//�̵��� ���ؼ� -> �浹������ ������Ʈ�� ������ ��ġ�� �Ű���
//�� �� �浹�ÿ� �Ͼ�°ſ� ���ؼ���(��ɰ�����...) �����ε� �ؾߵɰ� ���٤�0��
//�̵��� : �׻� �ϳ��� �������θ� �̵��Ѵٰ� ���� Vec2�� ������ x, y �� �ϳ��� 0�� ���� ������.
//player������ ���� �������� �浹�� ������Ʈ�� �ʿ��ߴ��� ������Ʈ�� �ڿ������� ���̷��� �׷��ſ��µ� �̰� ���ϰ� ���� moveLen �ȹ޴°ɷ� �ص�
//����� �浹�� �����ϰ� ������ �ʴµ� ��¦ ���� �浹�ϴ°� �ִ٤�0��...
//�Ʒ��ɷ� �ص� �ɰ� ���� �ѵ� �� ����� ������.. ���� �÷��̾� ������ �ҰŸ� ���߿� �Լ� �̸� �ٲ��ٷ�
bool ObjManager::checkMoveCollision(Obj *obj, Rect* exBox, cocos2d::Vec2* moveLen) {

	CCASSERT(!(moveLen->x != 0 && moveLen->y != 0), "moveLen : x or y val should be 0");
	
	//�������� ���� ���� boundingbox�� �޾Ƽ� �װɷ� ������ �� �ִ��� �˻�

	for each (Obj* i in objAvailList)
	{
		//�浹��
		if ((obj->objIndex != i->objIndex) && exBox->intersectsRect(i->objImg->getBoundingBox())) {

			//������Ʈ�� �̹� �����ִ� ��� pausedTime�� 0���� ŭ

			//ù �浹 üũ
			if (obj->pausedTime == 0) {
				obj->getActionManager()->pauseTarget(obj->objImg);

				//x������ �����̰� �־��� ��
				//��
				if (moveLen->x < 0) {
					obj->objImg->setPositionX(i->objImg->getBoundingBox().getMaxX() + exBox->size.width / 2 + 1);
				}
				//��
				else if (moveLen->x > 0) {
					obj->objImg->setPositionX(i->objImg->getBoundingBox().getMinX() - exBox->size.width / 2 - 1);

				}
				//y������ �����̰� �־��� ��
				//��
				else if (moveLen->y>0) {
					obj->objImg->setPositionY(i->objImg->getBoundingBox().getMinY() - exBox->size.height / 2 - 1);

				}
				//��
				else if (moveLen->y<0) {

					obj->objImg->setPositionY(i->objImg->getBoundingBox().getMaxY() + exBox->size.height / 2 + 1);
				}
				else {
					CCLOG("error");
				}

			}

			return false;
		}
	}

	//�浹���� �ʾ����� ��� ������
	obj->getActionManager()->resumeTarget(obj->objImg);
	obj->pausedTime = 0;

	return true;

}

bool ObjManager::checkAttackCollision(cocos2d::Rect* exBox) {
	bool hit = false;

	//�浹�� ��ü ����
	for each (Obj* i in objAvailList)
	{

		if (i->typecode != TYPECODE_RABBIT) {	//�ӽ�... ���߿� ��ü Ÿ�Ժ��� �з��� ��?
			continue;
		}

		//�浹��
		if (exBox->intersectsRect(i->objImg->getBoundingBox())) {

			playerCollisionList.pushBack(i);
			hit = true;
			CCLOG("%d collision",i->objIndex);
			
		}
	}

	//�� �浹�� ��ü�� ���� ó��
	for each (Obj* i in playerCollisionList) {
		i->loseHP();	//<-�Ѳ����� ���߿� ó��...?
	}
	playerCollisionList.clear();

	return hit;
}


bool ObjManager::checkSightCollision(ObjRabbit * obj) {

	Vec2* tri = obj->rabbitSight;

	//1
	float slope1 = (tri[0].y - tri[1].y) / (tri[0].x - tri[1].x);
	float b1 = tri[0].y - slope1 * tri[0].x;

	//2
	float slope2 = (tri[0].y - tri[2].y) / (tri[0].x - tri[2].x);
	float b2 = tri[0].y - slope2 * tri[0].x;

	for each (Obj* i in objAvailList)
	{
		if (i->typecode != TYPECODE_PEOPLE) {
			continue;
		}

		Vec2 rectPoint[4];
		rectPoint[0] = Vec2(i->objImg->getBoundingBox().getMaxX(), i->objImg->getBoundingBox().getMaxY());
		rectPoint[1] = Vec2(i->objImg->getBoundingBox().getMinX(), i->objImg->getBoundingBox().getMaxY());
		rectPoint[2] = Vec2(i->objImg->getBoundingBox().getMaxX(), i->objImg->getBoundingBox().getMinY());
		rectPoint[3] = Vec2(i->objImg->getBoundingBox().getMinX(), i->objImg->getBoundingBox().getMinY());


		//�ﰢ�� ���� �簢���� �������� ���ԵǴ��� Ȯ��
		for (int index = 0; index < 4; index++) {
			if (checkSightCond(obj->dir, slope1, b1, slope2, b2, &rectPoint[index])
				&& checkSightCond3(obj->dir, &tri[1], &rectPoint[index])) 
			{
				CCLOG("1 %d %d", index, i->objIndex);
				return true;
			}
				
		}

		//�簢�� ���� �ﰢ���� �������� ���ԵǴ��� Ȯ��
		for (int index = 1; index < 3; index++) {	//tri[0]�� �׻� object ���� �����Ƿ� ������ ����ģ��
			if (i->objImg->getBoundingBox().containsPoint(tri[index])) 
			{
				CCLOG("2 %d %d", index, i->objIndex);
				return true;
			}
				
		}

	}

	//��ġ�� �ʴ´ٸ� false ��ȯ
	return false;
}

bool ObjManager::checkSightCond(int dir, float slope1, float b1, float slope2, float b2, Vec2* rectPoint) {
	if (dir == DIR_LEFT) {
		return ((slope1 * rectPoint->x - rectPoint->y + b1 > 0)
			&& (slope2 * rectPoint->x - rectPoint->y + b2 < 0));
	}
	else if (dir == DIR_RIGHT) {
		return ((slope1 * rectPoint->x - rectPoint->y + b1 > 0)
			&& (slope2 * rectPoint->x - rectPoint->y + b2 < 0));
	}
	else if (dir == DIR_UP) {
		return ((slope1 * rectPoint->x - rectPoint->y + b1 < 0)
			&& (slope2 * rectPoint->x - rectPoint->y + b2 < 0));
	}
	else if (dir == DIR_DOWN) {
		return ((slope1 * rectPoint->x - rectPoint->y + b1 > 0)
			&& (slope2 * rectPoint->x - rectPoint->y + b2 > 0));
	}
	else {
		return false;
	}
	
}

bool ObjManager::checkSightCond3(int dir, Vec2* triP1, Vec2* rectPoint) {
	if (dir == DIR_LEFT) {
		if (triP1->x < rectPoint->x)
			return true;
	}
	else if (dir == DIR_RIGHT) {
		if (triP1->x > rectPoint->x)
			return true;
	}
	else if (dir == DIR_UP) {
		if (triP1->y > rectPoint->y)
			return true;
	}
	else if (dir == DIR_DOWN) {
		if (triP1->y < rectPoint->y)
			return true;
	}
	return false;
}