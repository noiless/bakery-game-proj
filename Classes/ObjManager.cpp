#include "ObjManager.h"

USING_NS_CC;

void ObjManager::addObject(Obj *obj) {
	objList.push_back(obj);
	CCLOG("initiated\n");
}

void ObjManager::deleteObject(Obj *obj) {
	//objList.remove(*obj);
}



//ȣ���� ������Ʈ obj�� ���� �ٸ� ������Ʈ�� �浹�ߴ��� Ȯ���Ѵ�.
//�̵��� ���ؼ� -> �浹������ ������Ʈ�� ������ ��ġ�� �Ű���
//�� �� �浹�ÿ� �Ͼ�°ſ� ���ؼ���(��ɰ�����...) �����ε� �ؾߵɰ� ���٤�0��
//�̵��� : �׻� �ϳ��� �������θ� �̵��Ѵٰ� ���� Vec2�� ������ x, y �� �ϳ��� 0�� ���� ������.

//player������ ���� �������� �浹�� ������Ʈ�� �ʿ��ߴ��� ������Ʈ�� �ڿ������� ���̷��� �׷��ſ��µ� �̰� ���ϰ� ���� moveLen �ȹ޴°ɷ� �ص�
//����� �浹�� �����ϰ� ������ �ʴµ� ��¦ ���� �浹�ϴ°� �ִ٤�0��...
//�Ʒ��ɷ� �ص� �ɰ� ���� �ѵ� �� ����� ������.. ���� �÷��̾� ������ �ҰŸ� ���߿� �Լ� �̸� �ٲ��ٷ�
bool ObjManager::checkCollision(Obj *obj, Rect exBox, cocos2d::Vec2 moveLen) {
	
	//�������� ���� ���� boundingbox�� �޾Ƽ� �װɷ� ������ �� �ִ��� �˻�

	for each (Obj* i in objList)
	{
		//�浹��
		if ((obj->objIndex != i->objIndex) && exBox.intersectsRect(i->objImg->getBoundingBox())) {

			//x������ �����̰� �־��� ��
			//��
			if (moveLen.x < 0) {
				obj->objImg->setPositionX(i->objImg->getBoundingBox().getMaxX() + exBox.size.width / 2 + 1);
			}
			//��
			else if (moveLen.x > 0) {
				obj->objImg->setPositionX(i->objImg->getBoundingBox().getMinX() - exBox.size.width / 2 - 1);
			}
			//y������ �����̰� �־��� ��
			//��
			else if (moveLen.y>0) {
				obj->objImg->setPositionY(i->objImg->getBoundingBox().getMinY() - exBox.size.height / 2 - 1);
			}
			//��
			else if (moveLen.y<0) {
				obj->objImg->setPositionY(i->objImg->getBoundingBox().getMaxY() + exBox.size.height / 2 + 1);
			}

			return false;
		}
	}

	return true;

}

//�Ϲ� ������Ʈ�� �浹 üũ
//�ѹ��� �ʰ� üũ�Ǿ ��ġ�� ���������� �ᱹ ���� �ٿ���ڽ��� �޾ƾ��Ѵ�..
//Ȥ�� �÷��̾� �̵��� �̰ɷ� ���ϵǰ� �ȴٸ� pause�� resume�� object update������ �Ű���� ��
//�� �÷��̾� �̵��� �̰ɷ� �ϸ� ���� �������� ���� ������Ʈ ��鸮�� ���̴� ���� ��Ÿ�� �� �����ɷ� ����
bool ObjManager::checkCollision(Obj *obj, Rect exBox) {

	for each (Obj* i in objList)
	{
		//�浹��
		if ((obj->objIndex != i->objIndex) && exBox.intersectsRect(i->objImg->getBoundingBox())) {
			//����
			obj->getActionManager()->pauseTarget(obj->objImg);
			return false;
		}
		//�浹���� �ʾ����� �׼��� ��� �Ѵ�.
		obj->getActionManager()->resumeTarget(obj->objImg);
	}

	return true;

}