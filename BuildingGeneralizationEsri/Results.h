#pragma once
#ifndef Resulths
#define Resulths

#include <list>
#include "Edge.h"


class Result
{
private:
	std::list <Edge> result;
	std::list <Edge> ::iterator i_result;

public:
	Result(){i_result=result.begin();};
	~Result(){};
	void add(Edge e){result.push_back(e);};
	Edge * getEdge();
	int getSize(){return result.size();};
	void setIterator(){i_result=result.begin();};
	void DrawBuilding(double angle, IPointCollectionPtr p_collection);
	void DrawBuildingTemp(double angle, IScreenDisplayPtr p_display);
	void print();
	void clear(){this->result.clear();}
	double LH();
	void rotate(double angle);
};

#endif