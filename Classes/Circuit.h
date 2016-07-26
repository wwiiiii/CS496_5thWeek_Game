#include <vector>

#ifndef __CIRCUIT_H__
#define __CIRCUIT_H__

#include "cocos2d.h"

#define NODE_AND 0
#define NODE_OR 1
#define NODE_XOR 2
#define NODE_NOT 3
#define NODE_START 4
#define NODE_END 5
#define DIR_LEFT 0
#define DIR_RIGHT 1
#define DIR_UP 2
#define DIR_DOWN 3
#define vector std::vector
#define make_pair std::make_pair
#define pair std::pair
USING_NS_CC;

class CircuitEdge;
class CircuitNode;
vector<pair<Sprite *, int> > lineSpr(POINT from, POINT to, int);
pair<Sprite *, int >lineLinearSpr(POINT from, POINT to, int);
class CircuitEdge
{
	public:
		DrawNode* spr;
		vector<pair<Sprite *, int> > lines;
		CircuitNode* inputNode;
		CircuitNode* outputNode;
		int isTrue;
		CircuitEdge(CircuitNode * from, CircuitNode * to);
		void setInputNode(CircuitNode *);
		void setOutputNode(CircuitNode *);
		void updateStatusByInput();
		void updateColor();

};

class CircuitNode
{
	public:
		Sprite* spr;
		vector<CircuitEdge*> inputEdges;
		CircuitEdge *outputEdge;
		POINT pos;
		int isTrue, type;
		CircuitNode(int type, int isTrue, POINT pos);
		void addInputEdge(CircuitEdge *);
		void setOutputEdge(CircuitEdge *);
		void updateStatusByInput();
		void updateColor();
};

#endif // __CIRCUIT_NODE_H__
