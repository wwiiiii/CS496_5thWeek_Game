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
������Ʈ�ϴ¹�


1. ��Ÿ�� ��� �� �ϳ��� Ŭ���ϸ� isUpdating�� true�� �Ǹ鼭 ���ο� ������Ʈ�� ���ŷ��
2. ��� ��Ÿ�� ��忡�� update()�� ȣ���Ѵ�.
3. �� ��忡�� update�� ȣ��Ǹ� updateCount�� �˻��Ѵ�
	-��Ÿ�� ���� �˻����� �ʰ� ������ update ����
	-�� ��尡 update�� �Ϸ����� �� �ڱ�� ����� ���� ��忡�� update ȣ��
	-ȣ����� ���� ��� ������ ���� ȣ��޾Ҵ��� �˻��ؼ� �ű⿡ �ش�Ǵ� updateCount�� 1 ����
		-ȣ���� �� CircuitEdge*���� �Ѱ��༭ inputEdges�� ��ȸ�ϸ� �ش�Ǵ� �ε����� ã���� ��
	-updateCount�� �˻����� �� ��� ��ǲ ������ ī��Ʈ�� 1 �̻��̸� ��� 1�� ���̰� update�� ����
4. update�� ����Ǹ� updateStatus, updateColor�� ������� ȣ���ϰ� ��� outputEdge���� update�� ȣ��
5. ���������� end ����� update�� ������ isUpdating�� false�� �ٲ�

*/