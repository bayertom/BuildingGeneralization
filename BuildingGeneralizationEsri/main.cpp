#include "stdafx.h"
#include "Point.h"
#include "Edge.h"
#include "Algorithms.h"

int main()
{
	double split_ratio_max = 5, angle;
	int split_two_point = 0;
	std::list <Point> points;
	std::list <Edge> res;
	
	points.push_back(Point(0, 0));
	points.push_back(Point(2, 0));
	points.push_back(Point(2, 2));
	points.push_back(Point(4, 2));
	points.push_back(Point(4, -2));
	points.push_back(Point(6, -2));
	points.push_back(Point(6, 2));
	points.push_back(Point(8, 2));
	points.push_back(Point(8, 0));
	points.push_back(Point(10, 0));
	points.push_back(Point(10, 4));
	points.push_back(Point(0, 4));

	Algorithms::simplify(points, split_ratio_max, split_two_point, res, angle);


}
