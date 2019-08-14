#include "stdafx.h"

#include <set>
#include <functional>
#include <algorithm>
#include <iostream>

#include "Algorithms.h"
#include "sortPointsByX.h"
#include "sortPointsByY.h"


#define Ro 57.29577951308
#define MIN 0.000000001

//extern std::ofstream file4;

void Algorithms::simplify(std::list<Point> &points, double split_ratio_max, int split_two_point, std::list <Edge> &res, double &angle)
{
	//Building simplification algorithm
	std::list<Point> c_hull;
	std::stack<Edge> edges;

	//Test orientation
	bool orientation_points = Algorithms::testOrientation(points);

	//Convex hull
	Algorithms::convexHull(points, c_hull);

	//Construct MBR
	//file4 << "CH" << '\n';
	Point m1, m2, m3, m4;
	
	//file4 << "MBR" << '\n';
	//Use the minimum bounding rectangle
	Algorithms::minimumBoundingRectangle(c_hull, m1, m2, m3, m4, angle);
	
	//file4 << "WA" << '\n';
	//Use the wall average
	//double angle2 = Algorithms::wallAverage(c_hull);

	//file4 << "WB" << '\n';
	//Use the weighted bisector
	//double angle3 = Algorithms::weightedBisector(points);

	//Rotate by -angle
	//file4 << "Rot" << '\n';
	//angle = 0.000001;
	Algorithms::rotate(points, -angle);

	//Create first edge (depends on orientation)
	Edge e;
	e.createFirstEdge(points, orientation_points);
	edges.push(e);

	//Process all edges
	int index = 0;

	//file4 << "Seznam zpracovanych hran" << '\n';

	//Process building edges using the recursive approach
	while (!edges.empty())
	{
		//Take edge from the top of the stack
		Edge e = edges.top();
		edges.pop();

		//Get orientation
		int orientation_edge = e.getOrientation();

		//Calculate split ratio
		e.computeSplitRatio(orientation_edge);
		double split_ratio = e.getSplitRatio();

		//file4 << "computeSplitRatio=" << computeSplitRatio << '\n';
		//e.printEdge();

		//Only first edge (approximation using MBR) => index ==0
		if (index == 0)
		{
			//Process edges with no intersections and split
			e.processEdgeNoIntersection(edges);
		}

		//Split edges using condition (@ points edges has not to be splitted)
		else if ((split_ratio> split_ratio_max) && (!(split_two_point == 1) || !(e.getPointsSize() > 3)))
		//else if ((split_ratio > split_ratio_max) && (!(split_two_point == 1) && (e.getPointsSize() > 2)))
		{
			//Calculate regression line for edge
			Point p1(0, 0), p2(0, 0);
			e.regressionLine(p1, p2, orientation_edge);

			//Intersections: regression line x edge segments
			e.intersectionsEdgeAndRegressionLine(p1, p2);

			//Process edges intersections and split
			e.processEdgeWithIntersections(edges, orientation_edge);
		}

		//Ratio < threshold
		else
		{
			//e.printEdge();
			res.push_back(e);  //Add actual edge to result
		}

		//Increment index
		index++;
	}

	//Rotate back
	Algorithms::rotate(points, angle);
}


void Algorithms::convexHull(std::list <Point> &points, std::list <Point> &c_hull)
{
	//Create convex hull
	Point q = *std::min_element(points.begin(), points.end(), sortPointsByY());
	c_hull.push_back(q);

	//Find left-most point
	Point s = *std::min_element(points.begin(), points.end(), sortPointsByX());

	//Assign initial points
	Point pj = q;
	Point pj_1(s.getX() - 1, q.getY());
	
	//If q==s
	if (s.getX() == q.getX())
		pj_1.setX(q.getX() - 1);

	//Loop through the vertices
	int index = 0;
	do
	{
		std::list <Point> ::iterator i_max = points.end();
		double a_max = 0.0, d_max = 0, EPS = 0.0001;

		for (std::list <Point> ::iterator i_point = points.begin(); i_point != points.end(); i_point++)
		{
			double a = fabs(angle2Lines(pj_1, pj, pj, *i_point));
			
			if ((!(*i_point == pj)) && (!(*i_point == pj_1)))
			{
				//Common point
				if (a > a_max)
				{
					a_max = a;
					i_max = i_point;
				}

				//Singular point
				else if (std::fabs(a - a_max) < EPS)
				{
					double dx = i_point->getX() - pj.getX();
					double dy = i_point->getY() - pj.getY();
					double d = sqrt(dx * dx + dy * dy);

					//Find furthest point
					if (d > d_max)
					{
						d_max = d;
						i_max = i_point;
					}
				}
			}
		}

		//Add point to hull
		c_hull.push_back(*i_max);

		//Assign point
		pj_1 = pj;
		pj = *i_max;

		index++;

	} while (!(pj == q));
}

double Algorithms::distance(const Point &p1, const Point &p2)
{
	//Compute distance between 2 points
	const double dx = p1.getX() - p2.getX();
	const double dy = p1.getY() - p2.getY();
	
	return sqrt(dx * dx + dy * dy);
}


double Algorithms::angle2Lines(Point &p1, Point &p2, Point &p3, Point &p4)
{
	//Angle between two lines
	double dx1 = p1.getX() - p2.getX();
	double dy1 = p1.getY() - p2.getY();
	double dx2 = p4.getX() - p3.getX();
	double dy2 = p4.getY() - p3.getY();

	double bear1 = atan2(dy1, dx1) * Ro;
	double bear2 = atan2(dy2, dx2) * Ro;

	double angle = bear2 - bear1;
	if (angle < -MIN) angle = angle + 360;
	
	return angle;
}


void Algorithms::minimumBoundingRectangle(std::list <Point> &points, Point &mb1, Point &mb2, Point &mb3, Point &mb4, double &min_angle)
{
	//Find min max box
	double area_minimum = 999999999999999;
	Point v1, v2, v3, v4;

	//Initialization
	minMaxBox(points, v1, v2, v3, v4);
	double area = fabs((v1.getX() - v2.getX())*(v2.getY() - v3.getY()));

	//Set min angle
	min_angle = 999;

	//Starting point of the edge
	std::list <Point> ::iterator i_points = points.begin();;
	Point pa = *i_points;
	i_points++;

	for ( ; i_points != points.end(); i_points++)
	{
		//Get edge
		Point pb = *i_points;
	
		//Get angle
		double dx = pb.getX() - pa.getX();
		double dy = pb.getY() - pa.getY();
		double angle = atan2(dy, dx) * Ro;
		
		//Rotate by - angle
		rotate(points, -angle);

		//Find min-max box
		minMaxBox(points, v1, v2, v3, v4);

		//Get area
		double vx = fabs(v1.getX() - v2.getX());
		double vy = fabs(v2.getY() - v3.getY());
		double area = fabs(vx * vy);

		//Rotate by angle
		rotate(points, angle);

		//Store minimum values
		if (area < area_minimum)
		{
			double angle_longest = vx > vy ? angle : angle + 90.0;
			min_angle = fmod(angle_longest, 180.0);
			area_minimum = area;
			
			mb1 = v1;
			mb2 = v2;
			mb3 = v3;
			mb4 = v4;
		}

		//Assign point
		pa = pb;
	}
}


void Algorithms::minMaxBox(std::list <Point> &points, Point &v1, Point &v2, Point &v3, Point &v4)
{
	//Find MBR in normal position
	const Point p_x_max = *std::max_element(points.begin(), points.end(), sortPointsByX());
	const Point p_x_min = *std::min_element(points.begin(), points.end(), sortPointsByX());
	const Point p_y_max = *std::max_element(points.begin(), points.end(), sortPointsByY());
	const Point p_y_min = *std::min_element(points.begin(), points.end(), sortPointsByY());

	//Min-max box coordinates
	const double xmin = p_x_min.getX();
	const double ymin = p_y_min.getY();
	const double xmax = p_x_max.getX();
	const double ymax = p_y_max.getY();

	//Create the min-max box
	v1.setX(xmin);
	v1.setY(ymax);
	v2.setX(xmax);
	v2.setY(ymax);
	v3.setX(xmax);
	v3.setY(ymin);
	v4.setX(xmin);
	v4.setY(ymin);
}


bool Algorithms::get2LinesIntersection(const Point &p1, const Point &p2, const Point &p3, const Point &p4, Point &p_int)
{
	//2 lines intersection
	bool intersection_exists = false;
	const double x1 = p1.getX();
	const double y1 = p1.getY();
	const double x2 = p2.getX();
	const double y2 = p2.getY();
	const double x3 = p3.getX();
	const double y3 = p3.getY();
	const double x4 = p4.getX();
	const double y4 = p4.getY();

	//Matrix of vectors
	const double a11 = x2 - x1;
	const double a12 = x3 - x4;
	const double b11 = x3 - x1;
	const double a21 = y2 - y1;
	const double a22 = y3 - y4;
	const double b21 = y3 - y1;

	//Determinant
	const double det = a11 * a22 - a21 * a12;
	if (det != 0)
	{
		//Inverse matrix
		const double ia22 = a22 / det;
		const double ia12 = -a12 / det;
		const double ia21 = -a21 / det;
		const double ia11 = a11 / det;

		//Parameters s, t
		const double s = ia22 * b11 + ia12 * b21;
		const double t = ia21 * b11 + ia11 * b21;

		//Compute intersection point and add the differential shift
		const double xp = x1 + s * (x2 - x1) + 0.00001;
		const double yp = y1 + s * (y2 - y1) - 0.00001;

		//Intersection of 2 lines exists
		if ((s > 0) && (s < 1) && (t >= 0) && (t <= 1))
		{
			p_int.setX(xp);
			p_int.setY(yp);

			//First point behind the intersection
			p_int.setP(const_cast<Point*> (&p2));

			//Set the flag
			intersection_exists = true;
		}
	}
	return intersection_exists;
}


void Algorithms::calipers(Point *p1_last, Point *p1, Point *p2, Point *p22, Point *p3, Point *p32, Point *p4, Point *p42)
{
	//Equations of calipers (p1,p2)=source edge
	double dx1 = p1->getX() - p1_last->getX();
	double dy1 = p1->getY() - p1_last->getY();

	//Normal vector
	double dx2 = -dy1;
	double dy2 = dx1;

	//P22
	double x22 = p2->getX() - dx2;
	double y22 = p2->getY() - dy2;
	p22->setX(x22);
	p22->setY(y22);

	//P32
	double x32 = p3->getX() + dx1;
	double y32 = p3->getY() + dy1;
	p32->setX(x32);
	p32->setY(y32);

	//P42
	double x42 = p4->getX() + dx2;
	double y42 = p4->getY() + dy2;
	p42->setX(x42);
	p42->setY(y42);
}


void Algorithms::rotate(std::list <Point> &points, double angle)
{
	//Rotate all points
	std::list<Point> ::iterator i_points = points.begin();
	while (i_points != points.end())
	{
		i_points->rotate(angle);
		i_points++;
	}
}


bool Algorithms::testOrientation(std::list <Point> &points)
{
	//Test, whether points are clockwise
	int  index = 0;
	double x, y, x1, y1, y2, area = 0;
	Point Pi, Pii, Piii;
	std::list <Point> ::iterator i_start;
	int n = points.size();

	std::list<Point> ::iterator i_points = points.begin();
	i_start = points.end();

	if (n > 0)
	{
		while (i_points != i_start)
		{
			if (i_start == points.end())
			{
				i_start = i_points;
			}

			//Get point of mass coordinates
			x = i_points->getX();
			y = i_points->getY();

			Pi = Pii;
			Pii = Piii;
			Piii.setX(x);
			Piii.setY(y);

			if (index == 0)
			{
				x1 = x;
				y1 = y;
			}
			else if (index == 1)
			{
				y2 = y;
			}
			else
			{
				area = area + Pii.getX()*(Piii.getY() - Pi.getY());
			}

			index++;
			i_points++;

			if (i_points == points.end())
			{
				i_points = points.begin();
				i_start++;
				i_start++;
			}
		}
	}
	//First point
	area = area + x1 * (y2 - Piii.getY());

	//Last point
	area = area + Piii.getX()*(y1 - Pii.getY());

	if (area > 0) return false;
	else return true;
}


double Algorithms::wallAverage(std::list <Point> &points)
{
	//Find wall average angle of the building
	double wa = 0, dsum = 0;

	std::list <Point> ::iterator i_points = points.begin();
	Point pa = *i_points;
	i_points++;

	for (; i_points != points.end(); i_points++)
	{
		//Get edge
		Point pb = *i_points;

		//Get length of the element
		double dx = pb.getX() - pa.getX();
		double dy = pb.getY() - pa.getY();
		double dist = sqrt(dx * dx + dy * dy);

		//Get bearing of the element
		double angle = atan2(dy, dx) * Ro;
		double div = angle / 90.0;
		 
		//Compute contribution
		double angle_mod = fabs(angle - round(div) * 90.0);

		//Summarize
		if (dist > 0 )
		{
			wa += dist * dist * angle_mod;
			dsum += dist * dist;
			std::cout << "d = " << dist << ", a =  " << angle_mod<< '\n';
		}

		//Assign point
		pa = pb;
	}

	//Compute wall average: dominant angle of the building
	wa = wa / dsum;
	
	//Rotate by -angle
	rotate(points, -wa);

	//Find min-max box
	Point v1, v2, v3, v4;
	minMaxBox(points, v1, v2, v3, v4);

	//Get area
	double vx = fabs(v1.getX() - v2.getX());
	double vy = fabs(v2.getY() - v3.getY());
	double area1 = vx * vy;

	//Rotate by angle
	rotate(points, wa);

	//Find angle of the longer edge of MBR
	double angle_longest = vx > vy ? wa : wa + 90.0;

	//Second solution: 180 - angle_longest
	double wa2 = 180 - angle_longest;
	
	//Rotate by -wa2
	rotate(points, -wa2);

	//Find min-max box
	minMaxBox(points, v1, v2, v3, v4);

	//Get area
	vx = fabs(v1.getX() - v2.getX());
	vy = fabs(v2.getY() - v3.getY());
	double area2 = vx * vy;

	//Rotate by angle
	rotate(points, wa2);

	//Find angle of the longer edge of MBR
	double angle_longest2 = vx > vy ? wa2 : wa2 + 90.0;

	return (area1 < area2 ? angle_longest : angle_longest2);
}

double Algorithms::weightedBisector(std::list <Point> &points)
{
	//Find weighted bisector angle of the building
	double dist_max1 = 0.0, dist_max2 = 0.0, alpha1 = 0.0, alpha2 = 0.0;
	using TKeyType = std::pair<double, double>;
	std::set <TKeyType, std::greater<TKeyType> > bisectors_pos, bisectors_neg, bisectors;

	for (std::list <Point> ::iterator i_points_start = points.begin(); i_points_start != points.end(); i_points_start++)
	{
		for (std::list <Point> ::iterator i_points_end = i_points_start; i_points_end != points.end(); i_points_end++)
		{
			//Get length of the element
			double dx = i_points_end->getX() - i_points_start->getX();
			double dy = i_points_end->getY() - i_points_start->getY();
			double dist = sqrt(dx * dx + dy * dy);

			//Get bearing of the element
			double angle = atan2(dy, dx) * Ro;
			double div = angle / 90.0;

			//Compute contribution
			double angle_mod = fabs(angle - round(div) * 90.0);
			if (angle < 0) angle += 360;
			bisectors.insert({ dist, angle_mod});
		}
	}
	
	//Use 5% of longest bisectors
	int i = 0, n = points.size();
	double wb = 0, dsum = 0;
	std::set <TKeyType, std::greater<TKeyType> > ::iterator i_bisectors = bisectors.begin();
	for (; i_bisectors != bisectors.end(); i_bisectors++)
	{
		//Get values
		double dist = i_bisectors->first;
		double alpha = i_bisectors->second;

		wb += dist * alpha;
		dsum += dist;

		if (++i > n / 10)
		break;
	}
	
	//Weighted bisector
	double bis = wb / dsum;

	//Rotate by -angle
	rotate(points, -bis);

	//Find min-max box
	Point v1, v2, v3, v4;
	minMaxBox(points, v1, v2, v3, v4);

	//Get area
	double vx = fabs(v1.getX() - v2.getX());
	double vy = fabs(v2.getY() - v3.getY());
	double area1 = vx * vy;

	//Rotate by angle
	rotate(points, bis);

	//Find angle of the longer edge of MBR
	double angle_longest = vx > vy ? bis : bis + 90.0;

	//Second solution: 180 - angle_longest
	double bis2 = 180 - angle_longest;

	//Rotate by -wa2
	rotate(points, -bis2);

	//Find min-max box
	minMaxBox(points, v1, v2, v3, v4);

	//Get area
	vx = fabs(v1.getX() - v2.getX());
	vy = fabs(v2.getY() - v3.getY());
	double area2 = vx * vy;

	//Rotate by angle
	rotate(points, bis2);

	//Find angle of the longer edge of MBR
	double angle_longest2 = vx > vy ? bis2 : bis2 + 90.0;

	return (area1 < area2 ? angle_longest : angle_longest2);
}

/*
double Algorithms::weightedBisector(std::list <Point> &points)
{
	//Find weighted bisector angle
	double dist_max1 = 0.0, dist_max2 = 0.0, alpha1 = 0.0, alpha2 = 0.0;
	using TKeyType = std::pair<double, double>;
	std::set <TKeyType, std::greater<TKeyType> > bisectors_pos, bisectors_neg, bisectors;

	for (std::list <Point> ::iterator i_points_start = points.begin(); i_points_start != points.end(); i_points_start++)
	{
		for (std::list <Point> ::iterator i_points_end = i_points_start; i_points_end != points.end(); i_points_end++)
		{
			//Get length of the element
			double dx = i_points_end->getX() - i_points_start->getX();
			double dy = i_points_end->getY() - i_points_start->getY();
			double dist = sqrt(dx * dx + dy * dy);

			double alpha = atan2(dy, dx) * Ro;
			if (alpha < 0) alpha += 360;

			bisectors.insert({ dist, alpha });
			//
			//double alpha2 = -alpha;
			////if (alpha > 0)
			//{
			//	bisectors_pos.insert({ dist, fmod(max(alpha, alpha2), 90.0) });
			//}

			////else
			//{
			//	bisectors_neg.insert({ dist, fmod(min(alpha, alpha2), 90.0) });
			//}
			
		}
	}
	
	////Use 5% of longest bisectors
	//int i = 0, n = points.size();
	//double wb = 0, dsum = 0;
	//std::set <TKeyType, std::greater<TKeyType> > ::iterator i_bisectors_pos = bisectors_pos.begin(), i_bisectors_neg = bisectors_neg.begin();
	//for (; i_bisectors_pos != bisectors_pos.end() && i_bisectors_neg != bisectors_neg.end(); i_bisectors_pos++, i_bisectors_neg++)
	//{
	//	//Get values
	//	double dist1 = i_bisectors_pos->first;
	//	double dist2 = i_bisectors_neg->first;
	//	double alpha1 = i_bisectors_pos->second;
	//	double alpha2 = i_bisectors_neg->second;

	//	double alpha_diff = alpha1;
	//	double dist_aver = 0.5 * (dist1 + dist2);

	//	wb += dist_aver * alpha_diff;
	//	dsum += dist_aver;

	//	if (++i > n / 10)
	//		break;
	//}
	//
	
	int i = 0, n = bisectors.size();

	double area_min = 9999999, bis_min = 999;
	Point v1, v2, v3, v4;
	while (n > 2)
	{
		double wb = 0, dsum = 0;
		std::set <TKeyType, std::greater<TKeyType> > ::iterator i_bisectors = bisectors.begin();
		for (; i_bisectors != bisectors.end(); i_bisectors++)
		{
			//Get values
			double dist = i_bisectors->first;
			double alpha = i_bisectors->second;

			wb += dist * alpha;
			dsum += dist;

			if (++i > n)
				break;
		}

		//Weighted bisector
		//double bis = (dist_max1 * alpha1 + dist_max2 * alpha2) / (dist_max1 + dist_max2);
		double bis = wb / dsum;

		//Rotate by -bis
		rotate(points, -bis);

		//Find min-max box
		minMaxBox(points, v1, v2, v3, v4);

		//Get area
		double area1 = fabs((v1.getX() - v2.getX())*(v2.getY() - v3.getY()));

		//Rotate by angle
		rotate(points, bis);

		double bis_90 = 180 - bis;
		rotate(points, -bis_90);

		//Find min-max box
		minMaxBox(points, v1, v2, v3, v4);

		//Get area
		double area2 = fabs((v1.getX() - v2.getX())*(v2.getY() - v3.getY()));

		//Rotate by angle
		rotate(points, bis_90);

		double bis12 = (area1 <= area2 ? bis : bis_90);
		double area12 = std::min(area1, area2);

		if (area12 < area_min)
		{
			area_min = area12;
			bis_min = bis12;
		}

		n = n / 2;
	}

	//Rotate by -(angle + 90)
	double bis_90 = 180 -bis_min;
	rotate(points, -bis_90);

	//Find min-max box
	minMaxBox(points, v1, v2, v3, v4);

	//Get area
	double area2 = fabs((v1.getX() - v2.getX())*(v2.getY() - v3.getY()));

	//Rotate by angle
	rotate(points, bis_90);

	return (area_min <= area2 ? bis_min : bis_90);
}
*/


void Algorithms::drawBuilding(std::list <Edge> &result, double angle, IPointCollectionPtr p_collection)
{
	//Draw source points
	int orientation, recursion_depth, fract, index = 0;
	double xt, yt, xt_old, yt_old;
	//CPen p2;
	std::list <Edge> ::iterator i_start;
	int n = result.size();
	//p2.CreatePen(PS_SOLID, 1, RGB(10,50, 105));  //Blue pen

	std::list <Edge> ::iterator i_result = result.begin();
	i_start = result.end();

	if (n>0)
	{
		while (i_result != i_start)
		{
			if (i_start == result.end())
			{
				i_start = i_result;
			}

			//Get orientation
			orientation = i_result->getOrientation();
			recursion_depth = i_result->getRecursion();
			if (recursion_depth>1)
			{
				fract = (orientation % 4 - (recursion_depth - 1) % 4);
				if (fract <= 0) fract += 4;
			}
			else fract = orientation;

			//Get mass coordinates
			xt = i_result->getXt();
			yt = i_result->getYt();

			if (fract == 2 || fract == 4)
			{
				if (index == 1)
				{
					Point p(xt, yt_old);
					p.rotate(angle);
					IPointPtr ip(CLSID_Point);
					ip->put_X(p.getX());
					ip->put_Y(p.getY());
					p_collection->AddPoint(ip);
					//dc.MoveTo((int)p.getX(),(int)p.getY());
				}
				else if (index>1)
				{
					Point p(xt, yt_old);
					p.rotate(angle);
					IPointPtr ip(CLSID_Point);
					ip->put_X(p.getX());
					ip->put_Y(p.getY());
					p_collection->AddPoint(ip);
					//dc.LineTo((int)p.getX(),(int)p.getY());
				}
			}
			else if (fract == 1 || fract == 3)
			{
				if (index == 1)
				{
					Point p(xt_old, yt);
					p.rotate(angle);
					IPointPtr ip(CLSID_Point);
					ip->put_X(p.getX());
					ip->put_Y(p.getY());
					p_collection->AddPoint(ip);
					//dc.MoveTo((int)p.getX(),(int)p.getY());
				}
				else if (index>1)
				{
					Point p(xt_old, yt);
					p.rotate(angle);
					IPointPtr ip(CLSID_Point);
					ip->put_X(p.getX());
					ip->put_Y(p.getY());
					p_collection->AddPoint(ip);
					//dc.LineTo((int)p.getX(),(int)p.getY());
				}
			}
			//Point el(xt,yt);
			//el.rotate(angle);
			//dc.Ellipse((int)el.getX()-2,(int)el.getY()-2,(int)el.getX()+2,(int)el.getY()+2);

			xt_old = xt;
			yt_old = yt;

			index++;
			i_result++;

			if (i_result == result.end())
			{
				i_result = result.begin();
				i_start++;
				i_start++;
			}
		}
		//DeleteObject(p2);
	}
}


void Algorithms::drawBuildingTemp(std::list <Edge> &result, double angle, IScreenDisplayPtr p_display)
{
	//Draw source points
	int orientation, recursion_depth, fract, index = 0;
	double xt, yt, xt_old, yt_old;

	std::list <Edge> ::iterator i_start;
	int n = result.size();

	std::list <Edge> ::iterator i_result = result.begin();
	i_start = result.end();

	//Create collection
	IPointCollectionPtr p_collection(CLSID_Polygon);
	IPolygonPtr p_polygon(p_collection);

	if (n>0)
	{
		while (i_result != i_start)
		{
			if (i_start == result.end())
			{
				i_start = i_result;
			}

			//Get orientation
			orientation = i_result->getOrientation();
			recursion_depth = i_result->getRecursion();
			if (recursion_depth>1)
			{
				fract = (orientation % 4 - (recursion_depth - 1) % 4);
				if (fract <= 0) fract += 4;
			}
			else fract = orientation;

			//Get mass coordinates
			xt = i_result->getXt();
			yt = i_result->getYt();

			if (fract == 2 || fract == 4)
			{
				if (index == 1)
				{
					Point p(xt, yt_old);
					p.rotate(angle);
					IPointPtr ip(CLSID_Point);
					ip->put_X(p.getX());
					ip->put_Y(p.getY());

					p_collection->AddPoint(ip);
					//dc.MoveTo((int)p.getX(),(int)p.getY());
				}
				else if (index>1)
				{
					Point p(xt, yt_old);
					p.rotate(angle);
					IPointPtr ip(CLSID_Point);
					ip->put_X(p.getX());
					ip->put_Y(p.getY());
					p_collection->AddPoint(ip);
					//dc.LineTo((int)p.getX(),(int)p.getY());
				}
			}

			else if (fract == 1 || fract == 3)
			{
				if (index == 1)
				{
					Point p(xt_old, yt);
					p.rotate(angle);
					IPointPtr ip(CLSID_Point);
					ip->put_X(p.getX());
					ip->put_Y(p.getY());
					p_collection->AddPoint(ip);
					//dc.MoveTo((int)p.getX(),(int)p.getY());
				}
				else if (index>1)
				{
					Point p(xt_old, yt);
					p.rotate(angle);
					IPointPtr ip(CLSID_Point);
					ip->put_X(p.getX());
					ip->put_Y(p.getY());
					p_collection->AddPoint(ip);
					//dc.LineTo((int)p.getX(),(int)p.getY());
				}
			}

			//Point el(xt,yt);
			//el.rotate(angle);
			//dc.Ellipse((int)el.getX()-2,(int)el.getY()-2,(int)el.getX()+2,(int)el.getY()+2);

			xt_old = xt;
			yt_old = yt;

			index++;
			i_result++;

			if (i_result == result.end())
			{
				i_result = result.begin();
				i_start++;
				i_start++;
			}
		}
		//DeleteObject(p2);
	}

	//Set color
	IRgbColorPtr p_color(CLSID_RgbColor);
	p_color->put_UseWindowsDithering(VARIANT_FALSE);
	p_color->put_Red(45);
	p_color->put_Green(45);
	p_color->put_Blue(45);

	//Set fill color
	IFillSymbolPtr p_fill(CLSID_SimpleFillSymbol);
	p_fill->put_Color(p_color);

	//Set symbol
	ISymbolPtr p_symbol(p_fill);
	p_symbol->put_ROP2(esriROPXOrPen);

	//Set up outline
	ILineSymbolPtr p_lineSymbol;
	p_fill->get_Outline(&p_lineSymbol);
	p_color->put_Red(145);
	p_color->put_Green(145);
	p_color->put_Blue(145);
	p_lineSymbol->put_Color(p_color);
	p_lineSymbol->put_Width(0.2);
	ISymbolPtr p_symbolL(p_lineSymbol);
	p_symbolL->put_ROP2(esriROPXOrPen);
	p_fill->put_Outline(p_lineSymbol);

	//Temporary draw
	p_display->StartDrawing(0, esriNoScreenCache);
	p_display->SetSymbol(p_symbol);
	p_display->DrawPolygon(p_polygon);
	//::Sleep(300);
	//p_display->DrawPolygon(p_polygon);
	p_display->FinishDrawing();
}


void Algorithms::print(std::list <Point> &points)
{
	//Print points list
	std::list<Point> ::iterator i_points = points.begin();
	while (i_points != points.end())
	{
		i_points->print();
		i_points++;
	}
}



