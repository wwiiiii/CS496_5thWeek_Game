#ifndef __SELECT_SCENE_H__
#define __SELECT_SCENE_H__

#include "cocos2d.h"

#define TAG_COLOR 10
#define TAG_BACK 20
#define TAG_SELECT 1000

USING_NS_CC;

class SelectScene : public cocos2d::Layer
{
public:
    static Scene* createScene();

    virtual bool init();
    CREATE_FUNC(SelectScene);

	virtual void onEnter();
	virtual void onExit();
	void initCloseOption();
	void initShowSelect();

	EventListenerTouchOneByOne* touchListener;
	Size winSize;
	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
};

#endif // __SELECT_SCENE_H__
