#include <iostream>
#include <string>
//#include <algorithm>
//#include "utility.h"
#include "PIC.h"
#include "FILTER.h" 
#include "POINT_2D.h"
//#include "FanoNode.h"
//#include "bitio.h"
//#include "HuffNode.h"
//#include "AdaptHuffNode.h"

#pragma warning(disable : 4996)  

using namespace std; 

//vector<pair<int, float> > getSymbolProb(const char* filename){
//	/* arg: input filename
//	   return: a vector of pair where [symbol, probability] (the symbol in the vector will at least show up once)
//	*/
//	FILE *fileIn=fopen(filename,"rb");
//	int table[257] = {}; // the value is the number of that symbol, the index is the symbols ASCII code
//	unsigned char byteBuff[1];
//	
//	while(fread(byteBuff, sizeof(unsigned char),1,fileIn)){
//		table[byteBuff[0]] += 1;
//	}
//	table[256] += 1; // add the EOF to table 
//	fclose(fileIn);
//	
//	// process the statistic data  
//	vector <pair <int, float> > pairList; 
//	float charSum=0;
//	for(int i=0;i<257;i++){
//		if(table[i]!=0){ // delete 0 occurance symbol
//			charSum += table[i];
//			pairList.push_back(make_pair(i,table[i]));
//		}
//	}
//	sort(pairList.begin(),pairList.end(),comparePairList); // sorted according to occurence	
//	return pairList;
//} 

int main(void){
	string fileNameIn;
	string fileNameOut;
	int sizex;
	int sizey;
	int sizez;
	int newsizex;
	int newsizey;
	cout << "please enter the input filename: ";
	cin >> fileNameIn;
	cout << "please enter the sizex: ";
	cin >> sizex;
	cout << "please enter the sizey: ";
	cin >> sizey;
	cout << "please enter the sizez: ";
	cin >> sizez;
	cout << "please enter the output filename: ";
	cin >> fileNameOut;

	//////// read image from .raw file and save it as .raw /////////
	//Pic a(fileNameIn, sizey, sizex, sizez); // need file name, all dimensional size
	//a.output(fileNameOut);

	//////// file to file Adaptive Huffman encoding and decoding ////////
	//AdaptHuffNode::encodeFile("text.dat", "testEncode.txt");
	//AdaptHuffNode::decodeFile("testEncode.txt", "testDecode.dat");

	/////// save image with Sannon-Fano coding ////////
	//Pic a(fileNameIn, sizey, sizex, sizez);
	//string treeFileName = "tree.csv";
	//a.encodeFano(fileNameOut, treeFileName); 

	/////// decode Sannon-Fano coding to image ////////
	//Pic a;
	//string treeFileName = "tree.csv";
	//a = Pic::decodeFano(fileNameIn, treeFileName, sizey, sizex, sizez);
	//a.output(fileNameOut);

	/////// save image with Huffman coding ////////
	//Pic a(fileNameIn, sizey, sizex, sizez);
	//string treeFileName = "tree.csv";
	//a.encodeHuff(fileNameOut, treeFileName); 

	/////// decode Huffman coding to image ////////
	//Pic a;
	//string treeFileName = "tree.csv";
	//a = Pic::decodeHuff(fileNameIn, treeFileName, sizey, sizex, sizez);
	//a.output(fileNameOut);

	/////// save image with adaptive Huffman coding //////
	//Pic a(fileNameIn, sizey, sizex, sizez);
	//a.encodeAdaptHuff(fileNameOut);
	
	/////// decode daptive Huffman coding to image //////
	//Pic a;
	//a = Pic::decodeAdaptHuff(fileNameIn, sizey, sizex, sizez);
	//a.output(fileNameOut);

	/////// homographic transform /////////
	//Pic a(fileNameIn, sizey, sizex, sizez);
	//Point_2D new_corner1(0, 0); // new top-left corner
	//Point_2D new_corner2(300, 100); // new buttom-left corner
	//Point_2D new_corner3(400, 500); // new buttom-right corner
	//Point_2D new_corner4(0, 400); // new top-right corner
	//// change original corner to new four corner
	//// the result image will be the rectangle that compass the new 4 corner
	//// the background will set to be black
	//Pic b = a.homoTransform(new_corner1, new_corner2, new_corner3, new_corner4);
	//b.output(fileNameOut);

	/////// change colorspace ///////
	//Pic a(fileNameIn, sizey, sizex, sizez);
	//a.RGBToCMY(); // a.RGBToHSL()
	//a.output(fileNameOut); 

	/////// crop image ///////
	//// way#1: return cropped copy
	//Pic a(fileNameIn, sizey, sizex, sizez);
	//int yStart=0, xStart=0, yLength=200, xLength=200;
	//Pic b = a.crop(yStart, xStart, yLength, xLength);
	//b.output(fileNameOut);
	//// way#2 take dst as input argument, return nothing
	//Pic c;
	//a.crop(c, yStart, xStart, yLength, xLength);
	//c.output(fileNameOut);

	/////// constrast_equalize ///////
	//Pic a(fileNameIn, sizey, sizex, sizez);
	//a.histEqualize();
	//a.output(fileNameOut);

	/////// NLM denoise ///////
	//Pic a(fileNameIn, sizey, sizex, sizez);
	//Pic b;
	//Filter kernel = Filter::makeFilter_gauss(3, 3, 3, 0.8);
	//kernel.print();
	//int searchSize = 10;
	//float h = 30;
	//b = a.NLM(kernel, searchSize, h);
	//b.output(fileNameOut);
	
	system("pause");
	return 0;
}

