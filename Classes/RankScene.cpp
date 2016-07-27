#include "RankScene.h"

USING_NS_CC;


Scene* RankScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = RankScene::create();

    // add layer as a child to scene
    scene->addChild(layer);
	
    // return the scene
    return scene;
}

void RankScene::onEnter()
{
	Layer::onEnter();
	touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(RankScene::onTouchBegan, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(touchListener, 1);
}

// on "init" you need to initialize your instance
bool RankScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
	winSize = Director::getInstance()->getWinSize();
	int colorOption = UserDefault::getInstance()->getIntegerForKey("colorOption");
	if (colorOption == 1) this->addChild(Sprite::create("whiteBG.png"));
	initCloseOption();
	initShowRank();

    return true;
}


void RankScene::initCloseOption()
{
	int nowOption = UserDefault::getInstance()->getIntegerForKey("colorOption");
	
	int nowwid = 50, nowhei = 50;
	int centerwid = winSize.width - 50, centerhei = winSize.height - 50;
	auto la = DrawNode::create();
	
	auto spr = Sprite::create();
	spr->setPosition(Point(centerwid, centerhei));
	spr->setTag(TAG_BACK);

	if (nowOption == 0)	{
		spr->setTexture("back_white.png");
	}	else {
		spr->setTexture("back_black.png");
	}
	spr->setScale(0.25);
	this->addChild(la);
	this->addChild(spr);
}

void RankScene::initShowRank()
{
	int bestStage = UserDefault::getInstance()->getIntegerForKey("bestStage");
	int nowOption = UserDefault::getInstance()->getIntegerForKey("colorOption");
	char cont[30]; sprintf(cont, "Best Stage : %d", bestStage);
	auto label = Label::createWithTTF(cont, "NanumGothicExtraBold.ttf", 40, Size(300,100), TextHAlignment::CENTER, TextVAlignment::CENTER);
	label->setPosition(winSize/2);
	if (nowOption == 0)label->setColor(Color3B::WHITE);
	else label->setColor(Color3B::BLACK);
	this->addChild(label);
}

bool RankScene::onTouchBegan(Touch *touch, Event *unused_event)
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

	return false;
}
