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
	vector<CircuitNode*> allNodes;
	vector<CircuitEdge*> allEdges;
	int nowZ;
	Size winSize;
	Point pastTouch;
	LayerColor * bglayer;
	int isUpdating;
	virtual bool init();
	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
	virtual void onTouchMoved(Touch * touch, Event * unused_event);

	void loadMapData(int);
};

#endif // __Game_SCENE_H__
