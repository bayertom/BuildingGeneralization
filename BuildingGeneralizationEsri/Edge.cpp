#include "stdafx.h"
#include "Edge.h"
#include "Algorithms.h"


#define MAX 999999999999999
extern std::ofstream file4;

Edge::Edge()
{
	this->recursion_depth = 0;
	this->xt=0;
	this->yt=0;
	this->split_ratio = 0;
	this->orientation = 0;
}


Edge::Edge(const double xt, const double yt)
{
	this->recursion_depth = 0;
	this->xt=xt;
	this->yt=yt;
	this->split_ratio = 0;
	this->orientation = 0;
}


void Edge::createFirstEdge(std::list<Point> &pl, const bool orientation)
{
	//Create first edge from points
	for ( std::list<Point> ::iterator i_points_list = pl.begin(); i_points_list != pl.end(); i_points_list++ )
	{
		//Clockwise
		if (orientation)
		{
			points.push_back(*i_points_list);
		}

		//Clock counter wise
		else
		{
			points.push_front(*i_points_list);
		}
	}
}


void Edge::computeSplitRatio (const int orientation_edge)
{
	//Calculate compute splitRatio (RMS) of points according to the edge
	int orientation_edge_new = 0, i = 0;
	double x = 0, y = 0, ax = 0, ay = 0, vv = 0, k2x = 0, k2y = 0, k3x = 0, k3y = 0, sigma = 0;

	//Compute new orientation of its split part
	if (recursion_depth > 1)
	{
		orientation_edge_new = (orientation_edge % 4 - (recursion_depth - 1) % 4);
		if (orientation_edge_new <= 0) orientation_edge_new += 4;
	}

	//Recalculate computeSplitRatio (squares of residuals of points from the actual edge) and center of mass
	//Compute on the fly with one for cycle
	//Can be replaced with the classical approach: mean and standard deviation
	std::list <Point> ::iterator i_points = points.begin();
	while (i_points != points.end())
	{
		//Get point properties
		Point *p = &*i_points;
		ax = p->getX();
		ay = p->getY();

		//Compute center of mass
		if (i > 0)
		{
			x = (x * i + ax) / (i + 1);
			y = (y * i + ay) / (i + 1);
		}

		else
		{
			x = ax;
			y = ay;
		}

		//Standard deviation coefficients in x/y direction
		k2x = k2x - 2 * ax;
		k2y = k2y - 2 * ay;
		k3x = k3x + ax * ax;
		k3y = k3y + ay * ay;

		i++;
		i_points++;
	}

	//Set new center of mass [xt, yt]
	this->xt = x;
	this->yt = y;

	//X direction (horizontal): standard deviation
	if (orientation_edge_new == 1 || orientation_edge == 3)
	{
		vv = fabs(i * y * y + y * k2y + k3y);
		sigma = sqrt(vv / (i - 1));
	}

	//Y direction (vertical): standard deviation
	else
	{
		vv = fabs(i * x * x + x * k2x + k3x);
		sigma = sqrt(vv / (i - 1));
	}

	this->split_ratio = sigma;
}


bool Edge::findNewVertices(const Point &v1, const Point &v2, const Point &v3, const Point &v4, int index, int posit, int orientation)
{
	//Create new vertices of the building which are the closest to the MBR vertices
	//Change orientation at these points
	double d1_min = MAX, d2_min = MAX, d3_min = MAX, d4_min = MAX, d1_min2 = MAX, d2_min2 = MAX, d3_min2 = MAX, d4_min2 = MAX;
	std::list <Point> ::iterator i1_min = points.end(), i1_min2 = points.end(), i2_min = points.end(), i2_min2 = points.end(), i3_min = points.end(), 
				     i3_min2 = points.end(), i4_min = points.end(), i4_min2 = points.end();

	//Find new building vertices
	//Rectangle left from the regression line
	if (posit == 0 || index == 2)
	{
		find2NearestPoints(v1, i1_min, i1_min2, d1_min, d1_min2);
		find2NearestPoints(v2, i2_min, i2_min2, d2_min, d2_min2);
	}

	//Rectangle right from the regression line
	if (posit == 1 || index == 2)
	{
		find2NearestPoints(v3, i3_min, i3_min2, d3_min, d3_min2);
		find2NearestPoints(v4, i4_min, i4_min2, d4_min, d4_min2);
	}

	//Correct thet points nearest to more than vertex (use the second nearest)
	correctIdenticalNearestPoints(i1_min, i1_min2, i2_min, i2_min2, d1_min, d2_min);
	correctIdenticalNearestPoints(i2_min, i2_min2, i3_min, i3_min2, d2_min, d3_min);
	correctIdenticalNearestPoints(i3_min, i3_min2, i4_min, i4_min2, d3_min, d4_min);
	correctIdenticalNearestPoints(i4_min, i4_min2, i1_min, i1_min2, d4_min, d1_min);

	//Change orientation of vertices if possible
	//If their orientation is wrong, return false
	return changeVerticesOrientation(i1_min, i2_min, i3_min, i4_min, index, posit, orientation);
}


bool Edge::changeVerticesOrientation(std::list <Point> ::iterator i1_min, std::list <Point> ::iterator i2_min, std::list <Point> ::iterator i3_min, std::list <Point> ::iterator i4_min, int index, int posit, int orientation)
{
	//Convert iterators to indices
	int i1 = (i1_min != points.end()) ? std::distance(points.begin(), i1_min) : -3;
	int i2 = (i2_min != points.end()) ? std::distance(points.begin(), i2_min) : -2;
	int i3 = (i3_min != points.end()) ? std::distance(points.begin(), i3_min) : -1;
	int i4 = (i4_min != points.end()) ? std::distance(points.begin(), i4_min) : 0;

	//Initial building approximnation
	if (index == 2)
	{
		//Differences between the closest points
		const int k1 = i2 - i1, k2 = i3 - i2;
		const int k3 = i4 - i3, k4 = i1 - i4;

		//All vertices are identical
		if (k1 * k2 * k3 * k4 == 0)
			return false;

		//Wrong order of vertices
		if ((k1 < 0) && (k2 < 0) || (k1 < 0) && (k3 < 0) || (k1 < 0) && (k4 < 0) || (k2 < 0) && (k3 < 0) || (k2 < 0) && (k4 < 0) || (k3 < 0) && (k4 < 0))
		{
			return false;
		}

		//Change orientation: V1 - here starts the new first edge
		if (i1_min != points.end())
			i1_min->setOrientation(orientation + 1);

		//Change orientation: V2 - here starts the new second edge
		if (i2_min != points.end())
			i2_min->setOrientation(orientation + 2);

		//Change orientation: V3 - here starts the new third edge
		if (i3_min != points.end())
			i3_min->setOrientation(orientation + 3);

		//Change orientation: V4 - here starts the new fourth edge
		if (i4_min != points.end())
			i4_min->setOrientation(orientation + 4);
	}

	//MBR left from the regression line: Vertices V1, V2 of MBR are found
	if (posit == 0)
	{
		if ((i1_min != points.end()) && (i2_min != points.end()))
		{
			//Change orientation: V1, V2 - here starts the new first and second edges
			//Reversed order
			if (i1 > i2)
			{
				i1_min->setOrientation(orientation + 2);
				i2_min->setOrientation(orientation + 1);
			}

			//Change orientation: V1, V2 - here starts the new first and second edges
			//Normal order
			else
			{
				i1_min->setOrientation(orientation + 1);
				i2_min->setOrientation(orientation + 2);
			}
		}
	}

	//MBR right from the regression line: Vertices V3, V4 of MBR are found
	else if (posit == 1)
	{
		if ((i3_min != points.end()) && (i4_min != points.end()))
		{
			//Change orientation: V3, V4 - here starts the new third and four edges
			//Change order
			if (i3 < i4)
			{
				i4_min->setOrientation(orientation + 4);
				i3_min->setOrientation(orientation + 1);
			}

			//Change orientation: V3, V4 - here starts the new third and four edges
			//Normal order
			else
			{
				i4_min->setOrientation(orientation + 1);
				i3_min->setOrientation(orientation + 4);
			}
		}
	}

	return true;
}

/*
bool Edge::findNewVertices(const Point &v1, const Point &v2, const Point &v3, const Point &v4, int index, int posit, int orientation)
{
	//Find MBR vertices and add new vertex if necessary
	int i1=-3, i2=-2, i3=-1, i4=0,i12=-3,i22=-2,i32=-1,i42=0,i=1;
	double d1, d2, d3, d4;		//Distances point-vertex
	double d1_min, d2_min, d3_min, d4_min, d1_min2, d2_min2, d3_min2, d4_min2;										//Minimal distances point-vertex
	bool result=true;
	Point *v1p=NULL, *v2p=NULL, *v3p=NULL, *v4p=NULL ;							//Result vertices of the edge
	Point *v1p2=NULL,*v2p2=NULL, *v3p2=NULL, *v4p2=NULL;

	//Initialization
	double x1=v1.getX();
	double y1=v1.getY();
	double x2=v2.getX();
	double y2=v2.getY();
	double x3=v3.getX();
	double y3=v3.getY();
	double x4=v4.getX();
	double y4=v4.getY();

	//Minimum distances
	d1_min=MAX;
	d2_min=MAX;
	d3_min=MAX;
	d4_min=MAX;

	//Second minimum distances
	d1_min2=MAX;
	d2_min2=MAX;
	d3_min2=MAX;
	d4_min2=MAX;

	std::list <Point> ::iterator i_points=points.begin();
	while (i_points!=points.end())
	{
		//Get point
		double xi=(*i_points).getX();
		double yi=(*i_points).getY();

		//Calculate distances
		d1=sqrt((xi-x1)*(xi-x1)+(yi-y1)*(yi-y1));
		d2=sqrt((xi-x2)*(xi-x2)+(yi-y2)*(yi-y2));
		d3=sqrt((xi-x3)*(xi-x3)+(yi-y3)*(yi-y3));
		d4=sqrt((xi-x4)*(xi-x4)+(yi-y4)*(yi-y4));

		//Left rectangle
		if (posit==0 || index==2)
		{
			//Find closest vertex to first vertex
			if (d1<d1_min)
			{
				d1_min2=d1_min;
				v1p2=v1p;
				i12=i1;
				i1=i;

				//New closest vertex
				v1p=&*i_points;
				d1_min=d1;
			}

			//Find second closest vertex
			else if (d1<d1_min2)
			{
				v1p2=&*i_points;
				d1_min2=d1;
				i12=i;
			}

			//Find closest vertex to second vertex
			if (d2<d2_min)
			{
				//Second closest vertex
				d2_min2=d2_min;
				v2p2=v2p;
				i22=i2;
				i2=i;

				//New closest vertex
				v2p=&*i_points;
				d2_min=d2;
			}

			//Find second closest vertex
			else if (d2<d2_min2)
			{
				v2p2=&*i_points;
				d2_min2=d2;
				i22=i;
			}
		}

		//Right rectangle
		if (posit==1 || index==2)
		{
			//Find closest vertex to third vertex
			if (d3<d3_min)
			{				
				//Second closest vertex
				d3_min2=d3_min;
				v3p2=v3p;
				i32=i3;
				i3=i;
				//New closest vertex
				v3p=&*i_points;
				d3_min=d3;
			}

			//Find second closest vertex
			else if (d3<d3_min2)
			{
				v3p2=&*i_points;
				d3_min2=d3;
				i32=i;
			}

			//Find closest vertex to fourth vertex
			if (d4<d4_min)
			{
				//Second closest vertex
				d4_min2=d4_min;
				v4p2=v4p;
				i42=i4;
				i4=i;

				//New closest vertex
				v4p=&*i_points;
				d4_min=d4;
			}

			//Find second closest vertex
			else if (d4<d4_min2)
			{
				v4p2=&*i_points;
				d4_min2=d4;
				i42=i;
			}
		}
		i_points++;
		i++;
	}

	//Vertex 1 and 2 are identical
	if ((v1p == v2p) && (v1p != NULL))
	{
		//Vertex closer to P1
		if(d1_min<d2_min)
		{
			v2p=v2p2;	//second closest vertex
			i2=i22;
		}

		//Vertex closer to P2
		else
		{
			v1p=v1p2;	//second closest vertex
			i1=i12;
		}
	}

	//Vertex 2 and 3 are identical
	if ((v2p == v3p) && (v2p != NULL))
	{
		//Vertex closer to P2
		if(d2_min<d3_min)
		{
			v3p=v3p2;	//second closest vertex
			i3=i32;
		}

		//Vertex closer to P3
		else
		{
			v2p=v2p2;	//second closest vertex
			i2=i22;
		}
	}

	//Vertex 3 and 4 are identical
	if ((v3p == v4p) && (v3p != NULL))
	{
		//Vertex closer to P3
		if(d3_min < d4_min)
		{
			v4p=v4p2;	//second closest vertex
			i4=i42;
		}

		//Vertex closer to P4
		else
		{
			v3p=v3p2;	//second closest vertex
			i3=i32;
		}
	}

	//Vertex 4 and 1 are identical
	if ((v4p == v1p) && (v4p != NULL))
	{
		//Vertex closer to P4
		if(d4_min < d1_min)
		{
			v1p=v1p2;	//second closest vertex
			i1=i12;
		}

		//Vertex closer to P1
		else
		{
			v4p=v4p2;	//second closest vertex
			i4=i42;
		}
	}

	//Are founded vertices in right order?
	//Use the vertex indices
	if (index==2)
	{
		int k1=i2-i1;
		int k2=i3-i2;
		int k3=i4-i3;
		int k4=i1-i4;

		//Identical vertices
		if (k1*k2*k3*k4==0)
		{
			return false;
		}

		//Wrong order of vertices
		if ((k1<0)&&(k2<0) ||(k1<0)&&(k3<0)||(k1<0)&&(k4<0)||(k2<0)&&(k3<0)||(k2<0)&&(k4<0)||(k3<0)&&(k4<0))
		{
			return false;
		}
	}

	int orientation_edge_actual;
	if (recursion_depth>1)
	{
		orientation_edge_actual=(orientation%4-(recursion_depth-1)%4);
		if (orientation_edge_actual<=0) orientation_edge_actual+=4;
	}

	else orientation_edge_actual=orientation;

	//Left rectangle
	if (posit==0||index==2) 
	{	
		if ((v1p!=NULL) &&(v2p!=NULL))
		{
			//Change order
			if ((i1>i2)&&(index!=2))
			{
				v1p->setOrientation(orientation+2);
				v2p->setOrientation(orientation+1);
			}
			//Normal order
			else
			{
				v1p->setOrientation(orientation+1);
				v2p->setOrientation(orientation+2);
			}
		}

		if (index==2) //Only MBR
		{
			if (v3p!=NULL) v3p->setOrientation(orientation+3);
			if (v4p!=NULL) v4p->setOrientation(orientation+4);
		}
	}

	//Change orientation of corners of the edge
	else if (posit==1) //Right rectangle
	{
		//Change order
		if (i3<i4)
		{
			v4p->setOrientation(orientation+4);
			v3p->setOrientation(orientation+1);
		}
		//Normal order
		else
		{
			v4p->setOrientation(orientation+1);
			v3p->setOrientation(orientation+4);
		}
	}

	return result;	
}
*/

void Edge::find2NearestPoints(const Point &v, std::list <Point> ::iterator &i1, std::list <Point> ::iterator &i2, double &d1, double &d2)
{
	//Find 2 nearest points to the given vertex
	d1 = MAX;
	d2 = MAX;
	i1 = points.begin();
	i2 = i1;

	//Coordinates of the vertex
	const double x = v.getX();
	const double y = v.getY();

	//Browse all points
	for (std::list <Point> ::iterator i_points = points.begin(); i_points != points.end(); i_points++)
	{
		//Get point coordinates
		const double xi = (*i_points).getX();
		const double yi = (*i_points).getY();

		//Calculate distance
		const double d = sqrt((xi - x) * (xi - x) + (yi - y)*(yi - y));

		//Find closest point to the vertex v
		if (d < d1)
		{
			//Assign p1->p2
			d2 = d1;
			i2 = i1;

			//New s1
			i1 = i_points;
			d1 = d;
		}

		//Find second closest point to the vertex V
		else if (d < d2)
		{
			d2 = d;
			i2 = i_points;
		}
	}
}


void Edge::correctIdenticalNearestPoints(std::list <Point> ::iterator &i1, std::list <Point> ::iterator &i12, std::list <Point> ::iterator &i2, std::list <Point> ::iterator &i22, const double d1, const double d2)
{
	//Vertex closer to 
	if ((i1 == i2) && (i1 != points.end()))
	{
		//Vertex closer to P1
		if (d1 < d2)
		{
			i2 = i22;
		}

		//Vertex closer to P2
		else
		{
			i1 = i12;
		}
	}
}


int Edge::getPointLinePosition(const Point &p, const Point &p2, const Point &p3)
{
	//Check point and line position: half edge test
	if (intersections.size()>0)
	{
		//Vectors
		const double u1x=p3.getX()-p2.getX();
		const double u1y=p3.getY()-p2.getY();
		
		const double ux=p.getX()-p2.getX();
		const double uy=p.getY()-p2.getY();

		//Calculate determinant
		double position = u1x * uy - u1y * ux;

		//Point in the left half plane
		if (position>0) 
		{
			return 0; 
		}

		//Point in the right half plane
		else 
		{
			return 1;
		}
	}

	//For empty structure returns -1
	else return -1;
}


void Edge::splitEdge(std::stack<Edge> &el, Edge &em, int index, int orientation_parent_edge, int &orientation_last_edge, int recursion_depth)
{
	//Split edge
	int orientation_actual_point=-2;		//Orientation of the actual point
	int orientation_common_point;			//Orientation of the common point of the edge (not a vertex, detect changes in numbering)
	bool second = false;				//Start point of the building is not a vertex, second iteration
	Edge ee, *eact = NULL;				//Common and actual edge

	//Initialization
	std::list <Point> ::iterator i_start=points.end();
	std::list <Point> ::iterator i_points=points.begin();

	//Process points until a start point is found 
	while (i_points!=i_start)
	{
		//Get point and its orientation
		Point p=*i_points;
		orientation_actual_point = p.getOrientation();

		//Last element reached, go to the first
		if ((orientation_actual_point > 0) && (i_start == points.end()))
		{
			//Get start point
			i_start = i_points;

			//MBR or first edge
			if (index==-1 || index==2)
			{
				//Initial MBR edge
				if (index == 2)
				{
					orientation_last_edge=i_start->getOrientation();
					orientation_common_point = 0;
				}

				//First edge (not MBR)
				else
				{
					orientation_common_point = orientation_parent_edge + 1;
					orientation_last_edge = orientation_common_point;
				}

				//Remember actual edge
				eact = &ee;
			}

			//Other edge
			else
			{
				orientation_common_point = orientation_parent_edge;
				eact = &em;
			}
		}

		//After founding of the first vertex test orientation change
		if (i_start != points.end())
		{
			//Orientation changed (third condition includes MBR, from)
			if ((orientation_actual_point > orientation_common_point) && ((i_points!=i_start) || ((i_points == i_start) && (index!=-1) && (index!=2))))
			{
				//Change point orientation
				p.setOrientation(orientation_last_edge);
				eact->setOrientation(orientation_last_edge);

				//Set recursion_depth for actual point
				eact->setRecursion(recursion_depth+1);

				//Add point to old edge
				eact->push_back(p);
				el.push(*eact);
				eact->printEdge();

				//file4 << ">> >>Nova hrana" << '\n';
				//eact->printEdge();

				//If start point of building != vertex
				if (!second)
				{
					//Create new edge
					ee.clear();						//Clear actual edge
					eact = &ee;						//Assign edge
					p.setOrientation(orientation_actual_point);		//Set orientation for point
					eact->push_back(p);					//Add edge to list

					//Remember orientation
					orientation_last_edge=orientation_actual_point;
				}
			}

			//Orientation of the edge did not change
			else
			{
				//Add point to edge
				p.setOrientation(orientation_last_edge);
				eact->push_back(p);
			}
		}

		//Remember last point of the edge (edge is not last...)
		if ((p==points.back())&&(index!=2)&&(index!=1))
		{
			//Change orientation to last vertex
			em=*eact;
		}

		//Increment point
		i_points++;

		//Tests, if this point is end point
		if (i_points==points.end())
		{
			second=true;
			i_points=points.begin();
			i_start++;

			//Not MBR, do not go around
			if (index!=2)
			{
				break;
			}
		}
	}
}


/*
void Edge::splitEdge(std::stack<Edge> &el, Edge &em, int index, int orientation_parent_edge, int &orientation_last_edge, int recursion_depth)
{
	//Split an edge given by the list of points according to their orientations
	//Browse the list as the circular list
	//Start with the first point whivh orientation differs from the parrent edge
	std::list <Point> ::iterator i_points = points.begin();

	//Run until the first suitable vertex with the different orientation is found
	while (i_points->getOrientation() ==  orientation_parent_edge)
		i_points ++;

	//Add first point to the edge
	Edge e;
	e.addPoint(*i_points);

	//Get orientation
	int orientation = i_points->getOrientation();

	//Set the start iterator: next point behind the start
	std::list <Point> ::iterator i_start = (i_points != points.end()) ? ++i_points : points.begin();
	//std::list <Point> ::iterator i_start  = i_points;
	//i_points = (i_points != points.end()) ? ++i_points : points.begin();

	//Process all points
	do
	{
		//Changed orientation
		if (i_points->getOrientation() != orientation)
		{
			//Orientation different from the parent edge orientation
			if (i_points->getOrientation() != orientation_parent_edge)
			{
				//Change orientation
				Point p_temp = *i_points;
				p_temp.setOrientation(orientation);

				//Add point with the previous orientation to the edge
				e.addPoint(p_temp);

				//Set oirientation and recursion
				e.setOrientation(orientation);
				e.setRecursion(recursion_depth + 1);

				//Add edge to the list
				el.push(e);
				e.printEdge();

				//Change orientation
				orientation = i_points->getOrientation();

				//Create new edge
				e.clear();

				//Add point to the edge
				e.addPoint(*i_points);
			}

			//Orientation analogous to the parent edge
			else
			{
				//Change its orientation
				i_points->setOrientation(orientation);

				//Add point to the edge
				e.addPoint(*i_points);
			}
		}

		else
		{
			//Add point to the edge
			e.addPoint(*i_points);
		}

		//Increment iterator
		i_points++;

		//Jump to the first element: the circular list
		if (i_points == points.end())
			i_points = points.begin();

	} while (i_points != i_start);
}
*/

void Edge::splitEdge3(std::stack<Edge> &el, Edge &em, int index, int orientation_parent_edge, int &orientation_last_edge, int recursion_depth)
{
	//Split an edge given by the list of points according to their orientations
	//Browse the list as the circular list
	//Start with the first point whivh orientation differs from the parrent edge
	std::list <Point> ::iterator i_points = points.begin();
	
	//Perform rotation of the list until the it starts/ends orientation differs from orientation_parent_edge
	for (int i = 0; ((points.front().getOrientation() == orientation_parent_edge) ) && i < points.size(); i++)
	{
		std::rotate(points.begin(), std::next(points.begin()), points.end());
	} 

	//Add first point after the last
	if (points.back().getOrientation() == orientation_parent_edge)
		points.push_back(points.front());
	double x1;
	/*
	//Add last point of the previous edge and a current point
	if (!el.empty() && index != -1)
	{
		//Get points
		Point p_last = el.top().getPoints().back();
		Point p_act = *i_points;

		int orient = p_last.getOrientation();

		const double dist = Algorithms::distance(p_last, p_act);
		if (dist > 0.001)
		{
			p_last.setOrientation(orient + 1);
			//p_act.setOrientation(orient+1);

			//Set oirientation and recursion
			Edge e_temp;
			e_temp.setOrientation(orient+1);
			e_temp.setRecursion(recursion_depth + 1);

			//Add points to the edge
			e_temp.push_back(p_last);
			e_temp.push_back(p_act);
			el.push(e_temp);

			x1 = p_last.getX();

			e_temp.printEdge();
		}
		//e_temp.printEdge();
	}
	*/


	//Get orientation
	int orientation = i_points->getOrientation();

	Edge e;
	e.push_back(*i_points);
	i_points++;

	//Process all points
	while(i_points != points.end())
	{
		//Changed orientation
		if (i_points->getOrientation() != orientation)
		{
			//Orientation different from the parent edge orientation
			if (i_points->getOrientation() != orientation_parent_edge)
			{
				//Change orientation
				Point p_temp = *i_points;
				p_temp.setOrientation(orientation);
				
				//Add point with the previous orientation to the edge
				e.push_back(p_temp);
				
				//Set oirientation and recursion
				e.setOrientation(orientation);
				e.setRecursion(recursion_depth + 1);

				//Add edge to the list
				el.push(e);
				e.printEdge();

				//Change orientation
				orientation = i_points->getOrientation();

				em = el.top();

				if (*i_points == points.back())
					break;

				//Create new edge
				e.clear();

				//Add point to the edge
				e.push_back(*i_points);
			}

			//Orientation analogous to the parent edge
			else
			{
				//Change its orientation
				i_points->setOrientation(orientation);

				//Add point to the edge
				e.push_back(*i_points);
			}
		}

		//Orientation does not changed
		else
		{
			//Add point to the edge
			e.push_back(*i_points);
		}

		//Increment iterator
		i_points++;
	}

	orientation_last_edge = orientation;
}

/*
void Edge::createEdge(Edge &e_new, Point &start, Point &end, std::list <Point> ::iterator &i_points, int orientation_edge_actual, int index, int posit)
{
	//Create new edge with points between intersections (between pointers)
	int vertex_amount = -1;
	Point p_temp(0, 0);
	Point *stop = end.getP();	//End point of the edge (point of the intersection)

	//Add inside points to edge
	while ( !(&*(i_points) == stop))
	{
		e_new.push_back(*i_points);

		i_points++;
	}

	//First edge has only 1 point
	if (e_new.getPointsSize() == 1 && index == -1)
		e_new.push_front(points.front());

	//Last edge has only 1 point
	if (e_new.getPointsSize() == 1 && index == 1)
		e_new.push_back(points.back());
}
*/



void Edge::createEdge(Edge &e_new, Point &start, Point &end, std::list <Point> ::iterator &i_points, int orientation_edge_actual, int index)
{
	//Create new edge with points between intersections (between pointers)
	int vertex_amount = -1;
	Point p_temp = start;

	//Add all internal points of the edge
	//Construct edges containing at least two points
	while (!(&(*i_points) == end.getP()))
	{
		//Write first stored point during the second step of the loop
		if (vertex_amount == 0)
		{
			e_new.push_back(p_temp);
		}

		//Remember the first point (and write during next iteration)
		if (*start.getP() == *i_points)
		{
			p_temp = *i_points;
		}

		//Write common point
		else
		{
			e_new.push_back(*i_points);
		}

		//Increment point
		i_points++;
		vertex_amount++;
	}

	//Add last point of the last edge
	if ((*i_points == points.back()) && (index == 1) && (vertex_amount >= 0))
	{
		//Last edge has only 2 points
		if (vertex_amount == 0)
		{
			e_new.push_back(p_temp);
		}

		//Last edge has more 2 points
		e_new.push_back(*i_points);
	}

	//Edge contains only 1 vertex, create this edge separately
	//Use its first point, second point is the construct
	if ((vertex_amount == 0) && (index <= 0) || (vertex_amount == -1) && (index == 1))
	{
		double x1, y1, x2, y2;

		//Create edge from 2 points: horizontal line
		if (orientation_edge_actual == 1 || orientation_edge_actual == 3)
		{
			//Line passes through 2 points: [x_temp | x_start, (y_i | y_temp + yt) / 2]
			x1 = (index == -1 ? p_temp.getX() : start.getX());
			y1 = (index == 1 ? (i_points->getY() + yt) / 2 : (p_temp.getY() + yt) / 2);
			x2 = (index == 1 ? i_points->getX() : end.getX());
			y2 = y1;
		}

		//Create edge from 2 points: vertical line
		else
		{
			//Line passes through 2 points: [(x_i | x_temp + xt) / 2, y_temp | y_start]
			x1 = (index == 1 ? (i_points->getX() + xt) / 2 : (p_temp.getX() + xt) / 2);
			y1 = (index == -1 ? p_temp.getY() : start.getY());
			x2 = x1;
			y2 = (index == 1 ? i_points->getY() : end.getY());
		}

		e_new.push_back(Point(x1, y1));
		e_new.push_back(Point(x2, y2));
	}
	
	/*
	//First edge has only 1 point
	if ((vertex_amount==0) && (index==-1))
	{
		//Create edge from 2 points: horizontal line
		if (orientation_edge_actual == 1 || orientation_edge_actual == 3)
		{
			double y_new=(p_temp.getY() + yt)/2;
			Point p1(p_temp.getX(), y_new);
			e_new.push_back(p1);
			Point p2(end.getX(), y_new);
			e_new.push_back(p2);
		}

		//Create edge from 2 points: vertical line
		else
		{
			double x_new=(p_temp.getX() + xt)/2;
			Point p1(x_new, p_temp.getY());
			e_new.push_back(p1);
			Point p2(x_new, end.getY());
			e_new.push_back(p2);
		}
	}

	//Last edge has only 1 point
	else if ((vertex_amount==-1) && (index==1))
	{
		//Create edge from 2 points: horizontal line
		if (orientation_edge_actual==1 || orientation_edge_actual==3)
		{
			double y_new=(i_points->getY() + yt)/2;
			Point p1(start.getX(), y_new);
			e_new.push_back(p1);
			Point p2(i_points->getX(), y_new);
			e_new.push_back(p2);
		}

		//Create edge from 2 points: vertical line
		else
		{
			double x_new=(i_points->getX() + xt)/2;
			Point p1(x_new, start.getY());
			e_new.push_back(p1);
			Point p2(x_new, i_points->getY());
			e_new.push_back(p2);
		}
	}
	
	//Common edge has only one point
	else if ((vertex_amount==0) && (index==0))
	{
		//Create edge from 2 points of mbr
		if (orientation_edge_actual==1||orientation_edge_actual==3)
		{
			double y_new=(p_temp.getY() + yt)/2;
			Point p1(start.getX(), y_new);
			e_new.push_back(p1);
			Point p2(end.getX(), y_new);
			e_new.push_back(p2);
		}
		//Create edge from 2 points of mbr
		else
		{
			double x_new=(p_temp.getX() + xt)/2;
			Point p1(x_new, start.getY());
			e_new.push_back(p1);
			Point p2(x_new, end.getY());
			e_new.push_back(p2);
		}
	}
	*/
}


void Edge::regressionLine(Point &p1, Point &p2, int orientation_edge)
{
	//Create regression line given by 2 points (p1, p2)
	double x1_l, y1_l, x2_l, y2_l;
	
	//First and last points of the edge
	const Point p_start = points.front();
	const Point p_end = points.back();

	//Recalculate orientation to actual edge
	if (recursion_depth > 1)
	{
		orientation_edge=(orientation_edge%4-(recursion_depth-1)%4);
		if (orientation_edge<=0) orientation_edge+=4;
	}

	//Horizontal line passing through T = [xt, yt]: P1 =[x_start, yt], P2 = [x_end, yt]
	if (orientation_edge == 1 || orientation_edge == 3) 
	{
		//P1
		x1_l=p_start.getX();
		y1_l=yt;

		//P2
		x2_l=p_end.getX();
		y2_l=yt;
	}

	//Vertical line passing through T = [xt, yt]: P1 =[xt, y_start, yt], P2 = [xt, y_end]
	else
	{
		//P1
		x1_l=xt;
		y1_l=p_start.getY();

		//P2
		x2_l=xt;
		y2_l=p_end.getY();
	}

	//Set start point of the regression line
	p1.setX(x1_l);
	p1.setY(y1_l);

	//Set end vertex of the regression line
	p2.setX(x2_l);
	p2.setY(y2_l);
}


void Edge::intersectionsEdgeAndRegressionLine(Point &p1, Point &p2)
{
	//Find intersections of the regression line and all segments of edge
	//Each intersection points on the end point of the edge segment (first edge point after intersection )
	std::list <Point> ::iterator i_points = points.begin(), i_last = points.begin();

	//Add first regression line point p1 to the list of intersections 
	//It points on the first point of the edge
	p1.setP(&*i_points);
	intersections.push_back(p1);
	
	//Increment
	i_points++;
	
	//Process all edge vertices
	while (i_points != points.end())
	{
		Point p_int(0, 0);
		if (Algorithms::get2LinesIntersection(*i_points, *i_last, p1, p2, p_int))
		{
			//Pointer on the end point of the edge segment (first edge point after intersection )
			p_int.setP(&*i_points);	
			
			//Add intersection
			intersections.push_back(p_int);

			//Add pointer top the intersection
			i_points->setP(&intersections.back());
		}

		//Assign and increment
		i_last = i_points;
		i_points++;
	}

	//Add last regression line point p2 to the list of intersections
	//It points on the last point of the edge
	p2.setP(&*points.rbegin());
	intersections.push_back(p2);
}


void Edge::processEdgeNoIntersection(std::stack<Edge>  &el)
{
	//Process edge without intersections with the regression line
	int orientation_edge = 0;					//Orientation of the edge
	int orientation_last_edge = 0;					//Orientation of the last edge <1,2,3,4>
	Edge eact;

	//Create min-max box given by V1-V4
	Point v1(0, 0), v2(0, 0), v3(0, 0), v4(0, 0);
	Algorithms::minMaxBox(this->points, v1, v2, v3, v4);

	//Assign min-max vertices
	Point v1p(0, 0), v2p(0, 0), v3p(0, 0), v4p(0, 0);
	if (findNewVertices(v1, v2, v3, v4, 2, -1, 0))
	{
		//Split vertices to 4 edges (index 2)
		splitEdge(el, eact, 2, orientation_edge, orientation_last_edge, this->recursion_depth);
		
		double xxx = 37;
	}

	//Building appproximation using the min-max box
	else
	{
		Edge e1, e2, e3, e4;
		v1.setOrientation(1);
		v2.setOrientation(1);
		e1.push_back(v1);
		e1.push_back(v2);

		v2.setOrientation(2);
		v3.setOrientation(2);
		e2.push_back(v2);
		e2.push_back(v3);

		v3.setOrientation(3);
		v4.setOrientation(3);
		e3.push_back(v3);
		e3.push_back(v4);

		v4.setOrientation(4);
		v1.setOrientation(4);
		e4.push_back(v4);
		e4.push_back(v1);

		//Add edges to the list
		el.push(e1);
		el.push(e2);
		el.push(e3);
		el.push(e4);
	}
}


void Edge::processEdgeWithIntersections(std::stack<Edge> &el, int orientation_edge)
{
	//Process edge and their intersections
	int  orientation_last_edge = 0;						//Orientation of the last edge <1,2,3,4>
	Edge eact;

	//Initialize iterators
	std::list <Point> ::iterator i_intersections = intersections.begin();
	std::list <Point> ::iterator i_points = points.begin();

	//Assign first intersection
	Point intersection_last = *i_intersections;
	i_intersections++;

	//Browse all intersections
	Edge *e_last = NULL;
	while (i_intersections!=intersections.end())
	{
		Edge e_new;

		//Compute index of the intersection
		int j = std::distance(intersections.begin(), i_intersections);

		//Check position (actual point and border intersections)
		int position_point_to_edge = getPointLinePosition(*intersection_last.getP(), intersections.front(), intersections.back());

		//Orientation to actual edge
		int orientation_edge_actual=orientation_edge;

		//Recalculate orientation to actual edge
		if (recursion_depth > 1)
		{
			orientation_edge_actual=(orientation_edge%4-(recursion_depth-1)%4);
			if (orientation_edge_actual<=0) orientation_edge_actual+=4;
		}

		else orientation_edge_actual=orientation_edge;

		//Index value: indicates the edge type
		int index = 0; //Common edge
		if (j == 1) index = -1; //First edge
		else if (j == intersections.size() - 1) index = 1; //Last edge

		//Create new edge and add new vertices if necessary
		createEdge(e_new, intersection_last, *i_intersections, i_points, orientation_edge_actual, index);

		//Find new vertices and split the edge
		Point v1(0, 0), v2(0, 0), v3(0, 0), v4(0, 0);

		//Find min-max box
		Algorithms::minMaxBox(e_new.getPoints(), v1, v2, v3, v4);

		//Find vertices using MBR depending on the orientation and split the edge
		if (orientation_edge_actual == 1) e_new.findNewVertices(v1, v2, v3, v4, index, position_point_to_edge, orientation_edge);
		else if (orientation_edge_actual == 2) e_new.findNewVertices(v2, v3, v4, v1, index, position_point_to_edge, orientation_edge);
		else if (orientation_edge_actual == 3) e_new.findNewVertices(v3, v4, v1, v2, index, position_point_to_edge, orientation_edge);
		else if (orientation_edge_actual == 4) e_new.findNewVertices(v4, v1, v2, v3, index, position_point_to_edge, orientation_edge);

		int n = e_new.getPoints().size();

		//Split the edge
		e_new.splitEdge(el, eact, index, orientation_edge, orientation_last_edge, this->recursion_depth);
		
		//Assign
		e_last = &el.top();
		intersection_last = *i_intersections;					//Store last intersection
		i_intersections++;							//Increment intersection
	}
}


void Edge:: printEdge()
{
	//Print edge
	file4 << "Edge:" << "o = " << orientation << "  s= " << split_ratio << "  r= " << recursion_depth;
	file4 << "  xt =" << xt << "  yt=" << yt << '\n';
	std::list <Point> ::iterator i_points=points.begin();
	while (i_points!=points.end())
	{
		i_points->print();
		i_points++;
	}
	file4 <<'\n';
}

/*
void Edge::printEdge()
{
	//Print edge
	std::cout << "Edge:" << "o = " << orientation << "  s= " << split_ratio << "  r= " << recursion_depth;
	std::cout << "  xt =" << xt << "  yt=" << yt << '\n';
	std::list <Point> ::iterator i_points = points.begin();
	while (i_points != points.end())
	{
		i_points->print();
		i_points++;
	}
	std::cout << '\n';
}
*/

