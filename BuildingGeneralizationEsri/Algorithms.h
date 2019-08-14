#ifndef Algorithms_H
#define Algorithms_H

#include <list>
#include <stack>

#include "Point.h"
#include "Edge.h"

class Algorithms
{
	public:
		static void simplify(std::list<Point> &points, double split_ratio_max, int split_two_point, std::list <Edge> &res, double &angle);
		static void convexHull(std::list <Point> &points, std::list <Point> &c_hull);
		static double distance(const Point &p1, const Point &p2);
		static double angle2Lines(Point &p1, Point &p2, Point &p3, Point &p4);
		static void minimumBoundingRectangle(std::list <Point> &points, Point &mb1, Point &mb2, Point &mb3, Point &mb4, double &min_angle);
		static double wallAverage(std::list <Point> &points);
		static double weightedBisector(std::list <Point> &points);
		static void minMaxBox(std::list <Point> &points, Point &v1, Point &v2, Point &v3, Point &v4);
		static bool get2LinesIntersection(const Point &p1, const Point &p2, const Point &p3, const Point &p4, Point &p_int);
		static void calipers(Point *p1_last, Point *p1, Point *p2, Point *p22, Point *p3, Point *p32, Point *p4, Point *p42);
		static void rotate(std::list <Point> &points, double angle);
		static bool testOrientation(std::list <Point> &points);
		static void drawBuilding(std::list <Edge> &result, double angle, IPointCollectionPtr p_collection);
		static void drawBuildingTemp(std::list <Edge> &result, double angle, IScreenDisplayPtr p_display);
		static void print(std::list <Point> &points);
};

#endif

