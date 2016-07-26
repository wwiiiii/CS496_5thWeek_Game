#ifndef __MAIN_SCENE_H__
#define __MAIN_SCENE_H__

#include "cocos2d.h"

#define TAG_PLAY 1
#define TAG_SETTING 2
#define TAG_CLOSE 3


USING_NS_CC;

class MainScene : public cocos2d::Layer
{
public:
    static Scene* createScene();

    virtual bool init();
    CREATE_FUNC(MainScene);
	
	EventListenerTouchOneByOne* touchListener;
	Size winSize;


	void initPlayOption();
	void initSettingOption();
	void initCloseOption();
	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
};

#endif // __MAIN_SCENE_H__
