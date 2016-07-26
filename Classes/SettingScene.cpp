#include "SettingScene.h"

USING_NS_CC;


Scene* SettingScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = SettingScene::create();

    // add layer as a child to scene
    scene->addChild(layer);
	
    // return the scene
    return scene;
}

void SettingScene::onEnter()
{
	Layer::onEnter();
	touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(SettingScene::onTouchBegan, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(touchListener, 1);
}

// on "init" you need to initialize your instance
bool SettingScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
	winSize = Director::getInstance()->getWinSize();
		
	initCloseOption();
	initColorOption();

    return true;
}


void SettingScene::initCloseOption()
{
	int nowwid = 50, nowhei = 50;
	int centerwid = winSize.width - 50, centerhei = winSize.height - 50;
	auto la = DrawNode::create();
	la->drawRect(Point(centerwid - nowwid / 2, centerhei - nowhei / 2), Point(centerwid + nowwid / 2, centerhei + nowhei / 2), Color4F::WHITE);
	
	auto spr = Sprite::create("back.png");
	spr->setPosition(Point(centerwid, centerhei));
	spr->setTag(TAG_BACK);

	this->addChild(la);
	this->addChild(spr);
}

void SettingScene::initColorOption()
{
	int nowOption = UserDefault::getInstance()->getIntegerForKey("colorOption");

	int width = 200, height = 80;
	auto spr = Sprite::create("colorOpt0.png");
	spr->setPosition(winSize.width / 2 - width, winSize.height / 2);
	this->addChild(spr);

	auto spr2 = Sprite::create("colorOpt1.png");
	spr2->setPosition(winSize.width / 2 + width, winSize.height / 2);
	this->addChild(spr2);

	auto spr3 = Sprite::create("selectColor.png");
	spr3->setPosition(winSize / 2);
	this->addChild(spr3);

	auto spr4 = Sprite::create("colorOpt2.png");
	spr4->setTag(TAG_COLOR);
	this->addChild(spr4);
	int wid = spr4->getContentSize().width, hei = spr4->getContentSize().height;
	if (nowOption == 0) spr4->setPosition(winSize.width / 2 + wid / 2, winSize.height / 2);
	else spr4->setPosition(winSize.width / 2 - wid / 2, winSize.height / 2);
}

bool SettingScene::onTouchBegan(Touch *touch, Event *unused_event)
{
	Point loc = touch->getLocation();
	auto sprBack = (Sprite*)this->getChildByTag(TAG_BACK);
	Rect rect = sprBack->getBoundingBox();
	if (rect.containsPoint(loc))//touched GAME PLAY
	{
		Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
		Director::getInstance()->popScene();
		return false;
	}

	auto sprColor = (Sprite*)this->getChildByTag(TAG_COLOR);
	rect = sprColor->getBoundingBox();
	if (rect.containsPoint(loc))//touched GAME PLAY
	{
		int nowOpt = UserDefault::getInstance()->getIntegerForKey("colorOption");
		if (nowOpt == 1)
		{
			UserDefault::getInstance()->setIntegerForKey("colorOption", 0);
			UserDefault::getInstance()->flush();
			auto act = MoveBy::create(0.5, Point(sprColor->getContentSize().width, 0));
			sprColor->runAction(act);
		}
		else
		{
			UserDefault::getInstance()->setIntegerForKey("colorOption", 1);
			UserDefault::getInstance()->flush();
			auto act = MoveBy::create(0.5, Point(-sprColor->getContentSize().width, 0));
			sprColor->runAction(act);
		}
		return false;
	}

	return false;
}
