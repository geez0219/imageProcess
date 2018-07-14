#ifndef UTILITY_H
#define UTILITY_H
using namespace std;

class Pic;
class Filter;
class Point_2D;

int truncate(int,int,int);
float getDistance(Pic&, Pic&, Filter&, float weightSum=0);
void rowChange(double*, int, int);
void rowChange2(double**, int, int);
void printMat(double**, int);
void printVec(double*, int);
void solveLinear(double**, double*, double*, int);
double** getTransformMat(Point_2D&, Point_2D&, Point_2D&, Point_2D&, Point_2D&, Point_2D&, Point_2D&, Point_2D&);
int isInRegion(Point_2D&, Point_2D&, Point_2D&, Point_2D&, double, double);
bool comparePairList(const pair<unsigned char,float> &,const pair<unsigned char,float> &);
//FILTER getTransferMatrix()

#endif
