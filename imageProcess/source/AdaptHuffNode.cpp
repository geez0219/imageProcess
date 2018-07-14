#include <iostream>
#include <vector> // vector
#include <utility> // pair
#include "bitio.h" 
#include "AdaptHuffNode.h"
#pragma warning(disable : 4996)  
using namespace std;

AdaptHuffNode::AdaptHuffNode(int Symbol, int Weight, int Order, AdaptHuffNode* LeftNode, AdaptHuffNode* RightNode, AdaptHuffNode* UpNode){
	symbol = Symbol; // 0~256: leaf node, 256:EOF, 257: not leaf node, 258: NYT node
	weight = Weight;
	order = Order;
	leftNode = LeftNode;
	rightNode = RightNode;
	upNode = UpNode;
}
	
void AdaptHuffNode::searchNode(AdaptHuffNode *node, int symbol, long code, int codeLen){
	/*
		a recursive function searching the node of certain symbol from root node. 
		This will not return anything but update some static variable.
		The last two augument should always set to zeros when call it.
		arg: node(the root node), 
		     code(the current code value), 
			 codeLen(the current code length)
		update: nodeSearch(the node it search)
				codeSearch(the code it search)
				codeLenSearch (the code length it search)
	*/
	if(node->symbol==symbol){
		nodeSearch = node;
		codeSearch = code;
		codeLenSearch = codeLen;
	}
	
	if(node->leftNode!=NULL){
		searchNode(node->leftNode,symbol,code<<1,codeLen+1);
		searchNode(node->rightNode,symbol,(code<<1)+1,codeLen+1);
	}
}	

pair<long, int> AdaptHuffNode::encode(int codeIn){
	/* 
		the function to encode one symbol. the object to call this function should be the root node.
		return: the int pair [codeValue, codeLength]
	*/
	
	if(showUpTable[codeIn]==0){ // if the symbol show up for first time  
		nodeSearch = NULL;
		codeSearch = 0;
		codeLenSearch = 0;
		searchNode(this, 258, 0, 0); // search for newNode (NYT)
		if(nodeSearch==NULL){
			cout << "cannot find the symbol: 258" << endl;
			system("pause");
			exit(1);
		}
		nodeSearch->leftNode = new AdaptHuffNode(258,0,nodeSearch->order-2,NULL,NULL,nodeSearch); // the left of NYT become new NYT
		nodeSearch->rightNode = new AdaptHuffNode(codeIn,1,nodeSearch->order-1,NULL,NULL,nodeSearch); // the right of NYT become new symbol
		nodeSearch->symbol = 257; // set old NYT become 257(non-leaf node)
		nodeSearch->weight = 1; // set old NYT weight to 1
		showUpTable[codeIn] = 1; // put new symbol in show-up table
		if (nodeSearch->upNode!=NULL){ // if old NYT has upNode (This is not the first time encoding)
			nodeSearch->upNode->weight ++; // update the upNode weight
			nodeSearch->upNode->update(this); // update the upNode position (check swaping)
		}
		
		return make_pair((codeSearch<<9)+codeIn,codeLenSearch+9); // the output code should be NYT position + codeIn(9 bits)
	}
	else{ // if the symbol showed up before 
		nodeSearch = NULL;
		codeSearch = 0;
		codeLenSearch = 0;
		searchNode(this, codeIn, 0, 0); // search the symbol's node
		if(nodeSearch==NULL){
			cout << "cannot find the symbol: " << codeIn << endl;
			system("pause");
			exit(1);
		}
		nodeSearch->weight++; // update the node's weight
		nodeSearch->update(this); // update the upNode position (check swaping)
		return make_pair(codeSearch,codeLenSearch);
	}
}
	
void AdaptHuffNode::printTree(void){
	printf("the symbol %c(%d) has weight: %d order: %d\n",this->symbol, this->symbol, this->weight, this->order);
	if(leftNode!=NULL){
		leftNode->printTree();
		rightNode->printTree();
	}
}
	
void AdaptHuffNode::update(AdaptHuffNode* root){
	/*
		a recursive function to check the object node wether need swap.
		if it does, then swap it. It wiil update all tree until there is no swapping needed.
		arg: root (the root node of tree)
	*/
	
	AdaptHuffNode* nodeBuff;
	int orderBuff;
	if(this == root) return;
	nodeSearch = NULL; // initialize the static varible for searchSwitch
	highestOrder = 0; // initialize the static varible for searchSwitch
		 
	searchSwitch(root, weight, order, upNode); 
	if(nodeSearch!=NULL){ // switch the data points
		// change the order first 
		orderBuff = nodeSearch->order; 
		nodeSearch->order = this->order;
		this->order = orderBuff;
			
		// change the node (the order eventually remain the same)
		if(this->upNode->leftNode == this && nodeSearch->upNode->leftNode == nodeSearch){ 
			this->upNode->leftNode = nodeSearch;
			nodeSearch->upNode->leftNode = this;
		}
		else if(this->upNode->leftNode == this && nodeSearch->upNode->rightNode == nodeSearch){
			this->upNode->leftNode = nodeSearch;
			nodeSearch->upNode->rightNode = this;
		}
		else if(this->upNode->rightNode == this && nodeSearch->upNode->leftNode == nodeSearch){
			this->upNode->rightNode = nodeSearch;
			nodeSearch->upNode->leftNode = this;
		}
		else{
			this->upNode->rightNode = nodeSearch;
			nodeSearch->upNode->rightNode = this;
		}	
		nodeBuff = this->upNode;
		this->upNode = nodeSearch->upNode;
		nodeSearch->upNode = nodeBuff;
	}
	this->upNode->weight++; 
	this->upNode->update(root); // update upward
}

void AdaptHuffNode::searchSwitch(AdaptHuffNode *node, int weight, int order, AdaptHuffNode* exception){
	/*
		A recursive function searching for swaping node.it find the node with highest order that
		1.its weight smaller than input weight and 
		2.its order larger than input order 
		3.can't be the exception node
		
		update: nodeSearch(the node it search)
				highestOrder(so far highest order)
	*/
	
	if(node->weight < weight && node->order > order && node->order > highestOrder){ 
		if (node != exception){ 
			nodeSearch = node;
			highestOrder = node->order;
		}
	}
	if(node->weight >= weight && node->leftNode!=NULL){
		searchSwitch(node->leftNode, weight, order, exception);
		searchSwitch(node->rightNode, weight, order, exception);
	}
}

void AdaptHuffNode::clearShowUpTable(){
	for(int i=0;i<257;i++){
		showUpTable[i] = 0;
	}
}

void AdaptHuffNode::encodeFile(const char* filenameIn, const char* filenameOut){
	FILE *fileIn = fopen(filenameIn,"rb");
	BIT_FILE *fileOut = OpenOutputBitFile(filenameOut);
	unsigned char byteBuff[1];
	AdaptHuffNode* root = new AdaptHuffNode(258,0,514,NULL,NULL,NULL);
	pair<long,int> code_pair; // the pair that [codeValue, codeLength] 
	
	clearShowUpTable();	
	while(fread(byteBuff, sizeof(unsigned char),1,fileIn)){
		code_pair = root->encode(byteBuff[0]);
		OutputBits(fileOut, code_pair.first, code_pair.second);
	}	
	// encode the EOF
	code_pair = root->encode(256); 
	OutputBits(fileOut, code_pair.first, code_pair.second);
	
   	CloseOutputBitFile(fileOut);
	cout << " Adaptive Huffman encoding " << filenameIn << " is done and the output file is " << filenameOut << endl;			
}
	
void AdaptHuffNode::decodeFile(const char* filenameIn, const char* filenameOut){
	int bit;
	int symbolGet; // 0~256
	unsigned char byte; // 0~255
	pair<long,int> code_pair;
	BIT_FILE *fileIn = OpenInputBitFile(filenameIn);
    FILE *fileOut = fopen(filenameOut, "wb");
    AdaptHuffNode* root = new AdaptHuffNode(258,0,514,NULL,NULL,NULL);
    AdaptHuffNode* crusor = root;
    
	clearShowUpTable();
	while(1){
		if(crusor->leftNode==NULL){ // if crusor hit the bottom
			if(crusor->symbol==258){ // hit the NYT
				symbolGet = InputBits(fileIn,9); // read the next 9 bit for first apperance
				if (symbolGet == 256) break; // read EOF
				byte = symbolGet;
			}
			else if(crusor->symbol==257){
				cout << "the bottom Node is 257" << endl;
				system("pause");
				exit(1);
			}
			else {
				byte = crusor->symbol;
			}
			putc(byte, fileOut);
			code_pair = root->encode(byte); // update the Tree
			crusor = root;
		} 

		bit = InputBit(fileIn);
		if (bit == 2) {
			//break;
			cerr << "the encoded code didn't have encoded EOF" << endl;
			system("pause");
			exit(1);
		} 
		if(bit==0) crusor = crusor->leftNode; // 0 for leftNode
		else crusor = crusor->rightNode; // 1 for rightNode
	}
	cout << "Adaptive Huffman decoding " << filenameIn << " is done and the output file is " << filenameOut << endl; 		
}

void AdaptHuffNode::decodeFileToVec(const char* filenameIn, vector<unsigned char>& outputVec) {
	int bit;
	int symbolGet; // 0~256
	unsigned char byte; // 0~255
	pair<long, int> code_pair;
	BIT_FILE *fileIn = OpenInputBitFile(filenameIn);
	AdaptHuffNode* root = new AdaptHuffNode(258, 0, 514, NULL, NULL, NULL);
	AdaptHuffNode* crusor = root;
	clearShowUpTable();
	while (1) {
		if (crusor->leftNode == NULL) { // if crusor hit the bottom
			if (crusor->symbol == 258) { // hit the NYT
				symbolGet = InputBits(fileIn, 9); // read the next 9 bit for first apperance
				if (symbolGet == 256) break; // read EOF
				byte = symbolGet;
			}
			else if (crusor->symbol == 257) {
				cout << "the bottom Node is 257" << endl;
				system("pause");
				exit(1);
			}
			else {
				byte = crusor->symbol;
			}
			outputVec.emplace_back(byte);
			code_pair = root->encode(byte); // update the Tree
			crusor = root;
		}

		bit = InputBit(fileIn);
		if (bit == 2) {
			//break;
			cerr << "the encoded code didn't have encoded EOF" << endl;
			system("pause");
			exit(1);
		}
		if (bit == 0) crusor = crusor->leftNode; // 0 for leftNode
		else crusor = crusor->rightNode; // 1 for rightNode
	}
}

bool AdaptHuffNode::showUpTable[257] = {};
AdaptHuffNode* AdaptHuffNode::nodeSearch = NULL;
long AdaptHuffNode::codeSearch = 0;
int AdaptHuffNode::codeLenSearch = 0;
int AdaptHuffNode::highestOrder = 0;
