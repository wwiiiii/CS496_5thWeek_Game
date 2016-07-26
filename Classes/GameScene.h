#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "Circuit.h"
USING_NS_CC;

class GameScene : public Layer
{
public:
    static Scene* createScene();

    CREATE_FUNC(GameScene);
	
	vector<CircuitNode*> startNodes;
	int nowZ;
	Size winSize;
	Point pastTouch;
	LayerColor * bglayer;

	virtual bool init();
	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
	virtual void onTouchMoved(Touch * touch, Event * unused_event);

	void loadMapData();
};

#endif // __Game_SCENE_H__
