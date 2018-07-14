#include "Point_2D.h"

Point_2D::Point_2D(){
	y = 0;
	x = 0;
}

Point_2D::Point_2D(int y_in, int x_in){
	y = y_in;
	x = x_in;
} 

Point_2D::~Point_2D(){
}

int Point_2D::getY(void){
	return y;
}

int Point_2D::getX(void){
	return x;
}

Point_2D& Point_2D::makePoint(int y_in ,int x_in){
	Point_2D* dummy = new Point_2D(y_in,x_in);
	return *dummy;
}
