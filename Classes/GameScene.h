#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "ObjRabbit.h"
#include "Player.h"
#include "ObjManager.h"
#include "Obj.h"
#include "UI.h"


class GameWorld : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(GameWorld);

	cocos2d::Sprite* sprite1;	//나중에 안쓰일것

	static ObjManager *objManager;
	static Player *player;
	static UI *ui;

	//static cocos2d::Scene* GameOverScene;

};



#endif // __GAME_SCENE_H__
