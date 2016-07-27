#include "MainScene.h"
#include "GameScene.h"
#include "SettingScene.h"
#include <string>

USING_NS_CC;


Scene* MainScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = MainScene::create();

    // add layer as a child to scene
    scene->addChild(layer);
	
    // return the scene
    return scene;
}

void MainScene::onEnter()
{
	Layer::onEnter();

	int colorOpt = UserDefault::getInstance()->getIntegerForKey("colorOption");
	if (colorOpt == 1) this->addChild(Sprite::create("whiteBG.png"));
	float margin = 200.0, mar2 = 100.0;
	
	auto rank = Sprite::create(); auto setting = Sprite::create();
	auto play = Sprite::create(); auto close = Sprite::create();
	if (colorOpt == 0)
	{
		play->setTexture("play_white.png"); setting->setTexture("setting_white.png");
		rank->setTexture("rank_white.png"); close->setTexture("close_white.png");
	}
	else
	{
		play->setTexture("play_black.png"); setting->setTexture("setting_black.png");
		rank->setTexture("rank_black.png"); close->setTexture("close_black.png");
	}
	
	play->setTag(TAG_PLAY); play->setScale(0.5);
	play->setPosition(winSize.width / 2 - margin, winSize.height / 2-mar2);
	
	rank->setTag(TAG_RANK); rank->setScale(0.25);
	rank->setPosition(winSize.width / 2 + margin, winSize.height / 2 - mar2);
	
	setting->setTag(TAG_SETTING); setting->setScale(0.5);
	setting->setPosition(winSize.width / 2 + 0.0, winSize.height / 2 - mar2);
	
	close->setTag(TAG_CLOSE); close->setScale(0.25);
	close->setPosition(winSize.width - 50, winSize.height - 50);

	this->addChild(play); this->addChild(setting); this->addChild(rank); this->addChild(close);

	touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(MainScene::onTouchBegan, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(touchListener, 1);
}

void MainScene::onExit()
{
	Layer::onExit();
	this->cleanup();
	this->removeAllChildrenWithCleanup(false);
}

// on "init" you need to initialize your instance
bool MainScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
	winSize = Director::getInstance()->getWinSize();
	/*initPlayOption();
	initSettingOption();
	*/
	
	auto size = Director::getInstance()->getOpenGLView()->getFrameSize();
	CCLOG("winSize %f %f", winSize.width, winSize.height);
	CCLOG("frameSize %f %f", size.width, size.height);


    return true;
}

bool MainScene::onTouchBegan(Touch *touch, Event *unused_event)
{
	Point loc = touch->getLocation();
	auto sprPlay = (Sprite*)this->getChildByTag(TAG_PLAY);
	Rect rect = sprPlay->getBoundingBox();
	if (rect.containsPoint(loc))//touched GAME PLAY
	{
		Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
		Director::getInstance()->pushScene(GameScene::createScene());
		return false;
	}
	
	auto sprSet = (Sprite*)this->getChildByTag(TAG_SETTING);
	rect = sprSet->getBoundingBox();
	if (rect.containsPoint(loc))//touched OPTION
	{
		Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
		Director::getInstance()->pushScene(SettingScene::createScene());
		return false;
	}

	auto sprClose = (Sprite*)this->getChildByTag(TAG_CLOSE);
	rect = sprClose->getBoundingBox();
	if (rect.containsPoint(loc))//touched X
	{
		Director::getInstance()->end();
		return false;
	}
}
