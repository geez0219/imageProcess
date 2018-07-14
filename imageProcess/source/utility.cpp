#include <iostream>
#include <cmath>
#include "utility.h"
#include "Pic.h"
#include "Filter.h"
#include "Point_2D.h"

using namespace std;

int truncate(int input, int min, int max){
	if(input < min) return min;
	if(input > max) return max;
	return input;
}

float getDistance(Pic &picA, Pic &picB, Filter &kernel, float weightSum){
	if(picA.sizeX != picB.sizeX || picA.sizeY != picB.sizeY || picA.sizeZ != picB.sizeZ){
		cout << "getDistance: the two input image should have same size" << endl;
		exit(1);
	}
	if(picA.sizeX != kernel.sizeX || picA.sizeY != kernel.sizeY || picA.sizeZ != kernel.sizeZ){
		cout << "getDistance: the kernel size should become the same as input image" << endl;
		exit(1);
	}
	
	if (weightSum==0){ // default value of weightSum
		weightSum = kernel.getSum();
	}
	
	float sum = 0;
	for(int y=0;y<picA.sizeY;y++){
		for(int x=0;x<picA.sizeX;x++){
			for(int z=0;z<picA.sizeZ;z++){
				sum += pow(float(picA.pixel[y][x][z]) - float(picB.pixel[y][x][z]),2) * kernel.pixel[y][x][z];
			}
		}	
	}
	
	return sum / weightSum;
}


void rowChange(double *A, int a, int b){ // change row for vector
	double c;
	c = A[a];
	A[a] = A[b];
	A[b] = c;
}

void rowChange2(double **A, int a, int b){ // change row for matrix
	double *c;
	c = A[a];
	A[a] = A[b];
	A[b] = c;
}

void printMat(double **A, int size){ // print the matrix
	for(int i=0;i<size;i++){
		for(int j=0;j<size;j++){
			cout << A[i][j] << " ";
		}
		cout  << endl;
	}
}

void printVec(double *A, int size){ // print the vector
	for(int i=0;i<size;i++){
		cout << A[i] << " ";
	}
	cout  << endl;
}

void solveLinear(double **A_real, double *x, double *b_real, int size){ // solve Ax=b 
	// copy a same array for A and b
	double **A = new double *[size];
	double *b = new double[size];
	double y;
	double alpha = 0.5;
	for(int i=0;i<size;i++){
		A[i] = new double[size];
		b[i] = b_real[i];
		for(int j=0;j<size;j++){
			A[i][j] = A_real[i][j];
		}
	}

	// gaussian elimination
	for(int i=0;i<size-1;i++){
		if(A[i][i]==0){ // diagonal entry can't be zero
			for(int k=i+1;k<size;k++){
				if(A[k][i]!=0){
					rowChange2(A,i,k);
					rowChange(b,i,k);
					break;
				}
				else if(k==size-1){
					cout << "the matrix is nonsingular" <<endl;
					cout << "y= " << i << "k= " << k;
					exit(1);
				}
			}
		}
		for(int j=i+1;j<=size-1;j++){
			y = A[j][i]/A[i][i];
			
			for(int k=i;k<=size-1;k++){
				A[j][k] -= y*A[i][k];
			}
			b[j] -= y*b[i];
		}
	}
	
	// backward substitutin
	for(int i=size-1;i>=0;i--){
		for(int j=i+1;j<=size-1;j++){
			b[i] -= A[i][j]*x[j];
		}
		x[i] = b[i]/A[i][i];
	}
	
	delete[] A;
	delete[] b;
}

double** getTransformMat(Point_2D& Corner1A, Point_2D& Corner1B, Point_2D& Corner2A, Point_2D& Corner2B,\
			 Point_2D& Corner3A, Point_2D& Corner3B, Point_2D& Corner4A, Point_2D& Corner4B){
			 	
	double **A = new double *[8]; // Ah = b 
	double *h = new double[8]; // the transformation matrix in vector form
	double *b = new double[8]; 
	double **output = new double*[3];
	
	for(int i=0;i<8;i++){
		A[i] = new double[8];
	}
	for(int i=0;i<3;i++){
		output[i] = new double[3];
	}

	double corner[4][2]; // the corner of trojan 
	double corner_new[4][2]; // the corner of field
	
	// set the corner position
	corner[0][0] = Corner1A.getY();
	corner[0][1] = Corner1A.getX();
	corner[1][0] = Corner2A.getY();
	corner[1][1] = Corner2A.getX();
	corner[2][0] = Corner3A.getY();
	corner[2][1] = Corner3A.getX();
	corner[3][0] = Corner4A.getY();
	corner[3][1] = Corner4A.getX();
	corner_new[0][0] = Corner1B.getY();
	corner_new[0][1] = Corner1B.getX();
	corner_new[1][0] = Corner2B.getY();
	corner_new[1][1] = Corner2B.getX();
	corner_new[2][0] = Corner3B.getY();
	corner_new[2][1] = Corner3B.getX();
	corner_new[3][0] = Corner4B.getY();
	corner_new[3][1] = Corner4B.getX();
	
	//  set the matrix
	for(int i=0;i<4;i++){
		A[2*i][0] = -1*corner[i][1];
		A[2*i][1] = -1*corner[i][0];
		A[2*i][2] = -1;
		A[2*i][3] = 0;
		A[2*i][4] = 0;
		A[2*i][5] = 0;
		A[2*i][6] = corner_new[i][1]*corner[i][1];
		A[2*i][7] = corner_new[i][1]*corner[i][0];
		A[2*i+1][0] = 0;
		A[2*i+1][1] = 0;
		A[2*i+1][2] = 0;
		A[2*i+1][3] = -1*corner[i][1];
		A[2*i+1][4] = -1*corner[i][0];
		A[2*i+1][5] = -1;
		A[2*i+1][6] = corner_new[i][0]*corner[i][1];
		A[2*i+1][7] = corner_new[i][0]*corner[i][0];
		b[2*i] = -1*corner_new[i][1];
		b[2*i+1] = -1*corner_new[i][0];
	}
 	solveLinear(A,h,b,8); // get the h parameter 
 	int index = 0;
 	for(int y=0;y<3;y++){
 		for(int x=0;x<3;x++){
 			if(index>=8) break;
 			output[y][x] = h[index++];
		 }
	 }
	 output[2][2] = 1;
	 return output;
}

int isInRegion(Point_2D& corner1, Point_2D& corner2, Point_2D& corner3, Point_2D& corner4, double y, double x){ // check whether the point is inside the area
	double corner[4][2];
	double c2c[4][2];
	double x2c[4][2];
	corner[0][0] = corner1.getY();
	corner[1][0] = corner2.getY();
	corner[2][0] = corner3.getY();
	corner[3][0] = corner4.getY();
	corner[0][1] = corner1.getX();
	corner[1][1] = corner2.getX();
	corner[2][1] = corner3.getX();
	corner[3][1] = corner4.getX();

	for(int i=0;i<4;i++){
		if(i<3){
			c2c[i][0] = corner[i][1]-corner[i+1][1];
			c2c[i][1] = corner[i+1][0]-corner[i][0];
		}
		else{
			c2c[i][0] = corner[i][1]-corner[0][1];
			c2c[i][1] = corner[0][0]-corner[i][0];
		}
		x2c[i][0] = y-corner[i][0];
		x2c[i][1] = x-corner[i][1];
	}
	
	for(int i=0;i<4;i++){
		if(c2c[i][0]*x2c[i][0]+c2c[i][1]*x2c[i][1]>0) break;
		if(i==3) return 1;
	}
	for(int i=0;i<4;i++){
		if(c2c[i][0]*x2c[i][0]+c2c[i][1]*x2c[i][1]<0) break;
		if(i==3) return 1;
	}
	return 0;
} 

bool comparePairList(const pair<unsigned char,float> &a,const pair<unsigned char,float> &b)
{
       return a.second > b.second;
}

