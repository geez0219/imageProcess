#include <iostream>
#include <string>
#include <algorithm>
#include <cmath>
#include <vector>
#include "utility.h"
#include "Pic.h"
#include "Filter.h" 
#include "Point_2D.h"
#include "bitio.h"
#include "FanoNode.h"
#include "HuffNode.h"
#include "AdaptHuffNode.h"

#pragma warning(disable : 4996)  
using namespace std;

Pic::Pic(){
	sizeX = 0;
	sizeY = 0;
	sizeZ = 0;
	pixel = NULL;
}

Pic::Pic(int SizeY, int SizeX, int SizeZ){
	sizeX = SizeX;
	sizeY = SizeY;
	sizeZ = SizeZ;
	colorSpace = (SizeZ == 1)? 0:1;
	
	pixel = new unsigned char**[sizeY];
	for(int y=0;y<sizeY;y++){
		pixel[y] = new unsigned char*[sizeX];
		for(int x=0;x<sizeX;x++){
			pixel[y][x] = new unsigned char[sizeZ];
			for(int z=0;z<sizeZ;z++){
				pixel[y][x][z] = 0;
			}
		}
	} 
}


Pic::Pic(string filename, int SizeY, int SizeX, int SizeZ){
	sizeX = SizeX;
	sizeY = SizeY;
	sizeZ = SizeZ;
	colorSpace = (SizeZ == 1)? 0:1;
	pixel = new unsigned char**[sizeY];
	for(int y=0;y<sizeY;y++){
		pixel[y] = new unsigned char*[sizeX];
		for(int x=0;x<sizeX;x++){
			pixel[y][x] = new unsigned char[sizeZ];
		}
	} 
	FILE *file;
	if (!(file = fopen(filename.c_str(), "rb"))) {
		cout << "cannot open file " << filename << endl;
		exit(1);
	}
	for (int y = 0; y < sizeY; y++) {
		for (int x = 0; x < sizeX; x++) {
			fread(pixel[y][x], sizeof(unsigned char), sizeZ, file);
		}
	}
}

Pic::~Pic(){
	for(int y=0;y<sizeY;y++){
		for(int x=0;x<sizeX;x++){
			delete [] pixel[y][x];
		}
		delete [] pixel[y];
	}
	delete [] pixel;
}

int Pic::getX(){
	return sizeX;
}

int Pic::getY(){
	return sizeY;
}

int Pic::getZ(){
	return sizeZ;
}

void Pic::setPixelValue(int y, int x, int z, unsigned char value){
	pixel[y][x][z] = value;
} 


int Pic::getColorSpace(){
	return colorSpace;
}

void Pic::free(){
	for(int y=0;y<sizeY;y++){
		for(int x=0;x<sizeX;x++){
			delete [] pixel[y][x];
		}
		delete [] pixel[y];
	}
	delete [] pixel;	
}

void Pic::output(string filename){
	FILE* file;
	if(!(file = fopen(filename.c_str(),"wb"))){
		cout << "annot open file " << filename << endl;
		exit(1);
	}
	for(int y=0;y<sizeY;y++){
		for(int x=0;x<sizeX;x++){
			fwrite(pixel[y][x], sizeof(unsigned char), sizeZ, file);
		}
	}
}

void Pic::print(){
	for(int z=0;z<sizeZ;z++){
		cout << "in channel: " << z << endl;
		for(int y=0;y<sizeY;y++){
			for(int x=0;x<sizeX;x++){
				cout << int(pixel[y][x][z]) << " ";
			}
			cout << endl;
		} 			
	}
}


void Pic::resize(int newX, int newY){
	unsigned char*** newImage = new unsigned char**[newY];
	for(int y=0;y<newY;y++){
		newImage[y] = new unsigned char*[newX];
		for(int x=0;x<newX;x++){
			newImage[y][x] = new unsigned char[sizeZ];
		}
	}
	
	float respondingX, respondingY;
	int respondingCeilX, respondingCeilY;
	for (int z=0;z<sizeZ;z++) {
		for (int y=0;y<newY;y++) {
			respondingY = float(y) / (newY-1) * (sizeY-1);
			respondingCeilY = (respondingY == 0)? 1 : int(ceil(respondingY));
			for (int x = 0; x<newX; x++) {
				respondingX = float(x) / (newX-1) * (sizeX-1);
				respondingCeilX = (respondingX == 0) ? 1: int(ceil(respondingX));

			//	cout << "respondingX respondingY respondingCeilX respondingCeilY" << respondingX << " " << respondingY << " " << respondingCeilX << " " << respondingCeilY << endl;
				newImage[y][x][z] = pixel[respondingCeilY][respondingCeilX][z] * (respondingY - respondingCeilY + 1) * (respondingX - respondingCeilX + 1) + 
					pixel[respondingCeilY][respondingCeilX - 1][z] * (respondingY - respondingCeilY + 1) * (respondingCeilX - respondingX) +
					pixel[respondingCeilY - 1][respondingCeilX][z] * (respondingCeilY - respondingY) * (respondingX - respondingCeilX + 1) +
					pixel[respondingCeilY - 1][respondingCeilX - 1][z] * (respondingCeilY - respondingY) * (respondingCeilX - respondingX);
			}
		}
	}
	free();
	sizeX = newX;
	sizeY = newY;
	pixel = newImage;
}

void Pic::RGBToCMY(){
	if(colorSpace!=1){
		cout << "PIC::RGBToHSL: " << "the picture's color space is not RGB " << endl;
		exit(1); 
	}
	for(int y=0;y<sizeY;y++){
		for(int x=0;x<sizeX;x++){
			for(int z=0;z<sizeZ;z++){
				pixel[y][x][z] = 255 - pixel[y][x][z];
			}		
		}
	}
	colorSpace = 2;
}

void Pic::CMYToRGB(){
	if(colorSpace!=2){
		cout << "PIC::CMYToRGB: " << "the picture's color space is not CMY " << endl;
		exit(1); 
	}
	for(int y=0;y<sizeY;y++){
		for(int x=0;x<sizeX;x++){
			for(int z=0;z<sizeZ;z++){
				pixel[y][x][z] = 255 - pixel[y][x][z];
			}		
		}
	}
	colorSpace = 1;
}

void Pic::RGBToHSL(){
	if(colorSpace!=1){
		cout << "PIC::RGBToHSL: " << "the picture's color space is not RGB " << endl;
		exit(1); 
	}
	for(int y=0;y<sizeY;y++){
		for(int x=0;x<sizeX;x++){
			float R = pixel[y][x][0] / 255.0;
			float G = pixel[y][x][1] / 255.0;
			float B = pixel[y][x][2] / 255.0;
			float M = max(max(R, G), B);
			float m = min(min(R, G), B);
			float C = M - m;
			float L = (M + m) / 2.0;
			float H, S;

			if (C == 0) {
				H = 0;
			}
			else if (M == R) {
				H = fmod((G - B) / C, 6) * 60;
			}
			else if (M == G) {
				H = ((B - R) / C + 2) * 60;
			}
			else {
				H = ((R - G) / C + 4) * 60;
			}
		
			if (L == 0) {
				S = 0;
			}
			else if (L<0.5) {
				S = C / 2 / L;
			}
			else {
				S = C / (2 - 2 * L);
			}
			pixel[y][x][0] = round(H / 360 * 255);
			pixel[y][x][1] = round(S * 255);
			pixel[y][x][2] = round(L * 255);			
		}
	}
	colorSpace = 3;
}

void Pic::HSLToRGB(){
	if(colorSpace!=3){
		cout << "PIC::HSLToRGB: " << "the picture's color space is not HSL " << endl;
		exit(1); 
	}
	
	for(int y=0;y<sizeY;y++){
		for(int x=0;x<sizeX;x++){
			float H = pixel[y][x][0]/255.0*360; // 0-255 to 0-360
			float S = pixel[y][x][1]/255.0;// 0-255 to 0-1
			float L = pixel[y][x][2]/255.0;// 0-255 to 0-1
			float C = (1-fabs(2*L-1))*S;
			float X = C*(1-fabs(fmod((H/60),2)-1));
			float m = L-C/2;
			
			if(H<60){
				pixel[y][x][0] = (C+m)*255;
				pixel[y][x][1] = (X+m)*255;
				pixel[y][x][2] = m*255;
			}
			else if(H<120){
				pixel[y][x][0] = (X+m)*255;
				pixel[y][x][1] = (C+m)*255;
				pixel[y][x][2] = m*255;
			}
			else if(H<180){
				pixel[y][x][0] = m*255;
				pixel[y][x][1] = (C+m)*255;
				pixel[y][x][2] = (X+m)*255;
			}
			else if(H<240){
				pixel[y][x][0] = m*255;
				pixel[y][x][1] = (X+m)*255;
				pixel[y][x][2] = (C+m)*255;
			}
			else if(H<300){
				pixel[y][x][0] = (X+m)*255;
				pixel[y][x][1] = m*255;
				pixel[y][x][2] = (C+m)*255;
			}
			else {
				pixel[y][x][0] = (C+m)*255;
				pixel[y][x][1] = m*255;
				pixel[y][x][2] = (X+m)*255;
			}
		}
	}
	colorSpace = 1;
}


void Pic::histEqualize(){
	float hist[3][256] = {};
	if(sizeZ==1){
		for(int y=0;y<sizeY;y++){  // complete histogram and get maximum and minimum
			for(int x=0;x<sizeX;x++){
				hist[0][pixel[y][x][0]] += 1;
			}
		}	
		// change for histogram to CDF
		for(int i=0;i<256;i++){
			if(i==0) hist[0][i] =  hist[0][i];
			else hist[0][i] = hist[0][i-1] + hist[0][i]; 
		}				
		// complete the CDF to 0~255 mapping
		for(int i=0;i<256;i++){
			hist[0][i] = round(double(hist[0][i])/(sizeX*sizeY)*255); 
		}					 
		// cdf equaliztion
		for(int y=0;y<sizeY;y++){
			for(int x=0;x<sizeX;x++){
				pixel[y][x][0] = hist[0][pixel[y][x][0]];  
			}
		}		
	}
	else if(sizeZ==3){
		this->RGBToHSL();
		for(int z=1;z<sizeZ;z++){ // only on the second and third channel
			for(int y=0;y<sizeY;y++){  // complete histogram and get maximum and minimum
				for(int x=0;x<sizeX;x++){
					hist[z][pixel[y][x][z]] += 1;
				}
			}	
			// change for histogram to CDF
			for(int i=0;i<256;i++){
				if(i==0) hist[z][i] =  hist[z][i];
				else hist[z][i] = hist[z][i-1] + hist[z][i]; 
			}				
			// complete the CDF to 0~255 mapping
			for(int i=0;i<256;i++){
				hist[z][i] = round(double(hist[z][i])/(sizeX*sizeY)*255); 
			}					 
			// cdf equaliztion
			for(int y=0;y<sizeY;y++){
				for(int x=0;x<sizeX;x++){
					pixel[y][x][z] = hist[z][pixel[y][x][z]];  
				}
			}	
		}
		this->HSLToRGB();
	}
	else{
		cout << "the image channel can't be two " << endl;
		exit(1);
	}
}

Pic& Pic::expandEdge(int pixel_num, int mode){ 
	Pic* newPic = new Pic(sizeY+pixel_num*2,sizeX+pixel_num*2,sizeZ);
	int x_trunc;
	int y_trunc;
	if(mode==0){ // expand the edge with closest pixel 
		for(int y=0;y<sizeY+pixel_num*2;y++){
			y_trunc = truncate(y-pixel_num,0,sizeY-1);
			for(int x=0;x<sizeX+pixel_num*2;x++){
				x_trunc = truncate(x-pixel_num,0,sizeX-1);
				for(int z=0;z<sizeZ;z++){
					newPic->pixel[y][x][z] = pixel[y_trunc][x_trunc][z];
				}
			}
		} 
	}
	else if(mode==1){ // zero padding
		for(int y=0;y<sizeY+2;y++){
			for(int x=0;x<sizeX+2;x++){
				for(int z=0;z<sizeZ;z++){
					if(y-pixel_num<0 || y-pixel_num>sizeY-1 || x-pixel_num<0 || x-pixel_num>sizeX-1){
						newPic->pixel[y][x][z] = 0;
					}
					else{
						newPic->pixel[y][x][z] = pixel[y-pixel_num][x-pixel_num][z];
					}
				}
			}
		} 
	}
	
	return *newPic;
}

Pic& Pic::crop(int y_corner, int x_corner, int lengthY, int lengthX){
	int lengthZ = sizeZ;
	
	Pic* newPIC = new Pic(lengthY,lengthX,lengthZ);
	for(int y=0;y<lengthY;y++){
		for(int x=0;x<lengthX;x++){
			for(int z=0;z<lengthZ;z++){
				newPIC->pixel[y][x][z] = pixel[y_corner+y][x_corner+x][z];
			}
		}
	}
	
	return *newPIC;
}

Pic& Pic::crop(Point_2D& pt, int lengthY, int lengthX){
	int lengthZ = sizeZ;
	int y_corner = pt.getY();
	int x_corner = pt.getX();
	
	Pic* newPIC = new Pic(lengthY,lengthX,lengthZ);
	for(int y=0;y<lengthY;y++){
		for(int x=0;x<lengthX;x++){
			for(int z=0;z<lengthZ;z++){
				newPIC->pixel[y][x][z] = pixel[y_corner+y][x_corner+x][z];
			}
		}
	}
	
	return *newPIC;
}

void Pic::crop(Pic& output, int y_corner, int x_corner, int lengthY, int lengthX){
	int lengthZ = sizeZ;
	if(output.sizeX==lengthX && output.sizeY==lengthY && output.sizeZ==lengthZ){
		for(int y=0;y<lengthY;y++){
			for(int x=0;x<lengthX;x++){
				for(int z=0;z<lengthZ;z++){
					output.pixel[y][x][z] = pixel[y_corner+y][x_corner+x][z];
				}
			}
		}		
	}
	else{
		Pic* newPIC = new Pic(lengthY,lengthX,lengthZ);
		for(int y=0;y<lengthY;y++){
			for(int x=0;x<lengthX;x++){
				for(int z=0;z<lengthZ;z++){
					newPIC->pixel[y][x][z] = pixel[y_corner+y][x_corner+x][z];
				}
			}
		}
		output.free();
		output = *newPIC;		
	}
}

void Pic::crop(Pic& output, Point_2D& pt, int lengthY, int lengthX){
	int lengthZ = sizeZ;
	int y_corner = pt.getY();
	int x_corner = pt.getX();
	
	if(output.sizeX==lengthX && output.sizeY==lengthY && output.sizeZ==lengthZ){
		for(int y=0;y<lengthY;y++){
			for(int x=0;x<lengthX;x++){
				for(int z=0;z<lengthZ;z++){
					output.pixel[y][x][z] = pixel[y_corner+y][x_corner+x][z];
				}
			}
		}		
	}
	else{
		Pic* newPIC = new Pic(lengthY,lengthX,lengthZ);
		for(int y=0;y<lengthY;y++){
			for(int x=0;x<lengthX;x++){
				for(int z=0;z<lengthZ;z++){
					newPIC->pixel[y][x][z] = pixel[y_corner+y][x_corner+x][z];
				}
			}
		}
		output.free();
		output = *newPIC;		
	}
}


Pic& Pic::NLM(Filter& kernel, int searchSize, float h){
	if(kernel.sizeX != kernel.sizeY){
		cout << "in PIC::NLM: the kernel size should be square" << endl;
		exit(1);
	}
	if(kernel.sizeX%2 != 1){
		cout << "in PIC::NLM: the kernel size of channel x and y should be odd" << endl;
		exit(1);
	}
	
	if(kernel.sizeZ != sizeZ){
		cout << "in PIC::NLM: the kernel and image should share the same sizeZ" << endl;
		exit(1);		
	}
	
 	int sizeXY_K = kernel.sizeX;
 	int sizeZ_K = kernel.sizeZ;
	int sizeXYHalf_K = (sizeXY_K-1)/2;
	
	Pic patchA(sizeXY_K,sizeXY_K,sizeZ_K); 
 	Pic patchB(sizeXY_K,sizeXY_K,sizeZ_K); 
 	Pic PIC_expand = expandEdge(sizeXYHalf_K,0);
 	Pic* output = new Pic(sizeY,sizeX,sizeZ);
 	
 	float data_sum[3];
	
	for(int y1=0;y1<sizeY;y1++){		
		cout << "the process of NLM " << float(y1)/sizeY*100<<"%"<<endl;
		for(int x1=0;x1<sizeX;x1++){
			// initialize 
			float weight_sum = 0; 
			for(int z=0;z<sizeZ;z++){
				data_sum[z] = 0;
			}
			// patch A 
			PIC_expand.crop(patchA,y1,x1,sizeXY_K,sizeXY_K);
			float y2_floor = truncate(y1-searchSize,0,sizeY-1);
			float y2_ceil = truncate(y1+searchSize,0,sizeY-1);
			float x2_floor = truncate(x1-searchSize,0,sizeX-1);
			float x2_ceil = truncate(x1+searchSize,0,sizeX-1);
			
			for(int y2=y2_floor; y2<=y2_ceil; y2++){
				for(int x2=x2_floor; x2<x2_ceil; x2++){
					// patch B 
					PIC_expand.crop(patchB,y2,x2,sizeXY_K,sizeXY_K);
					
					float dist = getDistance(patchA, patchB, kernel); //  calculate the similarity
					float weight = exp(-1*(dist)/(h*h));// get the wieight of the patch2
					weight_sum += weight; 
					for(int z=0;z<sizeZ;z++){
						data_sum[z] += pixel[y2][x2][z]* weight;
					}
				}
			}
			for(int z=0;z<sizeZ;z++){
				output->pixel[y1][x1][z] = data_sum[z] / weight_sum;
			}
		}
	}
	return *output;
}

unsigned char Pic::bilinear(float y, float x, int z){ // do the bilinear interpolation
	int u = floor(x);
	int v = floor(y);	
	if(u<0) u = 0;
	if(v<0) v = 0;
	if(v>=sizeY-1 && u>=sizeX-1){
		return pixel[u][v][z];
	}

	else if(v>=sizeY-1){
		return (v+1-y)*(u+1-x)*pixel[v][u][z] + (v+1-y)*(x-u)*pixel[v][u+1][z];
	}
	else if(u>=sizeX-1){
		return (v+1-y)*(u+1-x)*pixel[v][u][z] + (y-v)*(u+1-x)*pixel[v+1][u][z];
	}
	else {
		return (v+1-y)*(u+1-x)*pixel[v][u][z] + (v+1-y)*(x-u)*pixel[v][u+1][z] + 
	       	   (y-v)*(u+1-x)*pixel[v+1][u][z] + (y-v)*(x-u)*pixel[v+1][u+1][z];
	}
}


Pic& Pic::homoTransform(Point_2D& corner1_new, Point_2D& corner2_new, Point_2D& corner3_new, Point_2D& corner4_new){
	int x_max = max(max(corner1_new.getX(), corner2_new.getX()), max(corner3_new.getX(), corner4_new.getX())); 
	int y_max = max(max(corner1_new.getY(), corner2_new.getY()), max(corner3_new.getY(), corner4_new.getY()));
	Point_2D corner1(0,0);
	Point_2D corner2(sizeY-1,0);
	Point_2D corner3(sizeY-1,sizeX-1);
	Point_2D corner4(0,sizeX-1);
	double** H = getTransformMat(corner1_new, corner1, corner2_new, corner2, corner3_new, corner3, corner4_new, corner4);
	Pic* pic_output = new Pic(y_max, x_max, sizeZ);
	for(int y=0;y<y_max;y++){
		for(int x=0;x<x_max;x++){
			if(isInRegion(corner1_new, corner2_new, corner3_new, corner4_new, y, x)){
				double new_x = (H[0][0]*x + H[0][1]*y + H[0][2]) / (H[2][0]*x + H[2][1]*y + 1);
				double new_y = (H[1][0]*x + H[1][1]*y + H[1][2]) / (H[2][0]*x + H[2][1]*y + 1);
				for(int z=0;z<sizeZ;z++){
					pic_output->pixel[y][x][z] = bilinear(new_y, new_x, z);
				}
			}
			else{
				for(int z=0;z<sizeZ;z++){
					pic_output->pixel[y][x][z] = 0;
				}
			}
		}
	}
	return *pic_output;
}

void Pic::encodeFano(string filenameOut, string treeFilename){
	int buff[257] = {};
	// get the probability 
	for(int y=0;y<sizeY;y++){
		for(int x=0;x<sizeX;x++){
			for(int z=0;z<sizeZ;z++){
				buff[pixel[y][x][z]] ++;
			}
		}
	}
	buff[256] ++; // add EOF
	vector<pair<int,float> > pairList;
	for(int i=0;i<257;i++){
		if(buff[i]!=0){
			pairList.push_back(make_pair(i, buff[i])); // don't need to transform to probability
		}
	}
	sort(pairList.begin(),pairList.end(),comparePairList); // sorted according to occurence	
	FanoNode FanoRoot(pairList,0,0);
	FanoNode::clearCodeTable();
	FanoRoot.treeBuild();
	FanoNode::encodePic(pixel, sizeY, sizeX, sizeZ, filenameOut.c_str());
	FanoRoot.outputTree(treeFilename.c_str());
}

Pic& Pic::decodeFano(string filename, string treeFilename, int SizeY, int SizeX, int SizeZ) {
	FanoNode* tree = FanoNode::buildTreeCSV(treeFilename.c_str());
	Pic* output = new Pic(SizeY, SizeX, SizeZ);
	vector<unsigned char> decodedTxt;
	decodedTxt.reserve(SizeY*SizeX*SizeZ);
	FanoNode::decodeFileToVec(tree, filename.c_str(), decodedTxt);

	int idx = 0;
	int decodedTxtSize = decodedTxt.size();
	for (int y = 0; y < SizeY; y++) {
		for (int x = 0; x < SizeX; x++) {
			for (int z = 0; z < SizeZ; z++) {
				output->pixel[y][x][z] = (idx < decodedTxtSize ? decodedTxt[idx++] : 0);
			}
		}
	}
	cout << "Fano decoding to Pic object is done" << endl;
	return *output;
}

void Pic::encodeHuff(string filenameOut, string treeFilename) {
	int buff[257] = {};
	// get the probability 
	for (int y = 0; y<sizeY; y++) {
		for (int x = 0; x<sizeX; x++) {
			for (int z = 0; z<sizeZ; z++) {
				buff[pixel[y][x][z]] ++;
			}
		}
	}
	buff[256] ++; // add EOF
	vector<HuffNode*> HuffNodeList;
	for (int i = 0; i<257; i++) {
		if (buff[i] != 0) {
			HuffNodeList.push_back(new HuffNode(i, buff[i])); // don't need to transform to probability
		}
	}
	HuffNode *HuffRoot = HuffNode::treeBuild(HuffNodeList);
	HuffNode::clearCodeTable();
	HuffNode::tableBuild(HuffRoot, 0, 0);
	HuffNode::encodePic(pixel, sizeY, sizeX, sizeZ, filenameOut.c_str());
	HuffRoot->outputTree(treeFilename.c_str());
}

Pic& Pic::decodeHuff(string filename, string treeFilename, int SizeY, int SizeX, int SizeZ) {
	HuffNode* tree = HuffNode::buildTreeCSV(treeFilename.c_str());
	Pic* output = new Pic(SizeY, SizeX, SizeZ);
	vector<unsigned char> decodedTxt;
	decodedTxt.reserve(SizeY*SizeX*SizeZ);
	HuffNode::decodeFileToVec(tree, filename.c_str(), decodedTxt);

	int idx = 0;
	int decodedTxtSize = decodedTxt.size();
	for (int y = 0; y < SizeY; y++) {
		for (int x = 0; x < SizeX; x++) {
			for (int z = 0; z < SizeZ; z++) {
				output->pixel[y][x][z] = (idx < decodedTxtSize ? decodedTxt[idx++] : 0);
			}
		}
	}
	cout << "Huffman decoding to Pic object is done" << endl;
	return *output;
}

void Pic::encodeAdaptHuff(string filenameOut) {
	BIT_FILE *fileOut = OpenOutputBitFile(filenameOut.c_str());
	AdaptHuffNode *root = new AdaptHuffNode(258, 0, 514, NULL, NULL, NULL);
	pair<long, int> code_pair; // the pair is [codeValue, codeLength]
	AdaptHuffNode::clearShowUpTable();
	for (int y = 0; y < sizeY;y++){
		for (int x = 0; x < sizeX; x++) {
			for (int z = 0; z < sizeZ; z++) {
				code_pair = root->encode(pixel[y][x][z]);
				OutputBits(fileOut, code_pair.first, code_pair.second);
			}
		}
	}
	// encode the EOF
	code_pair = root->encode(256);
	OutputBits(fileOut, code_pair.first, code_pair.second);

	CloseOutputBitFile(fileOut);
	cout << " Adaptive Huffman encoding is done and the output file is " << filenameOut << endl;
}

Pic& Pic::decodeAdaptHuff(string filename, int SizeY, int SizeX, int SizeZ) {
	Pic *output = new Pic(SizeY, SizeX, SizeZ);
	vector<unsigned char> decodedTxt;
	decodedTxt.reserve(SizeY*SizeX*SizeZ);
	AdaptHuffNode::decodeFileToVec(filename.c_str(), decodedTxt);
	int decodedTxtSize = decodedTxt.size();
	int idx = 0;
	for (int y = 0; y < SizeY; y++) {
		for (int x = 0; x < SizeX; x++) {
			for (int z = 0; z < SizeZ; z++) {
				output->pixel[y][x][z] = (idx < decodedTxtSize ? decodedTxt[idx++] : 0);
			}
		}
	}
	cout << " Adaptive Huffman decoding to Pic object is done" << endl;
	return *output;
}
