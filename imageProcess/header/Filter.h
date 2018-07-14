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
	Filter(int, int, int);
	Filter(string, int, int, int);
	~Filter();
	int getX();
	int getY();
	int getZ();
	float getSum();
	void setPixelValue(int, int, int, float);
	void free();
	void output(string);
	void print();
	static Filter& makeFilter_gauss(int, int, int, float);
	
		
friend float getDistance(Pic&, Pic&, Filter&, float);
};

#endif 
