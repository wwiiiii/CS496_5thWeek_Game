#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "Circuit.h"
USING_NS_CC;

class GameScene : public Layer
{
public:
    static Scene* createScene();

    virtual bool init();
    CREATE_FUNC(GameScene);
	vector<CircuitNode*> startNodes;
	Size winSize;
	Point pastTouch;
	Layer * bglayer;
	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
	virtual void onTouchMoved(Touch * touch, Event * unused_event);
};

#endif // __Game_SCENE_H__
