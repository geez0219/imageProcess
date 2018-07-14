#ifndef PIC_H
#define PIC_H
#include <string>
class Filter;
class Point_2D; 

class Pic {
friend class Filter;	

private:
	unsigned char*** pixel;
	int sizeX;
	int sizeY;
	int sizeZ;
	int colorSpace; // 0: gray; 1:RGB; 2:CMY; 3:HSL
		
public:
	Pic();
	Pic(int, int, int);
	Pic(string, int, int, int);
	~Pic();
	int getX();
	int getY();
	int getZ();
	void setPixelValue(int, int, int, unsigned char);
	int getColorSpace();
	void free();
	void output(string);
	void print();
	void resize(int, int);
	void RGBToCMY();
	void RGBToHSL();
	void CMYToRGB();
	void HSLToRGB();
	Pic& expandEdge(int, int mode=0);
	Pic& crop(int, int, int, int);
	Pic& crop(Point_2D&, int, int);
	void crop(Pic&, int, int, int, int);
	void crop(Pic&, Point_2D&, int, int);
	void histEqualize();
	Pic& NLM(Filter&, int, float);
	unsigned char bilinear(float, float, int);
	Pic& homoTransform(Point_2D&, Point_2D&, Point_2D&, Point_2D&);
	void encodeFano(string filenameOut, string treeFilename = "tree.csv");
	static Pic& decodeFano(string filename, string treeFilename, int SizeY, int SizeX, int SizeZ);
	void encodeHuff(string filenameOut, string treeFilename = "tree.csv");
	static Pic& decodeHuff(string filename, string treeFilename, int SizeY, int SizeX, int SizeZ);
	void encodeAdaptHuff(string filenameOut);
	static Pic& decodeAdaptHuff(string filename, int SizeY, int SizeX, int SizeZ);
friend float getDistance(Pic&, Pic&, Filter&, float);
};

#endif
