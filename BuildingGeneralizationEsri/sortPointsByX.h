// Description: Sort point by x coordinate

#ifndef sortPointsByX_H
#define sortPointsByX_H

#include "Point.h"


//Sorter by X coordinate
class sortPointsByX
{
        public:
                
                bool operator() (const Point & p1, const Point & p2 ) const
                {
			return ( ( p1.getX() < p2.getX() ) || ( p1.getX() == p2.getX() ) && ( p1.getY() < p2.getY() ) );
                }
 };

#endif
