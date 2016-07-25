#include "GameScene.h"
#include "Circuit.h"

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
	winSize = Director::getInstance()->getWinSize();
	
	auto a1 = new CircuitNode(NODE_START, true, { 100, (long)(winSize.height - 100) });
	auto a2 = new CircuitNode(NODE_START, false, { 100, (long)(winSize.height - 200) });

	auto a3 = new CircuitNode(NODE_AND, false, { 400, (long)(winSize.height - 100) });

	auto a4 = new CircuitNode(NODE_OR, false, { 400, (long)(winSize.height - 200) });

	auto e1 = new CircuitEdge(a1, a3);
	auto e2 = new CircuitEdge(a2, a3);
	
	/*
	std::string shaderName = "res/STHologramShader";
	auto glCache = GLProgramCache::getInstance();
	GLProgram* prog = glCache->getGLProgram(shaderName);
	if (!prog)
	{
		auto fs = FileUtils::getInstance();
		auto fragPath = fs->fullPathForFilename(shaderName + ".fsh");
		auto vertPath = fs->fullPathForFilename(shaderName + ".vsh");
		std::string fragmentSource = fs->getStringFromFile(fragPath);
		std::string vertexSource = fs->getStringFromFile(vertPath);
		auto fragB = fragmentSource.c_str();
		auto vertB = vertexSource.c_str();
		prog = GLProgram::createWithByteArrays(vertB, fragB);
		glCache->addGLProgram(prog, shaderName);
	}
	e1->spr->setGLProgram(prog);
	prog->use();*/
	GLProgram * p = new GLProgram();
	p->initWithFilenames("gray.vsh", "gray.fsh");
	p->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_POSITION, GLProgram::VERTEX_ATTRIB_POSITION);
	p->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_COLOR, GLProgram::VERTEX_ATTRIB_COLOR);
	p->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_TEX_COORD, GLProgram::VERTEX_ATTRIB_TEX_COORDS);
	p->link();
	p->updateUniforms();
	auto gp = GLProgramState::create(p);
	gp->setUniformVec2("u_stepSize", Vec2(1.0 / winSize.width, 1.0 / winSize.height));
	/*a1->spr->setGLProgram(p);
	a1->spr->setGLProgramState(gp);
	a2->spr->setGLProgram(p);
	a3->spr->setGLProgram(p);*/

	this->addChild(e1->spr);
	this->addChild(e2->spr);
	this->addChild(a1->spr);
	this->addChild(a2->spr);
	this->addChild(a3->spr);
	this->addChild(a4->spr);

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(touchListener, 1);

    return true;
}


bool GameScene::onTouchBegan(Touch *touch, Event *unused_event)
{
	Point loc = touch->getLocation();
	/*auto sprPlay = (Sprite*)this->getChildByTag(TAG_PLAY);
	Rect rect = sprPlay->getBoundingBox();
	if (rect.containsPoint(loc))//touched GAME PLAY
	{
	
	
		return false;
	}
	*/
	return false;
}
