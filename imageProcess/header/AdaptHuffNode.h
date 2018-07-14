#include <utility>
#include <vector>

#ifndef ADAPTHUFFNODE_H
#define ADAPTHUFFNODE_H
using namespace std;

struct AdaptHuffNode{
	static bool showUpTable[257];
	static AdaptHuffNode* nodeSearch; // for function searchNode and searchSwitch 
	static long codeSearch; // for function searchNode 
	static int codeLenSearch; // for function searchNode 
	static int highestOrder; // for function searchSwitch
	int symbol;
	int weight;
	int order;
	AdaptHuffNode *leftNode;
	AdaptHuffNode *rightNode;
	AdaptHuffNode *upNode;
	AdaptHuffNode();
	AdaptHuffNode(int Symbol, int Weight, int Order, AdaptHuffNode* LeftNode, AdaptHuffNode* RightNode, AdaptHuffNode* UpNode);

	void searchNode(AdaptHuffNode *node, int symbol, long code, int codeLen);
	pair<long, int> encode(int codeIn);
	void printTree(void);
	void update(AdaptHuffNode* root);
	void searchSwitch(AdaptHuffNode *node, int weight, int order, AdaptHuffNode* exception);
	static void clearShowUpTable();
	static void encodeFile(const char* filenameIn, const char* filenameOut);
	static void decodeFile(const char* filenameIn, const char* filenameOut);
	static void AdaptHuffNode::decodeFileToVec(const char* filenameIn, vector<unsigned char>& outputVec);
};
# endif
