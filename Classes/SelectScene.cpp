#include "SelectScene.h"
#include "GameScene.h"

USING_NS_CC;


Scene* SelectScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = SelectScene::create();

    // add layer as a child to scene
    scene->addChild(layer);
	
    // return the scene
    return scene;
}

void SelectScene::onEnter()
{
	Layer::onEnter();

	int colorOption = UserDefault::getInstance()->getIntegerForKey("colorOption");
	if (colorOption == 1) this->addChild(Sprite::create("whiteBG.png"));
	
	initCloseOption();
	initShowSelect();

	auto label = Label::createWithTTF("STAGE", "NanumGothicExtraBold.ttf", 200, Size(600, 300), TextHAlignment::CENTER, TextVAlignment::CENTER);
	label->setPosition(winSize.width/2, winSize.height - 200);
	if (colorOption == 0)label->setColor(Color3B::WHITE);
	else label->setColor(Color3B::BLACK);
	this->addChild(label);

	touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(SelectScene::onTouchBegan, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(touchListener, 1);
}

void SelectScene::onExit()
{
	Layer::onExit();
	this->removeAllChildren();
}

// on "init" you need to initialize your instance
bool SelectScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
	winSize = Director::getInstance()->getWinSize();
	

    return true;
}


void SelectScene::initCloseOption()
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

void SelectScene::initShowSelect()
{
	int bestStage = UserDefault::getInstance()->getIntegerForKey("bestStage");
	int nowOption = UserDefault::getInstance()->getIntegerForKey("colorOption");
	char str[20]; float wid = winSize.width / 2, hei = winSize.height / 8 * 0.7;
	float margin = wid / 4 * 1.2; float mar2 = 100.0;
	float garo[] = {wid-1.5*margin, wid-0.5*margin, wid+0.5*margin, wid+1.5*margin}; 
	float sero[] = {hei*5 + mar2, hei*3 + mar2, hei + mar2};
	for (int i = 0; i < 3; i++)
	{
		for (int j = 1; j <= 4; j++)
		{
			int nowidx = i * 4 + j; sprintf(str, "%d", nowidx);
			auto label = Label::createWithTTF(str, "NanumGothicExtraBold.ttf", 80, Size(100, 100), TextHAlignment::CENTER, TextVAlignment::CENTER);
			label->setPosition(garo[j-1],sero[i]);
			if (nowOption == 0)label->setColor(Color3B::WHITE);
			else label->setColor(Color3B::BLACK);
			label->setTag(TAG_SELECT + nowidx);
			this->addChild(label);

			auto la = DrawNode::create();
			float centerwid = garo[j - 1], centerhei = sero[i], nowwid = 100, nowhei = 100;
			if (nowidx > bestStage) {
				la->drawRect(Point(centerwid - nowwid / 2, centerhei - nowhei / 2), Point(centerwid + nowwid / 2, centerhei + nowhei / 2), Color4F::GRAY);
			}
			else {
				if (nowOption == 0) la->drawRect(Point(centerwid - nowwid / 2, centerhei - nowhei / 2), Point(centerwid + nowwid / 2, centerhei + nowhei / 2), Color4F::WHITE);
				else la->drawRect(Point(centerwid - nowwid / 2, centerhei - nowhei / 2), Point(centerwid + nowwid / 2, centerhei + nowhei / 2), Color4F::BLACK);
			}
			this->addChild(la);

			if (nowidx > bestStage) {  label->setOpacity(128); }
		}
	}
}

bool SelectScene::onTouchBegan(Touch *touch, Event *unused_event)
{
	Point loc = touch->getLocation();
	for (int i = 1; i < 12; i++)
	{
		auto spr = this->getChildByTag(TAG_SELECT + i);
		Rect rec = spr->getBoundingBox();
		if (rec.containsPoint(loc))//touched GAME PLAY
		{
			CCLOG("selected %d stage", i);
			Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
			UserDefault::getInstance()->setIntegerForKey("nowStage", i);
			Director::getInstance()->pushScene(GameScene::createScene());
			return false;
		}
	}
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
