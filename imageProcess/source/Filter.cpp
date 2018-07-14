#include <iostream>
#include <string>
#include <algorithm>
#include <cmath>
#include "utility.h"
#include "Filter.h"
#pragma warning(disable : 4996) 
using namespace std;

Filter::Filter(){
	sizeX = 0;
	sizeY = 0;
	sizeZ = 0;
	pixel = NULL;
}

Filter::Filter(int SizeY, int SizeX, int SizeZ){
	sizeX = SizeX;
	sizeY = SizeY;
	sizeZ = SizeZ;
	
	pixel = new float**[sizeY];
	for(int y=0;y<sizeY;y++){
		pixel[y] = new float*[sizeX];
		for(int x=0;x<sizeX;x++){
			pixel[y][x] = new float[sizeZ];
			for(int z=0;z<sizeZ;z++){
				pixel[y][x][z] = 0;
			}
		}
	} 
}


Filter::Filter(string filename, int SizeY, int SizeX, int SizeZ){
	sizeX = SizeX;
	sizeY = SizeY;
	sizeZ = SizeZ;
	pixel = new float**[sizeY];
	for(int y=0;y<sizeY;y++){
		pixel[y] = new float*[sizeX];
		for(int x=0;x<sizeX;x++){
			pixel[y][x] = new float[sizeZ];
		}
	} 
	FILE *file;
	if (!(file = fopen(filename.c_str(), "rb"))) {
		cout << "cannot open file " << filename << endl;
		exit(1);
	}
	for (int y = 0; y < sizeY; y++) {
		for (int x = 0; x < sizeX; x++) {
			fread(pixel[y][x], sizeof(float), sizeZ, file);
		}
	}
	
}

Filter::~Filter(){
	for(int y=0;y<sizeY;y++){
		for(int x=0;x<sizeX;x++){
			delete [] pixel[y][x];
		}
		delete [] pixel[y];
	}
	delete [] pixel;
}

int Filter::getX(){
	return sizeX;
}

int Filter::getY(){
	return sizeY;
}

int Filter::getZ(){
	return sizeZ;
}

float Filter::getSum(){
	float sum = 0;
	for(int y=0;y<sizeY;y++){
		for(int x=0;x<sizeX;x++){
			for(int z=0;z<sizeZ;z++){
				sum += pixel[y][x][z]; 
			}
		}
	}
	return sum;
}

void Filter::setPixelValue(int y, int x, int z, float value){
	pixel[y][x][z] = value;
} 

void Filter::free(){
	for(int y=0;y<sizeY;y++){
		for(int x=0;x<sizeX;x++){
			delete [] pixel[y][x];
		}
		delete [] pixel[y];
	}
	delete [] pixel;	
}

void Filter::output(string filename){
	FILE* file;
	if(!(file = fopen(filename.c_str(),"wb"))){
		cout << "annot open file " << filename << endl;
		exit(1);
	}
	for(int y=0;y<sizeY;y++){
		for(int x=0;x<sizeX;x++){
			fwrite(pixel[y][x], sizeof(float), sizeZ, file);
		}
	}
}

void Filter::print(){
	for(int z=0;z<sizeZ;z++){
		cout << "in channel: " << z << endl;
		for(int y=0;y<sizeY;y++){
			for(int x=0;x<sizeX;x++){
				cout << float(pixel[y][x][z]) << " ";
			}
			cout << endl;
		} 			
	}
}

Filter& Filter::makeFilter_gauss(int SizeY, int SizeX, int SizeZ, float var){
	if(SizeX%2==0 || SizeY%2==0){
		cout << "In makeFilter_guass: the SizeX and SizeY should be odd"  << endl;
		exit(1);
	} 
	
	Filter* output = new Filter(SizeY,SizeX,SizeZ);
	
	for(int y=0;y<SizeY;y++){ 
		for(int x=0;x<SizeX;x++){
			float X = x-(SizeX-1)/2;
			float Y = y-(SizeY-1)/2; 
			float value = 1/(sqrt(2*3.14)*var)*exp((X*X+Y*Y)/(-2.0*var*var));
			for(int z=0;z<SizeZ;z++){
				output->pixel[y][x][z] = value;
			}
		}
	}
	return *output;
}


