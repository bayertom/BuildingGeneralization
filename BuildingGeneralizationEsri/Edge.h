#ifndef Edge_H
#define Edge_H

#include <list>
#include <cmath>
#include <algorithm>
#include <stack>
#include <fstream>
#include "Point.h"


class Edge
{
private:
	int recursion_depth;						//Recursion depth
	double xt, yt;							//Center of mass of points forming the edge
	double split_ratio;						//Split ratio: indicates, whether the ege will be split
	int orientation;						//Orientation of the edge

	std::list <Point> points;					//List of points forming the edge
	std::list <Point> intersections;				//List of intersections with the remaining edges
public:
	Edge();
	~Edge(){};
	Edge(const double xt, const double yt);
	
	int getRecursion() const {return recursion_depth;}
	double getXt() const {return xt;}
	double getYt() const {return yt;}
	double getSplitRatio() const { return split_ratio;}
	int getPointsSize() const {return points.size();}
	int getOrientation2() const { return orientation;}
	int getOrientation() const {return points.front().getOrientation();}
	std::list <Point> & getPoints() { return points;}

	void setRecursion(const int recursion_depth_){recursion_depth = recursion_depth_;}
	void setXt(const double xt_){xt = xt_;}
	void setYt(const double yt_) {yt = yt_; }
	void setSplitRatio(const double split_ratio_) { split_ratio = split_ratio_; }
	void setOrientation(const int orientation_) { orientation = orientation_; }

	void computeSplitRatio (const int orientation);
	void push_back(const Point &p) { points.push_back(p); };
	void push_front(const Point &p) { points.push_front(p); };
	void createFirstEdge(std::list<Point> &pl, const bool orientation);
	//bool intersection(Point *p1, Point *p2, Point *p3, Point *p4, Point &p_int);
	void splitEdge(std::stack<Edge> &el, Edge &em, int index, int orientation_parent_edge, int &orientation_last_edge, int recursion_depth);
	void splitEdge3(std::stack<Edge> &el, Edge &em, int index, int orientation_parent_edge, int &orientation_last_edge, int recursion_depth);

	bool findNewVertices(const Point &v1, const Point &v2, const Point &v3, const Point &v4, int index, int posit, int orientation);
	bool changeVerticesOrientation(std::list <Point> ::iterator i1_min, std::list <Point> ::iterator i2_min, std::list <Point> ::iterator i3_min, std::list <Point> ::iterator i4_min, int index, int posit, int orientation);

	//bool findNewVertices2(const Point &v1, const Point &v2, const Point &v3, const Point &v4, int index, int posit, int orientation);
	void find2NearestPoints(const Point &v, std::list <Point> ::iterator &i1, std::list <Point> ::iterator &i2, double &s1, double &s2);
	void correctIdenticalNearestPoints(std::list <Point> ::iterator &i1, std::list <Point> ::iterator &i12, std::list <Point> ::iterator &i2, std::list <Point> ::iterator &i22, const double d1, const double d2);

	int getPointLinePosition(const Point &p, const Point &p2, const Point &p3);
	void createEdge(Edge &e_new, Point &start, Point &end, std::list <Point> ::iterator &i_points, int orientation_edge_actual, int index);
	int intersectionsSize(){return intersections.size();};
	void clear() { points.clear(); intersections.clear(); };
	void regressionLine(Point &p1, Point &p2, int orientation);
	void intersectionsEdgeAndRegressionLine(Point &p1, Point &p2);
	void processEdgeWithIntersections(std::stack<Edge>  &el, int orientation_edge);
	void processEdgeNoIntersection(std::stack<Edge>  &el);
	
	void printEdge();
};

#endif
