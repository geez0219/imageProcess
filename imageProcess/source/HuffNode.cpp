#include <iostream>
#include <vector> // vector
#include <utility> // pair
#include <climits> // INT_MAX
#include <stack> // stack
#include <fstream> // ifstream ofstream
#include <string> // getline
#include "bitio.h"
#include "HuffNode.h"
#pragma warning(disable : 4996)  
using namespace std;

HuffNode::HuffNode(int _symbol, float _prob){
	symbol = _symbol; // symbol: 0~256 means that HuffNode is leaf Node; 256 means EOF; 257 means that is not leaf node
	prob = _prob; // the probability of that symbol or the probability sum of that node
	leftNode = NULL;
	rightNode = NULL;
}
	
HuffNode* HuffNode::treeBuild(vector<HuffNode*> nodeList){
	/*
		Build the Huffman tree with a vector of Huffman Node. 
		It returns the root node pointer of the tree.
	*/
	float min, min2;
	vector <HuffNode*> mergeList;
	HuffNode *NodeBuff;
	while(nodeList.size()>1){
		min = INT_MAX; // the node index with smallest prob
		min2 = INT_MAX; // the node index with second smallest prob
		// get the two smallest prob (can be the same)
		for(int i=0;i<nodeList.size();i++){
			if(nodeList[i]->prob < min){
				min2 = min;
				min = nodeList[i]->prob;
			}
			else if(nodeList[i]->prob < min2){
				min2 = nodeList[i]->prob;
			} 
		} 
		// get the two node which has prob = min or min2
		for(int i=nodeList.size()-1;i>=0;i--){
			if(nodeList[i]->prob == min || nodeList[i]->prob == min2){
				mergeList.push_back(nodeList[i]);
				nodeList.erase(nodeList.begin()+i);
				if (mergeList.size()==2) break;
			} 
		}
		NodeBuff = new HuffNode(257, mergeList[0]->prob + mergeList[1]->prob);
		NodeBuff->rightNode = mergeList[0];
		NodeBuff->leftNode = mergeList[1];
		mergeList[0]->upNode = NodeBuff;
		mergeList[1]->upNode = NodeBuff;
		nodeList.push_back(NodeBuff);
		mergeList.clear();
	}
	return nodeList[0];
}	
	
void HuffNode::tableBuild(HuffNode *node, int code, int codeBit){
	/*
		A recursive function to build the encoding table.
	*/
	if(node->symbol<=256 && node->symbol>=0){
		codeTable[node->symbol].first = code;
		codeTable[node->symbol].second = codeBit;
		return;
	}
	tableBuild(node->leftNode, code << 1, codeBit+1);
	tableBuild(node->rightNode, (code << 1)+1, codeBit+1);
}  

void HuffNode::clearCodeTable(){
	for(int i=0;i<257;i++){
		codeTable[i].first = 0;
		codeTable[i].second = 0;
	}
}
	
void HuffNode::encodeFile(const char *filenameIn, const char *filenameOut){
	/*
		Encode the raw file, filenameIn, as filenameOut.
		The encode process simply use look-up table (codeTable)
	*/
	FILE *fileIn = fopen(filenameIn,"rb");
	BIT_FILE *fileOut = OpenOutputBitFile(filenameOut);
	unsigned char byteBuff[1];
	
	while(fread(byteBuff, sizeof(unsigned char),1,fileIn)){
		OutputBits(fileOut, codeTable[byteBuff[0]].first, codeTable[byteBuff[0]].second);
	}	
	OutputBits(fileOut, codeTable[256].first, codeTable[256].second);
    CloseOutputBitFile(fileOut);
	cout << "Huffman encoding " << filenameIn << " is done and the output file is " << filenameOut << endl;	
}
	
void HuffNode::decodeFile(HuffNode *rootp, const char *filenameIn, const char *filenameOut){
	/*
		Decode the raw file, filenameIn, as filenameOut.
		Decoding need the root points of Huffman tree (root point = whole tree). 
		Walk from root to leaf and the symbol is at leaf point, and keep repeat it agian and agian
	*/	 	
	int bit;
    HuffNode *crusor = rootp;
    BIT_FILE *fileIn = OpenInputBitFile(filenameIn);
    FILE *fileOut = fopen(filenameOut, "wb");
	while(1){
		bit = InputBit(fileIn);
		if (bit == 2) {
			cerr << "the encoded code didn't have encoded EOF" << endl;
			system("pause");
			exit(1);
		} 
		if(bit==0) crusor = crusor->leftNode;
		else crusor = crusor->rightNode;
		if (crusor->symbol == 256) break; // read the encoded EOF
		else if(crusor->symbol<=255){
			putc(crusor->symbol, fileOut);
			crusor = rootp;
		}
	}
	cout << "Huffman decoding " << filenameIn << " is done and the output file is " << filenameOut << endl; 
}

void HuffNode::outputTree(const char* filenameOut) {
	HuffNode* ptr = this;
	ofstream fileOut(filenameOut);
	string buffString;
	if (fileOut.fail()) {
		cerr << "cannot create file " << filenameOut << endl;
		system("pause");
		exit(1);
	}
	stack<HuffNode*> theStack;
	while (1) {
		if (ptr == NULL) {
			fileOut << "NULL,";
			if (theStack.empty()) {
				break;
			}
			else {
				ptr = theStack.top();
				theStack.pop();
			}
		}
		else {
			theStack.push(ptr->rightNode);
			if (ptr->symbol >= 0 && ptr->symbol<257) {
				fileOut << ptr->symbol << ",";
			}
			else {
				fileOut << "-1,";
			}
			ptr = ptr->leftNode;
		}
	}
	fileOut.close();
}

HuffNode* HuffNode::buildTreeCSV(const char* filenameIn) {
	ifstream fileIn(filenameIn);
	if (fileIn.fail()) {
		cerr << "cannot open file " << filenameIn;
		system("pause");
		exit(1);
	}
	string buffStr;
	HuffNode *root = NULL;
	HuffNode *ptr = root;
	stack<HuffNode*> HuffPtrStack;
	getline(fileIn, buffStr, ',');

	if (buffStr == "NULL") {
		return NULL;
	}
	else {
		int buffInt = stoi(buffStr);
		if (buffInt > 256 || buffInt < -1) {
			cerr << "node value is over 256 or smaller than -1" << endl;
			system("pause");
			exit(1);
		}
		ptr = (buffInt == -1 ? new HuffNode(257, 0) : new HuffNode(buffInt, 0));
		root = ptr;
		HuffPtrStack.push(ptr);
	}

	bool leftOrRight = 0;
	while (fileIn.good()) {
		getline(fileIn, buffStr, ',');
		if (leftOrRight == 0) {
			if (buffStr == "NULL") {
				leftOrRight = 1;
				if (HuffPtrStack.empty()) {
					break;
				}
				else {
					ptr = HuffPtrStack.top();
					HuffPtrStack.pop();
				}
			}
			else {
				int buffInt = stoi(buffStr);
				if (buffInt > 256 || buffInt < -1) {
					cerr << "node value is over 256 or smaller than -1" << endl;
					system("pause");
					exit(1);
				}
				ptr->leftNode = (buffInt == -1 ? new HuffNode(257, 0): new HuffNode(buffInt, 0));
				ptr = ptr->leftNode;
				HuffPtrStack.push(ptr);
			}
		}
		else {
			if (buffStr == "NULL") {
				if (HuffPtrStack.empty()) {
					break;
				}
				else {
					ptr = HuffPtrStack.top();
					HuffPtrStack.pop();
				}
			}
			else {
				int buffInt = stoi(buffStr);

				if (buffInt > 256 || buffInt < -1) {
					cerr << "node value is over 256 or smaller than -1" << endl;
					system("pause");
					exit(1);
				}
				ptr->rightNode = (buffInt == -1 ? new HuffNode(257, 0) : new HuffNode(buffInt, 0));
				ptr = ptr->rightNode;
				HuffPtrStack.push(ptr);
				leftOrRight = 0;
			}
		}
	}
	return root;
}
void HuffNode::encodePic(unsigned char*** pic, int sizeY, int sizeX, int sizeZ, const char *filenameOut) {
	/*
	The function to encode raw data, filenameIn,  as filenameOut.
	Encoding simply use the look-up table (codeTable).
	*/
	BIT_FILE *fileOut = OpenOutputBitFile(filenameOut);
	for (int y = 0; y<sizeY; y++) {
		for (int x = 0; x<sizeX; x++) {
			for (int z = 0; z<sizeZ; z++) {
				OutputBits(fileOut, codeTable[pic[y][x][z]].first, codeTable[pic[y][x][z]].second);
			}
		}
	}
	OutputBits(fileOut, codeTable[256].first, codeTable[256].second); // encode the EOF
	CloseOutputBitFile(fileOut);
	cout << "Huffman encoding picture is done and the output file is " << filenameOut << endl;

}
void HuffNode::decodeFileToVec(HuffNode* rootp, const char* filenameIn, vector<unsigned char>& outputVec) {
	int bit;
	HuffNode *crusor = rootp;
	BIT_FILE *fileIn = OpenInputBitFile(filenameIn);
	while (1) {
		bit = InputBit(fileIn);
		if (bit == 2) {
			cerr << "the encoded code didn't have encoded EOF" << endl;
			exit(1);
		}
		if (bit == 0) crusor = crusor->leftNode;
		else crusor = crusor->rightNode;
		if (crusor->symbol == 256) break; // read the encoded EOF
		else if (crusor->symbol <= 255) {
			outputVec.emplace_back(crusor->symbol);
			crusor = rootp;
		}
	}
	cout << "Huffman decoding " << filenameIn << " is done " << endl;

}
pair<int, int>  HuffNode::codeTable[257] = {};
