#ifndef Point_H
#define Point_H

#include <fstream>

class Point
{
private:
	double x,y;						//Rectangular coordinates
	int orientation;					//Its orientation according to the edges 1 - 4 of the rectangle
	Point *p;						//Pointer a point
public:
	Point();
	Point(const double x_, const double y_);
	Point(const double x_, const double y_, Point *p_);
	Point(const double x_, const double y_, const int orientation_);
	~Point(){};
	Point (const Point &p);
	bool operator < (const Point &p);
	bool operator == (const Point &p);
	Point operator = (const Point &p); 

	double getX() const {return x;}
	double getY() const {return y;}
	int getOrientation() const { return orientation; };
	Point *getP() { return p; }

	void setX(const double x_){x = x_;}
	void setY(const double y_){y = y_;}
	void setOrientation(const int orientation_){orientation = orientation_;}
	void setP(Point *p_){p = p_;}
	void rotate (const double angle);
	void print(const int recursion_depth);
	void print();
	};

#endif