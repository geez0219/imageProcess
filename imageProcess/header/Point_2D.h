#ifndef POINT_H
#define POINT_H

class Point_2D {
	private:
		int y;
		int x;
		
	public:
		Point_2D();
		Point_2D(int y_in, int x_in);
		~Point_2D();
		int getY(void);
		int getX(void);
		static Point_2D& makePoint(int y_in, int x_in);
};
 
#endif
