#ifndef FILTER_H
#define FILTER_H

class Pic;

class Filter{
	
friend class Pic;

private:
	float *** pixel;
	int sizeX;
	int sizeY;
	int sizeZ;
		
public:
	Filter();
	Filter(int SizeY, int SizeX, int SizeZ);
	Filter(string filename, int SizeY, int SizeX, int SizeZ);
	~Filter();
	int getX();
	int getY();
	int getZ();
	float getSum();
	void setPixelValue(int y, int x, int z, float value);
	void free();
	void output(string filename);
	void print();
	static Filter& makeFilter_gauss(int SizeY, int SizeX, int SizeZ, float var);
	
friend float getDistance(Pic &picA, Pic &picB, Filter &kernel, float weightSum);
};

#endif 
