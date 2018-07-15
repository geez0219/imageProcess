#include <iostream>
#include <string>
#include <algorithm>
#include "utility.h"
#include "PIC.h"
#include "FILTER.h" 
#include "POINT_2D.h"
#include "FanoNode.h"
#include "bitio.h"
#include "HuffNode.h"
#include "AdaptHuffNode.h"

#pragma warning(disable : 4996)  

using namespace std; 

vector<pair<int, float> > getSymbolProb(const char* filename){
	/* arg: input filename
	   return: a vector of pair where [symbol, probability] (the symbol in the vector will at least show up once)
	*/
	FILE *fileIn=fopen(filename,"rb");
	int table[257] = {}; // the value is the number of that symbol, the index is the symbols ASCII code
	unsigned char byteBuff[1];
	
	while(fread(byteBuff, sizeof(unsigned char),1,fileIn)){
		table[byteBuff[0]] += 1;
	}
	table[256] += 1; // add the EOF to table 
	fclose(fileIn);
	
	// process the statistic data  
	vector <pair <int, float> > pairList; 
	float charSum=0;
	for(int i=0;i<257;i++){
		if(table[i]!=0){ // delete 0 occurance symbol
			charSum += table[i];
			pairList.push_back(make_pair(i,table[i]));
		}
	}
	sort(pairList.begin(),pairList.end(),comparePairList); // sorted according to occurence	
	return pairList;
} 

int main(void){

	// Huffman encoding and decoding 
	//AdaptHuffNode::encodeFile("text.dat", "testEncode.txt");
	//AdaptHuffNode::decodeFile("testEncode.txt", "testDecode.dat");


	////////////////////////////////////////////////////////////////////////////
	//string fileNameIn;
	//int sizeX;
	//int sizeY;
	//int sizeZ;
	//int newSizeX;
	//int newSizeY;
	//cout << "please enter the filename: ";
	//cin >> fileNameIn;
	//cout << "please enter the sizeX: ";
	//cin >> sizeX;
	//cout << "please enter the sizeY: ";
	//cin >> sizeY;
	//cout << "please enter the sizeZ: ";
	//cin >> sizeZ;
	//Pic a(fileNameIn,sizeY,sizeX,sizeZ);
	//a.encodeAdaptHuff("testEncode.txt");
	//Pic aDecode = Pic::decodeAdaptHuff("testEncode.txt", sizeY, sizeX, sizeZ);
	//aDecode.output("afterDecode.raw");

	//Pic b(fileNameIn, sizeY, sizeX, sizeZ);
	//b.encodeHuff("testEncodeB.txt", "treeB.csv");
	//Pic bDecode = Pic::decodeHuff("testEncodeB.txt", "treeB.csv", sizeY, sizeX, sizeZ);
	//bDecode.output("afterDecodeB.raw");

	//Pic c(fileNameIn, sizeY, sizeX, sizeZ);
	//c.encodeFano("testEncodeC.txt", "treeC.csv");
	//Pic cDecode = Pic::decodeFano("testEncodeC.txt", "treeC.csv", sizeY, sizeX, sizeZ);
	//cDecode.output("afterDecodeC.raw");
	//Pic a(fileNameIn, sizeY, sizeX, sizeZ);
	//a.encodeHuff("testEncode.txt", "tree.csv");
	//Pic aDecode = Pic::decodeHuff("testEncode.txt", "tree.csv", sizeY, sizeX, sizeZ);
	//aDecode.output("afterDecode.raw");

//	POINT_2D corner1(0,0);
//	POINT_2D corner2(300,100);
//	POINT_2D corner3(400,500);
//	POINT_2D corner4(0,400);
//	PIC b = a.homoTransform(corner1,corner2,corner3,corner4);
//	
//	b.output(fileNameOut);
//	a.encode(fileNameOut2);

//	FILTER b = FILTER::makeFilter_gauss(3,3,3,1);
//	
//	PIC denoised = a.NLM(b,5,5);
//
//	denoised.output(fileNameOut);

//	POINT_2D a(1,5);
//	cout << a.getY() << " " << a.getX() << endl; 
	system("pause");
	return 0;
}

