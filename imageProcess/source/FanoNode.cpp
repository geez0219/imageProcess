#include <iostream>
#include <vector> // vector
#include <utility> // pair
#include <stack> // stack
#include <fstream> // ifstream ofstream
#include <string> // getline
#include "bitio.h"
#include "FanoNode.h"
#pragma warning(disable : 4996) 
using namespace std;

FanoNode::FanoNode(vector<pair<int, float> > List, int Code, int CodeBit){
		list = List; // the list of symbol and its probability pair
		code = Code; // the code value
		codeBit = CodeBit; // the code length
		leftNode = NULL;
		rightNode = NULL;
}
	
void FanoNode::treeBuild(){
	/* 
		A recursive function to build the Fano coding tree, by keep splitting the list 
		until each node's list has only one symbol, and then update its code and codeBit to 
		codeTable
	*/
	
	if(list.size()==1){
		leftNode = NULL;
		rightNode = NULL;
		codeTable[list[0].first].first = code;
		codeTable[list[0].first].second = codeBit;
		return;
	}
	float probSum = 0;
	float listProbSum = 0;
	int separateLine;
	for(int i=0;i<list.size();i++){
		listProbSum += list[i].second; // get the prob sum of its list
	}
	
	for(int i=0;i<list.size();i++){
		probSum += list[i].second;
		if(probSum>listProbSum*0.5){ // split the node at the point closest to half of probability sum
			if(probSum - listProbSum*0.5 < listProbSum*0.5 - probSum + list[i].second){
				separateLine = i+1;
			}
			else{
				separateLine = i;
			}
			break;
		}
	}
	vector<pair<int ,float> > leftNodeList(list.begin(), list.begin()+separateLine);
	vector<pair<int ,float> > rightNodeList(list.begin()+separateLine, list.end());
	leftNode = new FanoNode(leftNodeList, code << 1, codeBit+1);
	rightNode = new FanoNode(rightNodeList, (code << 1) + 1, codeBit+1);
	leftNode->upNode = this;
	rightNode->upNode = this;
	leftNode->treeBuild();
	rightNode->treeBuild();
}

void FanoNode::clearCodeTable(){
	/*  
		the function to clear the static code table
	*/
	for(int i=0;i<257;i++){
		codeTable[i].first = 0;
		codeTable[i].second = 0;
	}
}

void FanoNode::encodeFile(const char *filenameIn, const char *filenameOut){
	/*
		The function to encode raw data, filenameIn,  as filenameOut.
		Encoding simply use the look-up table (codeTable).
	*/
	
	FILE *fileIn=fopen(filenameIn,"rb");
	BIT_FILE *fileOut = OpenOutputBitFile(filenameOut);
	unsigned char byteBuff[1];
	while(fread(byteBuff, sizeof(unsigned char),1,fileIn)){
		OutputBits(fileOut, codeTable[byteBuff[0]].first, codeTable[byteBuff[0]].second);
	}	
	OutputBits(fileOut, codeTable[256].first, codeTable[256].second); // encode the EOF
    CloseOutputBitFile(fileOut);
	cout << "Shannon Fano encoding " << filenameIn << " is done and the output file is " << filenameOut << endl;	
} 
	
void FanoNode::decodeFile(FanoNode *rootp, const char *filenameIn, const char *filenameOut){
	/*
		The function to decoded raw data, filenameIn,  as filenameOut.
		Decoding need the root points of Fano tree (root point = whole tree). 
		Walk from root to leaf and the symbol is at leaf point, and keep repeat it agian and agian
	*/

	int bit;
    FanoNode *crusor = rootp;
    BIT_FILE *fileIn = OpenInputBitFile(filenameIn);
    FILE *fileOut = fopen(filenameOut, "wb");
	while(1){
		bit = InputBit(fileIn);
		if(bit==2){
			cerr << "the encoded code didn't have encoded EOF" << endl;
			exit(1); 
		} 
		if(bit==0) crusor = crusor->leftNode;
		else crusor = crusor->rightNode;
		if((crusor->list).size()==1){
			if(crusor->list[0].first == 256) break; // read the encoded EOF
			putc(crusor->list[0].first, fileOut);
			crusor = rootp;
		}
	}
	cout << "Shannon Fano decoding " << filenameIn << " is done and the output file is " << filenameOut << endl;
}

void FanoNode::encodePic(unsigned char*** pic, int sizeY, int sizeX, int sizeZ, const char *filenameOut){
	/*
		The function to encode raw data, filenameIn,  as filenameOut.
		Encoding simply use the look-up table (codeTable).
	*/
	BIT_FILE *fileOut = OpenOutputBitFile(filenameOut);
	for(int y=0;y<sizeY;y++){
		for(int x=0;x<sizeX;x++){
			for(int z=0;z<sizeZ;z++){
				OutputBits(fileOut, codeTable[pic[y][x][z]].first, codeTable[pic[y][x][z]].second);
			}
		}
	}
	OutputBits(fileOut, codeTable[256].first, codeTable[256].second); // encode the EOF
    CloseOutputBitFile(fileOut);
	cout << "Shannon Fano encoding picture is done and the output file is " << filenameOut << endl;	
} 

void FanoNode::outputTree(const char* filenameOut){
	FanoNode* ptr = this;
	ofstream fileOut(filenameOut);
	string buffString;
	if(fileOut.fail()){
		cerr << "cannot create file " << filenameOut << endl;
		exit(1);
	}
	stack<FanoNode*> theStack;
	
	while(1){
		if(ptr==NULL){
			fileOut << "NULL,";
			if(theStack.empty()){
				break;
			}
			else{
				ptr = theStack.top();
				theStack.pop();
			}
		}
		else {
			theStack.push(ptr->rightNode);
			if(ptr->list.size()==1){
				fileOut << ptr->list[0].first << ",";
			}
			else{
				fileOut << "-1,"; 
			}
			ptr = ptr->leftNode;
		}
	}
	fileOut.close();
}

FanoNode* FanoNode::buildTreeCSV(const char* filenameIn){
	ifstream fileIn(filenameIn);
	if (fileIn.fail()) {
		cerr << "cannot open file " << filenameIn;
		exit(1);
	}
	string buffStr;
	FanoNode *root = NULL;
	FanoNode *ptr = root;
	vector<pair<int, float> > empty;
	stack<FanoNode*> FanoPtrStack;
	getline(fileIn, buffStr, ',');

	if (buffStr == "NULL") {
		return NULL;
	}
	else {
		ptr = new FanoNode(empty, 0, 0);
		root = ptr;
		int buffInt = stoi(buffStr);
		if (buffInt > 256 || buffInt < -1) {
			cerr << "node value is over 256 or smaller than -1" << endl;
			exit(1);
		}
		else if (buffInt != -1) {
			ptr->list.push_back(make_pair(buffInt, 0));
		}
		FanoPtrStack.push(ptr);
	}

	bool leftOrRight = 0;
	while (fileIn.good()){
		getline(fileIn, buffStr, ',');
		if (leftOrRight == 0) {
			if (buffStr == "NULL") {
				leftOrRight = 1;
				if (FanoPtrStack.empty()) {
					break;
				}
				else {
					ptr = FanoPtrStack.top();
					FanoPtrStack.pop();
				}
			}
			else {
				int buffInt = stoi(buffStr);
				ptr->leftNode = new FanoNode(empty, 0, 0);
				ptr = ptr->leftNode;
				if (buffInt > 256 || buffInt < -1) {
					cerr << "node value is over 256 or smaller than -1" << endl;
					exit(1);
				}
				else if (buffInt != -1) {
					ptr->list.push_back(make_pair(buffInt, 0));
				}
				FanoPtrStack.push(ptr);
			}
		}
		else {
			if (buffStr == "NULL") {
				if (FanoPtrStack.empty()) {
					break;
				}
				else {
					ptr = FanoPtrStack.top();
					FanoPtrStack.pop();
				}
			}
			else {
				int buffInt = stoi(buffStr);
				ptr->rightNode = new FanoNode(empty, 0, 0);
				ptr = ptr->rightNode;
				if (buffInt > 256 || buffInt < -1) {
					cerr << "node value is over 256 or smaller than -1" << endl;
					exit(1);
				}
				else if (buffInt != -1) {
					ptr->list.push_back(make_pair(buffInt, 0));
				}
				FanoPtrStack.push(ptr);
				leftOrRight = 0;
			}
		}
	}
	return root;
}
void FanoNode::decodeFileToVec(FanoNode* rootp, const char* filenameIn, vector<unsigned char>& outputVec){
	int bit;
	FanoNode *crusor = rootp;
	BIT_FILE *fileIn = OpenInputBitFile(filenameIn);
	while (1) {
		bit = InputBit(fileIn);
		if (bit == 2) {
			cerr << "the encoded code didn't have encoded EOF" << endl;
			exit(1);
		}
		if (bit == 0) crusor = crusor->leftNode;
		else crusor = crusor->rightNode;
		if ((crusor->list).size() == 1) {
			if (crusor->list[0].first == 256) break; // read the encoded EOF
			outputVec.emplace_back(crusor->list[0].first);
			crusor = rootp;
		}
	}
	cout << "Shannon Fano decoding " << filenameIn << " is done " << endl;

}

void FanoNode::printCodeTable() {
	for (int i = 0; i < 257; i++) {
		cout << "codeTable[" << i << "] = (" << codeTable[i].first << "," << codeTable[i].second << endl;
	}
}
pair<int, int>  FanoNode::codeTable[257] = {};
