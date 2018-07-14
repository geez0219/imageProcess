#include <vector> // vector
#include <utility> // pair

#ifndef FANONODE_H
#define FANONODE_H

using namespace std;

struct FanoNode{
	static pair<int, int> codeTable[257];  // the code Table for encoding, first element is code value, second element is bit long of code
	vector<pair<int, float> > list; // the list of symbol and it probability
	int code; // the code value
	int codeBit; // the code bit length
	FanoNode *leftNode; 
	FanoNode *rightNode;
	FanoNode *upNode;
	FanoNode();
	FanoNode(vector<pair<int, float> > List, int Code, int CodeBit);
	void treeBuild();
	void outputTree(const char* filenameOut);
	static void clearCodeTable(); 
	static void encodeFile(const char *filenameIn, const char *filenameOut);
	static void decodeFile(FanoNode *rootp, const char *filenameIn, const char *filenameOut);
	static void encodePic(unsigned char*** pic, int sizeY, int sizeX, int sizeZ, const char *filenameOut);
	static void decodeFileToVec(FanoNode* rootp, const char* filenameIn, vector<unsigned char>& outputVec);
	static FanoNode* buildTreeCSV(const char* filenameIn);
	static void printCodeTable();
};
#endif
