#include "GameScene.h"
#include "Circuit.h"
#include <vector>
#include <string>
#define COCOS2D_DEBUG 1
#define MAX_Z_ORDER 2100000000
USING_NS_CC;

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
	nowZ = 1;
	srand(time(NULL));
	winSize = Director::getInstance()->getWinSize();
	startNodes.clear();
	int colorOption = UserDefault::getInstance()->getIntegerForKey("colorOption");

	if (colorOption == 0) bglayer=LayerColor::create(Color4B::BLACK);
	else bglayer = LayerColor::create(Color4B::WHITE);

	this->addChild(bglayer);
	auto a1 = new CircuitNode(NODE_START, true, { 100, (long)(winSize.height - 100) });
	auto a2 = new CircuitNode(NODE_START, false, { 100, (long)(winSize.height - 400) });
	auto a6 = new CircuitNode(NODE_START, false, { 200, (long)(winSize.height - 200) });

	auto a3 = new CircuitNode(NODE_AND, false, { 600, (long)(winSize.height - 50) });

	auto a4 = new CircuitNode(NODE_OR, false, { 600, (long)(winSize.height - 400) });

	auto e1 = new CircuitEdge(a1, a3, colorOption);
	auto e2 = new CircuitEdge(a2, a3, colorOption);
	auto e3 = new CircuitEdge(a6, a4, colorOption);

	for (int i = 0; i < e1->lines.size(); i++)
		bglayer->addChild(e1->lines[i]->clip);
	for (int i = 0; i < e2->lines.size(); i++)
		bglayer->addChild(e2->lines[i]->clip);
	for (int i = 0; i < e3->lines.size(); i++)
		bglayer->addChild(e3->lines[i]->clip);
	startNodes.push_back(a1);
	startNodes.push_back(a2);
	startNodes.push_back(a3);
	startNodes.push_back(a4);
	startNodes.push_back(a6);
	/*
	this->addChild(a1->spr);
	this->addChild(a2->spr);
	this->addChild(a3->spr);
	this->addChild(a4->spr);
	this->addChild(a6->spr);
	*/
	
	for (int i =0;i<startNodes.size(); i++)
	{
		startNodes[i]->spr->setZOrder(MAX_Z_ORDER);
		bglayer->addChild(startNodes[i]->spr);
		CCLOG("spr pos %f %f", startNodes[i]->spr->getPosition().x, startNodes[i]->spr->getPosition().y);
	}
	loadMapData();
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(touchListener, 1);
	//log("%s", "A string");
    return true;
}


bool GameScene::onTouchBegan(Touch *touch, Event *unused_event)
{
	Point loc = touch->getLocation();
	Point backloc = loc;
	Rect layerPos = bglayer->getBoundingBox();
	loc.x -= layerPos.getMinX();
	for (int i = 0; i < startNodes.size(); i++) 
	{
		Rect rect = startNodes[i]->spr->getBoundingBox();
		if (rect.containsPoint(loc))
		{	
			startNodes[i]->isTrue = !(startNodes[i]->isTrue);
			startNodes[i]->updateColor(-1);
			if (startNodes[i]->outputEdge != NULL) {
				startNodes[i]->outputEdge->updateStatusByInput();
				startNodes[i]->outputEdge->updateColor(nowZ);
				nowZ += startNodes[i]->outputEdge->lines.size();
			}
			//log("%f %f %f %f", rect.getMaxX(), rect.getMaxY(), rect.getMinX(), rect.getMinY());
			return false;
		}
	}
	pastTouch = backloc;
	return true;
}

void GameScene::onTouchMoved(Touch *touch, Event *unused_event)
{
	float thres = -1.0;
	Point loc = touch->getLocation();
	
	//log("%f %f", loc.x, loc.y);
	bglayer->runAction(MoveBy::create(0.0, Point(loc.x - pastTouch.x, 0.0)));
	pastTouch = loc;
}

void GameScene::loadMapData()
{
	char mapName[20];
	int mapGaro, mapSero, nodecnt, edgecnt;
	int nodenum, nodetype, nodeval, nodex, nodey;
	int mapNum = UserDefault::getInstance()->getIntegerForKey("nowMapNum");
	sprintf(mapName, "\\maps\\map%d.txt", mapNum);
	std::string data = FileUtils::getInstance()->getStringFromFile(mapName);
	vector<std::string> datas;
	int pastidx =-1;
	for (int i = 0; i < data.size(); i++)
	{
		if (data[i] == '\n') {
			datas.push_back(data.substr(pastidx+1, i-pastidx-2));
			pastidx = i;
		}
	}
	sscanf(datas[0].c_str(), "%d %d", &mapGaro, &mapSero);
	sscanf(datas[1].c_str(), "%d %d", &nodecnt, &edgecnt);
	for (int i = 0; i < nodecnt; i++)
	{
		sscanf(datas[2 + i].c_str(), "%d %d %d %d %d", &nodenum, &nodetype, &nodeval, &nodex, &nodey);
		if (nodex < 0) nodex += mapGaro; if (nodey < 0) nodey += mapSero;
	}
}