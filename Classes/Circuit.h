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

static const char nodeNameByType[6][10] = { "and","or","xor","not","start","end" };
class CircuitEdge;
class CircuitNode;
class LineSeg;

vector<LineSeg*> lineSpr(POINT from, POINT to, int);
pair<Sprite *, int >lineLinearSpr(POINT from, POINT to, int);

class LineSeg
{
	public:
		Sprite * trueLine;
		Sprite * falseLine;
		int dir;
		ClippingNode * clip;
		int nowColor;
		LineSeg(POINT, POINT, int);
};

class CircuitEdge
{
	public:
		DrawNode* spr;
		float length;
		vector<LineSeg*> lines;
		CircuitNode* inputNode;
		CircuitNode* outputNode;
		int isTrue, nowColor;
		CircuitEdge(CircuitNode * from, CircuitNode * to,int);
		void setInputNode(CircuitNode *);
		void setOutputNode(CircuitNode *);
		void updateStatusByInput();
		void updateColor(int & nowZ);
		static void updateOutputNode(CircuitEdge*, int & nowZ);
};

class CircuitNode
{
	public:
		Sprite* spr;
		vector<CircuitEdge*> inputEdges;
		vector<CircuitEdge*> outputEdges;
		vector<int> updateCount;
		POINT pos;
		int isTrue, type, nowColor;
		CircuitNode(int type, int isTrue, POINT pos);
		void addInputEdge(CircuitEdge *);
		void addOutputEdge(CircuitEdge *);
		void updateStatusByInput();
		void updateColor(int & nowZ);
		void update(CircuitEdge *, int & nowZ);
};

#endif // __CIRCUIT_NODE_H__


/*
업데이트하는법


1. 스타팅 노드 중 하나를 클릭하면 isUpdating이 true가 되면서 새로운 업데이트는 블로킹됨
2. 모든 스타팅 노드에서 update()를 호출한다.
3. 한 노드에서 update가 호출되면 updateCount를 검사한다
	-스타팅 노드는 검사하지 않고 무조건 update 시작
	-각 노드가 update를 완료했을 때 자기랑 연결된 뒤쪽 노드에게 update 호출
	-호출받은 노드는 어느 엣지에 의해 호출받았는지 검사해서 거기에 해당되는 updateCount를 1 증가
		-호출할 때 CircuitEdge*값을 넘겨줘서 inputEdges를 순회하며 해당되는 인덱스를 찾으면 됨
	-updateCount를 검사했을 때 모든 인풋 엣지의 카운트가 1 이상이면 모두 1씩 줄이고 update를 진행
4. update가 진행되면 updateStatus, updateColor를 순서대로 호출하고 모든 outputEdge들의 update를 호출
5. 최종적으로 end 노드의 update가 끝나면 isUpdating을 false로 바꿈

*/