#include <vector>
#include "Circuit.h"
USING_NS_CC;


CircuitNode::CircuitNode(int type, int isTrue, POINT pos)
{
	this->spr = Sprite::create("node_false.png");
	this->isTrue = isTrue; this->type = type; this->pos = pos;
	this->spr->setPosition((float)pos.x, (float)pos.y);

	if (type == NODE_END) this->outputEdge = NULL;
	
	this->updateColor();
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

void CircuitNode::updateColor()
{
	if (this->isTrue == true) this->spr->setTexture("node_true.png");
	else if (this->isTrue == false) this->spr->setTexture("node_false.png");
}

CircuitEdge::CircuitEdge(CircuitNode * from, CircuitNode * to)
{
	this->setInputNode(from);
	this->setOutputNode(to);
	this->updateStatusByInput();
	/*this->spr = DrawNode::create();
	Vec2 origin = Vec2(from->pos.x, from->pos.y);
	Vec2 dest = Vec2(to->pos.x, to->pos.y);
	this->spr->drawLine(origin, dest, Color4F::WHITE);
	this->updateColor();*/
	this->lines = lineSpr(from->pos, to->pos, 0);
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
	this->isTrue = this->inputNode->isTrue;
}

void CircuitEdge::updateColor()
{
	auto actionRed = TintTo::create(0.1, 255, 0, 0);
	auto actionGreen = TintTo::create(0.1, 0, 255, 0);
	if (this->isTrue == true) {
		//this->spr->runAction(actionGreen);
		/*this->spr->setColor(Color3B::GREEN);
		this->spr->clear();
		Vec2 origin = Vec2(this->inputNode->pos.x, this->inputNode->pos.y);
		Vec2 dest = Vec2(this->outputNode->pos.x, this->outputNode->pos.y);
		this->spr->drawLine(origin, dest, Color4F::GREEN);*/
	}
	else if (this->isTrue == false) {
		//this->spr->runAction(actionRed);
		/*this->spr->setColor(Color3B::RED);
		this->spr->clear();
		Vec2 origin = Vec2(this->inputNode->pos.x, this->inputNode->pos.y);
		Vec2 dest = Vec2(this->outputNode->pos.x, this->outputNode->pos.y);
		this->spr->drawLine(origin, dest, Color4F::RED);*/
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
		/*res->setScaleY(height+margin);
		res->setScaleX(thick);*/

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
		/*res->setScaleX(width+margin);
		res->setScaleY(thick);*/
		
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

vector<pair<Sprite *, int> > lineSpr(POINT a, POINT b, int COLOR)
{
	float eps = 0.001;
	vector<pair<Sprite *,int> > res;
	if (COLOR < 0 || COLOR > 3) res;
	if (a.x - b.x < eps && b.x - a.x < eps)//세로선
	{
		res.push_back(lineLinearSpr(a, b, COLOR));
	}
	else if (a.y - b.y < eps && b.y - a.y < eps) //가로선
	{
		res.push_back(lineLinearSpr(a, b, COLOR));
	}
	else
	{
		POINT left = a; POINT right = b;
		if (a.x > b.x) { left = b; right = a; }
		POINT up = a, down = b;
		if (a.y < b.y) { up = b; down = a; }
		POINT first, second;
		first.x = (long)(left.x * 0.2 + right.x * 0.8);
		first.y = left.y;
		second.x = first.x;
		second.y = right.y;
		res.push_back(lineLinearSpr(left, first, COLOR));
		res.push_back(lineLinearSpr(first, second, COLOR));
		res.push_back(lineLinearSpr(second, right, COLOR));
	}
	return res;
}