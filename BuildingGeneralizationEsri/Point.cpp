#include "stdafx.h"

#include "Point.h"
#include <cmath>

#define Ro 57.29577951308

std::ofstream file4("E:\\pokus2_esri.txt");

Point::Point()
{
	x = 0.0;
	y = 0.0;
	orientation = 0;
	p = NULL;
}


Point::Point(const double x_, const double y_)
{
	x = x_;
	y = y_;
	orientation = 0;
	p = NULL;
}

Point::Point(const double x_, const double y_, Point *p_)
{
	x = x_;
	y = y_;
	orientation = 0;
	p = p_;
}

Point::Point(const double x_, const double y_, const int orientation_)
{
	x = x_;
	y = y_;
	orientation = orientation_;
	p = NULL;
}
Point::Point (const Point &point)
{
 	x=point.x;
	y=point.y;
	orientation=point.orientation;
	p=point.p;
}

bool Point::operator < (const Point &p)
{
	return (y < p.y) || (x > p.x) && (y == p.y);
};

bool Point::operator == (const Point &p)
{
	return ( x == p.x) && (y == p.y);
};

Point Point:: operator = (const Point &pp)
{	
	x = pp.x;
	y = pp.y;
	orientation = pp.orientation;
	p = pp.p;

	return *this;
}

void Point::rotate (const double angle)
{
	//Rotate point by an angle
	const double xr = x * cos(angle/Ro) - y * sin(angle/Ro);
	const double yr = x * sin(angle/Ro) + y * cos(angle/Ro);

	//Assign rotated coordinages
	x = xr;
	y = yr;
}


void Point::print(int recursion_depth)
{
	//Print information about the point
	int orientation2;
	if (recursion_depth>1)
	{
		orientation2 = (orientation % 4 - (recursion_depth - 1) %4);
		if (orientation2 <= 0) orientation2 += 4;
	}
	else orientation2 = orientation;
	file4 << std::fixed << std::setprecision(5) << "X= " <<x << " Y=" <<y <<" o=" <<orientation << " o2=" <<orientation2 <<'\n';
}


void Point::print()
{
	file4 << std::fixed << std::setprecision(5) << "X= " << x << " Y=" << y << " o=" << orientation;

	if ( p!= NULL)
		file4 << std::fixed << std::setprecision(5) << " PX=" << p->getX() << " PY=" << p->getY() << '\n';
	else
		file4 << " PX = x" << " PY= x" << '\n';
}

/*
void Point::print()
{
	std::cout << std::fixed << std::setprecision(5) << "X= " << x << " Y=" << y << " o=" << orientation;

	if (p != NULL)
		std::cout << std::fixed << std::setprecision(5) << " PX=" << p->getX() << " PY=" << p->getY() << '\n';
	else
		std::cout << " PX = x" << " PY= x" << '\n';
}
*/

