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

void GameScene::onEnter()
{
	Layer::onEnter();
	this->startNodes.clear(); this->allNodes.clear(); this->allEdges.clear();
	int colorOption = UserDefault::getInstance()->getIntegerForKey("colorOption");
	bglayer = Layer::create(); sceneStatus = -1;
	auto opt = Sprite::create(); auto resume = Sprite::create(); auto out = Sprite::create();

	if (colorOption == 0) {
		opt->setTexture("menu_white.png");
		resume->setTexture("resume_white.png");
		out->setTexture("out_white.png");
	}
	else {
		opt->setTexture("menu_black.png");
		resume->setTexture("resume_black.png");
		out->setTexture("out_black.png");
		bglayer->addChild(Sprite::create("whiteBG.png"));
	}

	opt->setScale(0.25); opt->setOpacity(128 / 2); opt->setTag(TAG_GAME_OPTION);
	opt->setPosition(winSize.width - 50, winSize.height - 50);
	resume->setPosition(winSize.width / 2 + 200, winSize.height / 2); resume->setVisible(false);
	out->setPosition(winSize.width / 2 - 200, winSize.height / 2); out->setVisible(false);
	resume->setTag(TAG_GAME_OPTION_RESUME); out->setTag(TAG_GAME_OPTION_OUT);
	
	bglayer->runAction(MoveBy::create(0.0, Point(100, 0)));
	bglayer->setCascadeOpacityEnabled(true);

	this->addChild(bglayer);	this->addChild(opt);
	this->addChild(out);	this->addChild(resume);

	loadMapData(colorOption);
	//this->isUpdating = 0;
	for (int i = 0; i<this->allNodes.size(); i++)
	{
		this->allNodes[i]->spr->setZOrder(MAX_Z_ORDER);
		bglayer->addChild(this->allNodes[i]->spr);
		CCLOG("spr pos %f %f", this->allNodes[i]->spr->getPosition().x, this->allNodes[i]->spr->getPosition().y);
		for (int j = 0; j < this->allNodes[i]->updateCount.size(); j++)this->allNodes[i]->updateCount[j] = 0;
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

	auto temp = Sprite::create("setting_black.png"); temp->setVisible(false);
	this->addChild(temp);
	auto delay = DelayTime::create(1.0);
	auto func = CallFunc::create(CC_CALLBACK_0(GameScene::loadComplete, this));
	auto next = Sequence::create(delay, func, NULL);
	temp->runAction(next);

}

void GameScene::onExit()
{
	Layer::onExit();
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
	nowZ = 1; isUpdating = 0; sceneStatus = 0;
	srand(time(NULL));
	this->winSize = Director::getInstance()->getWinSize();

	//log("%s", "A string");
    return true;
}


bool GameScene::onTouchBegan(Touch *touch, Event *unused_event)
{
	Point loc = touch->getLocation();
	Point backloc = loc;
	Rect layerPos = bglayer->getBoundingBox();
	debug();
	if (this->sceneStatus == 0)//playing game, normal
	{
		auto spr = this->getChildByTag(TAG_GAME_OPTION);
		Rect optRec = spr->getBoundingBox();
		//메뉴 버튼을 클릭하면
		if (optRec.containsPoint(loc))
		{
			auto resume = this->getChildByTag(TAG_GAME_OPTION_RESUME); auto out = this->getChildByTag(TAG_GAME_OPTION_OUT);
			resume->setVisible(true); out->setVisible(true);
			bglayer->setOpacity(128);
			sceneStatus = 1; return false;
		}
		CCLOG("isUpdating stat : %d", isUpdating);
		if (isUpdating < 0) isUpdating = 0;
		if (isUpdating == 0)
		{
			loc.x -= layerPos.getMinX();
			loc.y -= layerPos.getMinY();
			for (int i = 0; i < startNodes.size(); i++)
			{
				Rect rect = startNodes[i]->spr->getBoundingBox();
				if (rect.containsPoint(loc))
				{
					isUpdating += 1;
					startNodes[i]->isTrue = !(startNodes[i]->isTrue);
					for (int ii = 0; ii < startNodes.size(); ii++) startNodes[ii]->update(NULL, this->nowZ);
					return false;
				}
			}
		}
	}
	else if(sceneStatus == 1)//메뉴 보고 있을때
	{
		auto resume = this->getChildByTag(TAG_GAME_OPTION_RESUME); auto out = this->getChildByTag(TAG_GAME_OPTION_OUT);
		if (resume->getBoundingBox().containsPoint(loc) || this->getChildByTag(TAG_GAME_OPTION)->getBoundingBox().containsPoint(loc))
		{
			sceneStatus = 0;
			resume->setVisible(false); out->setVisible(false); bglayer->setOpacity(255);
			return false;
		}
		else if (out->getBoundingBox().containsPoint(loc))
		{
			Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
			Director::getInstance()->popScene();
			return false;
		}
		else
		{
			return false;
		}
	}
	else if (this->sceneStatus == 2)//winGame!
	{
		auto resume = this->getChildByTag(TAG_GAME_OPTION_RESUME); auto out = this->getChildByTag(TAG_GAME_OPTION_OUT);
		if (resume->getBoundingBox().containsPoint(loc) || this->getChildByTag(TAG_GAME_OPTION)->getBoundingBox().containsPoint(loc))
		{
			Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
			Director::getInstance()->replaceScene(GameScene::createScene());
			return false;
		}
		else if (out->getBoundingBox().containsPoint(loc))
		{
			Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
			Director::getInstance()->popScene();
			return false;
		}
		else
		{
			return false;
		}
	}
	else if (sceneStatus == -1)//blocking
	{
		return false;
	}
	pastTouch = backloc;
	return true;
}

void GameScene::onTouchMoved(Touch *touch, Event *unused_event)
{
	float thres = -1.0;
	Point loc = touch->getLocation();
	
	//log("%f %f", loc.x, loc.y);
	bglayer->runAction(MoveBy::create(0.0, Point(loc.x - pastTouch.x, loc.y - pastTouch.y)));
	pastTouch = loc;
}

void GameScene::loadMapData(int colorOption)
{
	char mapName[20];
	int mapGaro, mapSero, nodecnt, edgecnt, node1 ,node2;
	int nodenum, nodetype, nodeval, nodex, nodey;
	int mapNum = UserDefault::getInstance()->getIntegerForKey("nowStage");
	if (mapNum < 1 || mapNum>12) {
		mapNum = 1;
		UserDefault::getInstance()->setIntegerForKey("nowStage",1);
	}
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
		newNode->num = i;
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

void GameScene::winGame()
{
	CCLOG("Win Game!");
	int color = UserDefault::getInstance()->getIntegerForKey("colorOption");
	int nowStage = UserDefault::getInstance()->getIntegerForKey("nowStage");
	int bestStage = UserDefault::getInstance()->getIntegerForKey("bestStage");

	if (nowStage >= bestStage) {
		bestStage = nowStage;
		UserDefault::getInstance()->setIntegerForKey("bestStage", nowStage);
	}
	UserDefault::getInstance()->setIntegerForKey("nowStage", nowStage+1);
	this->sceneStatus = 2;
	auto resume = (Sprite*)this->getChildByTag(TAG_GAME_OPTION_RESUME); auto out = this->getChildByTag(TAG_GAME_OPTION_OUT);
	bglayer->setOpacity(128/2);
	if (color == 0)resume->setTexture("check_white.png");
	else resume->setTexture("check_white.png");
	((Sprite*)out)->setTexture("out_white.png");
	auto act1 = MoveBy::create(0.0, Point(0, -100));
	auto act2 = MoveBy::create(0.0, Point(0, -100));
	out->runAction(act1); resume->runAction(act2);

	auto label = Label::createWithTTF("Win!", "NanumGothicExtraBold.ttf", 200, Size(500, 200), TextHAlignment::CENTER, TextVAlignment::CENTER);
	label->setPosition(winSize.width/2, winSize.height/4*3);
	if (color == 0)label->setColor(Color3B::GREEN);
	else label->setColor(Color3B::GREEN);
	this->addChild(label);
	resume->setVisible(true); out->setVisible(true);
}	

void GameScene::loadComplete()
{
	for (int i = 0; i < allNodes.size(); i++)
	{
		for (int j = 0; j < allNodes[i]->updateCount.size(); j++)
		{
			allNodes[i]->updateCount[j] = 0;
		}
	}
	isUpdating = 1;
	for (int ii = 0; ii < startNodes.size(); ii++) startNodes[ii]->update(NULL, this->nowZ);
	sceneStatus = 0;
}

void GameScene::debug()
{
	for (int i = 0; i < allNodes.size(); i++)
	{
		auto now = allNodes[i];
		CCLOG("node%d : isTrue %d nowColor %d",allNodes[i]->num, allNodes[i]->isTrue, allNodes[i]->nowColor);
		std::string a = ""; char str[20];
		for (int j = 0; j < now->updateCount.size(); j++)
		{
			sprintf(str, "%d", now->updateCount[j]);
			a += std::string(str) + std::string(" ");
		}
		CCLOG(a.c_str());
	}
	for (int i = 0; i < allEdges.size(); i++)
	{
		auto now = allEdges[i];
		CCLOG("edge %d to %d : isTrue %d nowColor %d",now->inputNode->num, now->outputNode->num, now->isTrue, now->nowColor);
	}
}