#include <vector> // vector
#include <utility> // pair

#ifndef HUFFNODE_H
#define HUFFNODE_H

using namespace std;

struct HuffNode{
	static pair<int,int> codeTable[257];
	int symbol;
	float prob;
	HuffNode *leftNode;
	HuffNode *rightNode;
	HuffNode *upNode;
	HuffNode();
	HuffNode(int _symbol, float _prob);
	static HuffNode* treeBuild(vector<HuffNode*> nodeList);
	static void tableBuild(HuffNode *node, int code, int codeBit);
	static void clearCodeTable();
	static void encodeFile(const char *filenameIn, const char *filenameOut);
	static void decodeFile(HuffNode *rootp, const char *filenameIn, const char *filenameOut);
	static void encodePic(unsigned char*** pic, int sizeY, int sizeX, int sizeZ, const char *filenameOut);
	static void decodeFileToVec(HuffNode* rootp, const char* filenameIn, vector<unsigned char>& outputVec);
	void outputTree(const char* fileNameOut);
	static HuffNode* buildTreeCSV(const char* filenameIn);
};
#endif

