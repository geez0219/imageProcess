#ifndef UTILITY_H
#define UTILITY_H
using namespace std;

class Pic;
class Filter;
class Point_2D;

int truncate(int input, int min, int max);
float getDistance(Pic &picA, Pic &picB, Filter &kernel, float weightSum=0);
void rowChange(double *A, int a, int b);
void rowChange2(double **A, int a, int b);
void printMat(double **A, int size);
void printVec(double *A, int size);
void solveLinear(double **A_real, double *x, double *b_real, int size);
double** getTransformMat(Point_2D& Corner1A, Point_2D& Corner1B, Point_2D& Corner2A, \
	Point_2D& Corner2B, Point_2D& Corner3A, Point_2D& Corner3B, Point_2D& Corner4A, Point_2D& Corner4B);
int isInRegion(Point_2D& corner1, Point_2D& corner2, Point_2D& corner3, Point_2D& corner4, double y, double x);
bool comparePairList(const pair<unsigned char, float> &a, const pair<unsigned char, float> &b);
string addRecomFileExtension(const string& filename, const string& recomExt);
string getFileExtension(const string& filename);
#endif
