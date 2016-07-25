#include <vector>
#include "Circuit.h"
#define vector std::vector
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
	this->spr = DrawNode::create();
	Vec2 origin = Vec2(from->pos.x, from->pos.y);
	Vec2 dest = Vec2(to->pos.x, to->pos.y);
	this->spr->drawLine(origin, dest, Color4F::WHITE);
	this->updateColor();
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
		this->spr->runAction(actionGreen);
	}
	else if (this->isTrue == false) {
		this->spr->runAction(actionRed);
	}
}