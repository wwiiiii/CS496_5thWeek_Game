#ifndef __SETTING_SCENE_H__
#define __SETTING_SCENE_H__

#include "cocos2d.h"

#define TAG_COLOR 10
#define TAG_BACK 20

USING_NS_CC;

class SettingScene : public cocos2d::Layer
{
public:
    static Scene* createScene();

    virtual bool init();
    CREATE_FUNC(SettingScene);

	virtual void onEnter();
	void initCloseOption();
	void initColorOption();
	
	EventListenerTouchOneByOne* touchListener;
	Size winSize;
	static void refreshScene(void * useless);
	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
};

#endif // __SETTING_SCENE_H__
