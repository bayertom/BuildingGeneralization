// Description: Sort point by y coordinate

#ifndef sortPointsByY_H
#define sortPointsByY_H

#include "Point.h"

//Sorter by Y coordinate
class sortPointsByY
{
        public:
                
                bool operator() (const Point & p1, const Point & p2 ) const
                {
			return ( ( p1.getY() < p2.getY() ) || ( p1.getY() == p2.getY() ) && ( p1.getX() < p2.getX() ) );
                }
 };

#endif
