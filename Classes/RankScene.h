#ifndef __RANK_SCENE_H__
#define __RANK_SCENE_H__

#include "cocos2d.h"

#define TAG_COLOR 10
#define TAG_BACK 20

USING_NS_CC;

class RankScene : public cocos2d::Layer
{
public:
    static Scene* createScene();

    virtual bool init();
    CREATE_FUNC(RankScene);

	virtual void onEnter();
	void initCloseOption();
	void initShowRank();

	EventListenerTouchOneByOne* touchListener;
	Size winSize;
	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
};

#endif // __RANK_SCENE_H__
