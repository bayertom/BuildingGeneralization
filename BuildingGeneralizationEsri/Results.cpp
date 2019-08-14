#include "stdafx.h"
#include "Results.h"
#include "cmath"
extern std::ofstream file4;

Edge * Result::getEdge()
{
	if (i_result!=result.end())
	{
		return &*i_result++;
	}
	else
	{
		i_result=result.begin();
		return NULL;
	}

}


void Result::DrawBuilding(double angle, IPointCollectionPtr p_collection)
{
	//Draw source points
	int orientation, recursion, fract, index=0;
	double xt, yt, xt_old, yt_old;
	//CPen p2;
	std::list <Edge> ::iterator i_start;
	int n=result.size();
	//p2.CreatePen(PS_SOLID, 1, RGB(10,50, 105));  //Blue pen

	i_result=result.begin();
	i_start=result.end();

	if(n>0)
	{
		while (i_result!=i_start)
		{
			if (i_start==result.end())
			{
				i_start=i_result;
			}

			//Get orientation
			orientation=i_result->getOrientation();
			recursion=i_result->getRecursion();
			if (recursion>1)
			{
				fract=(orientation%4-(recursion-1)%4);
				if (fract<=0) fract+=4;
			}
			else fract=orientation;

			//Get mass coordinates
			xt=i_result->getXt();
			yt=i_result->getYt();

			if (fract==2||fract==4)
			{
				if (index==1)
				{
					Point p(xt,yt_old);
					p.rotate(angle);
					IPointPtr ip(CLSID_Point);
					ip->put_X(p.getX());
					ip->put_Y(p.getY());
					p_collection->AddPoint(ip);
					//dc.MoveTo((int)p.getX(),(int)p.getY());
				}
				else if (index>1)
				{
					Point p(xt,yt_old);
					p.rotate(angle);
					IPointPtr ip(CLSID_Point);
					ip->put_X(p.getX());
					ip->put_Y(p.getY());
					p_collection->AddPoint(ip);
					//dc.LineTo((int)p.getX(),(int)p.getY());
				}
			}
			else if (fract==1||fract==3)
			{
				if (index==1)
				{
					Point p(xt_old,yt);
					p.rotate(angle);
					IPointPtr ip(CLSID_Point);
					ip->put_X(p.getX());
					ip->put_Y(p.getY());
					p_collection->AddPoint(ip);
					//dc.MoveTo((int)p.getX(),(int)p.getY());
				}
				else if (index>1)
				{
					Point p(xt_old,yt);
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

			xt_old=xt;
			yt_old=yt;

			index++;
			i_result++;

			if (i_result==result.end())
			{
				i_result=result.begin();
				i_start++;
				i_start++;
			}
		}
		//DeleteObject(p2);
	}
}

void Result::DrawBuildingTemp(double angle, IScreenDisplayPtr p_display)
{
	//Draw source points
	int orientation, recursion, fract, index=0;
	double xt, yt, xt_old, yt_old;

	std::list <Edge> ::iterator i_start;
	int n=result.size();

	i_result=result.begin();
	i_start=result.end();

	//Create collection
	IPointCollectionPtr p_collection(CLSID_Polygon);
	IPolygonPtr p_polygon(p_collection);
	
	if(n>0)
	{
		while (i_result!=i_start)
		{
			if (i_start==result.end())
			{
				i_start=i_result;
			}

			//Get orientation
			orientation=i_result->getOrientation();
			recursion=i_result->getRecursion();
			if (recursion>1)
			{
				fract=(orientation%4-(recursion-1)%4);
				if (fract<=0) fract+=4;
			}
			else fract=orientation;

			//Get mass coordinates
			xt=i_result->getXt();
			yt=i_result->getYt();

			if (fract==2||fract==4)
			{
				if (index==1)
				{
					Point p(xt,yt_old);
					p.rotate(angle);
					IPointPtr ip(CLSID_Point);
					ip->put_X(p.getX());
					ip->put_Y(p.getY());
					
					p_collection->AddPoint(ip);
					//dc.MoveTo((int)p.getX(),(int)p.getY());
				}
				else if (index>1)
				{
					Point p(xt,yt_old);
					p.rotate(angle);
					IPointPtr ip(CLSID_Point);
					ip->put_X(p.getX());
					ip->put_Y(p.getY());
					p_collection->AddPoint(ip);
					//dc.LineTo((int)p.getX(),(int)p.getY());
				}
			}
			else if (fract==1||fract==3)
			{
				if (index==1)
				{
					Point p(xt_old,yt);
					p.rotate(angle);
					IPointPtr ip(CLSID_Point);
					ip->put_X(p.getX());
					ip->put_Y(p.getY());
					p_collection->AddPoint(ip);
					//dc.MoveTo((int)p.getX(),(int)p.getY());
				}
				else if (index>1)
				{
					Point p(xt_old,yt);
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

			xt_old=xt;
			yt_old=yt;

			index++;
			i_result++;

			if (i_result==result.end())
			{
				i_result=result.begin();
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

double Result::LH()
{
	int orientation, recursion, fract, index=0;
	double xt, yt, xt_old, yt_old,x1,y1,y2, area=0;
	Point Pi(0,0),Pii(0,0),Piii(0,0);
	std::list <Edge> ::iterator i_start;
	int n=result.size();

	i_result=result.begin();
	i_start=result.end();

	if(n>0)
	{
		while (i_result!=i_start)
		{
			if (i_start==result.end())
			{
				i_start=i_result;
			}

			//Get orientation
			orientation=i_result->getOrientation();
			recursion=i_result->getRecursion();
			if (recursion>1)
			{
				fract=(orientation%4-(recursion-1)%4);
				if (fract<=0) fract+=4;
			}
			else fract=orientation;

			//Get mass coordinates
			xt=i_result->getXt();
			yt=i_result->getYt();

			//Assign vertices
			Pi=Pii;
			Pii=Piii;

			if (fract==2||fract==4)
			{
				if (index==1)
				{
					x1=xt;
					y1=yt_old;
				}
				else if (index==2)
				{
					y2=yt_old;
					Piii.setX(xt);
					Piii.setY(yt_old);
				}
				else if (index>1)
				{
					Piii.setX(xt);
					Piii.setY(yt_old);
					area=area+Pii.getX()*(Piii.getY()-Pi.getY());
				}
			}
			else if (fract==1||fract==3)
			{
				if (index==1)
				{
					x1=xt_old;
					y1=yt;
				}
				else if (index==2)
				{
					y2=yt;
					Piii.setX(xt_old);
					Piii.setY(yt);
				}
				else if (index>1)
				{
					Piii.setX(xt_old);
					Piii.setY(yt);
					area=area+Pii.getX()*(Piii.getY()-Pi.getY());
				}
			}

			xt_old=xt;
			yt_old=yt;

			index++;
			i_result++;

			if (i_result==result.end())
			{
				i_result=result.begin();
				i_start++;
				i_start++;
			}
		}
	}

	//First point
	area=area+x1*(y2-Piii.getY());

	//Last point
	area=area+Piii.getX()*(y1-Pii.getY());
	return 0.5*fabs(area);
}

void Result::rotate(double angle)
{
	//Rotate edge
	i_result=result.begin();
	while (i_result!=result.end())
	{
		i_result->rotate(angle);
		i_result++;

	}
}
void Result:: print()
{
	i_result=result.begin();

	file4 << "Vysledne hrany" << '\n';
	while (i_result!=result.end())
	{
		i_result->printEdge();
		i_result++;
	}
}