#include "GameScene.h"
#include "Circuit.h"
#include <vector>
USING_NS_CC;


vector<CircuitNode*> startNodes;
Scene* GameScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameScene::create();

    // add layer as a child to scene
    scene->addChild(layer);
	
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
	winSize = Director::getInstance()->getWinSize();
	
	auto a1 = new CircuitNode(NODE_START, true, { 100, (long)(winSize.height - 100) });
	auto a2 = new CircuitNode(NODE_START, false, { 100, (long)(winSize.height - 400) });
	auto a6 = new CircuitNode(NODE_START, false, { 200, (long)(winSize.height - 200) });

	auto a3 = new CircuitNode(NODE_AND, false, { 600, (long)(winSize.height - 100) });

	auto a4 = new CircuitNode(NODE_OR, false, { 600, (long)(winSize.height - 400) });

	auto e1 = new CircuitEdge(a1, a3);
	auto e2 = new CircuitEdge(a2, a3);
	auto e3 = new CircuitEdge(a6, a4);

	for (int i = 0; i < e1->lines.size(); i++)
		this->addChild(e1->lines[i].first);
	for (int i = 0; i < e2->lines.size(); i++)
		this->addChild(e2->lines[i].first);
	for (int i = 0; i < e3->lines.size(); i++)
		this->addChild(e3->lines[i].first);
	startNodes.push_back(a1);
	startNodes.push_back(a2);
	startNodes.push_back(a3);
	startNodes.push_back(a4);
	startNodes.push_back(a6);

	
	for (auto i : startNodes)
	{
		//i->spr->setOpacity(128);
		this->addChild(i->spr);
	}

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(touchListener, 1);

    return true;
}


bool GameScene::onTouchBegan(Touch *touch, Event *unused_event)
{
	Point loc = touch->getLocation();
	for (auto i : startNodes)
	{
		Rect rect = i->spr->getBoundingBox();
		if (rect.containsPoint(loc))
		{
			i->isTrue = !i->isTrue;
			i->updateColor();
			return false;
		}
	}
	return false;
}
