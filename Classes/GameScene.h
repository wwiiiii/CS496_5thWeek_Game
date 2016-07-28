#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "Circuit.h"
#define TAG_GAME_OPTION 123
#define TAG_GAME_OPTION_OUT 12
#define TAG_GAME_OPTION_RESUME 14
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
	int sceneStatus; // 기본값 0, 1이면 옵션 선택 모드
	Size winSize;
	Point pastTouch;
	Layer * bglayer;
	int isUpdating;
	virtual bool init();
	virtual void onEnter();
	virtual void onExit();
	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
	virtual void onTouchMoved(Touch * touch, Event * unused_event);
	void winGame();
	void loadMapData(int);
	void loadComplete();
	void debug();
};

#endif // __Game_SCENE_H__
