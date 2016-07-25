#include "MainScene.h"
#include "SimpleAudioEngine.h"

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

	initPlayOption();
	initSettingOption();
	initCloseOption();

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(MainScene::onTouchBegan, this);
	/*touchListener->onTouchMoved = CC_CALLBACK_2(MainScene::onTouchMoved, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(MainScene::onTouchEnded, this);
	touchListener->onTouchCancelled = CC_CALLBACK_2(MainScene::onTouchCancelled, this);*/
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(touchListener, 1);

    return true;
}

void MainScene::initPlayOption()
{
	int nowwid = 300, nowhei = 60;
	int centerwid = winSize.width / 2, centerhei = winSize.height/2 - 50;
	auto la = DrawNode::create();
	la->drawRect(Point(centerwid - nowwid/2, centerhei - nowhei/2), Point(centerwid + nowwid/2, centerhei + nowhei/2), Color4F::WHITE);
	
	auto label = Label::createWithTTF("GAME PLAY", "NanumGothicExtraBold.ttf", 40, Size((float)nowwid, (float)nowhei), TextHAlignment::CENTER, TextVAlignment::CENTER );
	label->setPosition(Point(centerwid, centerhei));
	label->setTag(TAG_PLAY);
	label->setAnchorPoint(Point(0.5, 0.5));
	label->setColor(Color3B::WHITE);

	this->addChild(la);
	this->addChild(label);
}


void MainScene::initSettingOption()
{
	int nowwid = 300, nowhei = 60;
	int centerwid = winSize.width / 2, centerhei = winSize.height / 2 - 120;
	auto la = DrawNode::create();
	la->drawRect(Point(centerwid - nowwid / 2, centerhei - nowhei / 2), Point(centerwid + nowwid / 2, centerhei + nowhei / 2), Color4F::WHITE);

	auto label = Label::createWithTTF("OPTION", "NanumGothicExtraBold.ttf", 40, Size((float)nowwid, (float)nowhei), TextHAlignment::CENTER, TextVAlignment::CENTER);
	label->setPosition(Point(centerwid, centerhei));
	label->setTag(TAG_SETTING);
	label->setAnchorPoint(Point(0.5, 0.5));
	label->setColor(Color3B::WHITE);

	this->addChild(la);
	this->addChild(label);
}

void MainScene::initCloseOption()
{
	int nowwid = 50, nowhei = 50;
	int centerwid = winSize.width - 50, centerhei = winSize.height - 50;
	auto la = DrawNode::create();
	la->drawRect(Point(centerwid - nowwid / 2, centerhei - nowhei / 2), Point(centerwid + nowwid / 2, centerhei + nowhei / 2), Color4F::WHITE);

	auto label = Label::createWithTTF("X", "NanumGothicExtraBold.ttf", 40, Size((float)nowwid, (float)nowhei), TextHAlignment::CENTER, TextVAlignment::CENTER);
	label->setPosition(Point(centerwid, centerhei));
	label->setTag(TAG_CLOSE);
	label->setAnchorPoint(Point(0.5, 0.5));
	label->setColor(Color3B::WHITE);

	this->addChild(la);
	this->addChild(label);
}


bool MainScene::onTouchBegan(Touch *touch, Event *unused_event)
{
	Point loc = touch->getLocation();
	auto sprPlay = (Sprite*)this->getChildByTag(TAG_PLAY);
	Rect rect = sprPlay->getBoundingBox();
	if (rect.containsPoint(loc))//touched GAME PLAY
	{
	
	
		return false;
	}
	
	auto sprSet = (Sprite*)this->getChildByTag(TAG_SETTING);
	rect = sprSet->getBoundingBox();
	if (rect.containsPoint(loc))//touched OPTION
	{


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