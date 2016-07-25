#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"


USING_NS_CC;

class GameScene : public cocos2d::Layer
{
public:
    static Scene* createScene();

    virtual bool init();
    CREATE_FUNC(GameScene);

	Size winSize;
	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
};

#endif // __Game_SCENE_H__
