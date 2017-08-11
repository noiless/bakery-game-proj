#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"

#include "Player.h"
#include "ObjEnemy.h"
#include "ObjManager.h"
#include "UI.h"


class GameWorld : public cocos2d::Scene
{
private:
	void gameLoad();
	void setGuestInitAction();
public:
	
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(GameWorld);

	static bool initiated;

	static ObjManager *objManager;
	static Player *player;
	static ObjEnemy *enemy;
	static UI *ui;

	static void gameEnd();

};



#endif // __GAME_SCENE_H__
