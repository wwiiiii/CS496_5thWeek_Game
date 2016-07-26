#include <vector>
#include "Circuit.h"
USING_NS_CC;


//colorOption이 1이면 W/Y, 0이면 G/R
LineSeg::LineSeg(POINT a, POINT b, int colorOption)
{
	pair<Sprite*, int> res1, res2;
	if (colorOption == 0)
	{
		res1 = lineLinearSpr(a, b, 0);
		res2 = lineLinearSpr(a, b, 1);//make_pair(Sprite::create("pixel1.png"), 0);
	}
	else
	{
		res1 = lineLinearSpr(a, b, 2);
		res2 = lineLinearSpr(a, b, 3);//make_pair(Sprite::create("pixel1.png"), 0);
	}
	this->trueLine = res1.first;
	this->falseLine = res2.first;
	this->dir = res1.second;
	
	this->clip = ClippingNode::create();
	this->clip->setPosition(trueLine->getPosition());
	this->trueLine->setPosition(0, 0);
	
	this->clip->setAlphaThreshold(0.05f);
	this->clip->setContentSize(trueLine->getContentSize());
	this->clip->setStencil(trueLine);

	if (colorOption == 1)
	{
		this->falseLine->setOpacity(200);
	}
	this->clip->addChild(trueLine, 1);
	this->clip->addChild(falseLine, 2);

	falseLine->setPosition(0, 0);
	this->nowColor = -1;
}

CircuitNode::CircuitNode(int type, int isTrue, POINT pos)
{
	int nowOpt = UserDefault::getInstance()->getIntegerForKey("colorOption");
	if(nowOpt == 0)	this->spr = Sprite::create("node_false0.png");
	else this->spr = Sprite::create("node_false1.png");
	this->spr->setScale(0.25*0.5);
	this->isTrue = isTrue; this->type = type; this->pos = pos;
	this->spr->setPosition((float)pos.x, (float)pos.y);
	this->nowColor = -1;
	this->outputEdge = NULL;
	this->updateColor(0);
}

void CircuitNode::addInputEdge(CircuitEdge * newEdge)
{
	this->inputEdges.push_back(newEdge);
}

void CircuitNode::setOutputEdge(CircuitEdge * newEdge)
{
	this->outputEdge = newEdge;
}

void CircuitNode::updateStatusByInput()
{
	bool result, falseExist = false, trueExist = false;
	switch (this->type)
	{
	case NODE_OR:
		result = false;
		for (auto it = this->inputEdges.begin(); it != this->inputEdges.end(); it++)
		{
			if ((*it)->isTrue == true) {
				result = true; break;
			}
		}
		this->isTrue = result; 
		break;
	case NODE_AND:
		result = true;
		for (auto it = this->inputEdges.begin(); it != this->inputEdges.end(); it++)
		{
			if ((*it)->isTrue == false) {
				result = false; break;
			}
		}
		this->isTrue = result;
		break;
	case NODE_XOR:
		falseExist = false; trueExist = false;
		for (auto it = this->inputEdges.begin(); it != this->inputEdges.end(); it++)
		{
			if ((*it)->isTrue == false) {
				falseExist = true;
			}
			else if ((*it)->isTrue == true) {
				trueExist = true;
			}
		}
		if (falseExist == true && trueExist == true) this->isTrue = true;
		else this->isTrue = false;
		break;
	default:
		break;
	}
}

void CircuitNode::updateColor(int nowZ)
{
	int nowOpt = UserDefault::getInstance()->getIntegerForKey("colorOption");
	if (this->nowColor == this->isTrue) return;
	if (this->isTrue == true) {
		if (nowOpt == 0) this->spr->setTexture("node_true0.png");
		else this->spr->setTexture("node_true1.png");
		if(nowZ != -1)this->spr->setZOrder(nowZ);
		this->spr->setScale(0.25*0.5);
		this->nowColor = this->isTrue;
	}
	else if (this->isTrue == false) {
		if (nowOpt == 0) this->spr->setTexture("node_false0.png");
		else this->spr->setTexture("node_false1.png");
		if (nowZ != -1)this->spr->setZOrder(nowZ);
		this->spr->setScale(0.25*0.5);
		this->nowColor = this->isTrue;
	}
}

CircuitEdge::CircuitEdge(CircuitNode * from, CircuitNode * to, int colorOption)
{
	this->length = 0;
	this->nowColor = -1;
	this->setInputNode(from);
	this->setOutputNode(to);
	from->setOutputEdge(this);
	to->addInputEdge(this);
	this->updateStatusByInput();
	this->lines = lineSpr(from->pos, to->pos, colorOption);
	for (int i = 0; i < this->lines.size(); i++)
	{
		float h1 = this->lines[i]->trueLine->getContentSize().width;
		float h2 = this->lines[i]->trueLine->getContentSize().height;
		if (h1 < h2) this->length += h2; else this->length += h1;
	}
	this->updateColor(0);
}


void CircuitEdge::setInputNode(CircuitNode * newNode)
{
	this->inputNode = newNode;
}

void CircuitEdge::setOutputNode(CircuitNode * newNode)
{
	this->outputNode = newNode;
}

void CircuitEdge::updateStatusByInput()
{
	if(inputNode != NULL)this->isTrue = this->inputNode->isTrue;
}

void CircuitEdge::updateColor(int nowZ)
{
	float dur = 0.3;
	if (this->nowColor == this->isTrue) return;
	this->nowColor = this->isTrue;
	vector<LineSeg *> & lines = this->lines;
	auto act = Sequence::create(NULL);
	auto act0 = DelayTime::create(0.0);
	auto act1 = MoveBy::create(0, Point(0, 0));
	auto act2 = Place::create(Point(0, 0));
	if (this->isTrue == true) {
		CCLOG("change red to green");
		float accumDelay = 0.0;
		for (int i = 0; i < lines.size(); i++)
		{
			LineSeg * now = lines[i];
			now->clip->setZOrder(nowZ++);
			float height = now->trueLine->getContentSize().height;
			float width = now->trueLine->getContentSize().width;
			act0 = DelayTime::create(accumDelay);
			float nowDelay = 0.0;
			if (height < width) nowDelay = dur / this->length * width;
			else nowDelay = dur / this->length * height;
			accumDelay += nowDelay;
			switch (now->dir)
			{
			case DIR_DOWN:
				act1 = MoveBy::create(nowDelay, Point(0, -height));
				act2 = Place::create(Point(0,height));
				act = Sequence::create(act0, act1, act2, NULL);
				now->falseLine->runAction(act);
				break;
			case DIR_UP:
				act1 = MoveBy::create(nowDelay, Point(0, height));
				act2 = Place::create(Point(0, -height));
				act = Sequence::create(act0, act1, act2, NULL);
				now->falseLine->runAction(act);
				break;
			case DIR_RIGHT:
				act1 = MoveBy::create(nowDelay, Point(width, 0));
				act2 = Place::create(Point(-width, 0));
				act = Sequence::create(act0, act1, act2, NULL);
				now->falseLine->runAction(act);
				break;
			case DIR_LEFT:
				act1 = MoveBy::create(nowDelay, Point(-width, 0));
				act2 = Place::create(Point(width, 0));
				act = Sequence::create(act0, act1, act2, NULL);
				now->falseLine->runAction(act);
				break;
			default:
				break;
			}
			CCLOG("true %f %f false %f %f", now->trueLine->getPosition().x, now->trueLine->getPosition().y, now->falseLine->getPosition().x, now->falseLine->getPosition().y);
		}
	}
	else if (this->isTrue == false) {
		CCLOG("change green to red");
		float accumDelay = 0.0;
		for (int i = 0; i < lines.size(); i++)
		{
			LineSeg * now = lines[i];
			now->clip->setZOrder(nowZ++);
			float height = now->trueLine->getContentSize().height;
			float width = now->trueLine->getContentSize().width;
			act0 = DelayTime::create(accumDelay);
			float nowDelay = 0.0;
			if (height < width) nowDelay = dur / this->length * width;
			else nowDelay = dur / this->length * height;
			accumDelay += nowDelay;
			switch (now->dir)
			{
			case DIR_DOWN:
				act1 = MoveBy::create(nowDelay, Point(0, -height));
				act2 = Place::create(Point(0, height));
				act = Sequence::create(act0, act2, act1, NULL);
				now->falseLine->runAction(act);
				break;
			case DIR_UP:
				act1 = MoveBy::create(nowDelay, Point(0, height));
				act2 = Place::create(Point(0, -height));
				act = Sequence::create(act0, act2, act1, NULL);
				now->falseLine->runAction(act);
				break;
			case DIR_RIGHT:
				act1 = MoveBy::create(nowDelay, Point(width, 0));
				act2 = Place::create(Point(-width, 0));
				act = Sequence::create(act0, act2, act1, NULL);
				now->falseLine->runAction(act);
				break;
			case DIR_LEFT:
				act1 = MoveBy::create(nowDelay, Point(-width, 0));
				act2 = Place::create(Point(width, 0));
				act = Sequence::create(act0, act2, act1, NULL);
				now->falseLine->runAction(act);
				break;
			default:
				break;
			}
			CCLOG("true %f %f false %f %f", now->trueLine->getPosition().x, now->trueLine->getPosition().y, now->falseLine->getPosition().x, now->falseLine->getPosition().y);
		}
	}
}

/*
 * a랑 b의 x좌표가 같거나 y좌표가 같아야함
 * 1,2번째 인자는 출발/도착점, 3번째 인자는 색
 * 0은 초록, 1은 빨강, 2는 검정, 3은 하양
 */
pair<Sprite *, int> lineLinearSpr(POINT a, POINT b, int COLOR)
{
	float eps = 0.001;
	float margin = 10.0;
	float thick = 10.0;
	if (COLOR < 0 || COLOR > 3) return make_pair((Sprite*)NULL, 0);
	char str[20]; sprintf(str, "pixel%d.png", COLOR);
	auto res = Sprite::create(str);
	res->setPosition((a.x + b.x) / 2.0, (a.y + b.y) / 2.0);
	int dir = 0;
	if (a.x - b.x < eps && b.x - a.x < eps)//세로선
	{
		float height = b.y - a.y; if (height < 0) height = -height;

		auto renderTexture = RenderTexture::create(thick, height + margin, Texture2D::PixelFormat::RGBA8888);
		renderTexture->begin();
		res->visit();
		renderTexture->end();

		auto backOk = Sprite::createWithTexture(renderTexture->getSprite()->getTexture());
		backOk->setPosition((a.x + b.x) / 2.0, (a.y + b.y) / 2.0);
		if (a.y < b.y) dir = DIR_UP; else dir = DIR_DOWN;
		return make_pair(backOk, dir);
	}
	else if (a.y-b.y < eps && b.y - a.y < eps) //가로선
	{
		float width = b.x - a.x; if (width < 0) width = -width;

		auto renderTexture = RenderTexture::create(width + margin, thick, Texture2D::PixelFormat::RGBA8888);
		renderTexture->begin();
		res->visit();
		renderTexture->end();

		auto backOk = Sprite::createWithTexture(renderTexture->getSprite()->getTexture());
		backOk->setPosition((a.x + b.x) / 2.0, (a.y + b.y) / 2.0);
		if (a.x < b.x) dir = DIR_RIGHT; else dir = DIR_LEFT;
		return make_pair(backOk, dir);
	}
	else 
	{
			
	}
	//res->setOpacity(196);
	return make_pair(res,0);
}

vector<LineSeg*> lineSpr(POINT a, POINT b, int colorOption)
{
	float eps = 0.001;
	vector<LineSeg*> res;
	if (a.x - b.x < eps && b.x - a.x < eps)//세로선
	{
		auto t1 = new LineSeg(a, b, colorOption);
		res.push_back(t1);
	}
	else if (a.y - b.y < eps && b.y - a.y < eps) //가로선
	{
		auto t2 = new LineSeg(a, b, colorOption);
		res.push_back(t2);
	}
	else
	{
		POINT left = a; POINT right = b;
		if (a.x > b.x) { left = b; right = a; }
		POINT up = a, down = b;
		if (a.y < b.y) { up = b; down = a; }
		POINT first, second;
		float alpha = 0.2; //alpha += (float)(rand() % 60000 - 30000) / 30000.0 / 3;
		first.x = (long)(left.x * alpha + right.x * (1-alpha));
		first.y = left.y;
		second.x = first.x;
		second.y = right.y;
		res.push_back(new LineSeg(left, first, colorOption));
		res.push_back(new LineSeg(first, second, colorOption));
		res.push_back(new LineSeg(second, right, colorOption));
	}
	return res;
}

