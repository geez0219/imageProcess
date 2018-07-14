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
	Pic(int SizeY, int SizeX, int SizeZ);
	Pic(string filename, int SizeY, int SizeX, int SizeZ);
	~Pic();
	int getX();
	int getY();
	int getZ();
	void setPixelValue(int y, int x, int z, unsigned char value);
	int getColorSpace();
	void free();
	void output(string filename);
	void print();
	void resize(int newX, int newY);
	void RGBToCMY();
	void RGBToHSL();
	void CMYToRGB();
	void HSLToRGB();
	Pic& expandEdge(int pixel_num, int mode=0);
	Pic& crop(int y_corner, int x_corner, int lengthY, int lengthX);
	Pic& crop(Point_2D& pt, int lengthY, int lengthX);
	void crop(Pic& output, int y_corner, int x_corner, int lengthY, int lengthX);
	void crop(Pic& output, Point_2D& pt, int lengthY, int lengthX);
	void histEqualize();
	Pic& NLM(Filter& kernel, int searchSize, float h);
	unsigned char bilinear(float y, float x, int z);
	Pic& homoTransform(Point_2D& corner1_new, Point_2D& corner2_new, Point_2D& corner3_new, Point_2D& corner4_new);
	void encodeFano(string filenameOut, string treeFilename="tree.csv");
	static Pic& decodeFano(string filename, string treeFilename, int SizeY, int SizeX, int SizeZ);
	void encodeHuff(string filenameOut, string treeFilename = "tree.csv");
	static Pic& decodeHuff(string filename, string treeFilename, int SizeY, int SizeX, int SizeZ);
	void encodeAdaptHuff(string filenameOut);
	static Pic& decodeAdaptHuff(string filename, int SizeY, int SizeX, int SizeZ);
friend float getDistance(Pic &picA, Pic &picB, Filter &kernel, float weightSum);
};

#endif
