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
	nowZ = 1; isUpdating = 0;
	srand(time(NULL));
	this->winSize = Director::getInstance()->getWinSize();
	this->startNodes.clear(); this->allNodes.clear(); this->allEdges.clear();
	int colorOption = UserDefault::getInstance()->getIntegerForKey("colorOption");

	if (colorOption == 0) bglayer=LayerColor::create(Color4B::BLACK);
	else bglayer = LayerColor::create(Color4B::WHITE);

	this->addChild(bglayer);

	loadMapData(colorOption);

	for (int i =0;i<this->allNodes.size(); i++)
	{
		this->allNodes[i]->spr->setZOrder(MAX_Z_ORDER);
		bglayer->addChild(this->allNodes[i]->spr);
		CCLOG("spr pos %f %f", this->allNodes[i]->spr->getPosition().x, this->allNodes[i]->spr->getPosition().y);
	}
	for (int i = 0; i < this->allEdges.size(); i++)
	{
		for (int j = 0; j < this->allEdges[i]->lines.size(); j++)
		{
			bglayer->addChild(this->allEdges[i]->lines[j]->clip);
		}
	}
	CCLOG("scene ptr %p", this);
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
	if (isUpdating == 0)
	{
		loc.x -= layerPos.getMinX();
		for (int i = 0; i < startNodes.size(); i++)
		{
			Rect rect = startNodes[i]->spr->getBoundingBox();
			if (rect.containsPoint(loc))
			{
				isUpdating += 1;
				startNodes[i]->isTrue = !(startNodes[i]->isTrue);
				for (int ii = 0; ii < startNodes.size(); ii++) startNodes[ii]->update(NULL, this->nowZ);

				/*int t = -1; startNodes[i]->updateColor(t);
				if (startNodes[i]->outputEdge. != NULL) {
					startNodes[i]->outputEdge->updateStatusByInput();
					startNodes[i]->outputEdge->updateColor(nowZ);
					//nowZ += startNodes[i]->outputEdge->lines.size();
				}*/
				return false;
			}
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

void GameScene::loadMapData(int colorOption)
{
	char mapName[20];
	int mapGaro, mapSero, nodecnt, edgecnt, node1 ,node2;
	int nodenum, nodetype, nodeval, nodex, nodey;
	int mapNum = UserDefault::getInstance()->getIntegerForKey("nowMapNum");
	mapNum = 1;
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
	mapGaro = winSize.width; mapSero = winSize.height;
	sscanf(datas[1].c_str(), "%d %d", &nodecnt, &edgecnt);
	for (int i = 0; i < nodecnt; i++)
	{
		sscanf(datas[2 + i].c_str(), "%d %d %d %d %d", &nodenum, &nodetype, &nodeval, &nodex, &nodey);
		CCLOG("%s to %d %d %d %d %d\n", datas[2 + i].c_str(), nodenum, nodetype, nodeval, nodex, nodey);
		if (nodex < 0) nodex += mapGaro; if (nodey < 0) nodey += mapSero;
		auto newNode = new CircuitNode(nodetype, nodeval, { nodex, nodey });
		this->allNodes.push_back(newNode);
		if (nodetype == NODE_START) this->startNodes.push_back(newNode);
	}
	for (int i = 0; i < edgecnt; i++)
	{
		sscanf(datas[2 + nodecnt + i].c_str(), "%d %d", &node1, &node2);
		CCLOG("%s to %d %d\n", datas[2 + nodecnt+ i].c_str(), node1, node2);
		auto newEdge = new CircuitEdge(allNodes[node1], allNodes[node2], colorOption);
		this->allEdges.push_back(newEdge);
	}
}